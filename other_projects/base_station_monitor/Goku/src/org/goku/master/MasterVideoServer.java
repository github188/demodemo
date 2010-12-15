package org.goku.master;

import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.RouteServer;
import org.goku.db.DataStorage;
import org.goku.http.SimpleHTTPServer;
import org.goku.http.StartupListener;
import org.goku.settings.Settings;

/**
 * 监控管理服务器，负责调度不同的转发服务器，实现监控服务。
 * @author deon
 *
 */
public class MasterVideoServer {
	private Log log = LogFactory.getLog("main");
	private static MasterVideoServer ins = null;
	
	public Settings settings = null;
	public DataStorage storage = null;
	public RouteServerManager manager = null;
	private boolean running = true;
	
	private ThreadPoolExecutor threadPool = null;
	
	public static MasterVideoServer getInstance(){
		return ins;
	}
	
	public MasterVideoServer(Settings settings){
		ins = this;
		this.settings = settings;			
	}
	
	public void startUp() throws Exception{
		log.info("Starting master server...");
		
		log.info("init DB connection...");
		this.storage = DataStorage.create(settings);
		this.storage.checkConnect();
		
		int core_thread_count = settings.getInt(Settings.CORE_ROUTE_THREAD_COUNT, 50);
		log.info("init thread pool, core thread count " + core_thread_count);
		threadPool = new ThreadPoolExecutor(
				core_thread_count,
				settings.getInt(Settings.MAX_ROUTE_THREAD_COUNT, 500),
				60, 
				TimeUnit.SECONDS, 
				new LinkedBlockingDeque<Runnable>(core_thread_count * 2)
				);
		
		manager = new RouteServerManager(threadPool, storage);
		threadPool.execute(manager);
		log.info("Start route server manager...");
		
		//testing.......
		threadPool.execute(new SocketVideoServer(threadPool));
		
		int httpPort = settings.getInt(Settings.HTTP_PORT, 8080);
		log.info("Start http server at port " + httpPort);
		
		SimpleHTTPServer http = new SimpleHTTPServer("", httpPort);
		http.setServlet("org.goku.master.MasterServerServlet");
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
	
	public void addRouteServer(String ipaddr, String groupName){
		this.manager.addRouteServer(new RouteServer(ipaddr, groupName));
	}
}
