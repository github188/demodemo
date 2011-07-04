package org.notebook.events;

import java.awt.event.ActionEvent;
import java.util.ArrayList;
import java.util.regex.Pattern;

import javax.swing.Action;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 应用事件处理中心, 支持一个事件被多个Action处理。多个Action之间可以共享事件处理
 * 的中间数据。
 * 
 * @author deon
 */
public class EventQueue {
	private Log log = LogFactory.getLog("event.queue");
	//public
	private ArrayList<EventMap> queue = new ArrayList<EventMap>();
	public EventQueue(Action defaultAction){
		EventMap e = new EventMap("*");
		e.order = Integer.MAX_VALUE;
		e.action = defaultAction;
		queue.add(e);
	}
	
	public synchronized int registerAction(String name, int order, Action action){
		EventMap e = new EventMap(name);
		e.order = order;
		e.action = action;
		if(e.order < 0){
			e.order = queue.size() - 1;
			queue.add(queue.size() - 1, e);
		}else {
			for(int i = 0; i < queue.size(); i++){
				if(queue.get(i).order < order){
					//e.order = i;
					queue.add(i, e);
					break;
				}
			}
		}
		return e.order;
	}

	/**
	 * 返回事件最终被处理的次数。
	 * @param event
	 * @return
	 */
	public int dispatchEvent(ActionEvent event){
		String name = event.getActionCommand();
		BroadCastEvent bcEvent = new BroadCastEvent(event);
		int count = 0;
		for(EventMap action: queue){
			if(action.action != null && action.accept(name)){
				try{
					action.action.actionPerformed(bcEvent);
					count++;
				}catch(Exception e){
					log.error(e.toString(), e);
					break;
				}
			}
		}
		return count;
	}
	
	public int fireEvent(String name, Object source){
		return dispatchEvent(new ActionEvent(source, 0, name));
	}
	
	class EventMap{
		private Pattern p = null;
		String name;
		int order = 0;
		Action action;
		public EventMap(String name){
			this.name = name;
			if(name.indexOf('*') >= 0){
				p = Pattern.compile("^" + name.replaceAll("\\*", ".*") + "$", Pattern.CASE_INSENSITIVE);
			}
		}
		
		public boolean accept(String name){
			if(this.name.equals(name)){
				return true;
			}else if(p != null && p.matcher(name).find()){
				return true;
			}
			return false;
		}
	}
	//public int 
}
