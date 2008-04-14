/* FacadeServiceMBean.java
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

import com.sigma.demo.mbean.ServiceMBean;

public interface FacadeServiceMBean extends ServiceMBean{
	//noRealmPolicy
	public String getNoRealmPolicy();
	public void setNoRealmPolicy(String policy);
	
	public String getRealmName();
	public void setRealmName(String realm);
	
	public String getJndiName();
	public void setJndiName(String jndi);
	
	public void clearAllCache();

}

