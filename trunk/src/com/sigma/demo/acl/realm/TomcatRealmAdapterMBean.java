/* TomcatRealmAdapterMBean.java
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

import com.sigma.demo.acl.Realm;

public interface TomcatRealmAdapterMBean extends Realm {

    /**
     * Create and return a new {@link Group} defined in this user database.
     *
     * @param groupname The group name of the new group (must be unique)
     * @param description The description of this group
     */
    public void createGroup(String groupname, String description);


    /**
     * Create and return a new {@link Role} defined in this user database.
     *
     * @param rolename The role name of the new role (must be unique)
     * @param description The description of this role
     */
    public void createRole(String rolename, String description);


    /**
     * Create and return a new {@link User} defined in this user database.
     *
     * @param username The logon username of the new user (must be unique)
     * @param password The logon password of the new user
     * @param fullName The full name of the new user
     */
    public void createUser(String username, String password,
                           String fullName);


    public String findGroup(String groupname);


    public String findRole(String rolename);


    public String findUser(String username);


    /**
     * Remove the specified {@link Group} from this user database.
     *
     * @param group The group to be removed
     */
    public boolean removeGroup(String group);


    /**
     * Remove the specified {@link Role} from this user database.
     *
     * @param role The role to be removed
     */
    public boolean removeRole(String role);


    /**
     * Remove the specified {@link User} from this user database.
     *
     * @param user The user to be removed
     */
    public boolean removeUser(String user);


    /**
     * Save any updated information to the persistent storage location for
     * this user database.
     *
     * @exception Exception if any exception is thrown during saving
     */
    public void save() throws Exception;	
    
    public void addUserRoleGroup(String user, String role, String group) throws Exception;
    public void removeUserRoleGroup(String user, String role, String group) throws Exception;    
}

