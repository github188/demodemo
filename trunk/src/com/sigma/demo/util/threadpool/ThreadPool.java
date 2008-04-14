package com.sigma.demo.util.threadpool;

import com.sigma.demo.mbean.ServiceMBean;

public interface ThreadPool extends ServiceMBean{
	public void execute(Runnable task);
}
