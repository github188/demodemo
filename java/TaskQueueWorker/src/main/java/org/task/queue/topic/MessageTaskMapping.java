package org.task.queue.topic;

import hudson.model.BuildableItem;
import hudson.model.Project;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.Date;
import java.util.Queue;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import org.task.queue.TaskQueuePlugin;
import org.task.queue.task.AckMessageTask;

public class MessageTaskMapping {
	public Lock queueLock = new ReentrantLock();;
	public Lock triggerLock = new ReentrantLock();;
	public URL queue = null;
	public String filter = null;
	public BuildableItem project = null;
	public long lastPoll = 0;
	
	private Queue<Message> messages = new ArrayBlockingQueue<Message>(50);
	
	//ArrayBlockingQueue(int capacity) 
	
	public boolean canPoll(){
		return System.currentTimeMillis() - lastPoll > 1000 * 3;
	}
	
	/**
	 * 还没有trigger的message list
	 * @return
	 */
	public Queue<Message> pendingMessage(){
		return messages;
	}
	
	public void addMessage(Message msg){
		if(messages.size() < 50){
			this.messages.add(msg);
			msg.mapping = this;
		}
	}
	
	public int getPendingMessageSize(){
		return messages.size();
	}
	
	public Date getLastPoll(){
		return new Date(lastPoll);
	}
	
	public String getProjectStatus(){
		String status = "";
		Project p = (Project)project;
		if(p.isDisabled()){
			status = "disabled";
		}else if(p.isInQueue()) {
			status = "in queue";
		}else if(p.isBuilding()){
			status = "building";
		}else {
			status = "idle";
		}
		return status;
	}
	
	public void ackMessageDone(Message msg){
		URL ack = null;
		try {
			ack = new URL(queue.toString().replace("fetch", "ack"));
			TaskQueuePlugin.getInstance().threadPool.execute(new AckMessageTask(ack, msg.id));
		} catch (MalformedURLException e) {
			TaskQueuePlugin.getInstance().log.error(e.toString(), e);
		}
	}
	
	public boolean equals(Object o){
		if(o instanceof MessageTaskMapping){
			MessageTaskMapping other = (MessageTaskMapping)o;
			if(other.queue != null && queue != null && other.queue.equals(queue)){
				if(other.project != null && project != null && other.project.equals(project)){
					return true;
				}
			}
		}
		return false;
	}
}
