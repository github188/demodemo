package org.task.queue;

import hudson.Extension;
import hudson.model.RootAction;

import java.util.ArrayList;
import java.util.List;

import org.task.queue.topic.MessageTaskMapping;

@Extension
public class TaskQueueAction implements RootAction {

	@Override
	public String getDisplayName() {
		return "Task Queue status";
	}

	@Override
	public String getIconFileName() {
		// TODO Auto-generated method stub
		return "/plugin/TaskQueueWorker/robot.png";
	}

	@Override
	public String getUrlName() {
		return "/taskqueue";
	}
	
	
	public List<MessageTaskMapping> getTaskQueue(){
		List<MessageTaskMapping> queue = new ArrayList<MessageTaskMapping>();
		TaskQueuePlugin plugin = TaskQueuePlugin.getInstance();
		queue.addAll(plugin.mappingList);
		
		return queue;
	}

}
