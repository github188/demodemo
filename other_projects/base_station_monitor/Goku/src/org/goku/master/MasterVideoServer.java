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
import org.goku.socket.SimpleSocketServer;
import org.goku.socket.SocketManager;

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
	public SimpleHTTPServer httpServer = null;
	public SimpleSocketServer socketServer = null;
	public SocketManager socketManager = null;	
	public RouteServerManager routeManager = null;
	private boolean running = true;
	
	private ThreadPoolExecutor threadPool = null;
	private static final String servelt = "org.goku.master.MasterServerServlet";
	
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
		
		routeManager = new RouteServerManager(threadPool, storage);
		threadPool.execute(routeManager);
		log.info("Start route server manager...");
		
		socketManager = new SocketManager(threadPool);
		threadPool.execute(socketManager);		
		int port = settings.getInt(Settings.LISTEN_PORT, 8000);
		socketServer = new SimpleSocketServer(socketManager, port);
		socketServer.setServlet(servelt);
		threadPool.execute(socketServer);
		
		int httpPort = settings.getInt(Settings.HTTP_PORT, 8080);
		log.info("Start http server at port " + httpPort);		
		httpServer = new SimpleHTTPServer("", httpPort);
		httpServer.setServlet(servelt);
		httpServer.addStartupListener(new StartupListener(){
			@Override
			public void started() {
				log.info("started http...");	
			}
		});		
		
		threadPool.execute(httpServer);
		while(this.running){
			synchronized(this){
				this.wait();
			}
		}
		log.info("halt");
	}
	
	public RouteServer addRouteServer(String ipaddr, String groupName){
		return routeManager.addRouteServer(ipaddr, groupName);
	}
}
