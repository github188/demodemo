package org.goku.video.odip;

import java.nio.ByteBuffer;

public class MonitorClientEvent {
	public MonitorClient client = null;
	public ProtocolHeader header = null;
	public ByteBuffer buffer = null;
	
	public MonitorClientEvent(MonitorClient client){
		this.client = client;
	}

}
