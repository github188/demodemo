package org.goku.io;

public class AlarmRecord {
    public String uuid;
    public String code;
    public String btsID;
    public int status;
    public String level;
    public String startTime;
    public String endTime;
    public String category;
    public String devType;
    
    public boolean equals(Object e){
    	if(e instanceof AlarmRecord){
    		AlarmRecord o = (AlarmRecord)e;
    		if(o.uuid != null && uuid != null && uuid.equals(o.uuid)){
    			return true;
    		}
    	}
    	return false;
    }
    
    public int hashCode(){
    	return uuid != null ? uuid.hashCode() : 0;
    }    
}
