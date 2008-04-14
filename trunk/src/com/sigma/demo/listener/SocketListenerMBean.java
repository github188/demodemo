package com.sigma.demo.listener;

import com.sigma.demo.mbean.ServiceMBean;
import com.sigma.demo.service.FailServiceException;

public interface SocketListenerMBean extends ServiceMBean {
	public void shutDown()  throws FailServiceException;
	public void start()  throws FailServiceException;
	
	public int getListenPort();
	
	public String getBindAddress();
}
