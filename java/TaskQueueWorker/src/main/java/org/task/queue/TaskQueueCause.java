package org.task.queue;

import org.task.queue.topic.Message;

import hudson.model.Cause;

public class TaskQueueCause extends Cause {
	public Message msg = null;
	
	public TaskQueueCause(Message msg){
		this.msg = msg;
	}
	

	@Override
	public String getShortDescription() {
		return "trigger by message, id:" + msg.id;
	}

}
