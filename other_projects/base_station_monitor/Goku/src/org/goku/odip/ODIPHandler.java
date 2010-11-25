package org.goku.odip;

import java.nio.ByteBuffer;

public class ODIPHandler {
	public ByteBuffer buffer = ByteBuffer.allocate(1024 * 64);
	
	public ByteBuffer getDataBuffer(){
		return buffer;
	}
	
	public void processData(){
		
	}
}
