package org.goku.video.odip;

import java.io.IOException;
import java.nio.ByteBuffer;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class ODIPHandler {
	public ByteBuffer buffer = ByteBuffer.allocate(1024 * 64);
	public ProtocolHeader protoHeader = new ProtocolHeader();
	//public 
	private Log log = null; //#LogFactory.getLog(");
	
	/**
	 * 当前状态，正在等待的Ack消息，如果和期望的Ack不一致将丢弃包不处理。
	 * 如果waitAck 为0， 所有受到的消息均可以处理。
	 */
	private byte waitAck = 0;
	private MonitorClient client = null;
	
	public ODIPHandler(MonitorClient client){
		this.client = client;
		this.log = LogFactory.getLog("node." + client.info.uuid + ".odip");
	}
	
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
			if(!protoHeader.supportCommand()){
				log.warn(String.format("Drop unknown ODIP message, cmd=0x%x", 
						protoHeader.cmd));
			}else if(this.waitAck != 0 && protoHeader.cmd != this.waitAck){
				log.warn(String.format("Drop the ODIP message, waitAck=0x%x, return=0x%x", 
						this.waitAck, protoHeader.cmd));
			}else {
				this.processODIPMessage(protoHeader, buffer);
			}
		}
	}
	
	public void processODIPMessage(ProtocolHeader header, ByteBuffer buffer){
		switch(header.cmd){
			case ProtocolHeader.ACK_LOGIN:
				this.ackLogin(header, buffer);				
		}
	}
	
	public void resetBuffer(){
		this.protoHeader.cmd = 0;
		this.buffer.clear();
		this.buffer.limit(32);
	}
	
	public void login(String user, String password){
		ByteBuffer buffer = ByteBuffer.allocate(1024);
		ProtocolHeader header = new ProtocolHeader();
		header.cmd = ProtocolHeader.CMD_LOGIN;
		header.setByte(26, (byte)1);
		header.setByte(27, (byte)1);
		
		byte[] extra = this.encode(user, password);
		header.externalLength = extra.length;
		
		header.mapToBuffer(buffer);
		buffer.put(extra);
		
		buffer.flip();
		client.write(buffer);
		
		this.waitAck = ProtocolHeader.ACK_LOGIN;
	}
	
	private byte[] encode(String user, String password){
		return new byte[]{1, 2};
	}
	
	protected void ackLogin(ProtocolHeader header, ByteBuffer buffer){
		if(header.getByte(8) != 0){
			MonitorClientEvent event = new MonitorClientEvent(client);
			event.header = header;
			client.eventProxy.loginError(event);
		}else {
			
		}
	}
}
