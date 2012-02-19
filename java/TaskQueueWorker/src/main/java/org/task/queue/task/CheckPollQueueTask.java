package org.task.queue.task;

import hudson.model.Hudson;
import hudson.model.Project;
import hudson.model.Queue;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.ReentrantLock;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.task.queue.TaskQueuePlugin;
import org.task.queue.topic.MessageTaskMapping;

/**
 * 检查是否需要Poll message queue.
 * 
 * 1. 当前的Project，没有在queue的build.
 * 2. Message已经全部处理.
 * 3. 上次查询时间超过3秒.
 * @author deon
 */
public class CheckPollQueueTask implements Runnable {
	public transient static Log log = LogFactory.getLog("queue");
	//public Lock singltonLok = null;
	private final static ReentrantLock singltonLok = new ReentrantLock();

	@Override
	public void run() {
		int queueSize = TaskQueuePlugin.getInstance().threadPool.getQueue().size(); 
		if(queueSize > 100){
			log.warn("too many pending task in thread pool, size:" + queueSize);
		}
		if(singltonLok.tryLock()){
			try{
				this.checkQueue();
			}finally{
				singltonLok.unlock();
			}
		}
	}
	
	private void checkQueue(){
		List<MessageTaskMapping> mappingList = new ArrayList<MessageTaskMapping>(
				TaskQueuePlugin.getInstance().mappingList.size());
		
		mappingList.addAll(TaskQueuePlugin.getInstance().mappingList);
		Project p = null;
		for(MessageTaskMapping mapping: mappingList){
			if(!(mapping.project instanceof Project)){
				continue;
			}
			p = (Project)mapping.project;
			if(p.isDisabled()){
				continue;
			}
			if(p.isInQueue()){
				continue;
			}
			if(mapping.pendingMessage().size() > 0){
				TaskQueuePlugin.getInstance().threadPool.execute(new TriggerTaskWorkerTask(mapping));
			}else if(mapping.canPoll()){
				TaskQueuePlugin.getInstance().threadPool.execute(new PollQueueTask(mapping));
			}
		}
	}
}
