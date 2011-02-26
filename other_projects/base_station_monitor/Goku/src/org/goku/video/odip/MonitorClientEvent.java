package org.goku.video.odip;

import java.nio.ByteBuffer;

public class MonitorClientEvent {
	public MonitorClient client = null;
	public ProtocolHeader header = null;
	public ByteBuffer buffer = null;
	
	public String alarmCode = null;
	public int alarmChannel = 0; 
	
	public MonitorClientEvent(MonitorClient client){
		this.client = client;
	}

}
