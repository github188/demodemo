package org.goku.odip;

import java.io.IOException;
import java.nio.ByteBuffer;

public class ODIPHandler {
	public ByteBuffer buffer = ByteBuffer.allocate(1024 * 64);
	public ProtocolHeader protoHeader = new ProtocolHeader();
	//public 
	
	public ByteBuffer getDataBuffer(){
		return buffer;
	}
	
	public void processData(MonitorClient client) throws IOException{		
		this.buffer.flip();
		
		if(protoHeader.cmd == 0){
			protoHeader.loadBuffer(this.buffer);
			int extLen = this.protoHeader.externalLength;		
			if(extLen > 0){
				this.buffer.clear();
				this.buffer.limit(extLen);
				client.read(buffer);
				if(!buffer.hasRemaining()){
					buffer.flip();
				}
			}else {
				buffer.limit(0);
			}
		}
		
		//ODIP协议头和扩展数据都读完成，开始处理协议信息。
		if(protoHeader.cmd != 0 && !buffer.hasRemaining()){
			
		}
	}
	
	public void processODIPMessage(MonitorClient client, ProtocolHeader header, ByteBuffer buffer){
		
	}
	
	public void resetBuffer(){
		this.protoHeader.cmd = 0;
		this.buffer.clear();
		this.buffer.limit(32);
	}
}
