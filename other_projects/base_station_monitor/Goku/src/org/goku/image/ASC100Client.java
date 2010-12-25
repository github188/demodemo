package org.goku.image;

import java.io.IOException;
import java.nio.ByteBuffer;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class ASC100Client {
	private Log log = null;
	private ASC100MX mx = null;
	private String location = "";
	private ByteBuffer outBuffer = ByteBuffer.allocate(64 * 1024);
	
	private byte lastCmd = 0;
	
	public ASC100Client(String location){
		this.location = location;
		log = LogFactory.getLog("asc100." + location);
	};
	
	public void process(ByteBuffer buffer){
		//当前数据包处理状态。
		byte status = (byte)0xff; 
		byte cur = 0;
		byte cmd = 0;
		short len = 0;
		
		byte[] data = null;
		int position = 0;
		short checksum = 0;
		while(buffer.hasRemaining()){
			cur = buffer.get();
			if(status != 0 && cur != status) continue;
			if(status == 0xff){	//开始标志
				cmd = buffer.get();
				len = buffer.getShort();
				data = new byte[len];				
				status = 0;
				position = 0;
			}else if(status == 0){ //数据处理
				if(cur == 0xFD);
				cur += buffer.get(); //转义。
				data[position] = cur;
				position++;
				if(position == len){
					checksum = buffer.getShort();
					status = (byte)0xfe;
					if(checksum == this.getCheckSum(data)){
						processData(cmd, data);
					}else {
						log.warn("check sum error, drop data");
					}
				}
			}else if(status == 0xfe){
				status = (byte)0xff;	//当前读到结束标志了，等待下一个开始标志。
			}			
		}
		
	}
	
	public short getCheckSum(byte[] data){
		short sum = 0;
		for(byte b:data){
			sum += b;
		}
		return sum;
	}
	
	/**
	 * 发送一个终端命令，封装了“图像监控系统通信协议v1.34".
	 * @param cmd
	 * @param data
	 * @throws IOException 
	 */
	public void sendCommand(byte cmd, byte data[]) throws IOException{
		synchronized(outBuffer){
			outBuffer.reset();
			outBuffer.put((byte)0xff);
			outBuffer.put(cmd);
			outBuffer.putShort((short)data.length);
			for(int b: data){	//处理数据转义。
				if(b < 0xfd){
					outBuffer.put((byte)b);
				}else {
					outBuffer.put((byte)0xfd);
					outBuffer.put((byte)(b - 0xfd));
				}
			}
			
			outBuffer.putShort(getCheckSum(data));
			outBuffer.put((byte)0xfe);
			if(mx != null){
				mx.send(this, outBuffer);
			}else {
				log.warn("Can't send data, the client have not register to MX");
			}
		}
	}
	
	public void processData(byte cmd, byte data[]){
		if(this.lastCmd == 0x02){
			
		}
	}
	
	public void setASC100MX(ASC100MX mx){
		this.mx = mx;
	}
	
	public String getClientId(){
		return this.location;
	}
	
	public void readImage(){
		try {
			sendCommand((byte)0x02, new byte[]{06});
		} catch (IOException e) {
			log.error(e.toString(), e);
		}
	}
}
