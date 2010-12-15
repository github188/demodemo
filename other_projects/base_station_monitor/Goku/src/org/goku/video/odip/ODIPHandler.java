package org.goku.video.odip;

import static org.goku.video.odip.ProtocolHeader.ACK_GET_VIDEO;
import static org.goku.video.odip.ProtocolHeader.ACK_LOGIN;
import static org.goku.video.odip.ProtocolHeader.CMD_LOGIN;

import java.io.IOException;
import java.nio.ByteBuffer;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 处理视频协议数据，
 * 
 * 登录流程：
 * 1. 0xA0 --->          登录
 * 2.     <--- 0xB0     应答登录
 * 
 * 实时监控
 * 1. 0xf1 --->          请求实时视频
 *         <--- 0xF1     ????协议中要求返回，抓包分析发现未返回。
 *    0x11 --->          请求媒体数据 ???什么场景使用 
 * 4.      <--- 0xBC     视频数据
 * 
 * 设备信息查询：
 * 1. 0xA4 --->          查询设备信息, 01(基本信息), 07(设备ID)
 *         <--- 0xB4     应答
 * ---
 *    0xA8 --->          ??? 什么应用场景
 *         <--- 0xB8
 * --- 配置参数
 *    0xA3 ---> 
 *         
 * 告警查询：
 * 1. 0xA1 --->
 *         <--- 0xB1     告警状态
 *         
 * 
 * @author deon
 */
public class ODIPHandler {
	public ByteBuffer buffer = ByteBuffer.allocate(1024 * 64);
	public ProtocolHeader protoHeader = new ProtocolHeader();
	//public 
	private Log log = null; //#LogFactory.getLog(");
	
	/**
	 * 阻塞Login方法，等待应答消息.
	 */
	private Object lockLogin = new Object();
	private int loginStatus = -2;
	
	/**
	 * 当前状态，正在等待的Ack消息，如果和期望的Ack不一致将丢弃包不处理。
	 * 如果waitAck 为0， 所有受到的消息均可以处理。
	 */
	private byte waitAck = 0;
	private MonitorClient client = null;
	
	public ODIPHandler(MonitorClient client){
		this.client = client;
		this.log = LogFactory.getLog("node." + client.info.uuid + ".odip");
		this.resetBuffer();
	}
	
	public ByteBuffer getDataBuffer(){
		return buffer;
	}
	
	public void processData(MonitorClient client) throws IOException{
		//this.buffer.flip();
		
		if(protoHeader.cmd == 0){
			this.buffer.flip();
			protoHeader.loadBuffer(this.buffer);
			int extLen = this.protoHeader.externalLength;
			log.debug(String.format("Got cmd=0x%x, ext len=%s", 
					protoHeader.cmd, extLen));
			
			if(extLen > 0){
				this.buffer.clear();
				this.buffer.limit(extLen);
				client.read(buffer);
			}else {
				buffer.limit(0);
			}
		}
		
		//ODIP协议头和扩展数据都读完成，开始处理协议信息。
		if(protoHeader.cmd != 0 && !buffer.hasRemaining()){
			buffer.flip();
			if(!protoHeader.supportCommand()){
				log.warn(String.format("Drop unknown ODIP message, cmd=0x%x", 
						protoHeader.cmd));
			}else if(this.waitAck != 0 && protoHeader.cmd != this.waitAck){
				log.warn(String.format("Drop the ODIP message, waitAck=0x%x, return=0x%x", 
						this.waitAck, protoHeader.cmd));
			}else {
				this.processODIPMessage(protoHeader, buffer);
			}
			//处理完成后，重新开始读协议头。
			this.resetBuffer();
		}else {
			log.debug("waiting data remaining buffer:" + buffer.remaining());
		}
	}
	
	public void processODIPMessage(ProtocolHeader header, ByteBuffer buffer){
		//int unsignedBypte = header.cmd >= 0 ? header.cmd : header.cmd + 256;
		switch(header.cmd){
			case ACK_LOGIN:
				this.ackLogin(header, buffer);
				break;
			case ACK_GET_VIDEO:
				this.ackVideoData(header, buffer);
				break;
			default:
				log.warn(String.format("Not found handler for command:0x%x", header.cmd));
		}
	}
	
	public void resetBuffer(){
		this.protoHeader.cmd = 0;
		this.buffer.clear();
		this.buffer.limit(32);
	}
	
	public int login(String user, String password){
		return this.login(user, password, true);
	}
	/**
	 * 登录系统，等待回应后返回。
	 * @param user
	 * @param password
	 * @return -- 成功返回-1, 失败返回0-7的错误码。
	 */
	public int login(String user, String password, boolean sync){
		ByteBuffer buffer = ByteBuffer.allocate(1024);
		ProtocolHeader header = new ProtocolHeader();
		header.cmd = CMD_LOGIN;
		header.setByte(26, (byte)1);
		header.setByte(27, (byte)1);
		
		byte[] extra = this.encode(user, password);
		header.externalLength = extra.length;
		
		header.mapToBuffer(buffer);
		buffer.put(extra);
		
		buffer.flip();
		client.write(buffer);
		
		log.info("Login to " + client.info.locationId);
		this.waitAck = ACK_LOGIN;
		
		if(sync) {
			synchronized(this.lockLogin){
				try {
					lockLogin.wait(1000 * 10);
				} catch (InterruptedException e) {
					log.info("InterruptedException, lock Login");
				}
			}
			if(this.loginStatus == -2){
				log.warn("Time out wait login response.");
			}
			return this.loginStatus;
		}else {
			return -2;
		}
		
	}
	
