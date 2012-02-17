package org.task.queue.topic;

import hudson.model.BuildableItem;

import java.net.URL;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class MessageTaskMapping {
	public Lock queueLock = new ReentrantLock();;
	public Lock triggerLock = new ReentrantLock();;
	public URL queue = null;
	public BuildableItem project = null;
	public long lastPoll = 0;
	
	public boolean canPoll(){
		return System.currentTimeMillis() - lastPoll > 1000 * 3;
	}
	
	/**
	 * 还没有trigger的message list
	 * @return
	 */
	public List<Message> pendingMessage(){
		return null;
	}
	
	public void addMessage(Message msg){
		
	}
}
