package com.sigma.demo.mbean;

import com.sigma.demo.service.FailServiceException;
import com.sigma.demo.service.ServiceContext;

public interface ServiceMBean {
	public static final String STATE_NEW = "new";
	public static final String STATE_START = "starting";
	public static final String STATE_WAIT  = "wait";
	public static final String STATE_RUNNING = "running";
	public static final String STATE_SHUTDOWN = "shutdown";
	public static final String STATE_ERROR	 = "error";
	
	public static final String ATTR_STATE = "State";
	public static final String ATTR_ERRORMESSAGE = "ErrorMessage";
	public static final String ATTR_DEPENDS = "Depends";
	
	public static final String OP_START = "start";
	public static final String OP_SHUTDOWN = "shutDown";
	public static final String OP_CREATED = "createService";
	
	
	//public BaseService getServiceInstance();
	
	public String getName();
	public String getDepends();
	
	public String getState();
	public void setState(String state);
	
	public int getErrorCode();
	public void setErrorCode(int error);
	
	public String getErrorMessage();
	public void setErrorMessage(String errorMessage);

	public void createService(String name, ServiceContext context) throws FailServiceException;
	public void shutDown() throws FailServiceException;
	public void start() throws FailServiceException;	
}
