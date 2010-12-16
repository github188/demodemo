package org.goku.core.model;

import java.util.Date;

import org.goku.video.FileVideoRecorder;

public class AlarmRecord {
	public String uuid = null;
	public String baseStation = "";
	public String startTimeString;
	public String alarmType = "";
	public String user = "";
	public Date startTime;
	public Date endTime;
	public String videoPath;
	
	/**
	 * 零时保存。
	 */
	public transient FileVideoRecorder recorder = null;
	
	
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
