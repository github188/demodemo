package org.goku.core.model;

import java.util.Date;

public class SystemLog {
	/**
	 * 操作的发起对象，可以是一个设备，一个服务器，也可以是人。
	 */
	public String actionOwner;
	/**
	 * 操作的目标对象。
	 */
	public String actionObject;
	/**
	 * 操作的名称.
	 */	
	public String actionType;
	public String actionDate;
	/**
	 * 操作描述信息。
	 */		
	public String description;
	
	public Date createDate;

}
