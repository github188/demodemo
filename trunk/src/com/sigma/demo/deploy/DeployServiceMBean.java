package com.sigma.demo.deploy;

import com.sigma.demo.mbean.ServiceMBean;

public interface DeployServiceMBean extends ServiceMBean{
	public String getURLs();
	public void addURL(String url);
	//public void removeURL(String url);
	
	public int getScanPeriod();
	public void setScanPeriod(int period);
	
	public boolean getScanEnabled();
	public void setScanEnabled(boolean enabled);
	
	public void setFilter(String filter);
	public String getFilter();
	
	public int getScanTimes();
}
