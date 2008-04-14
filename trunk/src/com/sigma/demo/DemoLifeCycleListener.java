/* DemoStateListener.java
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

public interface DemoLifeCycleListener {
	
	/**
	 * <pre><code>
	 *	notify when a demo state changed. 
	 * @param demo - the changed demo.
	 * @param old  - the old state
	 * @param newState - the new state
	 * </code></pre>
	 */
	public void updateState(DemoService demo, DemoState old, DemoState newState);
	
	/**
	 * <pre><code>
	 *
	 * notify when a new demo deployed.
	 * @param demo
	 * </code></pre>
	 */
	public void newService(DemoService demo);
	
	/**
	 * <pre><code>
	 *
	 * notify when a demo remove.
	 * @param demo
	 * </code></pre>
	 */
	public void destoryService(DemoService demo);
	
}

