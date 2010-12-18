package org.goku.core.model;

import java.util.Date;

import org.goku.video.FileVideoRecorder;

public class AlarmRecord {
	public String uuid = null;
	public String baseStation = "";
	public String startTimeString;
	public String alarmType = "";
	/**
	 * 告警处理状态，超时，正在发生，手动取消，删除
	 */
	public String alarmStatus = "";
	public String user = "";
	public Date startTime = null;
	public Date endTime = null;
	public String videoPath;
	
	/**
	 * 零时保存。
	 */
	public transient FileVideoRecorder recorder = null;
	
	public String getLevel(){
		return "error";
	}	
	
	public void generatePK(){
		long t = System.currentTimeMillis();
		t = t % (1000L * 3600 * 24 * 365);
		uuid = baseStation + String.format("%011d", t);
	}
	
	public static void main(String[] args){
		AlarmRecord alarm = new AlarmRecord();
		alarm.generatePK();
	}
}
