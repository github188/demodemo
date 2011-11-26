package org.goku.io;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class GokuResult {
	public int status = -2;
	public String message = null;
	
	public int totalCount = 0;	
	public int count = 0;
	public String session = null;
	
	public List<AlarmRecord> alarmList = null;
	
	public List<BaseStation> btsList = null;
	public Map<String, BaseStation> dataMap = null;

	public void addAlarmRecord(AlarmRecord alarm){
		if(alarmList == null){
			alarmList = new ArrayList<AlarmRecord>(Math.max(1, count));
		}
		alarmList.add(alarm);
	}
	
	public void addBaseStation(BaseStation bs){
		if (btsList == null){
			dataMap = new HashMap<String, BaseStation>();
			btsList = new ArrayList<BaseStation>(Math.max(1, count));
		}
		if(bs.devType.equals("3")){
			dataMap.put(bs.uuid, bs);
		}else {
			if(bs.parent != null){
				bs.parentNode = dataMap.get(bs.parent);
			}
			btsList.add(bs);
		}
	}
	
	public String getMessage(){
		return this.message;
	}
}
