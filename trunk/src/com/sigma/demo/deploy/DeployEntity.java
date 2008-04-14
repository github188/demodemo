package com.sigma.demo.deploy;

/*
 * ---------------------------------------------------------
 * $Id$
 * ---------------------------------------------------------
 */
import java.io.File;

import com.sigma.demo.DemoService;

public class DeployEntity {
	private long lastModified = 0;
	private File entity;
	private File rootPath;
	private DemoService service;
	private String serviceName;
	private String deployer;
		
	public DeployEntity(File root, File file, String deployer){
		
		this.rootPath = root;
		this.entity = file;
		this.deployer = deployer;
		this.isUpdated();
	}
	
	public DeployEntity(File root, File entity){
		this(root, entity, null);
	}
	
	public boolean isUpdated(){
		if(lastModified < entity.lastModified()){
			lastModified = entity.lastModified();
			return true;
		}
		return false; 
	}
	
	public boolean isRemoved(){
		return !this.entity.exists();
	}
	
	public String getDeployer(){
		return this.deployer;
	}
	
	public void setDeployer(String deployer){
		this.deployer = deployer;
	}
	
	public File getServiceFile(){
		return this.entity;
	}
	
	public File getServiceRoot(){
		return this.rootPath;
	}

	public DemoService getService() {
		return service;
	}

	public void setService(DemoService service) {
		this.service = service;
	}

	public String getServiceName() {
		return serviceName;
	}

	public void setServiceName(String serviceName) {
		this.serviceName = serviceName;
	}

	public String toString(){
		return String.format("[Root=%1$s,path=%2$s,serviceName=%4$s,deployer=%3$s,lastModified=%5$s]",
				this.rootPath.getAbsoluteFile(),
				this.getServiceFile().getName(),
				this.deployer,
				this.serviceName,
				this.lastModified
				);
	}
	
}
