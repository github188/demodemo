package org.task.queue.task;

import hudson.model.Hudson;

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
		for(Iterator<Message> iter = mapping.pendingMessage().iterator();
		iter.hasNext();){
			if(!this.isProjectInQueue()){
				m = iter.next();
				this.triggerWithMessage(m);
				iter.remove();
			}
		}
	}
	
	private boolean isProjectInQueue(){
		for(int i = 0; i < 3; i++){
			if(Hudson.getInstance().getQueue().contains(mapping.project)){
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
