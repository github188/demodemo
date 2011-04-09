package org.goku.image;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ThreadPoolExecutor;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 告警管理中心，定时向设备发送。告警查询命令。根据告警图片信息，生成一个客户端通知消息。
 */
public class AlarmMonitorCenter implements Runnable {
	private Log log = LogFactory.getLog("image.alarm");
	//private VideoRouteServer server = null;
	private Collection<ASC100Client> clients = Collections.synchronizedCollection(new ArrayList<ASC100Client>());
	private ThreadPoolExecutor executor = null;
	private Timer timer = new Timer();
	private boolean isRunning = false;
	
	/**
	 * 告警检测间隔时间，单位秒。
	 */
	private int alarmCheckTime = 10;
	
	public AlarmMonitorCenter(ThreadPoolExecutor executor){
		this.executor = executor;
	}

	@Override
	public void run() {
		isRunning = true;
	}
	
	public void checkAllClient(){
		synchronized(clients){
			for(ASC100Client c: clients){
				c.getAlarmImage();
			}
		}
	}
	
	public void setAlarmCheckTime(int seconds){
		timer.cancel();
		timer.scheduleAtFixedRate(new TimerTask(){
			@Override
			public void run() {
				if(isRunning){
					checkAllClient();
				}
			}
		}, 100, 1000 * seconds);
		this.alarmCheckTime = seconds;
	}
	
	public void addClient(ASC100Client client){
		if(!clients.contains(client)){
			clients.add(client);
		}
	}
	
	public void removeClient(ASC100Client client){
		if(clients.contains(client)){
			clients.remove(client);
		}
	}	
	
	/**
	 * 发送终端告警查询请求。
	 * @param client
	 */
	protected void checkMonitorClient(final ASC100Client client){
		log.debug("Check alarm, client id:" + client.info.uuid);
		client.getAlarmImage();
	}
}
