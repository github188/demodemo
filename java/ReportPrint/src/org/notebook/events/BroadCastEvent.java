package org.notebook.events;

import java.awt.event.ActionEvent;
import java.util.HashMap;
import java.util.Map;

/**
 * 扩展了ActionEvent，增强为一个广播的事件，可以由多个Listener处理。
 * 1. 支持附加数据到Event。
 * 2. 定义了事件处理的状态。
 * @author deon
 */
public class BroadCastEvent extends ActionEvent {
	public static final int STATUS_ACTIVE = 1;
	public static final int STATUS_ERROR = 2;
	public static final int STATUS_CANCEL = 3;
	
	private static final long serialVersionUID = 1L;
	private ActionEvent event = null;
	private Map<String, Object> attached = new HashMap<String, Object>();
	
	public BroadCastEvent(ActionEvent event) {
		super(event.getSource(), event.getID(), event.getActionCommand(), 
			  event.getWhen(), event.getModifiers());
		this.event = event;
	}

	public Object get(String name){
		return attached.get(name);
	}
	
	public void set(String key, Object obj){
		attached.put(key, obj);
	}
}
