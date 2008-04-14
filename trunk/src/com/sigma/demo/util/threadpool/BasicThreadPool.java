package com.sigma.demo.util.threadpool;

import java.util.concurrent.*;

import com.sigma.demo.service.*;

import static java.util.concurrent.ThreadPoolExecutor.*;
import static java.util.concurrent.TimeUnit.MILLISECONDS;

public class BasicThreadPool extends AbstractService implements BasicThreadPoolMBean{
	
	private ThreadPoolExecutor pool = null;
	private String blockMode = null;
	private String queuePolicy = null;
	
 	public void execute(Runnable task) {
		if(pool == null || pool.isShutdown()){
			log.warn("The thread pool not start or shutdown now, name:" + this.getName());
		}else {
			pool.execute(task);
		}
	}

	public void shutDown() throws FailServiceException {
		pool.shutdown();
	}

	public void start() throws FailServiceException {
		String group = context.getStringParameter("ThreadGroupName", null);
		int minimum = context.getIntParameter("MinimumPoolSize", 2);
		int aliveTime = context.getIntParameter("KeepAliveTime", 5 * 60 * 1000);
		int maximum = context.getIntParameter("MaximumPoolSize", 255);
		String blockMode = context.getStringParameter("BlockingMode", "run");
		String queuePolicy = context.getStringParameter("QueuePolicy", "none");
		
		ThreadFactory threadFactory = null;
		if(group != null){
			threadFactory = new BasicThreadFactory(group);
		}else {
			threadFactory = Executors.defaultThreadFactory();
		}
		//BlockingQueue<Runnable> queue = createQueue;
			//new ArrayBlockingQueue<Runnable>(queueSize);
		//if(qu)
		
		if(log.isInfoEnabled()){
			log.info(String.format("Start thread pool. " + 
							"[group = %1$s, min = %2$s, max = %3$s," +
							" aliveTime = %4$s, blockMode = %5$s]",
					group, minimum, maximum, aliveTime, blockMode));
		}
		
		pool = new ThreadPoolExecutor(minimum, maximum, aliveTime, MILLISECONDS,
										createQueue(queuePolicy), 
										threadFactory);
		
		this.setBlockingMode(blockMode);
		
		//pool.prestartAllCoreThreads();
	}

	public String getBlockingMode() {
		return blockMode;
	}

	public long getCompletedTaskCount() {
		return pool.getCompletedTaskCount();
	}

	public int getMinimumPoolSize() {
		return pool.getCorePoolSize();
	}

	public int getCurrentSize() {
		return pool.getPoolSize();
	}

	public long getKeepAliveTime() {
		return pool.getKeepAliveTime(MILLISECONDS);
	}

	public int getLargestPoolSize() {
		return pool.getLargestPoolSize();
	}

	public int getMaximumPoolSize() {
		return pool.getMaximumPoolSize();
	}

	/**
	 * The behavior of the pool when a task is added and the queue is full.
     * abort - a RuntimeException is thrown
     * run - the calling thread executes the task
     * discard - the task is silently discarded without being run
     * discardOldest - check to see if a task is about to complete and enque
     * the new task if possible, else run the task in the calling thread
	 */
	public void setBlockingMode(String mode) {
		if(mode != null){
			this.blockMode = mode;
			if(mode.equals("abort")){
				pool.setRejectedExecutionHandler(new AbortPolicy());
			}else if(mode.equals("run")){
				pool.setRejectedExecutionHandler(new CallerRunsPolicy());
			}else if(mode.equals("discard")){
				pool.setRejectedExecutionHandler(new DiscardPolicy());
			}else if(mode.equals("discardOldest")){
				pool.setRejectedExecutionHandler(new DiscardOldestPolicy());
			}else {
				this.setErrorMessage("Not support block mode:" + mode);
			}
		}
	}

	public void setMinimumPoolSize(int corePoolSize) {
		pool.setCorePoolSize(corePoolSize);
	}

	public void setKeepAliveTime(long time) {
		pool.setKeepAliveTime(time, MILLISECONDS);
	}

	public void setMaximumPoolSize(int maximumPoolSize) {
		pool.setMaximumPoolSize(maximumPoolSize);
	}
	
	/**
	 * the thread factory set thread group and name when the pool create a new thread.
	 */
	private class BasicThreadFactory implements ThreadFactory{
		String groupName = "main";
		ThreadGroup group = null;
		int count = 0;
		
		public BasicThreadFactory(String group){
			this.groupName = group;
			this.group = new ThreadGroup(groupName);
		}
		
		public Thread newThread(Runnable task) {
			log.debug("Create new thead, name:" + groupName + ", count:" + count);
			return new Thread(group, task, groupName + "_" + (count++));
		}
	}

	public int getActiveCount() {
		return pool.getActiveCount();
	}

	public long getTaskCount() {
		return pool.getTaskCount();
	}
	
	public String getQueuePolicy(){
		return queuePolicy;
	}
	
	private BlockingQueue<Runnable> createQueue(String queuePolicy){
		BlockingQueue<Runnable> q = null;
		this.queuePolicy = queuePolicy;
		
		if(queuePolicy != null){
			if(queuePolicy.equals("unbound")){
				q = new LinkedBlockingQueue<Runnable>();
			}else if(queuePolicy.equals("bound")){
				int queueSize = context.getIntParameter("MaximumQueueSize", 300);
				q = new ArrayBlockingQueue<Runnable>(queueSize);
			}
		}
		
		if(q == null){
			q = new SynchronousQueue<Runnable>(true);
			queuePolicy = "none";
		}
		
		return q;
	}

}
