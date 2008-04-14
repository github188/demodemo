/* DeployerNotification.java
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
package com.sigma.demo.mbean.notification;

import java.io.File;

import javax.management.Notification;
import javax.management.NotificationFilter;

import com.sigma.demo.DemoService;
import com.sigma.demo.deploy.DeployEntity;

public class DeployerNotification extends Notification {
	
	private static final long serialVersionUID = -4656930846688077225L;
	/**
	 * find a new demo deployed
	 */
	public static final String NEW_DEPLOY = "demo.deploy.new";
	public static final String UPDATED_DEPLOY = "demo.deploy.updated";
	public static final String REMOVE_DEPLOY = "demo.deploy.remove";
	
	public static final String  PRE_DEMO_DEPLOY = "demo.deploy.pre_start";	
	public static final String  POST_DEMO_DEPLOY = "demo.deploy.post_start";

	public static final String  PRE_DEMO_START = "demo.deploy.pre_start";	
	public static final String  POST_DEMO_STARTED = "demo.deploy.post_start";
	
	public static final String  PRE_DEMO_STOP = "demo.deploy.pre_stop";	
	public static final String  POST_DEMO_STOPED = "demo.deploy.post_stop";
	
	public static final String  PRE_DEMO_UNDEPLOY = "demo.deploy.pre_undeploy";	
	public static final String  POST_DEMO_UNDEPLOYED = "demo.deploy.post_undeploy";
	
	public static final String  DEPLOY_EXCEPTION = "demo.deploy.deploy_exception";
	
	
	private DeployEntity deployEntity;
	
	public static final NotificationFilter filter =	new NotificationFilter(){
		private static final long serialVersionUID = 759438662077193940L;
		public boolean isNotificationEnabled(Notification notify) {
			return notify instanceof DeployerNotification;
		}
	};

	public DeployerNotification(String type, Object source, long sequenceNumber,
			DeployEntity deployEntity) {
		this(type, source, sequenceNumber);
		this.deployEntity = deployEntity;
		this.setUserData(deployEntity);
	}
	
	public DeployerNotification(String type, Object source, long sequenceNumber) {
		super(type, source, sequenceNumber, System.currentTimeMillis());
	}

	public File getDeployRoot() {
		return this.deployEntity.getServiceRoot();
	}

	public DemoService getService() {
		return this.deployEntity.getService();
	}

	public File getServicePath() {
		return this.deployEntity.getServiceFile();
	}

	public String toString(){
		StringBuffer buffer = new StringBuffer();
		buffer.append("DeployerNotification");
		buffer.append(String.format("[type=%1$s,source=%2$s,deployEntity=%3$s]", this.getType(), this.source, this.deployEntity));
		
		return buffer.toString();
	}
}

