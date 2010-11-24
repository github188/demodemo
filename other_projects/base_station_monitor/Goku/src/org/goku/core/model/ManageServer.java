package org.goku.core.model;

import java.util.Collection;
import java.util.Date;

public class ManageServer {
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
	public Date lastActive;	
	
	public int alarmCount;
	
	
	public Collection<BaseStation> listBaseStation(){
		return null;
	}
}
