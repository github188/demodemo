/* AbstractDeployer.java
 *
 * Sigma Resources & Technologies, Inc.
 * Copyright (C) 2005-2008, sigma-rt.com
 *
 * PROPRIETARY RIGHTS of Sigma Resources & Technologies are involved in
 * the subject matter of this material. All manufacturing, reproduction,
 * use, and sales rights pertaining to this subject matter are governed
 * by the license agreement. The recipient of this software implicitly
 * accepts the terms of the license.
 *
 * Created on 2008/3/6, by Alexandre Tsu.
 * 
 * Project:	DemoServer
 * Author :	James Che
 * Auditor: 
 * 
 * # any description here # 
 *
 * For more information, visit:
 * http://www.sigma-rt.com
 * 
 * Change History
 * ---------------------------------------------------
 * REVISION          DATE/TIME        AUTHOR
 * ---------------------------------------------------
 * $Log$
 * ---------------------------------------------------
 *
 */
package com.sigma.demo.deploy;


import static com.sigma.demo.cfg.ServerConfig.CFG_GROUP_MANAGER;
import static com.sigma.demo.cfg.ServerConfig.NAME_KEY_GROUP;
import static com.sigma.demo.cfg.ServerConfig.NAME_KEY_NAME;
import static com.sigma.demo.mbean.notification.DeployerNotification.DEPLOY_EXCEPTION;
import static com.sigma.demo.mbean.notification.DeployerNotification.NEW_DEPLOY;
import static com.sigma.demo.mbean.notification.DeployerNotification.POST_DEMO_DEPLOY;
import static com.sigma.demo.mbean.notification.DeployerNotification.POST_DEMO_STARTED;
import static com.sigma.demo.mbean.notification.DeployerNotification.POST_DEMO_STOPED;
import static com.sigma.demo.mbean.notification.DeployerNotification.POST_DEMO_UNDEPLOYED;
import static com.sigma.demo.mbean.notification.DeployerNotification.PRE_DEMO_DEPLOY;
import static com.sigma.demo.mbean.notification.DeployerNotification.PRE_DEMO_START;
import static com.sigma.demo.mbean.notification.DeployerNotification.PRE_DEMO_STOP;
import static com.sigma.demo.mbean.notification.DeployerNotification.PRE_DEMO_UNDEPLOY;
import static com.sigma.demo.mbean.notification.DeployerNotification.REMOVE_DEPLOY;
import static com.sigma.demo.mbean.notification.DeployerNotification.UPDATED_DEPLOY;

import java.io.File;

import javax.management.InstanceNotFoundException;
import javax.management.MBeanException;
import javax.management.MBeanNotificationInfo;
import javax.management.Notification;
import javax.management.NotificationListener;
import javax.management.ObjectName;
import javax.management.ReflectionException;
import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;

import org.apache.log4j.Logger;

import com.sigma.demo.BasicDemoService;
import com.sigma.demo.acl.GroupServiceMBean;
import com.sigma.demo.cfg.ServerConfig;
import com.sigma.demo.mbean.notification.DeployerNotification;
import com.sigma.demo.mbean.notification.TyppedNotificationFilter;
import com.sigma.demo.service.AbstractService;
import com.sigma.demo.service.FailServiceException;

