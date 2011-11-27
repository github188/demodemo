package org.goku.io;


public class AlarmRecord {
    public String uuid;
    public String alarmName;
    public String btsID;
    public int status;
    public String level;
    public String startTime;
    public String endTime;
    public int category; //1视频2图片3无
    public String devType; //1
    
    public int channel = 0;
    public BaseStation bts = null;
    
    //private static Map<String, String> alarmName = new HashMap<String, String>();
    static{
    	//alarmName.put(paramK, paramV);
    }
    public boolean equals(Object e){
    	if(e instanceof AlarmRecord){
    		AlarmRecord o = (AlarmRecord)e;
    		if(o.uuid != null && uuid != null && uuid.equals(o.uuid)){
    			return true;
    		}
    	}
    	return false;
    }
    
    public void setBTS(String n){
    	if(n.indexOf(":") > 0){
    		String[] _tmp = n.split(":");
    		btsID = _tmp[0];
    		channel = Integer.parseInt(_tmp[1]);
    	}
    }
    
    public int hashCode(){
    	return uuid != null ? uuid.hashCode() : 0;
    } 
    
    public String getBTSName(){
    	return bts != null ? bts.name : btsID;
    }
    
    public String getAlaramType(){
    	switch(this.category){
    		case 1: return "视频";
    		case 2: return "图片";
    		case 3: return "无图片";
    	}
    	return category + "";
    }
}
