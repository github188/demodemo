package org.goku.io;

public class BaseStation {
	public String uuid;
	public String devType;
	public String routeServer;
	public String status;
	public String parent;
	public String name;
	
	public BaseStation parentNode = null;
	public void setChannels(String e){
		
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
    
    public int hashCode(){
    	return uuid != null ? uuid.hashCode() : 0;
    }
}
