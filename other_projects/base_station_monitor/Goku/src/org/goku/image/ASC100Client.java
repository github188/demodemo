package org.goku.image;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
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
	public long lastActive = 0;

	private Log log = null;
	private ASC100MX mx = null;
	private String srId = "";
	private String channel = "";
	private String defaultMx = null;
	private ByteBuffer outBuffer = ByteBuffer.allocate(64 * 1024);
	private ImageInfo image = null;
	private ASC100Package inBuffer = new ASC100Package();
	
	
	public ASC100Client(String location){
		if(location.indexOf(':') > 0){
			String[] tmp = location.split(":", 2);
			if(tmp[0].trim().length() > 0){
				defaultMx = tmp[0].trim();
			}
			tmp = tmp[1].split("\\.", 3);
			if(tmp.length < 3) throw new Error("Error Image client location:" + location);
			this.srId = tmp[0] + "." + tmp[1];
			this.channel = tmp[2];
		}
		
		log = LogFactory.getLog("asc100." + srId + "." + this.channel);
		outBuffer.order(ByteOrder.LITTLE_ENDIAN);
		inBuffer.clear();
	};
	
	public ASC100Client(BaseStation info){
		this(info.locationId);
		this.info = info;
	};
	
	/**
	 * 开始处理一个串口通道的数据。
	 * 1. 不是每次处理的Buffer都是一个完整的图片通信协议包。
	 * @param buffer
	 */
	public void process(ByteBuffer buffer){
		//当前数据包处理状态。
		byte cur = 0;
		while(buffer.hasRemaining()){
			//log.info("current status:" + inBuffer.status);
			switch(inBuffer.status){
				case ASC100Package.STATUS_INIT:
					if(buffer.get() == (byte)0xFF){
						inBuffer.status = ASC100Package.STATUS_CMD;
					}
					break;
				case ASC100Package.STATUS_CMD:
					inBuffer.cmd = buffer.get();
					inBuffer.status = ASC100Package.STATUS_LENGTH;
					inBuffer.paddingIndex = 0;
					break;
				case ASC100Package.STATUS_LENGTH:
					if(inBuffer.escaped){ //正在读一个转义字符。
						cur = buffer.get();
						inBuffer.padding[inBuffer.paddingIndex++] = (byte)(0xfd + cur);
						inBuffer.escaped = false;
					}else {
						cur = buffer.get();
						if(cur == (byte)0xfd){
							inBuffer.escaped = true;
						}else if(cur == (byte)0xfe){
							inBuffer.clear();
							eventProxy.connectionError(new ImageClientEvent(this));
						}else {
							inBuffer.padding[inBuffer.paddingIndex++] = cur;
						}
					}
					if(inBuffer.paddingIndex >=2){
						inBuffer.len = ((inBuffer.padding[1] << 8) | 0x00ff) & 
						 			    (inBuffer.padding[0]       | 0xff00); 
						inBuffer.inBuffer.limit(inBuffer.len);
						inBuffer.status = ASC100Package.STATUS_DATA;
						inBuffer.paddingIndex = 0;
						log.debug(String.format("Reading cmd:%x, length:%s", inBuffer.cmd, inBuffer.len));
					}
					break;
				case ASC100Package.STATUS_DATA:
					//图片传输数据，不需要转义数据里面的. 0xFF,0xFE等字符。
					if (inBuffer.cmd == 0x06 && inBuffer.len > 0){
						inBuffer.inBuffer.put(buffer);
					}else if(inBuffer.escaped){ //正在读一个转义字符。
						cur = buffer.get();
						inBuffer.inBuffer.put((byte)(0xfd + cur));
						inBuffer.escaped = false;
					}else {
						cur = buffer.get();
						if(cur == (byte)0xfd){
							inBuffer.escaped = true;
						}else if(cur == (byte)0xfe){
							inBuffer.clear();
							eventProxy.connectionError(new ImageClientEvent(this));
						}else {
							inBuffer.inBuffer.put(cur);
						}
					}
					if(!inBuffer.inBuffer.hasRemaining()){
						inBuffer.inBuffer.flip();
						inBuffer.status = ASC100Package.STATUS_CHECKSUM;
					}
					break;
				case ASC100Package.STATUS_CHECKSUM:
					if(inBuffer.escaped){ //正在读一个转义字符。
						cur = buffer.get();
						inBuffer.padding[inBuffer.paddingIndex++] = (byte)(0xfd + cur);
						inBuffer.escaped = false;
					}else {
						cur = buffer.get();
						if(cur == (byte)0xfd){
							inBuffer.escaped = true;
						}else if(cur == (byte)0xfe){
							inBuffer.clear();
							eventProxy.connectionError(new ImageClientEvent(this));
						}else {
							inBuffer.padding[inBuffer.paddingIndex++] = cur;
						}
					}	
					if(inBuffer.paddingIndex >=2){
						inBuffer.checkSum =  (short)(((inBuffer.padding[1] << 8) | 0x00ff) & 
		 			    					          (inBuffer.padding[0]       | 0xff00));
						//inBuffer.inBuffer.flip();
						//ByteBuffer tmp = inBuffer.inBuffer.asReadOnlyBuffer();
						//tmp.get();
						inBuffer.bufferCheckSum = getCheckSum(
								new byte[]{(byte)inBuffer.cmd, (byte)(inBuffer.len & 0xff), (byte)(inBuffer.len >> 8 & 0xff)},
								inBuffer.inBuffer.asReadOnlyBuffer());
						processData(inBuffer);
						inBuffer.status = ASC100Package.STATUS_END;
						inBuffer.paddingIndex = 0;
					}
					break;
				case ASC100Package.STATUS_END:
					if(buffer.get() == (byte)0xFE){
						inBuffer.clear();						
					}else if(buffer.get() == (byte)0xFF){
						inBuffer.clear();
						inBuffer.status = ASC100Package.STATUS_CMD;
					}
					
					break;
				default:
					log.warn("Invalid package status:%s" + ASC100Package.STATUS_END);
					inBuffer.clear();
			}			
		}
		
		//报告设备处于活动状态。
		if(System.currentTimeMillis() - this.lastActive > 60 * 1000){
			this.eventProxy.active(new ImageClientEvent(this));
			this.lastActive = System.currentTimeMillis();
		}
	}
	
	public short getCheckSum(byte[] head, ByteBuffer data){
		ByteBuffer temp = ByteBuffer.allocate(head.length + data.remaining());
		temp.put(head);
		temp.put(data);
		temp.flip();
		return getCheckSum(temp);
	}
	
	public short getCheckSum(ByteBuffer data){
		int sum = 0;
		while(data.hasRemaining()){
			sum += data.get();
		}
		//sum = ((~(sum % 0x10000)) + 1) & 0xffff;
		
		return (short)(((~(sum % 0x10000)) + 1) & 0xffff);
	}	
	
	/**
	 * 发送一个终端命令，封装了“图像监控系统通信协议v1.34".
	 * @param cmd
	 * @param data
	 * @throws IOException 
	 */
	public void sendCommand(byte cmd, byte data[]) throws IOException{
		ByteBuffer temp = ByteBuffer.allocate(data.length +5);
		temp.order(ByteOrder.LITTLE_ENDIAN);
		temp.clear();
		temp.put(cmd);
		temp.putShort((short)data.length);
		temp.put(data);
		temp.flip();
		short checkSum = getCheckSum(temp);
		temp.limit(temp.capacity());
		temp.putShort(checkSum);
		temp.flip();
		
		synchronized(outBuffer){
			outBuffer.clear();
			outBuffer.put((byte)0xff);
			byte cur = 0;
			while(temp.hasRemaining()){
				cur = temp.get();
				switch(cur){
					case (byte)0xff:
						outBuffer.put((byte)0xfd);
						outBuffer.put((byte)2);
						break;
					case (byte)0xfe:
						outBuffer.put((byte)0xfd);
						outBuffer.put((byte)1);
						break;
					case (byte)0xfd:
						outBuffer.put((byte)0xfd);
						outBuffer.put((byte)0);
						break;
					default:
						outBuffer.put(cur);
				}
			}
			
			outBuffer.put((byte)0xfe);
			if(mx != null){
				outBuffer.flip();
				mx.send(this, outBuffer);
			}else {
				log.warn("Can't send data, the client have not register to MX");
			}
		}
	}
	
	public void processData(ASC100Package data){
		if(data.checkSum != data.bufferCheckSum){
			log.debug(String.format("Drop package the check sum error. excepted:%x, actual:%x", data.checkSum, data.bufferCheckSum));
		}else {
			log.debug(String.format("Client command:0x%x, length:%s", data.cmd, data.len));
			if(data.cmd == 0x00){
				eventProxy.notFoundImage(new ImageClientEvent(this));
			}else if(data.cmd == 0x06){
				try {
					processImageData(data.inBuffer);
				} catch (IOException e) {
					log.error(e.toString(), e);
				}
			}else{
				log.debug(String.format("Drop unkown command:0x, length:%s", data.cmd, data.len));
			}
		}
	}

	
	private void processImageData(ByteBuffer inBuffer) throws IOException{
		int count = inBuffer.getShort();
		int curFrame = inBuffer.getShort();
		int len = inBuffer.getShort();
		int tem = inBuffer.get();
		len += (tem << 24);
				
		if(curFrame == 0){
			inBuffer.getShort();
			image = new ImageInfo();
			image.setFameCount(count);
			image.setDataSize(len);
			image.imageStatus = inBuffer.get();
			image.channel = inBuffer.get();
			
			int xxLen = inBuffer.getShort();
			tem = inBuffer.get();
			len += (tem << 24);
			if(len != xxLen){
				log.error(String.format("The picture length error, %s(head) != %s(picdata)", len, xxLen));
			}
			image.imageSize = inBuffer.get();
			image.zipRate = inBuffer.get();
			this.sendCommand((byte)0x21, new byte[]{});
		}else if (image != null){
			int paddingLen = inBuffer.getShort();
			if(paddingLen == inBuffer.remaining()){
				image.recieveData(curFrame, inBuffer);
			}else {
				log.error(String.format("The picture data error, %s(remaining) != %s(buffer)", paddingLen, inBuffer.remaining()));				
			}
			if(!image.haveMoreData()){
				int[] retry = image.getReTryFrames();
				if(retry == null){
					sendRetryFrame(new int[]{});
					image.buffer.position(0);
					ImageClientEvent event = new ImageClientEvent(this);
					event.image = this.image;
					this.eventProxy.recevieImageOK(event);
					this.image = null;
				}else {
					sendRetryFrame(retry);
				}
			}
		}else {
			log.error(String.format("Get picture data, but not found picture head."));
		}
	}
	
	private void sendRetryFrame(int[] frames) throws IOException{
		outBuffer.clear();
		outBuffer.putShort((short)frames.length);
		for(int i: frames){
			outBuffer.putShort((short)i);
		}
		byte[] data = new byte[outBuffer.position()];
		outBuffer.flip();
		outBuffer.get(data);
		this.sendCommand((byte)0x20, data);
	}
	
	public void setASC100MX(ASC100MX mx){
		this.mx = mx;
	}
	
	public String getClientId(){
		return srId + "." + channel;
	}
	
	public String getSrId(){
		return srId;
	}
	
	public String defaultMx(){
		return defaultMx;
	}
	
	public void getAlarmImage(){
		try {
			sendCommand((byte)0x02, new byte[]{06});
		} catch (IOException e) {
			log.error(e.toString(), e);
		}
	}
	
	/**
	 * 点播实时图片。
	 */
	public void getRealImage(int channel){
		byte cmd = 0;
		switch(channel){
			case 1: cmd = 0x02; break;
			case 2: cmd = 0x02; break;
			case 3: cmd = 0x07; break;
			case 4: cmd = 0x08; break;
		}
		if(cmd != 0){
			try {
				sendCommand((byte)0x02, new byte[]{06});
			} catch (IOException e) {
				log.error(e.toString(), e);
			}
		}else {
			log.debug("Invalid image channel:" + channel);
		}
	}
	
	public void getDateTime(){
		try {
			sendCommand((byte)0x02, new byte[]{01});
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
			//在Listener处理过程中，可能需要修改Listener
			Collection<ImageClientListener> lss = new ArrayList<ImageClientListener>();
			lss.addAll(ls);
			for(ImageClientListener l: lss){
				l.recevieImageOK(event);
			}
		}

		@Override
		public void notFoundImage(ImageClientEvent event) {
			for(ImageClientListener l: ls){
				l.notFoundImage(event);
			}
		}

		@Override
		public void connectionError(ImageClientEvent event) {
			for(ImageClientListener l: ls){
				l.connectionError(event);
			}
		}

		@Override
		public void message(ImageClientEvent event) {
			for(ImageClientListener l: ls){
				l.message(event);
			}
		}
		
		@Override
		public void active(ImageClientEvent event) {
			for(ImageClientListener l: ls){
				l.active(event);
			}
		}	
	};

}
