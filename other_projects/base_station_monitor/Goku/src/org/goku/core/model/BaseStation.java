package org.goku.core.model;

import java.util.Date;

public class BaseStation {
	public static final int TYPE_VIDEO = 1;
	public static final int TYPE_IMAGE = 2;
	
	/**
	 * 基站的唯一标识，在客户端界面显示，初始化时配置。
	 */
	public String uuid;
	
	/**
	 * 基站的连接状态，
	 */
	public String connectionStatus;
	
	/**
	 * 基站的分组编号，同一组的基站，转发服务器实现自动分配。某个服务器负载过高，或关闭
	 * 后，由组内其他服务器分担，监控任务。
	 * 
	 * 由基站初始化时定义。
	 */	
	public String groupName;

	/**
	 * 基站的转发服务器的地址，<ip>:<port>. 由中心服务器分配更新。由运行时动态调整。
	 */
	public String manageServer;

	/**
	 * 基站的内部连接字符串，转发服务器使用其，连接监控终端。
	 * 
	 * 由基站初始化时定义。
	 */	
	public String locationId;
	
	/**
	 * 基站配置信息的最后更新时间。
	 */
	public Date lastUpdate;
	
	/**
	 * 基站配置信息的创建时间。
	 */	
	public Date createDate;
	
	/**
	 * 最后活动时间，用来计算心跳时间。
	 */	
	public Date lastActive;

	/**
	 * alarm状态。
	 */		
	public String alarmStatus;
	
	/**
	 * 设备类型， (视频｜图片)
	 */
	public int type;
	//public 
}
