package com.sigma.demo.util.threadpool;

import com.sigma.demo.mbean.ServiceMBean;

public interface BasicThreadPoolMBean extends ThreadPool{
	
	public int getCurrentSize();
	public long getCompletedTaskCount();
	public int getLargestPoolSize();
	
	public void setMinimumPoolSize(int corePoolSize);
	public int getMinimumPoolSize();

	public void setMaximumPoolSize(int maximumPoolSize);
	public int getMaximumPoolSize();
	
	/**
	 * @param time thead keep alive miliseconds.  
	 */
	public void setKeepAliveTime(long time);
	public long getKeepAliveTime();
	
	/**
	 * The behavior of the pool when a task is added and the queue is full.
     * abort - a RuntimeException is thrown
     * run - the calling thread executes the task
     * discard - the task is silently discarded without being run
     * discardOldest - check to see if a task is about to complete and enque
     * the new task if possible, else run the task in the calling thread
	 */
	public String getBlockingMode(); 
	public void setBlockingMode(String mode);
	
	public long getTaskCount();
	public int  getActiveCount();
	
	public String getQueuePolicy();
}
