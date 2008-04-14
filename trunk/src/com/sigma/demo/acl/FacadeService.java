/* FacadeService.java
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


import java.security.AccessControlContext;
import java.security.AccessControlException;
import java.security.AccessController;
import java.security.Principal;
import java.util.*;

import javax.management.InstanceNotFoundException;
import javax.management.ObjectName;
import javax.naming.*;
import javax.security.auth.Subject;
import javax.security.auth.login.AccountException;
import javax.security.auth.login.CredentialException;

import com.sigma.demo.*;
import com.sigma.demo.cfg.ServerConfig;
import com.sigma.demo.deploy.DeployEntity;
import com.sigma.demo.mbean.notification.TyppedNotificationFilter;
import com.sigma.demo.service.AbstractService;
import com.sigma.demo.service.FailServiceException;
import static com.sigma.demo.mbean.notification.DeployerNotification.*;


public class FacadeService extends AbstractService implements DemoFacadeService, FacadeServiceMBean{
	public static final String POLICY_ALL = "all";
	public static final String POLICY_NONE = "none";
	
	private Context ctx = null;
	private Collection<ListenerPair> listeners = null;
	private Collection<DemoGroup> groups = null;
	private Collection<DemoService> demos = null;
	private Map<String, Collection<DemoService>> demoByGroup = null;
	
	private String noRealmPolicy = POLICY_ALL; 
	private Realm realm = null;
	
	private Map<String,Comparator> comparator = new HashMap<String,Comparator>();
    private com.sigma.demo.deploy.DemoLifeCycleListener deployListener = null;
    
    private String jndiName = null;
	
	public DemoFacade createFacade(Principal principal){
		
		return new DemoFacade(this, principal);
	}
	
	 public Principal authenticate(String username, String credentials) throws AccountException, CredentialException{
		 Principal prin = null;
		 if(realm == null) {
			log.warn("Not located realm service, fail to authenticate!");
		 } else {
			AccessControlContext context = AccessController.getContext();
			Subject subject = Subject.getSubject(context);
			subject.getPrincipals().clear();
			prin = realm.authenticate(username, credentials);
			if(prin != null) {
				subject.getPrincipals().add(prin);
			}
		 }
		 
		 return prin;
	 }

	public void shutDown() throws FailServiceException {
		
		try {
			ctx.unbind(jndiName);
		} catch (NamingException e) {
			log.error(e, e);
		}
		
		try{
			context.removeNotificationListener(context.getServerName(), deployListener);
		}catch(Exception e){
			log.error(e, e);
		}
		
		demoByGroup.clear();
		demoByGroup = null;
		listeners.clear();
		listeners = null;
	}

	public void start() throws FailServiceException {
		
		demoByGroup = new HashMap<String, Collection<DemoService>>();
		listeners = new Vector<ListenerPair>();
		jndiName = context.getStringParameter("jndi", ServerConfig.JNDI_DEMO_FACADE);
		if(context.getStringParameter("realm", null) != null){
			realm = (Realm) context.getService(context.getStringParameter("realm", null), Realm.class);
		}
		noRealmPolicy = context.getStringParameter("defaultRealm", POLICY_NONE);
		
		try {
			deployListener = new com.sigma.demo.deploy.DemoLifeCycleListener(this) {
				protected void handleDeployEvent(String type, ObjectName deployer,
						DeployEntity entity) {
					if (type.equals(POST_DEMO_DEPLOY)) {
						fireNewDemoEvent(entity);
					} else if (type.equals(POST_DEMO_UNDEPLOYED)) {
						fireDestoryDemoEvent(entity);
					}
				}
			};			
			context.addNotificationListener(context.getServerName(),
					deployListener, new TyppedNotificationFilter("deployer", this),
					null);
		} catch (InstanceNotFoundException e) {
			log.warn(e, e);
		}

		try {
			ctx = new InitialContext();
			ctx.bind(jndiName, this);
		} catch (NamingException e) {			
			log.error(e, e);
		}		
	}
	
	protected Collection<DemoService> sortDemo(String order, Collection<DemoService> list){
		return list;
	}
	
	protected Collection<DemoGroup> sortDemoGroup(String order, Collection<DemoGroup> list){
		return list;
	}

	@SuppressWarnings("unchecked")
	public Collection<DemoService> listDemo(Principal principal, int from, int offset, String order) {
		if(demos == null) {
			demos = (List<DemoService>) listBinding(ServerConfig.JNDI_DEMOS_PREFIX, ctx, DemoService.class);			
		}
		
		demos = sortDemo(order, demos);
		
		return pagedDemoList(principal, demos, from, offset);
	}

	@SuppressWarnings("unchecked")
	public Collection<DemoService> listDemoByGroup(Principal principal, DemoGroup group, int from, int offset, String order) {
		
		if(group == null) {
			return Collections.EMPTY_LIST;
		}
		Collection<DemoService> demoList = demoByGroup.get(group.getName()); //new Vector<DemoService>();		
		if(demoList == null) {
			demoList = (List<DemoService>) listBinding(ServerConfig.JNDI_DEMOS_PREFIX + "/"
						+ group.getName(),
						ctx,
						DemoService.class);
			//demoList
			demoByGroup.put(group.getName(), demoList);
		}
		
		demoList = sortDemo(order, demoList);
		
		
		return pagedDemoList(principal, demoList, from, offset);
	}

	@SuppressWarnings("unchecked")
	public Collection<DemoGroup> listDemoGroup(Principal principal, int from, int offset, String order) {
		if(groups == null) {
			groups = (List<DemoGroup>) listBinding(ServerConfig.JNDI_GROUPS_PREFIX, ctx, DemoGroup.class);			
		}
		
		groups = sortDemoGroup(order, groups);
		
		return pagedDemoGroupList(principal, groups, from, offset);
	}

	public DemoService lookupDemo(Principal principal, String id) {
		DemoService demo = null;
		try {
			demo = (DemoService) ctx.lookup(ServerConfig.JNDI_DEMOS_PREFIX + "/" + id);			
			if((realm == null && noRealmPolicy.equals(POLICY_NONE)) ||
			    (realm != null && !realm.hasAccessDemoPermission(principal, demo))){
				demo = null;
			}
			
		} catch(NameNotFoundException e){
			log.warn("NameNotFoundException," + e.getMessage());
		} catch (NamingException e) {
			log.error(e, e);			
		}
		return demo;
	}

	public DemoService lookupDemo(Principal principal, DemoGroup group, String name) {
		DemoService demo = null;
		try {
			demo = (DemoService) ctx.lookup(ServerConfig.JNDI_DEMOS_PREFIX + "/"
					+ group.getName() + "/" + name);
			if((realm == null && noRealmPolicy.equals(POLICY_NONE)) ||
				    (realm != null && !realm.hasAccessDemoPermission(principal, demo))){
					demo = null;
			}
			
		} catch(NameNotFoundException e) {
			log.warn("NameNotFoundException," + e.getMessage());
		} catch (NamingException e) {
			log.error(e, e);
		}
		return demo;
	}
	
	public DemoGroup lookupDemoGroup(Principal principal, String id) {
		DemoGroup group = null;
		if(log.isDebugEnabled()){
			log.debug("lookDemoGroup ID:" + id);
		}
		try {
			group = (DemoGroup) ctx.lookup(ServerConfig.JNDI_GROUPS_PREFIX + "/"
					+ id);
			
			if((realm == null && noRealmPolicy.equals(POLICY_NONE)) ||
				(realm != null && !realm.hasAccessGroupPermission(principal, group))){
				group = null;
			}
			
		} catch(NameNotFoundException e) {
			log.warn("NameNotFoundException," + e.getMessage());
		} catch (NamingException e) {
			log.error(e, e);
		}
		return group;
	}

	public void removeLifeCycleListener(Principal principal, DemoLifeCycleListener listener) {
		//listeners.remove(listener);
		for(Iterator<ListenerPair> iter = listeners.iterator(); iter.hasNext();) {
			ListenerPair pair = iter.next();
			if(pair.l == listener 
			/*
			 *  Not must check the principal, the Object's hashCode is unique, and the principal can
			 *  be changed by authentication.
			 */		
			//		&& pair.p.equals(principal)  
			){
				iter.remove();
			}
		}
	}
	
	public void addLifeCycleListener(Principal principal, DemoLifeCycleListener listener) {
		listeners.add(new ListenerPair(principal, listener));
	}
	
	public boolean lockDemo(Principal principal, DemoService demo) throws DemoException {
		if((realm == null && noRealmPolicy.equals(POLICY_NONE)) ||
			    (realm != null && !realm.hasLockDemoPermission(principal, demo))){
				throw new AccessControlException("no lock demo permission.");
			}
		/*
		// check schecdule also.
		DemoState state = demo.state();
		if(state.isSchedule()) {
			
		}
		*/
		
		demo.lockDemo(principal);
		return true;
	}

	public boolean unlockDemo(DemoService demo, boolean isForce) throws DemoException {
		/*
		if((realm == null && noRealmPolicy.equals(POLICY_NONE)) ||
			    (realm != null && !realm.hasLockDemoPermission(null, demo))){
				throw new AccessControlException("no unlock demo permission.");
			}
		*/
		
		demo.unlockDemo(isForce);
		return true;
	}	

	private Collection<DemoService> pagedDemoList(Principal principal, Collection<DemoService> demoList, int from, int offset) {
		Collection<DemoService> result = new Vector<DemoService>();		
		if(realm != null){
			int index = 0;
			for(DemoService d : demoList) {
				if(realm.hasAccessDemoPermission(principal, d)){
					index++;
					if(index > from + offset) {
						break;
					}else if(index > from){
						result.add(d);
					}
				}
			}
			if(log.isDebugEnabled()){
				log.debug("look up demo, result count=" + result.size());
			}		
		}else if(noRealmPolicy.equals(POLICY_ALL)){
			log.warn("Not located realm system, The user allow to access all resource!");
			result = demoList;
		}else {
			log.warn("Not located realm system, The user can't access any resource!");
		}
		
		return result;
	}
	
	private Collection<DemoGroup> pagedDemoGroupList(Principal principal, Collection<DemoGroup> groupList, int from, int offset) {
		Collection<DemoGroup> result = new Vector<DemoGroup>();		
		if(realm != null){
			int index = 0;
			for(DemoGroup g : groupList) {
				if(realm.hasAccessGroupPermission(principal, g)){
					index++;
					if(index > from + offset) {
						break;
					}else if(index > from){
						result.add(g);
					}
				}
			}
			if(log.isDebugEnabled()){
				log.debug("look up demo, result count=" + result.size());
			}		
		}else if(noRealmPolicy.equals(POLICY_ALL)){
			log.warn("Not located realm system, The user allow to access all resource!");
			result = groupList;
		}else {
			log.warn("Not located realm system, The user can't access any resource!");
		}
		return result;
	}	
	
	
	@SuppressWarnings("unchecked")
	private Collection<?> listBinding(String name, Context ctx, Class clazz) {
		
		Collection result = new Vector();
		
		try {
			for(Enumeration<Binding> enumeration = ctx.listBindings(name); enumeration.hasMoreElements();){
				Binding b = enumeration.nextElement();
				if(log.isDebugEnabled()) {
					log.debug("List binding, name:" + b.getName() + ", object:" + b.getObject());
				}
				if(b.getObject() instanceof Context) {
					result.addAll(listBinding(b.getName(), (Context)b.getObject(), clazz));
				}else if(clazz.isAssignableFrom(b.getObject().getClass())){
					result.add(b.getObject());
				}
			}
		} catch (NamingException e) {
			log.error(e, e);
		}
		
		return result;
	}
	
	//public void newService(DemoService demo);DeployEntity
	
	private void fireNewDemoEvent(DeployEntity entity) {
		for(ListenerPair p : listeners) {
			if(realm == null || realm.hasAccessDemoPermission(p.p, entity.getService())) {
				p.l.newService(entity.getService());
			}
		}
	}
	
	private void fireDestoryDemoEvent(DeployEntity entity) {
		for(ListenerPair p : listeners) {
			if(realm == null || realm.hasAccessDemoPermission(p.p, entity.getService())) {
				p.l.destoryService(entity.getService());
			}
		}
	}
	
	private void fireChangedStateEvent(DeployEntity entity) {
		
	}
	
	class ListenerPair {
		public Principal p;
		public DemoLifeCycleListener l;
		ListenerPair(Principal p1, DemoLifeCycleListener l1){
			p = p1;
			l = l1;
		}
	}

	/**
	 * JMX management
	 */	
	public void clearAllCache() {		
		groups = null;
		demos = null;
		demoByGroup.clear();
	}

	/**
	 * JMX management
	 */	
	public String getJndiName() {	
		return this.jndiName;
	}
	
	/**
	 * JMX management
	 */
	public String getNoRealmPolicy() {
		return this.noRealmPolicy;
	}

	/**
	 * JMX management
	 */	
	public String getRealmName() {
		return (realm != null) ? realm.getName() : "No Realm";
	}
	
	/**
	 * JMX management
	 */
	public void setRealmName(String realm){
		String oldName = getRealmName();
		Realm temp = (Realm)context.getService(realm, Realm.class); 
		if(temp != null){
			this.realm = temp;
			if(log.isDebugEnabled()){
				log.debug("FacadeService Realm changed, old=" + oldName + ", new:" + getRealmName());
			}
		}
	}

	/**
	 * JMX management
	 */
	public void setJndiName(String jndi) {
		String oldName = this.jndiName;
		try {
			if (this.jndiName != null) {
				ctx.unbind(jndiName);
			}
			ctx.bind(jndi, this);
			this.jndiName = jndi;
			if(log.isDebugEnabled()){
				log.debug("FacadeService JNDI changed, oldName=" + oldName + ", newName:" + jndiName);
			}
		} catch (NamingException e) {
			log.error(e, e);
		}
	}

	/**
	 * JMX management
	 */
	public void setNoRealmPolicy(String policy) {
		String oldName = this.noRealmPolicy;
		if(policy != null &&
		   (policy.equals(POLICY_ALL) ||
				   policy.equals(POLICY_NONE))){
			this.noRealmPolicy = policy;
			if(log.isDebugEnabled()){
				log.debug("FacadeService NoRealmPolicy changed, old=" + oldName + ", new:" + noRealmPolicy);
			}
		}
	}

}

