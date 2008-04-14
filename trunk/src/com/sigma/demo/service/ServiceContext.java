package com.sigma.demo.service;

import static com.sigma.demo.mbean.ServiceMBean.ATTR_STATE;
import static com.sigma.demo.mbean.ServiceMBean.OP_CREATED;
import static com.sigma.demo.mbean.ServiceMBean.STATE_NEW;

import java.util.Collection;
import java.util.Map;
import java.util.Vector;

import javax.management.Attribute;
import javax.management.AttributeNotFoundException;
import javax.management.InstanceAlreadyExistsException;
import javax.management.InstanceNotFoundException;
import javax.management.InvalidAttributeValueException;
import javax.management.ListenerNotFoundException;
import javax.management.MBeanException;
import javax.management.MBeanRegistrationException;
import javax.management.MBeanServer;
import javax.management.NotificationFilter;
import javax.management.NotificationListener;
import javax.management.ObjectName;
import javax.management.ReflectionException;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

import com.sigma.demo.cfg.ServiceMBean;
import com.sigma.demo.util.JMX;
import com.sigma.demo.util.Parameter;
/**
 * The context of per service.
 * @author user
 * 
 */
public class ServiceContext {
	protected static final Logger log = LogManager.getLogger(ServiceContext.class);
	
	private String name = null;
	private MBeanServer mbs = null;
	private Parameter param = null;
	private String[] dependOn = null;
	private ObjectName serverName = null;
	
	public ServiceContext(ServiceMBean mbean, MBeanServer server, ObjectName serverName){
		this(mbean.getName(), server, serverName, mbean.getAttribute());
		
		String depends = mbean.getDepends();
		dependOn = (depends != null) ? depends.split(";") :
					new String[]{};
	}
	
	public ServiceContext(String name, MBeanServer server, ObjectName serverName, Map<String,String> attrs) {
		this.name = name;
		this.mbs = server;
		this.serverName = serverName;
		this.param = new Parameter(attrs);
	}
	
	public void addNotificationListener(ObjectName name,
            NotificationListener listener,
            NotificationFilter filter,
            Object handback)
            throws InstanceNotFoundException{
		this.mbs.addNotificationListener(name, listener, filter, handback);
	}
	
	public void removeNotificationListener(ObjectName name,
            NotificationListener listener)
            throws InstanceNotFoundException,
                   ListenerNotFoundException{
		
		this.mbs.removeNotificationListener(name, listener);
	}
	
	public Object invoke(ObjectName name,
            String operationName,
            Object[] params,
            String[] signature)
            throws InstanceNotFoundException,
                   MBeanException,
                   ReflectionException{
		return this.mbs.invoke(name, operationName, params, signature);
	}	
	
	public void setAttribute(ObjectName name, Attribute attribute)
			throws InstanceNotFoundException, AttributeNotFoundException,
			InvalidAttributeValueException, MBeanException, ReflectionException {
		this.mbs.setAttribute(name, attribute);
	}
	
	public Object getAttribute(ObjectName name, String attr) {
		Object value = null;
		try {
			value = this.mbs.getAttribute(name, attr);
		} catch (Exception e) {
			log.error(e, e);
		}
		return value;
	}
	
	public Object getService(String name) {
		return getService(name, null);
	}
	
	public Object getService(String name, Class exported) {
				
		Object result = null; 
		try {
			ObjectName mBeanName = null;
			mBeanName = new ObjectName(name);
			if(mbs.isRegistered(mBeanName)){
				result = (exported == null) ? mBeanName : JMX.export(mbs, mBeanName, exported);
			}
		}catch(Exception e) {
			log.error(e,e);
		}
		
		return result;
	}
	
	@SuppressWarnings("unchecked")
	public Collection<?> queryService(ObjectName  name) {
		//QueryExp exp = new 
		return mbs.queryNames(name,null);
	}
	
	public void removeService(String name){
		ObjectName mBeanName = null;
		try {
			mBeanName = new ObjectName(name);
			mbs.unregisterMBean(mBeanName);
		} catch (InstanceNotFoundException e){
		} catch (Exception e) {
			log.error(e,e);
		}
	}
	
	public Object createService(ObjectName mBeanName, String className) throws InstanceAlreadyExistsException {
		
		return createService(mBeanName, className, null);
	}
	
	public Object createService(ObjectName mBeanName, String className, Class exported) throws InstanceAlreadyExistsException {
		return createService(mBeanName, className, null, null);
	}
	
	public Object createService(ObjectName mBeanName, String className, Class exported, Map<String, String> param) throws InstanceAlreadyExistsException {
		try {
			mbs.createMBean(className, mBeanName);
			ServiceContext ctx = new ServiceContext(mBeanName.getCanonicalName(), mbs, this.serverName, param);
			
			mbs.setAttribute(mBeanName, new Attribute(ATTR_STATE, STATE_NEW));
			
			mbs.invoke(mBeanName, OP_CREATED,
						new Object[]{mBeanName.getCanonicalName(),
									ctx},
						new String[]{String.class.getName(),
									ServiceContext.class.getName()
									});
			log.info("created Service:" + mBeanName.getCanonicalName() + ", class:" + className);
		}catch(InstanceAlreadyExistsException e) {
			throw e;
		}catch(Exception e) {
			if(mBeanName != null){
				try {
					mbs.unregisterMBean(mBeanName);
				}catch(InstanceNotFoundException e1) {
				}catch(MBeanRegistrationException e1) {
					log.error(e,e);
				}
			}
			log.error(e,e);
		}
		
		return (exported != null) ? JMX.export(this.mbs, mBeanName, exported) : mBeanName;
	}
	
	public void bindMBean(String name, Object bean){
		try {
			mbs.registerMBean(bean, new ObjectName(name));
		}catch(Exception e) {
			log.error(e,e);
		}
	}
	
	public ObjectName getServerName() {
		return this.serverName;
	}
	
	public String getName(){
		return this.name;
	}
	
	public String[] dependOn(){
		return this.dependOn;
	}
	
	public MBeanServer currentMBeanServer(){
		return mbs;
	}
	
	public String getStringParameter(String name, String def) {
		return param.getStringParameter(name, def);
	}

	public int getIntParameter(String name, int def) {
		return param.getIntParameter(name, def);
	}

	public boolean getBooleanParameter(String name, boolean def) {
		return param.getBooleanParameter(name, def);
	}
	
	public Parameter getParameter() {
		return param;
	}
	
}