	/**
	 * 0x11
	 * @param action 1--开始监控， 0--关闭监控。
	 */
	public void videoStreamControl(int action){
		if(this.client.getClientStatus() == null)
			throw new UnsupportedOperationException("Can't connection before login.");
		
		ByteBuffer buffer = ByteBuffer.allocate(64);
		ProtocolHeader header = new ProtocolHeader();
		header.cmd = ProtocolHeader.CMD_GET_VIDEO;
		header.externalLength = 16;
		header.version = 0;
		
		//header.setByte(this.client.channelId + 7, (byte)1);
		header.setByte(this.client.channelId + 7, (byte)action);
		
		header.mapToBuffer(buffer);

		buffer.position(32 + 16);
		buffer.flip();
		client.write(buffer);
		
		this.client.status.realPlaying = action == Constant.CTRL_VIDEO_START;	
	}
	
	/**
	 *CMD_CONNECT
	 */
	public void requestConnection(byte type){
		if(this.client.getClientStatus() == null)
			throw new UnsupportedOperationException("Can't connection before login.");
		
		
		for(int i = 1; i < 2; i++){
			ByteBuffer buffer = ByteBuffer.allocate(64);
			ProtocolHeader header = new ProtocolHeader();
			header.cmd = ProtocolHeader.CMD_CONNECT;
			header.externalLength = 0;
			header.version = 0;
			
			header.setInt(8, this.client.getClientStatus().sessionId);
			header.setByte(12, type);
			header.setByte(13, (byte)(this.client.channelId));
			//header.setByte(13, (byte)i);
			
			header.mapToBuffer(buffer);
			
			buffer.flip();
			client.write(buffer);		
		}
	}
	
	/*
	public void login(String user, String password){
		
	}*/
	
	/**
49:75:59:74: 31:70:4a:53:
44:51:45:3d: 26:26:32:55:
67:32:52:59: 67:34:74:58:
30:3d
	 * @param user
	 * @param password
	 * @return
	 */
	private byte[] encode(String user, String password){
		/*
		return new byte[]{0x49, 0x75, 0x74, 0x31, 0x31, 0x70, 0x41, 0x53,
						  0x44, 0x51, 0x45, 0x3d, 0x26, 0x26, 0x32, 0x55,
						  0x67, 0x32, 0x52, 0x59, 0x67, 0x34, 0x74, 0x58,
						  0x30, 0x3d
						  };
						  */
		return new byte[]{0x49, 0x75, 0x59, 0x74, 0x31, 0x70, 0x4a, 0x53,
						  0x44, 0x51, 0x45, 0x3d, 0x26, 0x26, 0x32, 0x55,
						  0x67, 0x32, 0x52, 0x59, 0x67, 0x34, 0x74, 0x58,
						  0x30, 0x3d
				  };		
	}
	
	protected void ackLogin(ProtocolHeader header, ByteBuffer buffer){
		if(header.getByte(8) != 0){
			log.info("Failed to login, error code:" + header.getByte(9));
			MonitorClientEvent event = new MonitorClientEvent(client);
			event.header = header;
			client.eventProxy.loginError(event);
			this.loginStatus = header.getByte(9);
		}else {
			ClientStatus status = new ClientStatus();
			status.channelCount = header.getByte(10);
			status.videoType = header.getByte(11);
			status.devType = header.getByte(12);
			status.sessionId = header.getInt(16);
			status.devMode = header.getByte(28);
			
			this.client.setClientStatus(status);
			this.waitAck = 0;
			
			log.info(String.format("Login successful, channel count:%s, videoType:%s" +
					", devType:%s, sessionId:%s, devMode:%s",
					status.channelCount, 
					status.videoTypeName(),
					status.devTypeName(), 
					status.sessionId, 
					status.devModeName()));
			
			client.eventProxy.loginOK(new MonitorClientEvent(client));
			this.loginStatus = -1;
		}
		synchronized(this.lockLogin){
			this.lockLogin.notifyAll();
		}
	}
	
	protected void ackVideoData(ProtocolHeader header, ByteBuffer buffer){
		
		int channel = header.getByte(8) + 1; 
		if(channel != this.client.channelId){
			log.warn(String.format("Got other channel video data, %s!=%s", 
					channel, this.client.channelId));
		}else {
			log.debug("Get video data, len:" + header.externalLength);
			this.client.route.route(buffer, 0);
		}
	}
}
