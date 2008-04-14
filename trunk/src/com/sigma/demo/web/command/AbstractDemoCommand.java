/* AbstractDemoCommand.java
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
package com.sigma.demo.web.command;

import java.io.IOException;
import java.security.Principal;

import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;

import com.sigma.demo.acl.DemoFacade;
import com.sigma.demo.acl.DemoFacadeService;
import com.sigma.demo.acl.ServletDemoFacade;
import com.sigma.demo.acl.security.auth.UserPrincipal;
import com.sigma.demo.cfg.ServerConfig;
import com.sigma.demo.web.SiteLoader;
import com.sigma.web.AbstractCommand;

public abstract class AbstractDemoCommand extends AbstractCommand {

	protected DemoFacade lookupDemoFacade(){
		//this.get
		DemoFacade facade = (DemoFacade)req.getSession(true).getAttribute(SiteLoader.DEMO_FACADE);
		if(facade == null){
			try {
				Context ctx = new InitialContext();
				DemoFacadeService service = (DemoFacadeService)ctx.lookup(ServerConfig.JNDI_DEMO_FACADE);
				facade = new ServletDemoFacade(service, null);
				req.getSession(true).setAttribute(SiteLoader.DEMO_FACADE, facade);				
			} catch (NamingException e) {
				log.error(e, e);
			}			
		}
		
		return facade;
	}	
}

