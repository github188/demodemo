package org.notebook.gui;

public class Events {
	/**
	 * 状态栏事件
	 */
	public static final String STATUS_EVENT = "UpdateStateBar";
	public static final String STATUS_PARAM = "StatusInfo";	
	//关键消息需要特殊处理。
	public static final String STATUS_WARN = "StatusWarn";	

	/**
	 * 进度条事件
	 */	
	public static final String PROGRESS_EVENT = "UpdateProgressBar";
	public static final String PROGRESS_PARAM = "UpdatePrgressInfo";

	/**
	 * FTP事件
	 */
	public static final String FTP_DOWN_DONE_EVENT = "FtpDownloadDone";
	public static final String FTP_UP_DONE_EVENT = "FtpUploadDone";
	
	public static final String FTP_PATH_PARAM = "FtpPathParam";
	
	//public static final String VERSION_EVENT = "UpdateProgressBar";
}
