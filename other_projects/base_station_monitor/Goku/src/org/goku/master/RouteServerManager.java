package org.goku.master;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;
import java.util.TreeSet;
import java.util.Vector;
import java.util.concurrent.ThreadPoolExecutor;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.BaseStation;
import org.goku.core.model.RouteServer;
import org.goku.db.DataStorage;

/**
 * 转发服务管理器，实现动态的调度监控资源。
 * @author deon
 */
public class RouteServerManager implements Runnable {
	private Log log = LogFactory.getLog("route.manager");
	private Collection<RouteServer> servers = Collections.synchronizedCollection(new ArrayList<RouteServer>());
	
	private ThreadPoolExecutor executor = null;
	private DataStorage storage = null;
	private Timer timer = new Timer();
	private long expiredTime = 1000 * 30;
	
	public RouteServerManager(ThreadPoolExecutor executor, DataStorage storage){
		this.executor = executor;
		this.storage = storage;
	}
	
	@Override
	public void run() {
		timer.scheduleAtFixedRate(new TimerTask(){
			@Override
			public void run() {
				try{
					checkAllRouteServer();
				}catch(Throwable e){
					log.error(e);
				}
			}
		}, 100, expiredTime);
		
		this.restoreRoute();
	}
	
	public void addRouteServer(final RouteServer route){
		if(!this.servers.contains(route)){
			this.servers.add(route);
			executor.execute(new Runnable(){
				@Override
				public void run() {
					balanceGroup(route.groupName);
				}});
		}
	}
	
	public void removeRouteServer(final RouteServer route){
		if(this.servers.contains(route)){
			this.servers.remove(route);
			executor.execute(new Runnable(){
				@Override
				public void run() {
					storage.removeRouteServer(route);
					balanceGroup(route.groupName);
				}});
		}
	}
	
	/**
	 * 重新调度组内Route的服务器负载。
	 * @param groupName
	 */
	protected synchronized void balanceGroup(String groupName){
		TreeSet<RouteServer> routeList = new TreeSet<RouteServer>();
		
		log.info("balance route group, name:" + groupName);		
		int count = 0;
		synchronized(servers){
			for(RouteServer s: servers){
				if(s.groupName.equals(groupName)){
					s.setBaseStationList(storage.listStation(s));
					count += s.listBaseStation().size();
					routeList.add(s);
				}
			}
		}
		if(routeList.size() > 0){
			log.info("active base station count:" + count);
			Collection<BaseStation> bsPool = new Vector<BaseStation>();		
			bsPool.addAll(storage.listDeadStation(groupName));
			log.warn("dead station count:" + bsPool.size());
			
			count += bsPool.size();
			int average = count / routeList.size() + 1;			
			log.info(String.format("balance route server, station count:%s, route count:%s, average:%s",
						count, routeList.size(), average));
			for(RouteServer s: routeList){
				s.balanceBaseStation(count, bsPool, storage);
			}
			/**
			 * 如果最后还存在没有被监控的终端，全部放到最后一个路由。
			 */
			routeList.last().balanceBaseStation(Integer.MAX_VALUE, bsPool, storage);
		}else {
			log.info("Not found any route server for group " + groupName);
		}
	}
	
	protected void checkAllRouteServer(){
		Collection<RouteServer> routeList = new Vector<RouteServer>();		
		routeList.addAll(servers);
		//log.info("check All RouteServer...");
		for(RouteServer s: routeList){
			if(s.ping()){
				s.lastActive = System.currentTimeMillis();
				log.info("Route group:" + s.groupName + ", ipaddr:" + s.ipAddress + ", Status OK!");
			}else {
				log.info("Route group:" + s.groupName + ", ipaddr:" + s.ipAddress + ", Status ERR!");
				this.removeRouteServer(s);
			}
		}
	}
	
	/**
	 * 恢复数据库中，存在的路由服务器，让后做一次调度。
	 */
	protected void restoreRoute(){
		String routeList = "select routeServer, groupName from base_station " +
						   "group by routeServer, groupName";
		Collection<Map<String, Object>> xx = storage.query(routeList, new Object[]{});
		
		//Collection<String> groups = new ArrayList<String>();
		for(Map<String, Object> route: xx){
			//groups.add((String)route.get("groupName"));
			servers.add(new RouteServer((String)route.get("routeServer"),
						(String)route.get("groupName")));
		}
		/*
		for(String group: groups){
			balanceGroup(group);
		}*/
	}
}
