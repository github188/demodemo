package org.goku.odip;

import java.nio.ByteBuffer;

public class ProtocolHeader {
	public byte cmd = 0;
	public byte headerLength = 0;
	public byte version = 0;
	
	public int externalLength = 0;
	
	public byte[] data = new byte[24];
	
	public void loadBuffer(ByteBuffer buffer){
		this.cmd = buffer.get();
		buffer.get();
		buffer.get();
		version = buffer.get();
		externalLength = buffer.getInt();
		
		buffer.get(data);		
	}
	
	public void mapToBuffer(ByteBuffer buffer){
		buffer.put(cmd);
		buffer.put((byte)0);
		buffer.put((byte)0);
		buffer.put(version);
		buffer.putInt(externalLength);
		
		buffer.put(data);
	}
}
