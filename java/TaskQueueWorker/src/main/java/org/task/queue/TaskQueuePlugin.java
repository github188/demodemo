package org.task.queue;

import hudson.Plugin;
import hudson.model.BuildableItem;

import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.task.queue.task.CheckPollQueueTask;
import org.task.queue.topic.MessageTaskMapping;

/**
 * 管理处理中的Message状态。
 * @author deon
 *
 */
public class TaskQueuePlugin extends Plugin{
	public transient static Log log = LogFactory.getLog("queue");
	
	private transient static TaskQueuePlugin ins = null;
	public transient ThreadPoolExecutor threadPool = new ThreadPoolExecutor(
			3, 20, 60, TimeUnit.SECONDS, 
			new LinkedBlockingDeque<Runnable>(100000)
			);
	
	public transient List<MessageTaskMapping> mappingList = null;
	private transient Timer timer = new Timer();
	
		
	public static TaskQueuePlugin getInstance(){
		return ins;
	}
	
    @Override 
    public void start() throws Exception {
    	super.start();
    	ins = this;
    	log.info("start task queue plugin...");
    	mappingList = new ArrayList<MessageTaskMapping>();
    	
    	timer.scheduleAtFixedRate(new TimerTask(){
    			@Override
    			public void run() {
    				threadPool.execute(new CheckPollQueueTask());
    			}}, 1000 * 10, 1000);
    }
	
	public synchronized void registerTopic(URL messageQueue, BuildableItem pri, String filter){
		MessageTaskMapping mapping = new MessageTaskMapping();
		mapping.queue = messageQueue;
		mapping.project = pri;
		mapping.filter = filter;
		if(!mappingList.contains(mapping)){
			mappingList.add(mapping);
		}else {
			for(MessageTaskMapping map: mappingList){
				if(map.equals(mapping)){
					map.filter = filter;
				}
			}
		}
	}
	
	public void unregisterTopic(URL messageQueue, BuildableItem pri){
		MessageTaskMapping mapping = new MessageTaskMapping();
		mapping.queue = messageQueue;
		mapping.project = pri;
		if(mappingList.contains(mapping)){
			mappingList.remove(mapping);
		}
	}
	
}
