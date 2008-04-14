/* SNMPWatcher.java
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
 * Created on 2008-3-14, by Alexandre Tsu.
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
package com.sigma.demo.service;

import com.sigma.demo.mbean.ServiceMBean;

/**
 * SNMPWatcher is an exported interface of SNMPWatcherService JMX MBean.
 *  
 * @author Deon wu
 */
public interface SNMPWatcher extends ServiceMBean{
	
	/**
	 * add new watching OID.
	 * @param name
	 * @param oid
	 * @param ip
	 * @param community
	 * @param poll
	 * @param type
	 * </code></pre>
	 */
	public void addWatcher(String name, String oid, String ip, String community, int poll, int type);
	public void removeWatcher(String name);
	public void updateWatcher(String name, String oid, String ip, String community, int poll, int type);
}

