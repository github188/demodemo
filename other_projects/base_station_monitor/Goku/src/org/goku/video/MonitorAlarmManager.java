package org.goku.video;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ThreadPoolExecutor;

import org.goku.video.odip.MonitorClient;

/**
 * Alarm管理中心，检测Alarm，自动收集告警录像，自动清理过期录像。
 * 
 * 同时负责终端的连接状态管理，如果不能连接成功，需要向中心服务器，报告状态。
 * @author deon
 */
public class MonitorAlarmManager implements Runnable{
	//private VideoRouteServer server = null;
	private Collection<MonitorClient> clients = Collections.synchronizedCollection(new ArrayList<MonitorClient>());
	private ThreadPoolExecutor executor = null;
	private Timer timer = new Timer();
	
	public MonitorAlarmManager(ThreadPoolExecutor executor){
		this.executor = executor;
	}
	
	@Override
	public void run() {
		timer.scheduleAtFixedRate(new TimerTask(){
			@Override
			public void run() {
				checkAllClient();				
			}
		}, 100, 1000 * 5);		
	}
	
	
	public void checkAllClient(){
		
	}
	
	public void addClient(MonitorClient client){
		
	}

	public void removeClient(MonitorClient client){
		
	}	
}
