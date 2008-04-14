/* AuthenticateCommand.java
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
 * Created on 2008/3/11, by Alexandre Tsu.
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
import java.io.PrintStream;

import javax.security.auth.login.AccountException;
import javax.security.auth.login.CredentialException;

import org.json.simple.JSONObject;

import com.sigma.demo.acl.DemoFacade;

public class AuthenticateCommand extends AbstractDemoCommand {
	private DemoFacade facade = null;
	
	@SuppressWarnings("unchecked")
	public String doCommand() throws IOException {
		facade = lookupDemoFacade();
		String user = this.getStringParameter("user", null);
		String password = this.getStringParameter("password", null);
		JSONObject obj = new JSONObject();
		if(user == null) {
			if(facade.getPrincipal() != null) {
				obj.put("user", facade.getPrincipal().getName());
			}else {
				obj.put("error", "Not defined guest account in the realm.");
			}
		}else {
			try {
				facade.authenticate(user, password);
				obj.put("user", facade.getPrincipal().getName());
			} catch (AccountException e) {
				log.info(String.format("Fail to login,[user=%3$s,password=%4$s,ip=%1$s,error=%2$s], ", req.getRemoteAddr() ,e.getMessage(), user, password));
				obj.put("error", e.getMessage());
			} catch (CredentialException e) {
				log.info(String.format("Fail to login,[user=%3$s,password=%4$s,ip=%1$s,error=%2$s], ", req.getRemoteAddr() ,e.getMessage(), user, password));				
				//log.info("fail to login[ip=], message:" + e.getMessage());
				obj.put("error", e.getMessage());
			} finally {
				//login as Guest account, if occur exeception in authenticate action.
				if(facade.getPrincipal() == null) {
					try {
						facade.authenticate(null, null);
					} catch (AccountException e) {
					} catch (CredentialException e) {
					}
				}
			}
		}
		
		new PrintStream(resp.getOutputStream()).println(obj);

		return null;
	}

}

