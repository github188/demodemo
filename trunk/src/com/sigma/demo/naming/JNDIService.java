/* JNDIService.java
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
package com.sigma.demo.naming;

import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;

import org.huihoo.jfox.jndi.*;

import com.sigma.demo.cfg.ServerConfig;
import com.sigma.demo.service.AbstractService;
import com.sigma.demo.service.FailServiceException;

/**
 * Wrapped a JFox NamingServer for service location. 
 * The JNDI Server only serve same JVM client in current.
 * 
 * 
 * 		
 * 
 * @author Deon Wu
 */
public class JNDIService extends AbstractService implements JNDIServiceMBean{

	private NamingService server = null;
	
	public void shutDown() throws FailServiceException {
		//Clean the local naming service.
		ServiceLocator.bindNamingService("localhost", 1099, null);
	}

	public void start() throws FailServiceException {
		
		server = new NamingServiceSupport();
		
		//Just put the Server object to a map, Have not create socket server.
		//then the NamingServer only be located by client of same JVM.
		ServiceLocator.bindNamingService("localhost", 1099, server);	
		try{
			Context ctx = new InitialContext();
			ctx.createSubcontext(ServerConfig.JNDI_GROUPS_PREFIX);
			ctx.createSubcontext(ServerConfig.JNDI_DEMOS_PREFIX);
		}catch(NamingException e) {
			throw new FailServiceException(e.getMessage());
		}
		
	}
	
	public void test() throws NamingException {
		Context ctx = new InitialContext();
		ctx = (Context)ctx.lookup(ServerConfig.JNDI_GROUPS_PREFIX);
		
		ctx.bind("g2/test1", "test1");
		Object obj = ctx.lookup("g2/test1");
		log.debug("jndi G2/test1:" + obj);
		
		//obj = ctx.lookup("test/g2");
		//log.debug("jndi test/g2:" + obj);
	}

}

