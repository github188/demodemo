package com.sigma.demo.service;

import com.sigma.demo.util.threadpool.ThreadPool;

/**
 * the abstract service need config a thread pool. and the service started in alone thread.  
 */
public abstract class ThreadPoolSupportService extends 
	AbstractService implements Runnable {
	
	private static final String THREAD_POOL = "thread_pool";
	protected ThreadPool threadPool = null;

	public void start() throws FailServiceException {
		String serviceName = context.getStringParameter(THREAD_POOL, null);
		if(serviceName == null) {
			throw new FailServiceException("Not define thread pool MBean name in attributes."); 
		}
		threadPool = (ThreadPool)context.getService(serviceName, ThreadPool.class);
		if(threadPool == null) {
			throw new FailServiceException("Can't lookup thread pool MBean, Name:" + serviceName);
		}
		
		log.info("Thread pool is supported! Service name:" + serviceName);
		threadPool.execute(this);
	}
}