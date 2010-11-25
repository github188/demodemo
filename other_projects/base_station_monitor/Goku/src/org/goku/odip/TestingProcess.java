package org.goku.odip;

import java.io.IOException;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.BaseStation;

public class TestingProcess {

	/**
	 * @param args
	 * @throws IOException 
	 * @throws InterruptedException 
	 */
	public static void main(String[] args) throws IOException, InterruptedException {
		final Log log = LogFactory.getLog("main");
		BaseStation station = new BaseStation();
		station.uuid = "1234";
		station.locationId = "10.56.126.77:820";
		
		ThreadPoolExecutor threadPool = new ThreadPoolExecutor(2, 5, 60, 
				TimeUnit.SECONDS, 
				new LinkedBlockingDeque<Runnable>(300));
		ChannelSelector selector = new ChannelSelector(threadPool);
		
		log.info("start connection...");
		threadPool.execute(selector);
		for(int i = 0; i < 1; i++){
			MonitorClient client = new MonitorClient(station);
			
			client.addListener(new MonitorClientListener(){
				@Override
				public void connected(MonitorClientEvent event) {
					log.info("connected ok...");
					try {
						event.client.write("GET /\n\n".getBytes());
					} catch (IOException e) {
						log.error(e, e);
					}
				}
	
				@Override
				public void alarm(MonitorClientEvent event) {
					log.info("alarm ok...");				
				}});
			
			client.connect(selector);
		}
		//selector.run();
		
		log.info("waiting...");
		
		Thread.sleep(1000 * 10);
		//client.wait();
		//log.info("done...");
	}
}
