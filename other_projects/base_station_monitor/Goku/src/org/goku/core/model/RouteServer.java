package org.goku.core.model;

import java.util.Collection;
import java.util.Iterator;

import org.goku.http.HTTPRemoteClient;

/**
 * 转发服务器。
 * @author deon
 */
public class RouteServer {	
	/**
	 * 转发服务器的分组名。
	 * 
	 * 由服务器启动时配置。
	 */
	public String groupName;
	
	/**
	 * 中心管理服务器地址。<ip>:<port>
	 * 
	 * 由服务器启动时配置。
	 */	
	public String centerServer;
	
	/**
	 * 转发服务器地址, <ip>:<port>
	 * 
	 * 由服务器启动时配置。
	 */		
	public String ipAddress;
	public String status;	
	/**
	 * 最后活动时间，用来计算心跳时间。
	 */	
	public long lastActive;	
	
	public int alarmCount;
	private HTTPRemoteClient http = null;
	
	public Collection<BaseStation> clients = null;	
	
	
	public RouteServer(String ipaddr, String groupName){
		this.ipAddress = ipaddr;
		this.groupName = groupName;
		http = new HTTPRemoteClient("http://" + ipaddr);
	}
	
	public Collection<BaseStation> listBaseStation(){
		return clients;
	}
	
	public void setBaseStationList(Collection<BaseStation> clients){
		this.clients = clients;
	}
	
	/**
	 * 根据最大终端数量，调整RouteServer需要监控的终端数量，如果大于max,把删除的
	 * 终端放到pool, 如果小于max, 从pool取出相应的终端。
	 * @param max
	 * @param pool
	 */
	public void balanceBaseStation(int max, Collection<BaseStation> pool){
		if(this.clients.size() > max){
			Iterator<BaseStation> iter = this.clients.iterator();
			for(int i = this.clients.size() - max; i > 0 && iter.hasNext(); i--){
				BaseStation bs = iter.next();
				if(this.http.removeBaseStaion(bs)){
					pool.add(bs);
					iter.remove();
				}else {
					continue;
				}
			}
		}else if(this.clients.size() < max){
			Iterator<BaseStation> iter = pool.iterator();
			for(int i = max - this.clients.size(); i > 0 && iter.hasNext(); i--){
				BaseStation bs = iter.next();
				if(this.http.addBaseStaion(bs)){
					this.clients.add(bs);
					iter.remove();
				}else {
					continue;
				}
			}
		}
	}
	
	public boolean ping(){
		return http.checkConnection();
	}
	
	public boolean equals(Object o){
		if(o instanceof RouteServer){
			RouteServer route = (RouteServer)o;
			return this.ipAddress.equals(((RouteServer) o).ipAddress);
		}
		return false;
	}
}
