/* ServerConfig.java
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
 * Created on 2008/3/3, by Alexandre Tsu.
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
package com.sigma.demo.cfg;

public interface ServerConfig {
	
	public String DEFAULT_DOMAIN_DEMO = "com.sigma.demos";
	public String DEFAULT_DOMAIN_GROUP = "com.sigma.demos.group";
	public String DEFAULT_DOMAIN_SERVICE = "com.sigma.service";
	
	public String DEFAULT_DEPLOY_SERVICE = "com.sigma.deployer";
	public String DEFAULT_REALM_SERVICE = "com.sigma.realm";
	
	public String NAME_KEY_TYPE = "type";
	public String NAME_KEY_GROUP = "group";
	public String NAME_KEY_NAME = "name";
	
	public String NAME_TYPE_GROUP = "group";
	public String NAME_TYPE_DEMO  = "demo";
	
	public String CFG_DOMAIN_GROUP  = "demo";
	public String CFG_DOMAIN_DEMO  = "domain";
	
	//public String CFG_DEMO_SERVICE  = "RealmService";
	public String CFG_DEPLOY_SERVICE  = "DeployService";
	public String CFG_REALM_SERVICE  = "RealmService";
	public String CFG_GROUP_MANAGER  = "GroupManager";
	public String CFG_THREAD_POOL = "thread_pool";
	public String CFG_DEMO_FACTORY  = "demo_factory";
	public String CFG_DEMO_CONFIG  = "demo_config";
	public String CFG_DEMO_SINGLE_SHELL  = "single_shell";
	//singleShellThread
	public String CFG_SNMP_SERVICE  = "snmp_service";
	public String CFG_SHELL_ROOT_PATH  = "root_path";
	public String CFG_SHELL_NAME  = "shell_name";
	
	
	/*
	 * demo service class.
	 */
	public String CFG_DEMO_SERVICE_CLASS  = "demo_service_class";
	//
	//public String TYPE_GROUP_KEY = "group";
	
	public String LOGGER_NOTIFICATION  = "notification";
	
	public String JNDI_DEMOS_PREFIX  = "java:/demos";
	public String JNDI_GROUPS_PREFIX  = "java:/groups";
	public String JNDI_DEMO_FACADE  = "java:/facade_manager";
}

