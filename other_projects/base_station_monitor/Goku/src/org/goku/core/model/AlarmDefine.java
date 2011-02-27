package org.goku.core.model;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class AlarmDefine implements Cloneable{
	public static final String AL_001 = "001"; //外部报警
	public static final String AL_002 = "002"; //视频丢失
	public static final String AL_003 = "003"; //动态检测
	
	public String alarmCode = "";
	public String alarmLevel = "1";
	public String alarmName = "";	
	public String alarmCategory = "1";
	
	public boolean video = false;
	
	public String[] videoDepend = null;
	public int[] channels = null;
	
	public static Map<String, AlarmDefine> alarms = new HashMap<String, AlarmDefine>();
	static{
		alarms.put(AL_001, new AlarmDefine(AL_001, "外部报警", true));
		alarms.put(AL_002, new AlarmDefine(AL_002, "视频丢失", false));
		alarms.put(AL_003, new AlarmDefine(AL_003, "动态检测", true));
	}
	public static AlarmDefine alarm(String code){
		AlarmDefine alarm = alarms.get(code);		
		if(alarm == null){
			alarm = new AlarmDefine(code, code, false);
		}else {
			try {
				alarm = (AlarmDefine) alarm.clone();
			} catch (CloneNotSupportedException e) {
				alarm = new AlarmDefine(code, code, false);
			}
		}
		return alarm;
	}
	
	private AlarmDefine(String code, String name, boolean video){
		this.alarmCode = code;
		this.alarmName = name;
		this.video = video;
	}
	
	public void fillAlarmChannels(int mask){
		List<Integer> channels = new ArrayList<Integer>();
		for(int i = 0; i < 32; i++){
			if((mask & 1 << i) == 1){
				channels.add(i+1);
			}
		}
		this.channels = new int[channels.size()];
		for(int i = channels.size() -1; i >= 0; i--){
			this.channels[i] = channels.get(i);
		}
	}
	
	public String toString(){
		String xx = "";
		if(this.channels != null){
			for(int x : channels){xx += x+ ",";}
		}
		return String.format("<%s>%s channel:%s", this.alarmCode, this.alarmName, xx);
	}
}
