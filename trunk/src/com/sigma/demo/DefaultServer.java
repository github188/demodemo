package com.sigma.demo;

import static com.sigma.demo.mbean.ServiceMBean.ATTR_DEPENDS;
import static com.sigma.demo.mbean.ServiceMBean.ATTR_ERRORMESSAGE;
import static com.sigma.demo.mbean.ServiceMBean.ATTR_STATE;
import static com.sigma.demo.mbean.ServiceMBean.OP_CREATED;
import static com.sigma.demo.mbean.ServiceMBean.OP_START;
import static com.sigma.demo.mbean.ServiceMBean.STATE_NEW;
import static com.sigma.demo.mbean.ServiceMBean.STATE_RUNNING;
import static com.sigma.demo.mbean.ServiceMBean.STATE_START;
import static com.sigma.demo.mbean.ServiceMBean.STATE_WAIT;

import java.io.InputStream;
import java.lang.management.ManagementFactory;
import java.util.Collection;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.Properties;
import java.util.Vector;

import javax.management.Attribute;
import javax.management.MBeanInfo;
import javax.management.MBeanServer;
import javax.management.Notification;
import javax.management.NotificationBroadcasterSupport;
import javax.management.NotificationListener;
import javax.management.ObjectName;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

import com.sigma.demo.cfg.DemoServerConfig;
import com.sigma.demo.cfg.ServerConfig;
import com.sigma.demo.cfg.ServiceMBean;
import com.sigma.demo.service.ServiceContext;

