/* ServletDemoFacade.java
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
 * Created on 2008/2/29, by Alexandre Tsu.
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
import javax.servlet.http.HttpSessionBindingEvent;
import javax.servlet.http.HttpSessionBindingListener;
import com.sigma.demo.service.ServiceContext;

public class ServletDemoFacade extends DemoFacade implements HttpSessionBindingListener{

	public ServletDemoFacade(DemoFacadeService facade, Principal principal) {
		super(facade, principal);
	}
	
	public void valueBound(HttpSessionBindingEvent evt) {
		if(log.isInfoEnabled()){
			log.info("Bound demo facade, Session Id:" + evt.getSession().getId());
		}
	}

	public void valueUnbound(HttpSessionBindingEvent evt) {
		if(log.isInfoEnabled()){
			log.info("Unbound demo facade, Session Id:" + evt.getSession().getId());
		}
		this.destory();
	}

}

