package org.task.queue.topic;

import java.io.Serializable;
import java.util.Map;

import org.task.queue.TaskQueuePlugin;

public class Message implements Serializable{
	public int id;
	public Map<String, Object> data = null;
	public transient MessageTaskMapping mapping = null;
}
