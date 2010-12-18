package org.goku.video;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ThreadPoolExecutor;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.video.odip.MonitorClient;

/**
 * Alarm管理中心，检测Alarm，自动收集告警录像，自动清理过期录像。
 * 
 * 同时负责终端的连接状态管理，如果不能连接成功，需要向中心服务器，报告状态。
 * @author deon
 */
public class AlarmMonitorCenter implements Runnable{
	private Log log = LogFactory.getLog("video.alarm");
	//private VideoRouteServer server = null;
	private Collection<MonitorClient> clients = Collections.synchronizedCollection(new ArrayList<MonitorClient>());
	private ThreadPoolExecutor executor = null;
	private Timer timer = new Timer();
	
	public AlarmMonitorCenter(ThreadPoolExecutor executor){
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
		synchronized(clients){
			for(MonitorClient c: clients){
				executor.execute(new CallBack(c));
			}
		}
	}
	
	/**
	 * 发送终端告警查询请求。
	 * @param client
	 */
	protected void checkMonitorClient(final MonitorClient client){
		log.debug("Check alarm, client id:" + client.info.uuid);
		if(client.getClientStatus() == null && client.retryError < 5){
			client.login();
		}
		if(client.getClientStatus() != null){
			client.checkAlarm();
			//测试视频传输
			//client.realPlay();
		}else {
			log.warn("The client is disconneted, can't check alarm event. the client uuid is" + client.info.uuid);
		}
	}
	
	public void addClient(MonitorClient client){
		if(!clients.contains(client)){
			clients.add(client);
		}
	}

	public void removeClient(MonitorClient client){
		if(clients.contains(client)){
			clients.remove(client);
		}
	}
	
	public void close(){
		this.timer.cancel();
	}
	
	/**
	 * 线程池调度的转发类。
	 */
	class CallBack implements Runnable{
		private MonitorClient c;
		CallBack(MonitorClient c){this.c = c;}
		public void run() {
			try{
				checkMonitorClient(this.c);
			}catch(Throwable e){
				String msg = String.format("Error at checking alarm, client id:%s, error:%s",
						c.info.uuid, e.toString());
				log.warn(msg, e);
			}
		}
	}
}
