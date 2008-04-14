/* DemoState.java
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
 * Created on 2008/2/28, by Alexandre Tsu.
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
package com.sigma.demo;

import java.security.Principal;
import java.util.Date;

public interface DemoState {
	public static final String IDLE = "idle";
	public static final String LOCKED = "locked";
	public static final String RUNNING = "running";
	public static final String SCHEDULE = "scheduled";
	public static final String STOPED = "stoped";
	
	public String activeAction();
	
	public Principal locked();
	public String name();
	public Date activeTime();
	public Principal scheduler();
	public Date scheduleTime();
	
	public boolean isSchedule();
	public boolean isLocked();
	public boolean isRunning();
	public boolean isIdel();
}

