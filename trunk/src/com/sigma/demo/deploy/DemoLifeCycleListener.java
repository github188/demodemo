/* DemoLifeCycleListener.java
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

import javax.management.Notification;
import javax.management.NotificationListener;
import javax.management.ObjectName;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

import com.sigma.demo.cfg.ServerConfig;
import com.sigma.demo.mbean.ServiceMBean;

public abstract class DemoLifeCycleListener implements NotificationListener{
	protected Logger eventLogger = null; //LogManager.getLogger(SocketListener.class);
	protected ServiceMBean mbean = null;
	
	public DemoLifeCycleListener(ServiceMBean mbean){
		eventLogger = LogManager.getLogger(ServerConfig.LOGGER_NOTIFICATION);
		this.mbean = mbean;
	}
	
	public void handleNotification(Notification notify, Object arg1) {
		String type = notify.getType();
		if(eventLogger.isDebugEnabled()){
			eventLogger.debug("Handler Name:" + mbean.getName() + ",Notification:" + notify);
		}
		DeployEntity entity = (DeployEntity)notify.getUserData();		
		handleDeployEvent(type, (ObjectName)notify.getSource(), entity);
		if(eventLogger.isDebugEnabled()){
			eventLogger.debug("end handleNotification...");
		}
	}
	
	protected abstract void handleDeployEvent(String type, ObjectName deployer, DeployEntity entity);
}

