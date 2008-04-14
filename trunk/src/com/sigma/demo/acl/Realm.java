/* Realm.java
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
package com.sigma.demo.acl;

import java.security.Principal;

import javax.security.auth.login.AccountException;
import javax.security.auth.login.CredentialException;

import com.sigma.demo.DemoService;
import com.sigma.demo.mbean.ServiceMBean;

public interface Realm extends ServiceMBean {
	//public String GUEST_ACCOUNT = "guest";
	
    /**
     * Return the Principal associated with the specified username and
     * credentials, if there is one; otherwise return <code>null</code>.
     * 
     * look up associated user in the JAAS Subject, if the username is null, or 
     * Return a Geust user associated with Subject.
     *
     * @param username Username of the Principal to look up. 
     * @param credentials Password or other credentials to use in
     *  authenticating this username
     * @throws AccountException 
     * @throws CredentialException 
     */
    public Principal authenticate(String username, String credentials) throws AccountException, CredentialException;
    
    /**
     * Return <code>true</code> if the specified Principal has the specified
     * security role, within the context of this Realm; otherwise return
     * <code>false</code>.
     *
     * @param principal Principal for whom the role is to be checked
     * @param role Security role to be checked
     */
    public boolean hasRole(Principal principal, String role);
    
    
    //hasResourcePermission
    public boolean hasAccessGroupPermission(Principal princial, DemoGroup group);
    
    public boolean hasAccessDemoPermission(Principal princial, DemoService demo);
    public boolean hasLockDemoPermission(Principal princial, DemoService demo);
    public boolean hasAdminDemoPermission(Principal princial, DemoService demo);
}

