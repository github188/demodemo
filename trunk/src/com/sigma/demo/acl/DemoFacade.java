/* DemoManager.java
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
 * Created on 2008/2/28, by Alexandre Tsu.
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
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import javax.security.auth.login.AccountException;
import javax.security.auth.login.CredentialException;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

import com.sigma.demo.DemoException;
import com.sigma.demo.DemoLifeCycleListener;
import com.sigma.demo.DemoService;

/**
 * A Demofacade managed demo services of accessable by user/subject.   	
 *  
 * should call destory to release the resource, otherwise never release by GC.
 * @author Deon Wu
 */
public class DemoFacade {
	protected static final Logger log = LogManager.getLogger(DemoFacade.class);
	private DemoFacadeService facade = null;
	private Principal principal = null;
	private Map<String, DemoGroup> cache = new HashMap<String, DemoGroup>();
	private Collection<DemoLifeCycleListener> listeners = new Vector<DemoLifeCycleListener>();
	
	/**
	 * @param context - A ServiceContext of the demo server, for looking up service of demo server. 
	 * @param principal - A demo user for facade, looking up UserPrincipal if principal is null.  
	 */
	public DemoFacade(DemoFacadeService facade, Principal principal) {
		 if(log.isDebugEnabled()) {
			 log.debug("new  DemoFacade, service:" + facade + ", principal:" + principal);
		 }

		this.facade = facade;
		this.principal = principal;
		if(this.principal == null) {
			try {
				authenticate(null, null);
			} catch (AccountException e) {
			} catch (CredentialException e) {
			}
		}		
	}
	
	public boolean authenticate(String username, String credentials) throws AccountException, CredentialException{
		 principal = null;
		 
		 if(log.isInfoEnabled()) {
			 log.info("Authentication username:" + username + ", passoword:" + credentials);
		 }

		 principal = facade.authenticate(username, credentials);
		 
		 return principal != null;
	}
	 
	public Principal getPrincipal() {
		return this.principal;
	} 
	
	public Collection<DemoGroup> listDemoGroup(int from, int offset,
			String order) {
		return facade.listDemoGroup(principal, from, offset, order);
	}

	public DemoService lookupDemo(String id) {
		return facade.lookupDemo(principal, id);
	}

	public DemoService lookupDemo(DemoGroup group,
			String name) {
		return facade.lookupDemo(principal, group, name);
	}
	
	public boolean lockDemo(DemoService demo) throws DemoException{
		return this.facade.lockDemo(principal, demo);
	}
	
	public boolean unlockDemo(DemoService demo, boolean isForce) throws DemoException{
		return this.facade.unlockDemo(demo, isForce);
	}

	public Collection<DemoService> listDemo(int from,
			int offset, String order) {
		return facade.listDemo(principal, from, offset, order);
	}
	
	public Collection<DemoService> listDemoByGroup(String name, int from, int offset, String order) {
		//return facade.listDemoByGroup(principal, group, from, offset, order);
		if(!cache.containsKey(name)){
			cache.put(name, facade.lookupDemoGroup(principal, name));
		}
		return listDemoByGroup(cache.get(name), from, offset, order);
	}

	public Collection<DemoService> listDemoByGroup(DemoGroup group, int from, int offset, String order) {
		return facade.listDemoByGroup(principal, group, from, offset, order);
	}

	public void addLifeCycleListener(DemoLifeCycleListener listener) {
		facade.addLifeCycleListener(principal, listener);
		listeners.add(listener);
	}

	public void removeLifeCycleListener(DemoLifeCycleListener listener) {
		facade.removeLifeCycleListener(principal, listener);
		listeners.remove(listener);
	}

	public void destory() {
		log.debug("Destory the demo facade, principal=" + principal.getName());
		
		cache.clear();
		for(DemoLifeCycleListener l : listeners) {
			facade.removeLifeCycleListener(principal, l);
		}
		listeners.clear();
		
		principal = null;
	}

}

