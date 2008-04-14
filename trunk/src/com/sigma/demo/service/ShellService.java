/* ScriptPipeService.java
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

import static com.sigma.demo.cfg.ServerConfig.CFG_SHELL_NAME;
import static com.sigma.demo.cfg.ServerConfig.CFG_SHELL_ROOT_PATH;
import static com.sigma.demo.cfg.ServerConfig.LOGGER_NOTIFICATION;
import static com.sigma.demo.util.HelpUtil.join;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Vector;
import java.util.concurrent.atomic.AtomicBoolean;

import javax.management.Notification;

import org.apache.log4j.Logger;

import com.sigma.demo.mbean.NotificationType;


/**
 * ShellService provide a service to execute a shell command, and managed the shell
 * input/output stream. It can simply write data to executing process. and all the output
 * of command is sent by JMX notification. 
 */
public class ShellService extends ThreadPoolSupportService implements ShellServiceMBean{

	private File pwd = null;
	
	private String[] shell = new String[]{}; //"cmd.exe ";
	private String[] env = new String[]{};
	private Process process = null;
	private OutputStream out = null;
	private long notificationSequence = 0;
	private Logger eventLogger = null;
	private String currentCommand = null;
	
	//锁用于保证在发送shell.exits时，所有的shell.out已完成。
	private AtomicBoolean condition = new AtomicBoolean(true);	
	
	public void setShell(String shell){
		if(shell != null){
			this.shell = shell.split("\\s");
		}
	}
	
	public boolean setRootPath(String path){
		pwd = new File(path);
		if(pwd.exists() && pwd.isDirectory()){
			return true;			
		}else {
			return false;
		}
	}

	public void shutDown() throws FailServiceException {
		this.destory();
	}

	public void start() throws FailServiceException {
		super.start();
		
		eventLogger = this.getLogger(LOGGER_NOTIFICATION);
		String path = context.getStringParameter(CFG_SHELL_ROOT_PATH, ".");
		this.setRootPath(path);
		String name = context.getStringParameter(CFG_SHELL_NAME, "");
		this.setShell(name);
	}
	public void exeCommand(String[] command) {
		this.exeCommand(command, false);
	}
	public void exeCommand(String[] command, boolean isBlocking) {
		if(!isRunning()){
			try {
				currentCommand = join(" ", command);
				String[] tmp = new String[shell.length + command.length];
				System.arraycopy(shell, 0, tmp, 0, shell.length);
				System.arraycopy(command, 0, tmp, shell.length, command.length);
				if(log.isDebugEnabled()){
					log.debug("cmds: " + join(",", tmp));
				}
				process = Runtime.getRuntime().exec(tmp, env, pwd);
				if(isBlocking) {	//wating the shell exits.
					this.run();
				}else {
					threadPool.execute(this);
				}
			} catch (IOException e) {
				log.error("Execute shell commands error! " +
						"cmds: " + join(",", command) +
						"\npwd:" + pwd.getAbsolutePath() +
						"\n env:" + join(",", env));
				log.error(e,e);
			}
		}else {
			throw new Error("The shell service just support one command in the same time.");
		}
	}

	public void exeCommand(String command) {
		this.exeCommand(command.split("\\s+"));
	}

	public synchronized void setEnvironment(String name, String value) {
		ArrayList<String> temp = new ArrayList<String>();
		for(String s : env) {
			temp.add(s);
			temp.add(name + "=" + value);
		}
		env = temp.toArray(new String[]{});
	}

	public void write(String in) {
		write(in.getBytes());
	}

	public void write(byte[] in) {
		if(out != null) {
			try {
				out.write(in);
				out.flush();
			} catch (IOException e) {
				log.error(e, e);
			}
		}else {
			throw new Error("Not found acvtived process!");
		}
	}

	public void run() {
		if(log.isDebugEnabled()) {
			log.debug("shell proccess start. cmd:" + currentCommand);
		}
		if(this.isRunning()){
			condition.set(false);
			threadPool.execute(new ProcessPipeReader());
			out = this.process.getOutputStream();
			try {
				process.waitFor();
			} catch (InterruptedException e) {
				log.error(e, e);
			} finally {
				if(this.process != null) {
					int exitCode = this.process.exitValue();
					this.process = null;
					shellExit(exitCode);
				}
			}
		}
		if(log.isDebugEnabled()) {
			log.debug("shell proccess end. cmd:" + currentCommand);
		}
	}
	
	public boolean destory() {
		if(this.process != null) {
			this.process.destroy();
			return true;
		}else {
			return false;
		}
	}

	public boolean isRunning() {
		return this.process != null;
	}
	
	private void shellExit(int code) {
		synchronized(condition){
			if(!condition.get())
				try {
					condition.wait();
				} catch (InterruptedException e) {
				}
		}
		Notification n = new Notification(
				NotificationType.SHELL_EXITS,
				ShellService.this,
				notificationSequence++,
				System.currentTimeMillis(),
				"");
		n.setUserData(code);
		if (eventLogger.isDebugEnabled()) {
			eventLogger.debug("Start:" + n);
		}
		sendNotification(n);
	}
	
	private void setNotification(Collection<String> msgs) {
		Notification n = new Notification(
				NotificationType.SHELL_OUT,
				ShellService.this,
				notificationSequence++,
				System.currentTimeMillis(),
				"");
		n.setUserData(msgs);
		if (eventLogger.isDebugEnabled()) {
			eventLogger.debug("Start:" + n);
		}
		sendNotification(n);
	}
	
	/**
	 * A thread poll to read the proccess output/error message.
	 * 
	 * @author DeonWu
	 */
	class ProcessPipeReader implements Runnable {
		public void run() {
			if(log.isDebugEnabled()) {
				log.debug("Start process pipe reader.");
			}
			BufferedReader[] readers = new BufferedReader[] {
					new BufferedReader(
							new InputStreamReader(process.getInputStream())),
					new BufferedReader(
							new InputStreamReader(process.getErrorStream()))
			};
			String tmp = null;
			try {
				while(isRunning()){
					for(BufferedReader reader : readers) {
						if(reader.ready()) {
							Collection<String> msgs = new Vector<String>();
							while(reader.ready()) {
								tmp = reader.readLine();
								if(tmp == null) break;
								msgs.add(tmp);
							}
							setNotification(msgs);
						}
					}
					try {
						Thread.sleep(100);
					} catch (InterruptedException e) {
					}
				}
			} catch (IOException e) {
				log.error(e,e);
			} finally{
				for(BufferedReader reader : readers){
					try {
						reader.close();
					} catch (IOException e) {}
				}
			}
			condition.set(true);
			synchronized(condition){				
				condition.notifyAll();
			}
			if(log.isDebugEnabled()) {
				log.debug("shutdown process pipe reader.");
			}
		}
	}

	public String getRootPath() {
		return (pwd != null) ? pwd.getAbsolutePath() : "null";
	}

	public String getShell() {
		return join(" ", this.shell);
	} 
	
	public String getCurrentCommand() {
		return currentCommand;
	}
	
	public void enter() {
		this.write("\n");
	}
}

