package org.goku.video;

import java.io.IOException;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.goku.core.model.AlarmDefine;
import org.goku.core.model.BaseStation;
import org.goku.socket.SocketManager;
import org.goku.video.odip.AbstractMonitorListener;
import org.goku.video.odip.MonitorClient;
import org.goku.video.odip.MonitorClientEvent;
import org.goku.video.odip.MonitorClientListener;
import org.goku.video.odip.VideoRoute;

public class TestFileRecorder {

	/**
	 * @param args
	 * @throws IOException 
	 * @throws InterruptedException 
	 */
	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		BaseStation station = new BaseStation();
		
		station.uuid = "1234";
		station.locationId = "192.168.1.67:9001";
		
		ThreadPoolExecutor threadPool = new ThreadPoolExecutor(
				3,
				20,
				60, 
				TimeUnit.SECONDS, 
				new LinkedBlockingDeque<Runnable>(6)
				);
		
		SocketManager socketManager = new SocketManager(threadPool);
		threadPool.execute(socketManager);
		
		MonitorClient client = new MonitorClient(station, 
				 new VideoRoute(threadPool),
				 socketManager);
		
		client.login();
		client.sendAlarmRequest();
		
		MonitorClientListener alarmListener = new AbstractMonitorListener(){
			@Override
			public void alarm(MonitorClientEvent event) {
				for(AlarmDefine alarm: event.alarms){
					System.out.println("===============");
					System.out.println("alarms:" + alarm.toString());
				}
			}
		};
		client.addListener(alarmListener);
		
		for(int i = 0; i < 100; i++){
			System.out.println("sendAlarmRequest......");
			client.sendAlarmRequest();
			Thread.sleep(1000 * 5);
		}
		Thread.sleep(1000 * 10);
		
		client.close();
	}

}
