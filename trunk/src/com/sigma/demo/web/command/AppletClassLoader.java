/* AppletClassLoader.java
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
 * Created on 2008-3-19, by Alexandre Tsu.
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
import java.io.InputStream;
import java.io.OutputStream;

import javax.servlet.http.HttpServletRequest;

import com.sigma.web.AbstractCommand;

public class AppletClassLoader extends AbstractCommand {
	private static final String CLASS = ".class";
	private byte[] buffer = new byte[64 * 1024];

	public String doCommand() throws IOException {
		HttpServletRequest httpReq = (HttpServletRequest )req;
		InputStream in = null;

		String uri = httpReq.getRequestURI();
		String servlet = httpReq.getServletPath();
		log.debug("uri:" + uri);
		log.debug("servlet:" + servlet);
		if(uri.endsWith(CLASS)) {
			String className = uri.substring(servlet.length() + 1);
			log.debug("className:" + className);
			in = Thread.currentThread().getContextClassLoader().getResourceAsStream(className);
			if(in != null) {
				resp.setContentType("application/octet-stream");
				int len = buffer.length;
				OutputStream out = resp.getOutputStream();
				while(len == buffer.length) {
					len = in.read(buffer);
					out.write(buffer, 0, len);
				}
			} else {
				log.warn("not found class:" + className);
			}
		}

		return null;
	}

}

