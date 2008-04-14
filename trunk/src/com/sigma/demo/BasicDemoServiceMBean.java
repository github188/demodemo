package com.sigma.demo;

import com.sigma.demo.deploy.DeployEntity;
import com.sigma.demo.mbean.ServiceMBean;

public interface BasicDemoServiceMBean extends DemoService{
	public static String ATTR_DEPLOY_INFO = "DeployEntity";
	
	public DeployEntity getDeployEntity();
	public void setDeployEntity(DeployEntity entity);
	
	public String getServiceFile();
	public String getDeployer();
	
	public String getDemoState();
	public String getActiveAction();
	public String getSchedule();
}
