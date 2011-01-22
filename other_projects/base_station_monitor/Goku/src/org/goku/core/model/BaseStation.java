package org.goku.core.model;

import java.io.IOException;
import java.io.Writer;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import org.json.simple.JSONStreamAware;
import org.json.simple.JSONValue;

public class BaseStation  implements JSONStreamAware{
	public static final String ORM_TABLE = "base_station";
	public static final String[] ORM_FIELDS = new String[]{"uuid", "connectionStatus",
		"groupName", "routeServer", "locationId", 
		"lastUpdate", "lastActive", "createDate",
		"alarmStatus",
		"devType"};
	public static final String[] ORM_PK_FIELDS = new String[]{"uuid"};
	
	public static final int TYPE_VIDEO = 1;
	public static final int TYPE_IMAGE = 2;
	
	/**
	 * 基站的唯一标识，在客户端界面显示，初始化时配置。
	 */
	public String uuid;
	
	public String name;
	
	/**
	 * 基站的连接状态，
	 */
	public String connectionStatus = "01";
	
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
	public String routeServer;

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
	 * 设备类型， (视频｜图片|地点)
	 */
	public int devType;
	
	/**
	 * 摄像头, 例如：1:通道1$1:通道2
	 */
	public String channels;
	
	/**
	 * 端局类型. (客户定义)
	 */	
	public String btsCategory;
	//public 
	
	public String getStatus(){
		if(alarmStatus != null && !"".equals(this.alarmStatus)){
			return this.alarmStatus;
		}else {
			return this.connectionStatus;
		}
	}
	
	public String getName(){
		return name == null || "".equals(name.trim())
				? uuid : name;
	}
	
	public boolean equals(Object o){
		if(o instanceof BaseStation){
			BaseStation bs = (BaseStation)o;
			return this.uuid.equals(((BaseStation) o).uuid);
		}
		return false;
	}	
	
	@Override
	public void writeJSONString(Writer out) throws IOException {
        Map<String, Object> obj = new HashMap<String, Object>();
        obj.put("uuid", uuid);
        obj.put("name", name);
        obj.put("devType", devType);
        obj.put("routeServer", routeServer);
        obj.put("status", getStatus());
        
        JSONValue.writeJSONString(obj, out);
	}
}
