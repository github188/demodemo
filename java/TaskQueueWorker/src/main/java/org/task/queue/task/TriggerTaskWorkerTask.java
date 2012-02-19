package org.task.queue.task;

import hudson.model.Hudson;
import hudson.model.Queue;

import java.util.Iterator;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.task.queue.TaskQueueCause;
import org.task.queue.topic.Message;
import org.task.queue.topic.MessageTaskMapping;

public class TriggerTaskWorkerTask implements Runnable {
	public transient static Log log = LogFactory.getLog("queue");
	private MessageTaskMapping mapping = null;
	
	public TriggerTaskWorkerTask(MessageTaskMapping m){
		this.mapping = m;
	}
	@Override
	public void run() {
		if(mapping.triggerLock.tryLock()){
			try{
				trigger();
			}finally{
				mapping.triggerLock.unlock();
			}
		}
	}
	
	private void trigger(){
		Message m = null;
		java.util.Queue<Message> q = mapping.pendingMessage();
		while(q.size() > 0){
			if(!this.isProjectInQueue()){
				this.triggerWithMessage(q.poll());
			}			
		}
	}
	
	private boolean isProjectInQueue(){
		Queue q = Hudson.getInstance().getQueue(); 
		
		for(int i = 0; i < 3; i++){
			if(q != null && q.contains(mapping.project)){
				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
				}
			}else {
				return false;
			}
		}
		return true;
	}
	
	private void triggerWithMessage(Message msg){
		log.info("trigge project:" + mapping.project.getName() + ", message id:" + msg.id);
		
		mapping.project.scheduleBuild(0, new TaskQueueCause(msg));
	}
}
