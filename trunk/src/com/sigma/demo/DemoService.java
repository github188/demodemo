package com.sigma.demo;

import java.security.Principal;
import java.util.Date;

import com.sigma.demo.acl.DemoGroup;
import com.sigma.demo.data.DemoData;
import com.sigma.demo.mbean.ServiceMBean;

public interface DemoService extends ServiceMBean{
	
	public void initDemoContext(DemoData context);

	/**
	 * The lockDemo method of demo service must be called by DemoFacadeService.
	 * because of permission checking in the demo facade service. 
	 * @param principal
	 * @param demo
	 * @return
	 * @throws DemoException
	 * </code></pre>
	 */	
	public boolean lockDemo(Principal user) throws DemoException;
	/**
	 * The unlockDemo method of demo service must be called by DemoFacadeService.
	 * because of permission checking in the demo facade service. 
	 * @param principal
	 * @param demo
	 * @return
	 * @throws DemoException
	 * </code></pre>
	 */	
	//occur error on JMX proxy, when the mothed overwriting.
	//public boolean unlockDemo() throws DemoException;
	
	/**
	 *
	 * @param isForce force to unlock the demo. 
	 * @return
	 * @throws DemoException
	 * </code></pre>
	 */
	public boolean unlockDemo(boolean isForce) throws DemoException;

	/**
	 * The startDemo method must be called by the locked user, and the demo service get 
	 * current principal by JAAS subject. then the demo server need work with Java Security
	 * Manager.
	 * @param principal
	 * @param demo
	 * @return
	 * @throws DemoException
	 * </code></pre>
	 */		
	public void startDemo() throws DemoException;
	
	/**
	 * The stopDemo method must be called by the locked user, and the demo service get 
	 * current principal by JAAS subject. then the demo server need work with Java Security
	 * Manager.
	 * @param principal
	 * @param demo
	 * @return
	 * @throws DemoException
	 * </code></pre>
	 */	
	public void stopDemo() throws DemoException;

	public void startDemoAction(String actionName) throws DemoException;

	public void stopDemoAction(String actionName) throws DemoException;

	public void addListener(DemoListener l) throws DemoException;

	public void removeListener(DemoListener l) throws DemoException;

	public void runTimeListener(DemoListener l) throws DemoException;

	public DemoState state();
	
	/**
	 * It's only one schedule in demo service, It's ignored the schedule which the time is after
	 * current time, otherwise instead it. 
	 * @param principal
	 * @param time 
	 * @throws DemoException
	 * </code></pre>
	 */	
	public void schedule(Principal user, Date time) throws DemoException;
	public void clearSchedule() throws DemoException;

	public DemoData getDemoData();
	// public void stopDemo();
	public String getDemoName();

	//public DemoGroup getDemoGroup();
}
