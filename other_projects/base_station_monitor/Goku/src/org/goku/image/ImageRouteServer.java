package org.goku.image;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ThreadPoolExecutor;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.BaseStation;
import org.goku.db.DataStorage;
import org.goku.http.HTTPRemoteClient;
import org.goku.settings.Settings;
import org.goku.socket.SocketManager;
import org.goku.video.MonitorAlarmManager;
import org.goku.video.VideoRouteServer;
import org.goku.video.odip.MonitorClient;
import org.goku.video.odip.VideoRoute;

public class ImageRouteServer {
	private Log log = LogFactory.getLog("main");
	private static ImageRouteServer ins = null;
	public Map<String, MonitorClient> clients = Collections.synchronizedMap(new HashMap<String, MonitorClient>());
	
	public Settings settings = null;
	public DataStorage storage = null;	
	public HTTPRemoteClient master = null;
	public MonitorAlarmManager manager = null;
	
	private ThreadPoolExecutor threadPool = null; 
	private boolean running = true;
	
	public static ImageRouteServer getInstance(){
		return ins;
	}
	
	public ImageRouteServer(Settings settings){
		ins = this;
		this.settings = settings;
	}
	
	public void startUp() throws Exception{
		log.info("Starting image routing server...");	
	}
	
	public void addMonitorClient(String uuid){
		BaseStation station = (BaseStation)storage.load(BaseStation.class, uuid);
		//client.connect(selector);
	}

}
