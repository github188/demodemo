package com.sigma.tomcat;

import com.sigma.demo.mbean.ServiceMBean;

public interface Tomcat6ServiceMBean extends ServiceMBean{
	
	public int getListenPort();
	public String getListenAddress();
}
