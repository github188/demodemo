package org.goku.core.model;

import java.io.IOException;
import java.io.Writer;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import org.goku.video.FileVideoRecorder;
import org.json.simple.JSONStreamAware;
import org.json.simple.JSONValue;

public class AlarmRecord implements JSONStreamAware{
	protected DateFormat format= new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
	public static final String ORM_TABLE = "alarm_record";
	public static final String[] ORM_FIELDS = new String[]{"uuid", "baseStation",
		"alarmType", "alarmStatus", "user", 
		"startTime", "endTime", "videoPath"};
	public static final String[] ORM_PK_FIELDS = new String[]{"uuid"};
	
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
	
	@Override
	public void writeJSONString(Writer out) throws IOException {
        Map<String, Object> obj = new HashMap<String, Object>();
        obj.put("uuid", uuid);
        obj.put("baseStation", baseStation);
        obj.put("alarmStatus", alarmStatus);
        obj.put("alarmType", alarmType);
        obj.put("level", getLevel());
        obj.put("startTime", format.format(startTime));
        obj.put("endTime", format.format(endTime));
        
        JSONValue.writeJSONString(obj, out);
	}		
	
	public static void main(String[] args){
		AlarmRecord alarm = new AlarmRecord();
		alarm.generatePK();
	}
}
