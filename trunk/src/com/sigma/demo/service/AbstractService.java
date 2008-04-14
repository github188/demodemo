package com.sigma.demo.service;

import javax.management.NotificationBroadcasterSupport;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

import com.sigma.demo.mbean.ServiceMBean;

public abstract class AbstractService extends NotificationBroadcasterSupport  
		implements ServiceMBean {
	protected Logger log = null; //LogManager.getLogger(SocketListener.class);
	private String name = null;
	//private
	protected ServiceContext context;
	private String state = null;
	private int error = 0;
	private String errorMessage = null;
	
	public AbstractService(){
		log = LogManager.getLogger(this.getClass());
	}

	public void createService(String name, ServiceContext context) throws FailServiceException{
		//this.setState(STATE_NEW);
		this.name = name;
		this.context = context;
	}

	public String[] depends() {
		return context.dependOn();
	}

	public String getName() {
		return name;
	}
	
	public String getState(){
		return state;
	}
	public int	  getErrorCode(){
		return error;
	}
	public String getErrorMessage(){
		return errorMessage;
	}

	public void setState(String state){
		this.state = state;
	}

	public void setErrorCode(int error){
		this.error = error;
	}

	public void setErrorMessage(String errorMessage){
		this.errorMessage = errorMessage;
	}
	
	public String getDepends(){
		StringBuffer buffer = new StringBuffer();
		if(context.dependOn() != null){
			for(String s : context.dependOn()){
				buffer.append(s + ";");
			}
			if(buffer.length() > 2){
				buffer.delete(buffer.length() -1, buffer.length());
			}
		}
		
		return buffer.toString();
	}
	
	public Logger getLogger(String name){
		return LogManager.getLogger(name);
	}
}
