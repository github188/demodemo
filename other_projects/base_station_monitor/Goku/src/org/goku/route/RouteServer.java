package org.goku.route;

import java.util.Collection;

import org.goku.core.model.BaseStation;
import org.goku.db.DataStorage;
import org.goku.odip.ChannelSelector;
import org.goku.odip.MonitorClient;
import org.goku.settings.Settings;

/**
 * 路由服务器，处理基站的连接，和告警查询。录像保存等操作。如果监控客户端，需要连接到
 * 监控。由此服务器转发。
 * 服务启动时，只是初始化一个HTTP服务，具体需要监控的终端，由控制服务器，通过HTTP接口
 * 调度后，才开始收集终端告警。
 * @author deon
 */
public class RouteServer {
	private Settings settings = null;	
	public Collection<MonitorClient> clients = null;
	protected ChannelSelector selector = null;
	
	protected DataStorage storage = null;
	
	public RouteServer(Settings settings){
		
	}
	
	public void startUp(){
		
	}
	
	private void startHttpServer(){
		
	}
	
	/**
	 * 启动路由功能，
	 */
	private void startRouting(){
		
	}
	
	protected void addMonitorClient(BaseStation station){
		MonitorClient client = new MonitorClient(station);
		client.connect();
	}
	
	//private void connectMonitorClient(){
	//	selector = new ChannelSelector();		
	//}
	
	/*
	protected void addMonitor(){
		
	}
	*/
	
	class HttpAction{
		public void shutdown(){
			
		}
		
		public void status(){
			
		}
		
		public void add_monitor(String uuid){
			BaseStation bs = (BaseStation)storage.load(BaseStation.class, uuid);
			addMonitorClient(bs);
			
		}
		
		public void remove_monitor(){
			
		}
	}
}