public class DefaultServer extends NotificationBroadcasterSupport  
	implements Server, DefaultServerMBean, NotificationListener{
	protected static final Logger log = LogManager.getLogger(DefaultServer.class);
	protected static Logger eventLogger = null;
	
	private ObjectName serverName = null;
	
	public void init(Properties initParam) {
		Enumeration keys = initParam.propertyNames();
		log.info("===========Init Config Parameters============");
		for(;keys.hasMoreElements();){
			String key = (String)keys.nextElement();
			log.info(key + " = " + initParam.getProperty(key));
		}
		log.info("===========END Config Parameters====== ======");
	}

	public void launcher() {
		eventLogger = LogManager.getLogger(ServerConfig.LOGGER_NOTIFICATION);
		
		MBeanServer mbs = createMBeanServer();
		DemoServerConfig config = loadServerConfig();
		
		loadServiceMBean(config, mbs);
	}
	
	private DemoServerConfig loadServerConfig(){
		
		DemoServerConfig config = null;
		 InputStream in = Thread.currentThread().
		 				getContextClassLoader().
		 				getResourceAsStream("ds-service.xml");
		 
		 if(in != null){
			 config = new DemoServerConfig(in);
		 }else {
			 log.error("Not found service config file.");
		 }
		
		return config;
	}
	
	private MBeanServer createMBeanServer(){
		log.info("create JMX Server....");
		MBeanServer mbs = ManagementFactory.getPlatformMBeanServer();
		return mbs;
	}
	
	private void loadServiceMBean(DemoServerConfig serverConfig, MBeanServer mbs) {
		log.info("loading Service MBean...");
		Collection<ObjectName> padding = new Vector<ObjectName>();
		//Stack<BaseService> depends = new Stack<BaseService>();
		launcherDemoServer(serverConfig, mbs);
		
		for(ServiceMBean serviceConfig : serverConfig.getServiceMBean()) {
			try {
				ObjectName serviceName = new ObjectName(serviceConfig.getName());
				mbs.createMBean(serviceConfig.getCode(), serviceName);
				
				mbs.setAttribute(serviceName, new Attribute(ATTR_STATE, STATE_NEW));
				
				ServiceContext context = new ServiceContext(serviceConfig, mbs, serverName);				
				mbs.invoke(serviceName, OP_CREATED,
							new Object[]{serviceConfig.getName(), context},
							new String[]{String.class.getName(),
										ServiceContext.class.getName()
										});
				
				registerNotificationBroadcaster(serviceName, mbs);
				startWithDependCheck(serviceName, padding, mbs);
				
			}catch(Exception e) {
				log.error(e,e);
			} 
			//service.getCode();
		}
		
		for(int i = padding.size(); i > 0; i--){
			Iterator<ObjectName> iter = padding.iterator();
			padding = new Vector<ObjectName>();
			for (; iter.hasNext();) {
				startWithDependCheck(iter.next(), padding, mbs);
			}
		}
		
		//The Service can't start.
		if(padding.size() > 0){
			log.warn("=======Can't start service =========");
			for(ObjectName s : padding){
				log.warn("Name:" + s.toString() + dumpDepends(s, mbs));
			}
		}
	}
	
	private void launcherDemoServer(DemoServerConfig serverConfig, MBeanServer mbs){
		
		log.debug("launching main service...");
		try {
			serverName = new ObjectName(serverConfig.getName());
			mbs.registerMBean(this, serverName);
		} catch(Exception e) {
			log.error(e,e);
		}		
		log.debug("launched main service...");
	}
	
	/**
	 * register the main service as notification broadcaser, forward all notification to all services.
	 * @param serviceName
	 * @param mbs
	 * </code></pre>
	 */
	private void registerNotificationBroadcaster(ObjectName serviceName, MBeanServer mbs) {
		try {
			 MBeanInfo mBeanInfo = mbs.getMBeanInfo(serviceName);
			 if(mBeanInfo.getNotifications().length > 0 ||
				serverName != null){
				 mbs.addNotificationListener(serviceName, serverName, null, null);
				 //mbs.removeNotificationListener(serviceName, serverName, null, null);
			 }
		} catch(Exception e) {
			log.error(e,e);
		}
	}
	
	private void startWithDependCheck(ObjectName service, 
									  Collection<ObjectName> padding,
									  MBeanServer mbs 
									  ){
		try{
			if(allDependsStarted(service, mbs)){
				log.info("Starting service, name:" + service.toString());
				mbs.setAttribute(service, new Attribute(ATTR_STATE, STATE_START));
				mbs.invoke(service, OP_START,
						new Object[]{},
						new String[]{});
				mbs.setAttribute(service, new Attribute(ATTR_STATE, STATE_RUNNING));
				log.info("Started service, name:" + service);
			}else {
				mbs.setAttribute(service, new Attribute(ATTR_STATE, STATE_WAIT));
				padding.add(service);
			}
		}catch(Exception e){
			try{
				mbs.setAttribute(service, new Attribute(ATTR_ERRORMESSAGE, e.getMessage()));
				log.error("Fail to start service, name:" + service.toString(), e);
			}catch(Exception e1){}
		}
	}
	
	private boolean allDependsStarted(ObjectName service, MBeanServer mbs){
		ObjectName objectName = null;		
		String depends = null;
		try{
			depends = (String)mbs.getAttribute(service, ATTR_DEPENDS);
		}catch(Exception e){}
		if(depends == null || depends.trim().equals("")){
			return true;
		}
		
		try {
			for(String name: depends.split(";")){
				objectName = new ObjectName(name);
				String _state = (String)mbs.getAttribute(objectName, ATTR_STATE);
				if(_state == null || !_state.equals(STATE_RUNNING)){
					return false;
				}
			}
		}catch(Exception e){
			return false;
		}
		
		return true;
	}
	
	private String dumpDepends(ObjectName service, MBeanServer mbs){
		ObjectName objectName = null;		
		String depends = null;
		StringBuffer buffer = new StringBuffer("\n\tDepend on:\n");
		try{
			depends = (String)mbs.getAttribute(service, ATTR_DEPENDS);
		}catch(Exception e){}
		if(depends != null){
			for(String name: depends.split(";")){
				try{
					buffer.append("\t\tMBean:");
					buffer.append(name);
					buffer.append("\n");
					objectName = new ObjectName(name);
					buffer.append("\t\t\texists:" + mbs.isRegistered(objectName));
					buffer.append("\n");
					String _state = (String)mbs.getAttribute(objectName, ATTR_STATE);
					buffer.append("\t\t\tstate:" + _state);
					buffer.append("\n");
				}catch(Exception e){
					buffer.append("\t\t\tError:" + e.getMessage() + "\n");
				}
				
			}
		}
		return buffer.toString();
	}

	public String getName() {
		return serverName.getCanonicalName();
	}

	/**
	 * Forward the notification to all service.
	 */
	public void handleNotification(Notification notification, Object handback) {
		if(eventLogger.isDebugEnabled()){
			eventLogger.debug("Forwarder:" + notification.toString());
		}
		
		sendNotification(notification);		
	}

}
