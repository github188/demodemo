/* ScriptPipe.java
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
 * Created on 2008-3-14, by Alexandre Tsu.
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
package com.sigma.demo.service;

import com.sigma.demo.mbean.ServiceMBean;

/**
 * Shell is an exported interface of ShellService JMX MBean. It isn't defined operation 
 * of output/error of shell. All output of shell is apply to JMX notification asynchronously.  
 * 
 * @author Deon wu
 */
public interface Shell extends ServiceMBean{
	public String MODE_TEXT = "text";
	public String MODE_BIN = "bin";
	public String MODE_STREAM = "stream";
	//public void exec
	
	public void setEnvironment(String name, String value);
	/**
	 * Set the pipd outputstream mode.
	public void setMode(String mode);
	 */
	
	/**
	 * execute a command with arguments, the front proccess will be destroied which isn't
	 * exiting before new command executing. 
	 * @param command
	 * </code></pre>
	 */
	public void exeCommand(String[] command);
	public void exeCommand(String[] command, boolean isBlocking);
	
	/**
	 * execute a command, the comand should not include any blank.
	 * @param command
	 * </code></pre>
	 */
	public void exeCommand(String command);
	
	/**
	 * Write a string to current proccess Standard input stream. it's ignored if current 
	 * proccess has exited.
	 */
	public void write(String in);
	
	/**
	 * Write a string to current proccess Standard input stream. it's ignored if current 
	 * proccess has exited.
	 */
	public void write(byte[] in);
	
	/**
	 *
	 * @return
	 * </code></pre>
	 */
	public boolean isRunning();
	
	public boolean destory();
}

