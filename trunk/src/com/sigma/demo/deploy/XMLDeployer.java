package com.sigma.demo.deploy;

import static com.sigma.demo.cfg.ServerConfig.*;
import static com.sigma.demo.mbean.notification.DeployerNotification.POST_DEMO_DEPLOY;
import static com.sigma.demo.mbean.notification.DeployerNotification.POST_DEMO_STARTED;
import static com.sigma.demo.mbean.notification.DeployerNotification.POST_DEMO_STOPED;
import static com.sigma.demo.mbean.notification.DeployerNotification.POST_DEMO_UNDEPLOYED;
import static com.sigma.demo.mbean.notification.DeployerNotification.PRE_DEMO_DEPLOY;
import static com.sigma.demo.mbean.notification.DeployerNotification.PRE_DEMO_START;
import static com.sigma.demo.mbean.notification.DeployerNotification.PRE_DEMO_STOP;
import static com.sigma.demo.mbean.notification.DeployerNotification.PRE_DEMO_UNDEPLOY;

import java.io.File;
import java.io.FileInputStream;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Map;

import javax.management.Attribute;
import javax.management.InstanceAlreadyExistsException;
import javax.management.ObjectName;

import com.sigma.demo.BasicDemoService;
import com.sigma.demo.BasicDemoServiceMBean;
import com.sigma.demo.DemoService;
import com.sigma.demo.cfg.ServerConfig;
import com.sigma.demo.data.DemoData;
import com.sigma.demo.data.XMLDemoData;
import com.sigma.demo.service.FailServiceException;

public class XMLDeployer extends AbstractDeployer implements XMLDeployerMBean{

	//protected Logger log = LogManager.getLogger(XMLDeployer.class);
	private String domain = null;
	
	//private static final String serviceClass = BasicDemoService.class.getName();
	private String serviceClass = null;
	//private String serviceClass = null;
	private Map<String, String> demoConfig = null;
	
	@SuppressWarnings("unchecked")
	public void start() throws FailServiceException {		
		domain = context.getStringParameter(CFG_DOMAIN_DEMO, null);
		String configServiceName = context.getStringParameter(CFG_DEMO_CONFIG, null);
		try {
			demoConfig = (Map<String, String>) context.invoke(new ObjectName(
					configServiceName), "config", null, null);
			
			if(domain == null) {
				domain = demoConfig.get(CFG_DOMAIN_DEMO);
				domain = (domain == null) ? DEFAULT_DOMAIN_DEMO : domain; 
				serviceClass = demoConfig.get(CFG_DEMO_SERVICE_CLASS);
				serviceClass = (serviceClass == null ) ? BasicDemoService.class.getName() : serviceClass;
			}
		} catch (Exception e) {
			log.error(e, e);
			throw new FailServiceException(e.getMessage(), e);
		}
		
		super.start();
	}
	
	@Override
	protected boolean accept(DeployEntity entity) {
		return entity.getServiceFile().getName().endsWith("-demo.xml");
	}

	@Override
	protected void deploy(DeployEntity entity) throws DeployException {
		DemoService demoService = null;
		File file = entity.getServiceFile();
		ObjectName demoObjectName = null;
		log.info("Deploy demo service, Path:" + entity.getServiceFile().getAbsolutePath());
		try {
			entity.setDeployer(getName());			
			sendNotification(PRE_DEMO_DEPLOY, entity);
			
			DemoData d = new XMLDemoData(new FileInputStream(file));
			log.info("Create demo service, name:" + d.getFullName());
			
			//demoObjectName = new ObjectName(this.domain);
			Hashtable<String,String> attr = new Hashtable<String,String>();
			attr.put(NAME_KEY_NAME, d.getFullName().trim());
			attr.put(NAME_KEY_TYPE, "demo");
			attr.put(NAME_KEY_GROUP, lookupGroup(file));
			demoObjectName = new ObjectName(this.domain, attr);
			entity.setServiceName(demoObjectName.getCanonicalName());
			
			demoService = (DemoService)context.createService(
					demoObjectName,
					serviceClass, 
					DemoService.class,
					demoConfig);
			
			if(demoService != null){
				entity.setService(demoService);
				
				demoService.initDemoContext(d);
				sendNotification(PRE_DEMO_START, entity);
				log.info("Starting demo service:" + d.getFullName());
				context.setAttribute(demoObjectName, new Attribute(BasicDemoServiceMBean.ATTR_DEPLOY_INFO, entity));
				context.setAttribute(demoObjectName, new Attribute(ATTR_STATE, STATE_START));
				context.invoke(demoObjectName, OP_START, new Object[]{}, new String[]{});
				sendNotification(POST_DEMO_STARTED, entity);
				context.setAttribute(demoObjectName, new Attribute(ATTR_STATE, STATE_RUNNING));
				log.info("Started demo service:" + d.getFullName());
				//log.info("Started demo service:" + d.getFullName());
				bindDemoToJNDI(demoObjectName, entity);
				
				sendNotification(POST_DEMO_DEPLOY, entity);
				log.info("New demo service started, name:" + d.getFullName());
			}else {
				throw new DeployException(entity,String.format("Can't create demo seriver, demoName:%1$s,serviceClass:%2$s",
						demoObjectName.getCanonicalName(),
						serviceClass));
			}
		}catch(InstanceAlreadyExistsException e){
			log.error("Exists same demo service deployed. name:" + demoObjectName.getCanonicalName(),e);
		}catch (Throwable e) {
			if(demoObjectName != null){
				try {
					context.setAttribute(demoObjectName, new Attribute(ATTR_STATE, STATE_ERROR));
				} catch(Exception e1){}
			}
			log.error(e,e);
			throw new DeployException(entity, e.getMessage()); 	
		}
		log.info("Deployed demo service, Path:" + entity.getServiceFile().getAbsolutePath());
		
	}

	@Override
	protected void unDeploy(DeployEntity entity) throws DeployException {
		log.info("unDeploy demo service, Path:" + entity.getServiceFile().getAbsolutePath());		
		sendNotification(PRE_DEMO_UNDEPLOY, entity);
		try {
			ObjectName demoName = new ObjectName(entity.getServiceName());
			
			log.info("Stopping demo service," + demoName);
			
			unbindJNDI(demoName);			
			sendNotification(PRE_DEMO_STOP, entity);
			context.setAttribute(demoName, new Attribute(ATTR_STATE, STATE_SHUTDOWN));
			context.invoke(demoName, OP_SHUTDOWN, new Object[]{}, new String[]{});
			log.info("Stoped demo service," + demoName);
			
			sendNotification(POST_DEMO_STOPED, entity);
			context.removeService(entity.getServiceName());
			sendNotification(POST_DEMO_UNDEPLOYED, entity);			
		}catch (Throwable e) {
			log.error(e,e);
			throw new DeployException(entity, e.getMessage()); 	
		}
		log.info("unDeployed demo service, Path:" + entity.getServiceFile().getAbsolutePath());
	} 

}
