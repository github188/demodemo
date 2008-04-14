/* JMX.java
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
 * Created on 2008/3/11, by Alexandre Tsu.
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
package com.sigma.demo.util;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.HashMap;
import java.util.Map;

import javax.management.Attribute;
import javax.management.MBeanInfo;
import javax.management.MBeanOperationInfo;
import javax.management.MBeanServer;
import javax.management.ObjectName;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

public class JMX {
	private static Logger log = LogManager.getLogger(JMX.class);
	
	public static Object export(MBeanServer mbs,ObjectName o, Class clazz) {
		InvocationHandler handler = new JMXInvocation(o, mbs);
		
		return Proxy.newProxyInstance(clazz.getClassLoader(),
                new Class[] {clazz},
                handler);		
	}
	
	private static class JMXInvocation implements InvocationHandler{
		private ObjectName mbean = null;
		private MBeanServer mbs = null;
		private Map<String, String[]> singers = new HashMap<String, String[]>();
		private String clazz = null;
		
		public JMXInvocation(ObjectName object, MBeanServer mbs){
			this.mbean = object;
			this.mbs = mbs;
			try {
				MBeanInfo info = mbs.getMBeanInfo(object);
				clazz = info.getClassName();
				for(MBeanOperationInfo ops : info.getOperations()) {
					String name = ops.getName();
					String[] singer = new String[ops.getSignature().length];
					for(int i = 0; i < singer.length; i++){
						singer[i] = ops.getSignature()[i].getType();
					}
					singers.put(name, singer);
				}
			} catch(Exception e) {
				log.error(e, e);
			}
		}

		public Object invoke(Object obj, Method method, Object[] args) throws Throwable {
			String _mName = method.getName();
			Object result = null;
			try {
				if (_mName.startsWith("set")) {
					mbs.setAttribute(mbean, new Attribute(_mName.substring(3), args[0]));
				} else if (_mName.startsWith("get")) {
					result = mbs.getAttribute(mbean, _mName.substring(3));
				} else if (_mName.startsWith("is")) {
					result = mbs.getAttribute(mbean, _mName.substring(2));
				} else if (_mName.equals("toString")) {
					result = "[JMX Proxy name=" + mbean.toString() + ", class=" + clazz + "]";
				} else {
					result = mbs.invoke(mbean, _mName, args,
							(args != null) ? singers.get(_mName) : null);
				}
			} catch (Throwable e) {
				throw e.getCause();
			}
			
			return result;
		}
	}
}

