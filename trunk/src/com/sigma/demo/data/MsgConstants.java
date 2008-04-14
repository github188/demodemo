/* MsgConstants.java
 *
 * Sigma Resources & Technologies, Inc.
 * Copyright (C) 2005-2007, sigma-rt.com
 *
 * PROPRIETARY RIGHTS of Sigma Resources & Technologies are involved in
 * the subject matter of this material. All manufacturing, reproduction,
 * use, and sales rights pertaining to this subject matter are governed
 * by the license agreement. The recipient of this software implicitly
 * accepts the terms of the license.
 *
 * Created on Oct 30, 2007, by Alexandre Tsu.
 * 
 * Project:	DemoLauncher
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
package com.sigma.demo.data;

/**
 * @author James Che
 * 
 */
public interface MsgConstants {
	
	//Have not get the demo lock, or released lock, when take an action on the demo.
	public static final String NOT_LOCKED_DEMO = "msg.not.locked.demo";
	
	public static final String NOT_AUTH = "msg.not.auth";
	
	//call script error
	public static final String DEMO_SCRIPT_ERR = "msg.demo.script.err";
	
	public static final String DEMO_NO_READY = "msg.demo.no.ready";	
	public static final String LOCKED_BY_OTHER = "msg.locked.by.other";
	public static final String SCHEDULED_INTIME = "msg.scheduled.intime";
	public static final String SCHEDULE_BY_OTHER = "msg.schedule.by.other";
	
	public static final String DEMO_STOP_SCHEDULER = "msg.demo.stop.scheduler";
	
}