public abstract class AbstractDeployer extends AbstractService
	implements NotificationListener{
	private long sequenceNumber = 0;
	private Logger eventLogger = null;
	private Context ctx = null;
	private ObjectName groupManager = null;
	
	public void shutDown() throws FailServiceException {
		try {
			context.removeNotificationListener(context.getServerName(), this);
		} catch(Exception e) {
			throw new FailServiceException(e.getMessage(),e);
		}
	}

	public void start() throws FailServiceException {
		
		eventLogger = this.getLogger(ServerConfig.LOGGER_NOTIFICATION);
		//this.getName()
		try {
			String serviceName = context.getStringParameter(CFG_GROUP_MANAGER, null);
			if(serviceName != null) {
				groupManager = new ObjectName(serviceName);
			}
		}catch(Exception e) {
			log.warn("The groupManager server not located.");
		}

		
		try{
			context.addNotificationListener(context.getServerName(), this,
					new TyppedNotificationFilter("scanner", this),
					//DeployerNotification.filter,
					null);
			ctx = new InitialContext();
		}catch(Exception e){
			throw new FailServiceException(e.getMessage());
		}
	}
	
    public MBeanNotificationInfo[] getNotificationInfo() { 
        String[] types = new String[] {
        		PRE_DEMO_DEPLOY,
        		POST_DEMO_DEPLOY,
        		PRE_DEMO_START,
        		POST_DEMO_STARTED,
        		PRE_DEMO_STOP,
        		POST_DEMO_STOPED,
        		PRE_DEMO_UNDEPLOY,
        		POST_DEMO_UNDEPLOYED,
        		DEPLOY_EXCEPTION
        };
        
        String name = DeployerNotification.class.getName(); 
        String description = "The lifecycle of demo deploy/undeploy";        
        MBeanNotificationInfo info = 
            new MBeanNotificationInfo(types, name, description); 
        return new MBeanNotificationInfo[] {info};
    }

	public void handleNotification(Notification notify, Object arg1) {
		String type = notify.getType();
		
		if(log.isDebugEnabled()){
			eventLogger.debug("Handler Name:" + getName() + ",Notification:" + notify);
		}
		
		DeployEntity entity = null;
		try{
			entity = (DeployEntity)notify.getUserData();
			if(type.equals(NEW_DEPLOY)){
				if(this.accept(entity)){
					deploy(entity);
				}
			}else if(type.equals(UPDATED_DEPLOY)) {
				//if(entity.getDeployer() != null && entity
				if(isDeployed(entity)){
					unDeploy(entity);
					deploy(entity);
				}
			}else if(type.equals(REMOVE_DEPLOY)) {
				if(isDeployed(entity)){
					unDeploy(entity);
				}
			}else {
				log.error("Unknow deploy notification, type:" + type);
			}
		}catch(DeployException e){
			log.error(e,e);
			if(entity != null){
				sendNotification(DEPLOY_EXCEPTION, entity);
			}
		}
		if(log.isDebugEnabled()){
			eventLogger.debug("End handleNotification...");
		}
	}
	
	protected void sendNotification(String type, DeployEntity entity){
		Notification n = new DeployerNotification(type, this,
				sequenceNumber++, entity);
		if(eventLogger.isDebugEnabled()){
			eventLogger.debug("Start:" + n);
		}
		sendNotification(n);
	}
	
	/**
	 * <pre><code>
	 * Is deployed the DeployEntity.
	 * @param e
	 * @return
	 * </code></pre>
	 */
	protected boolean isDeployed(DeployEntity e) {
		return e.getDeployer() != null &&
			e.getDeployer().equals(getName());
	}
	
	/**
	 * bind a JMX demo service to JNDI.
	 *  java:/demos/{group name}/{demo name}
	 * @param name
	 * @param demoService
	 * </code></pre>
	 * @throws NamingException 
	 */
	protected void bindDemoToJNDI(ObjectName demoService, DeployEntity entity) throws NamingException{
		//String name = 
		//public String JNDI_DEMO_PREFIXS  = "java:/demos/";
		//log.debug("Binding demo server on JNDI, name:" + genJNDIName(demoService));
		ctx.bind(genJNDIName(demoService), entity.getService());
		log.debug("Binded demo on JNDI, name:" + genJNDIName(demoService));
	}
	
	protected void unbindJNDI(ObjectName demoService) throws NamingException{
		
		ctx.unbind(genJNDIName(demoService));
		log.debug("UnBinded demo on JNDI, name:" + genJNDIName(demoService));
		//ctx.bind(genJNDIName(demoService), new BasicDemoService());
	}
	
	private String genJNDIName(ObjectName demoService) {
		return String.format(ServerConfig.JNDI_DEMOS_PREFIX+"/%1$s/%2$s",
				demoService.getKeyProperty(NAME_KEY_GROUP),
				demoService.getKeyProperty(NAME_KEY_NAME)
				);
	}
	
	protected String lookupGroup(String f) {
		String groupName = null;
		try{
			if(groupManager != null){
				groupName = (String) context.invoke(groupManager, GroupServiceMBean.OP_LOOKUP,
						new Object[]{},
						new String[]{});
			}
		}catch(Exception e) {
			groupName = f;
			log.warn("LookupGroup error:" + e.getMessage());
		}
		return groupName;
	}
	
	protected String lookupGroup(File f) {
		String groupName = null;
		try{
			if(groupManager != null){
				groupName = (String) context.invoke(groupManager, GroupServiceMBean.OP_LOOKUP,
						new Object[]{f},
						new String[]{File.class.getName()});
			}
		}catch(Exception e) {
			groupName = f.getParentFile().getName();
			log.warn("LookupGroup error:" + e.getMessage());
		}
		return groupName;
	}	
	
	protected abstract boolean accept(DeployEntity entity);
	protected abstract void deploy(DeployEntity entity) throws DeployException;
	protected abstract void unDeploy(DeployEntity entity) throws DeployException;

}

