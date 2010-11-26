package org.goku.odip;

import java.nio.ByteBuffer;

public class ProtocolHeader {
	private static final byte[] SUPPORT_COMMAND = 
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //00-0F
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //10-1F
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //20-2F
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //30-3F
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //40-4F
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //50-5F
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //60-6F
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //70-7F
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //80-8F
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //90-9F
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //A0-AF
	 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //B0-BF
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //C0-CF
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //D0-DF
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //E0-EF
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 //F0-FF
	};
	
	public byte cmd = 0;
	public byte reserved_1 = 0;
	public byte reserved_2 = 0;
	
	public byte headerLength = 0;
	public byte version = 0;
	
	public int externalLength = 0;
	
	public byte[] data = new byte[24];
	
	public void loadBuffer(ByteBuffer buffer){
		this.cmd = buffer.get();
		this.reserved_1 = buffer.get();
		this.reserved_2 = buffer.get();
		version = buffer.get();
		externalLength = buffer.getInt();
		
		buffer.get(data);		
	}
	
	public void mapToBuffer(ByteBuffer buffer){
		buffer.put(cmd);
		buffer.put(this.reserved_1);
		buffer.put(this.reserved_2);
		buffer.put(version);
		buffer.putInt(externalLength);
		
		buffer.put(data);
	}
	
	public boolean supportCommand(){
		return SUPPORT_COMMAND[this.cmd] == 1;
	}
}
