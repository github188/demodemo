package org.task.queue.topic;

import java.io.Serializable;
import java.util.Map;

public class Message implements Serializable{
	public int id;
	public Map<String, Object> data = null;
	
}
