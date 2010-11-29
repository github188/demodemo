package org.goku.video;

import java.io.IOException;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.BaseStation;
import org.goku.db.DataStorage;
import org.goku.http.SimpleHTTPServer;
import org.goku.http.StartupListener;
import org.goku.master.MasterClient;
import org.goku.settings.Settings;
import org.goku.video.odip.SocketManager;
import org.goku.video.odip.MonitorClient;
import org.goku.video.odip.VideoRoute;

/**
 * 路由服务器，处理基站的连接，和告警查询。录像保存等操作。如果监控客户端，需要连接到
 * 监控。由此服务器转发。
 * 服务启动时，只是初始化一个HTTP服务，具体需要监控的终端，由控制服务器，通过HTTP接口
 * 调度后，才开始收集终端告警。
 * @author deon
 */
public class VideoRouteServer {
	private Log log = LogFactory.getLog("main");
	private static VideoRouteServer ins = null;
	public Map<String, MonitorClient> clients = Collections.synchronizedMap(new HashMap<String, MonitorClient>());
	
	public Settings settings = null;
	public SocketManager socketManager = null;	
	public DataStorage storage = null;	
	public MasterClient master = null;
	public MonitorAlarmManager manager = null;
	
	private ThreadPoolExecutor threadPool = null; 
	private boolean running = true;
	
	public static VideoRouteServer getInstance(){
		return ins;
	}
	
	public VideoRouteServer(Settings settings){
		ins = this;
		this.settings = settings;
	}
	
	public void startUp() throws Exception{
		log.info("Starting video routing server...");
				
		log.info("init DB connection...");
		this.storage = DataStorage.create(settings);
		
		int core_thread_count = settings.getInt(Settings.CORE_ROUTE_THREAD_COUNT, 50);
		
		log.info("init thread pool, core thread count " + core_thread_count);
		threadPool = new ThreadPoolExecutor(
				core_thread_count,
				settings.getInt(Settings.MAX_ROUTE_THREAD_COUNT, 500),
				60, 
				TimeUnit.SECONDS, 
				new LinkedBlockingDeque<Runnable>(core_thread_count * 2)
				);
		socketManager = new SocketManager(threadPool);
		
		
		String masterUrl = settings.getString(Settings.MASTER_SERVER_URL, "http://127.0.0.1:8081");
		log.info("Check master server in running, url:" + masterUrl);
		master = new MasterClient(masterUrl);
		if(master.checkConnection()){
			log.info("Connected master server.");
		}else {
			log.warn("Failed to connect master server.");
		}
		
		log.info("Starting alarm manager server...");
		manager = new MonitorAlarmManager(threadPool);
		threadPool.execute(manager);
		
		int httpPort = settings.getInt(Settings.HTTP_PORT, 8082);
		log.info("Start http server at port " + httpPort);
		
		SimpleHTTPServer http = new SimpleHTTPServer("", httpPort);
		http.setServlet("org.goku.video.DefaultRouteServerServlet");
		http.addStartupListener(new StartupListener(){
			@Override
			public void started() {
				log.info("started http...");	
			}
		});
		
		threadPool.execute(http);
		while(this.running){
			synchronized(this){
				this.wait();
			}
		}
		log.info("halt");		
	}
	
	protected void loadingMonitorClient(){
		
	}
	
	public void shutdown(){
		this.running = false;
		synchronized(this){
			this.notifyAll();
		}
	}
	
	public void addMonitorClient(String uuid){
		BaseStation station = (BaseStation)storage.load(BaseStation.class, uuid);
		MonitorClient client = new MonitorClient(station, 
												 new VideoRoute(threadPool),
												 socketManager);
		this.clients.put(uuid, client);
		this.manager.addClient(client);
		
		//client.connect(selector);
	}
	
	
	/**
	 * 根据UUID取得监控客户端对象。
	 * @param uuid
	 * @return
	 */
	public MonitorClient getMonitorClient(String uuid){
		return clients.get(uuid);
	}
	
	//private void connectMonitorClient(){
	//	selector = new ChannelSelector();		
	//}
	
	/*
	protected void addMonitor(){
		
	}
	*/
	public static void main(String[] a) throws Exception{
		new VideoRouteServer(new Settings("video.conf")).startUp();
	}
}
