package org.goku.image;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.BaseStation;

/**
 * 图片监控客户端。
 * @author deon
 */
public class ASC100Client {
	public BaseStation info = null;
	public Collection<ImageClientListener> ls = Collections.synchronizedCollection(new ArrayList<ImageClientListener>());

	private Log log = null;
	private ASC100MX mx = null;
	private String location = "";
	private ByteBuffer outBuffer = ByteBuffer.allocate(64 * 1024);
	private ByteBuffer inBuffer = ByteBuffer.allocate(64 * 1024);
	
	private byte lastCmd = 0;
	
	public ASC100Client(String location){
		this.location = location;
		log = LogFactory.getLog("asc100." + location);
	};
	
	public ASC100Client(BaseStation info){
		this(info.locationId);
		this.info = info;
	};
	
	public void process(ByteBuffer buffer){
		//当前数据包处理状态。
		byte status = (byte)0xff; 
		short cur = 0;
		byte cmd = 0;
		short len = 0;
		
		inBuffer.clear();
		short checksum = 0;
		while(buffer.hasRemaining()){
			cur = buffer.get();
			if(status != 0 && cur != status) continue;
			if(status == 0xff){	//开始标志
				cmd = buffer.get();
				len = buffer.getShort();
				status = 0;
			}else if(status == 0){ //数据处理
				if(cur == 0xFD){
					cur += buffer.get(); //转义。
				}
				len--;
				inBuffer.put((byte)cur);
				if(len == 0){
					checksum = buffer.getShort();
					status = (byte)0xfe;
					inBuffer.flip();
					if(checksum == this.getCheckSum(inBuffer.asReadOnlyBuffer())){
						processData(cmd, inBuffer);
					}else {
						log.warn("check sum error, drop data");
					}
				}
			}else if(status == 0xfe){
				status = (byte)0xff;	//当前读到结束标志了，等待下一个开始标志。
			}			
		}
		
	}
	
	public short getCheckSum(ByteBuffer data){
		short sum = 0;
		while(data.hasRemaining()){
			sum += data.get();
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
		ByteBuffer temp = null;
		synchronized(outBuffer){
			outBuffer.clear();
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
			temp = outBuffer.asReadOnlyBuffer();
			temp.limit(outBuffer.position());
			temp.position(4);
			
			outBuffer.putShort(getCheckSum(temp));
			outBuffer.put((byte)0xfe);
			if(mx != null){
				mx.send(this, outBuffer);
			}else {
				log.warn("Can't send data, the client have not register to MX");
			}
		}
	}
	
	public void processData(byte cmd, ByteBuffer inBuffer){
		if(this.lastCmd == 0x02){
			if(cmd == 0x00){
				eventProxy.notFoundImage(new ImageClientEvent(this));
			}else if(cmd == 0x06){
				processImageData(inBuffer);
			}
		}
	}
	
	private void processImageData(ByteBuffer inBuffer){
		
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
	
	public void addListener(ImageClientListener l){
		if(!this.ls.contains(l)){
			this.ls.add(l);
		}
	}
	
	public void removeListener(ImageClientListener l){
		if(this.ls.contains(l)){
			this.ls.remove(l);
		}
	}
	
	public ImageClientListener eventProxy = new ImageClientListener(){

		@Override
		public void recevieImageOK(ImageClientEvent event) {
			for(ImageClientListener l: ls){
				l.recevieImageOK(event);
			}
		}

		@Override
		public void notFoundImage(ImageClientEvent event) {
			for(ImageClientListener l: ls){
				l.notFoundImage(event);
			}
		}
	};
}
