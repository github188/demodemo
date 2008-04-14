/* GroupService.java
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
 * Created on 2008/3/10, by Alexandre Tsu.
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
package com.sigma.demo.acl;

import java.io.File;

import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NameNotFoundException;
import javax.naming.NamingException;

import com.sigma.demo.cfg.ServerConfig;
import com.sigma.demo.service.AbstractService;
import com.sigma.demo.service.FailServiceException;

public class GroupService extends AbstractService implements GroupServiceMBean{
	private Context ctx = null;
	
	public String lookupGroupName(File path) {
		if(log.isDebugEnabled()){
			log.debug("lookup group by file path, path:" + path.getAbsolutePath());
		}
		return lookupGroupName(path.getParentFile().getName());
	}

	public String lookupGroupName(String path) {
		if(log.isDebugEnabled()){
			log.debug("lookup group, flag:" + path);
		}
		
		DemoGroup group = null;
		try {
			try{
				group = (DemoGroup)ctx.lookup(path);
			}catch(NameNotFoundException e){
				if(log.isDebugEnabled()){
					log.debug(String.format("create a new group, name=%1$s, path=%2$s", path, path));
				}
				group = new DemoGroup(path, path);
				ctx.bind(path, group);
			}
		} catch (NamingException e) {
			log.error(e, e);
		}
		
		if(log.isDebugEnabled()){
			log.debug("lookup group, group name:" + group.getName());
		}

		return group.getName();
	}

	public void shutDown() throws FailServiceException {
		
	}

	public void start() throws FailServiceException {
		try {
			ctx = new InitialContext();
			try {
				ctx = (Context)ctx.lookup(ServerConfig.JNDI_GROUPS_PREFIX);
			}catch(NameNotFoundException e){
				ctx = ctx.createSubcontext(ServerConfig.JNDI_GROUPS_PREFIX);
			}
			
		} catch (NamingException e) {
			log.error(e, e);
			throw new FailServiceException(e.getMessage(), e.getCause());
		}		
	}

}

