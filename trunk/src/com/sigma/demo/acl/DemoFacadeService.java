/* DemoFacadeService.java
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

import java.security.Principal;
import java.util.Collection;

import javax.security.auth.login.AccountException;
import javax.security.auth.login.CredentialException;

import com.sigma.demo.DemoException;
import com.sigma.demo.DemoLifeCycleListener;
import com.sigma.demo.DemoService;

public interface DemoFacadeService {
	//public Collection<DemoService> listDemo();
	
	public Principal authenticate(String username, String credentials) throws AccountException, CredentialException;
	
	public Collection<DemoGroup> listDemoGroup(Principal principal, int from, int offset, String order);
	
	public DemoService lookupDemo(Principal principal, String id);
	public DemoService lookupDemo(Principal principal, DemoGroup group, String name);	
	public DemoGroup lookupDemoGroup(Principal principal, String id);
	
	public Collection<DemoService> listDemo(Principal principal, int from, int offset, String order);
	public Collection<DemoService> listDemoByGroup(Principal principal, DemoGroup group, int from, int offset, String order);
	
	
	public void addLifeCycleListener(Principal principal, DemoLifeCycleListener listener);
	public void removeLifeCycleListener(Principal principal, DemoLifeCycleListener listener);
	
	/**
	 * The lockDemo method of demo service must be called by DemoFacadeService.
	 * because of permission checking in the demo facade service. 
	 * @param principal
	 * @param demo
	 * @return
	 * @throws DemoException
	 * </code></pre>
	 */
	public boolean lockDemo(Principal principal, DemoService demo) throws DemoException ;
	
	/**
	 * The unlockDemo method of demo service must be called by DemoFacadeService.
	 * because of permission checking in the demo facade service. 
	 * @param principal
	 * @param demo
	 * @return
	 * @throws DemoException
	 * </code></pre>
	 */
	public boolean unlockDemo(DemoService demo, boolean isForce) throws DemoException ;
}

