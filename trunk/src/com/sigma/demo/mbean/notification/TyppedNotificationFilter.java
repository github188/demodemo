/* TyppedNotificationFilter.java
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
 * Created on 2008/3/7, by Alexandre Tsu.
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
package com.sigma.demo.mbean.notification;

import javax.management.Notification;
import javax.management.NotificationFilter;
import javax.management.ObjectName;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

import com.sigma.demo.cfg.ServerConfig;
import com.sigma.demo.deploy.DeployEntity;
import com.sigma.demo.mbean.ServiceMBean;

public class TyppedNotificationFilter implements NotificationFilter {
	private String type = null;
	private ServiceMBean mbean = null;	
	private static final Logger eventLogger = LogManager.getLogger(ServerConfig.LOGGER_NOTIFICATION);
	
	public TyppedNotificationFilter(String type, ServiceMBean mbean){
		this.type =  type;
		this.mbean = mbean;
	}
	
	private static final long serialVersionUID = 759438662077193940L;
	public boolean isNotificationEnabled(Notification notify) {
		boolean isAccept = true;
		String error = "";
		if(notify.getSource() instanceof ObjectName){
			ObjectName source = (ObjectName)notify.getSource();
			if(!source.getKeyProperty("type").equals(type)){
				isAccept = false;
				if(eventLogger.isDebugEnabled()){
					error += "Not an scanner.";
				}
			}
		}
		if(notify.getUserData() == null ||
		  !(notify.getUserData() instanceof  DeployEntity)
		){
			isAccept = false;
			if(eventLogger.isDebugEnabled()){
				error += "Not found DeployEntity.";
			}
		}
		if(eventLogger.isDebugEnabled()){
			eventLogger.debug(
					String.format("Filter by:%1$s, result=%2$s, message=%3$s, notify=%4$s",
							mbean.getName(),
							isAccept,
							error,
							notify));
		}
		return isAccept;
	}
}

