package org.notebook.services;

import java.io.Serializable;
import java.util.Date;

import org.notebook.cache.Category;

public class SyncTask implements Serializable{
	public static final String TASK_UP = "up";
	public static final String TASK_UP_DATA = "up_data";
	public static final String TASK_DOWN = "down";
	public static final String TASK_DOWN_DATA = "down_data";

	public static final String TASK_SYNC = "sync";	
	
	public static final String NOT_START = "not_start";
	public static final String CHECKING = "checking";
	public static final String PROCESSING = "processing";
	public static final String NO_UPDATE = "no_update";
	public static final String DOWN_LOAD = "down_load";
	public static final String DOWN_CONFLICT = "down_conflict";
	public static final String NEW_CREATED = "new_created";
	public static final String UP_LOAD = "up_load";
	public static final String IGNORED = "ignored";
	
	public String task = null;	
	public boolean force = false;
	public Category local = null;
	public Category remote = null;
	
	public Date createTime = null;
	//public Date remoteDate = null;
	public Date doneDate = null;

	public String status = null;
	public boolean newCreated = false;
	public Exception exception = null;
	public String cause = "";
	
	
	public SyncTask(String task, Category local){
		this.task = task;
		this.local = local;
		this.createTime = new Date(System.currentTimeMillis());
		this.status = NOT_START;
	}
}
