/* DemoServiceFactory.java
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
 * Created on 2008-3-16, by Alexandre Tsu.
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
package com.sigma.demo.deploy;

import java.util.HashMap;
import java.util.Map;

import com.sigma.demo.BasicDemoService;
import com.sigma.demo.service.AbstractService;
import com.sigma.demo.service.FailServiceException;

public class DemoConfigService extends AbstractService implements DemoConfigServiceMBean{
	
	private String domain = null;
	
	private static final String serviceClass = BasicDemoService.class.getName();
	private Map<String, String>	config = null; 

	public void shutDown() throws FailServiceException {
		config.clear();
	}

	public void start() throws FailServiceException {
		config = new HashMap<String, String>();
		for(String s : context.getParameter().getKeySet()) {
			config.put(s, context.getStringParameter(s, null));
		}
	}

	public Map<String, String> config() {
		return this.config;
	}


}

