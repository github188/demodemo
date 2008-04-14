/* TomcatRealmAdapter.java
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
 * Created on 2008-3-13, by Alexandre Tsu.
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
package com.sigma.demo.acl.realm;

import java.security.AccessControlContext;
import java.security.AccessController;
import java.security.Principal;
import java.util.Iterator;

import javax.management.ObjectName;
import javax.security.auth.Subject;
import javax.security.auth.login.AccountException;
import javax.security.auth.login.CredentialException;

import org.apache.catalina.Group;
import org.apache.catalina.Role;
import org.apache.catalina.User;
import org.apache.catalina.users.MemoryUserDatabase;

import com.sigma.demo.DemoService;
import com.sigma.demo.acl.DemoGroup;
import com.sigma.demo.cfg.ServerConfig;
import com.sigma.demo.service.AbstractService;
import com.sigma.demo.service.FailServiceException;
import com.sigma.demo.util.HelpUtil;

public class TomcatRealmAdapter extends AbstractService implements TomcatRealmAdapterMBean {
	private Role admin = null;
	private User guest = null;
	//private Role demoUser = null;
	
	MemoryUserDatabase db = null;//new MemoryUserDatabase(name.toString());	

	public Principal authenticate(String username, String credentials) throws AccountException, CredentialException {
		User user = null;
		if(username != null) {		
			user = db.findUser(username);
			if(user == null){
				throw new AccountException("Not found Account"); 
			}else if(!HelpUtil.eq(user.getPassword(), credentials)){
				throw new CredentialException("Invalied credentials");
			}
		}else {	//look up user in JAAS Subject, or return Guest user;
			AccessControlContext context = AccessController.getContext();
			Subject subject = Subject.getSubject(context);
			if(subject != null) {
				for(Principal prin : subject.getPrincipals()) {
					if(prin instanceof User) {
						user = (User)prin;
						break;
					}
				}
			}
			user = guest;
		}
		
		return user;
	}

	public boolean hasAccessDemoPermission(Principal princial, DemoService demo) {
		return demoGroupCheck(princial, demo);
	}

	public boolean hasAccessGroupPermission(Principal princial, DemoGroup group) {
		if(!(princial instanceof User)) {
			return false;
		} 
		
		User user = (User)princial;
		Group g = db.findGroup(group.getName());
		if(user.isInRole(admin) || g != null && user.isInGroup(g)) {
			return true;
		}
		return false;		
	}

	public boolean hasAdminDemoPermission(Principal princial, DemoService demo) {
		return demoGroupCheck(princial, demo);
	}

	public boolean hasLockDemoPermission(Principal princial, DemoService demo) {
 		return demoGroupCheck(princial, demo);
	}
	
	public boolean hasRole(Principal princial, String role) {
		if(!(princial instanceof User)) {
			return false;
		} 
		
		User user = (User)princial;
		Role _role = db.findRole(role);
		if(_role != null) {
			return user.isInRole(_role);
		}

		return false;
	}

	public void shutDown() throws FailServiceException {
		try {
			db.save();
			db.close();
		} catch (Exception e) {
			throw new FailServiceException(e.toString(), e);
		}
	}

	public void start() throws FailServiceException {
		db = new MemoryUserDatabase(getName());
		try {
			db.open();			
			admin = db.findRole("admin");
			guest = db.findUser("guest");
			if(guest == null) {
				log.warn("Not defined guest account in the realm. name:" + getName());
			}
		} catch (Exception e) {
			throw new FailServiceException(e.toString(), e);
		}
	}
	
	private boolean demoGroupCheck(Principal princial, DemoService demo) {
		
		if(!(princial instanceof User)) {
			return false;
		} 
		
		User user = (User) princial;
		ObjectName demoName = null;
		Group g = null;
		try {
			demoName = new ObjectName(demo.getName());
			g = db.findGroup(demoName.getKeyProperty(ServerConfig.NAME_KEY_GROUP));
		} catch (Exception e) {
		}
		if (admin != null && user.isInRole(admin) || g != null
				&& user.isInGroup(g)) {
			return true;
		}

		return false;
	}

	/**
	 * JMX management
	 */
	public void createGroup(String groupname, String description) {		
		db.createGroup(groupname, description);
	}

	/**
	 * JMX management
	 */	
	public void createRole(String rolename, String description) {
		db.createRole(rolename, description);
	}

	/**
	 * JMX management
	 */	
	public void createUser(String username, String password, String fullName) {
		db.createUser(username, password, fullName);
	}

	/**
	 * JMX management
	 */	
	public String findGroup(String groupname) {
		StringBuffer buf = new StringBuffer();
		Group g = db.findGroup(groupname);
		if(g != null) {
			buf.append("Group=" + g.getGroupname());
			buf.append(" desc=" + g.getDescription());
		}else {
			buf.append("Not found role " + groupname);
		}
		
		return buf.toString() ;
	}
	
	/**
	 * JMX management
	 */
	public String findRole(String rolename) {
		StringBuffer buf = new StringBuffer();
		Role r = db.findRole(rolename);
		if(r != null) {
			buf.append("Role=" + r.getRolename());
			buf.append(" desc=" + r.getDescription());
		}else {
			buf.append("Not found role " + rolename);
		}
		
		return buf.toString() ;
	}
	
	/**
	 * JMX management
	 */
	public String findUser(String username) {
		StringBuffer buf = new StringBuffer();
		User u = db.findUser(username);
		if(u != null) {
			buf.append(u.getName());
			buf.append(" fullname=" + u.getFullName());
			buf.append(" Role=");
			for(Iterator iter = u.getRoles(); iter.hasNext(); ) {
				Role r = (Role)iter.next();
				buf.append(r.getRolename());
				buf.append(",");
			}
			buf.append(" Group=");
			for(Iterator iter = u.getGroups(); iter.hasNext(); ) {
				Group g = (Group)iter.next();
				buf.append(g.getGroupname());
				buf.append(",");
			}
		} else {
			buf.append("Not found user " + username);
		}
		
		return buf.toString();
	}
	
	/**
	 * JMX management
	 */
	public boolean removeGroup(String group) {
		Group g = db.findGroup(group);
		if(g != null) {
			db.removeGroup(g);
		}
		return g != null;
	}

	/**
	 * JMX management
	 */	
	public boolean removeRole(String role) {
		Role r = db.findRole(role);
		if(r != null) {
			db.removeRole(r);
		}
		
		return r != null;
	}

	/**
	 * JMX management
	 */	
	public boolean removeUser(String user) {
		User u = db.findUser(user);
		if(u != null) {
			db.removeUser(u);
		}
		
		return u != null;
	}

	/**
	 * JMX management
	 */	
	public void save() throws Exception {
		db.save();
	}
	
	/**
	 * JMX management
	 */	
    public void addUserRoleGroup(String user, String role, String group) throws Exception {
    	User u = db.findUser(user);
    	if(u != null) {
	    	if(role != null) {
	    		Role r = db.findRole(role);
	    		if(r != null && !u.isInRole(r)) {
	    			u.addRole(r);
	    		}
	    	}
	    	if(group != null) {
	    		Group g = db.findGroup(group);
	    		if(g != null && !u.isInGroup(g)) {
	    			u.addGroup(g);
	    		}
	    	}
    	}
    }
    
	/**
	 * JMX management
	 */	
    public void removeUserRoleGroup(String user, String role, String group) throws Exception {
    	User u = db.findUser(user);
    	if(u != null) {
	    	if(role != null) {
	    		Role r = db.findRole(role);
	    		if(r != null && u.isInRole(r)) {
	    			u.removeRole(r);
	    		}
	    	}
	    	if(group != null) {
	    		Group g = db.findGroup(group);
	    		if( g != null && u.isInGroup(g)) {
	    		u.removeGroup(g);
	    		}	    		
	    	}
    	}    	
    }    


}

