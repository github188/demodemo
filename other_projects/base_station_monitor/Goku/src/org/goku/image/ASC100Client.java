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
	public ASC100Package inBuffer = new ASC100Package();

	protected Log log = null;
	protected ASC100MX mx = null;
	private String srId = "";
	private String channel = "";
	private String defaultMx = null;
	private ByteBuffer outBuffer = ByteBuffer.allocate(64 * 1024);
	private ImageInfo image = null;
	private byte lastCmd = 0;
	private long readCount = 0;
	private long lastBenchTime = 0;
	
	
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
		lastBenchTime = System.currentTimeMillis();
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
		readCount += buffer.remaining();
		if(System.currentTimeMillis() - this.lastBenchTime > 1000){
			log.debug(String.format("Read %s byte in %1.2f", readCount, (System.currentTimeMillis() - this.lastBenchTime) / 1000.0));
		}
		ImageClientEvent event = new ImageClientEvent(this);
		event.raw = buffer.asReadOnlyBuffer();
		eventProxy.debugRawData(event);		
		
		//当前数据包处理状态。
		byte cur = 0;
		byte last = 0, last2 = 0;
		//autoEscaped
		
		while(buffer.hasRemaining()){
			cur = buffer.get();
			//数据需要转义处理。
			if(inBuffer.autoEscaped){
				if(cur == (byte)0xFF ||cur == (byte)0xFE){
					inBuffer.clear();
					continue;
				}else if(cur == (byte)0xFD){
					inBuffer.escaped = true;
					continue;
				}else if(inBuffer.escaped){
					cur = (byte)(0xfd + cur);
					inBuffer.escaped = false;
				}
			}
			//如果传输图片的时候出现中间掉包了，修正协议头。
			if(inBuffer.cmd == 0x06 && this.image != null){
				if(last == (byte)0xff && last2 == (byte)0xfe && cur == 06){
					log.debug("Drop incomplete image package, lengeth:" + buffer.position());
					inBuffer.status = ASC100Package.STATUS_LENGTH;
					inBuffer.paddingIndex = 0;
					continue;
				}else {
					last2 = last;
					last = cur;
				}
			}
			switch(inBuffer.status){
				case ASC100Package.STATUS_CMD:
					if(cur != 0 && cur != 1 && cur != 2 && cur != 6){
						inBuffer.clear();
						break;
					}
					inBuffer.cmd = cur;
					inBuffer.status = ASC100Package.STATUS_LENGTH;
					inBuffer.paddingIndex = 0;
					//图片数据不需要转义。
					if(inBuffer.cmd == 0x06 && this.image != null){
						inBuffer.autoEscaped = false;
					}
					
					log.debug(String.format("Reading cmd:%x, ", inBuffer.cmd));
					break;
				case ASC100Package.STATUS_LENGTH:
					if(inBuffer.paddingIndex < 2){
						inBuffer.padding[inBuffer.paddingIndex++] = cur;
					}
					if(inBuffer.paddingIndex >=2){
						inBuffer.len = ((inBuffer.padding[1] << 8) | 0x00ff) & 
						 			    (inBuffer.padding[0]       | 0xff00); 
						if(inBuffer.len > 0 && inBuffer.len < 1024 * 10){
							log.debug(String.format("Reading cmd:%x, length:%s, b1:0x%x, b2:0x%x", inBuffer.cmd, inBuffer.len, inBuffer.padding[0], inBuffer.padding[1]));
							inBuffer.inBuffer.clear();
							inBuffer.inBuffer.limit(inBuffer.len);
							inBuffer.status = ASC100Package.STATUS_DATA;
							inBuffer.paddingIndex = 0;
							if(inBuffer.cmd == 0x06 && this.image != null && inBuffer.len > 24){
								inBuffer.inBuffer.limit(inBuffer.len -2);
								log.debug("read image data length:" +inBuffer.len);
							}							
						}else {
							log.debug(String.format("Error length data:%s, b1:0x%x, b2:0x%x", inBuffer.len, inBuffer.padding[0], inBuffer.padding[1]));
							inBuffer.clear();
							eventProxy.connectionError(new ImageClientEvent(this));
							//System.exit(1);
						}
					}
					break;
				case ASC100Package.STATUS_DATA:
					//图片传输数据，不需要转义数据里面的. 0xFF,0xFE等字符。					
					inBuffer.inBuffer.put(cur);					
					if(!inBuffer.inBuffer.hasRemaining()){
						inBuffer.inBuffer.flip();
						inBuffer.status = ASC100Package.STATUS_CHECKSUM;
						inBuffer.paddingIndex = 0;
					}
					break;
				case ASC100Package.STATUS_CHECKSUM:
					if(inBuffer.paddingIndex < 2){
						inBuffer.padding[inBuffer.paddingIndex++] = cur;
					}
					if(inBuffer.paddingIndex >=2){
						inBuffer.checkSum =  (short)(((inBuffer.padding[1] << 8) | 0x00ff) & 
		 			    					          (inBuffer.padding[0]       | 0xff00));
						inBuffer.bufferCheckSum = getCheckSum(
								new byte[]{(byte)inBuffer.cmd, (byte)(inBuffer.len & 0xff), (byte)(inBuffer.len >> 8 & 0xff)},
								inBuffer.inBuffer.asReadOnlyBuffer());
						processData(inBuffer);
						inBuffer.status = ASC100Package.STATUS_END;
						inBuffer.paddingIndex = 0;
						//无论是否在传图片数据，读到一个包结束始终进入转义状态。
						inBuffer.autoEscaped = true;
					}
					break;
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
		byte b = 0; 
		while(data.hasRemaining()){
			b = data.get();
			sum += b;
			if(b < 0) sum += 256;
			sum = sum % 0x10000;
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
		
		this.lastCmd = cmd;
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
			if(data.cmd == 0x06 && image != null){
				image.waitingFrames--;
			}
		}else {
			log.debug(String.format("process ASC100 message:0x%x, length:%s, remaining:%s", data.cmd, data.len, data.inBuffer.remaining()));
			if(data.cmd == 0x06){
				try {
					processImageData(data.inBuffer);
				} catch (IOException e) {
					log.error(e.toString(), e);
				}
			}else {
				ImageClientEvent event = new ImageClientEvent(this);
				event.data = data;
				this.eventProxy.message(event);
			}
		}
	}

	
	private void processImageData(ByteBuffer inBuffer) throws IOException{
		int count = ASC100MX.unsignedShort(inBuffer); // inBuffer.getShort();
		int curFrame = ASC100MX.unsignedShort(inBuffer);
		int len = ASC100MX.unsignedShort3(inBuffer);
				
		if(curFrame == 0){
			inBuffer.getShort();
			image = new ImageInfo();
			image.setFameCount(count);
			image.setDataSize(len);
			image.imageStatus = inBuffer.get();
			image.channel = inBuffer.get();
			
			int xxLen = ASC100MX.unsignedShort3(inBuffer); //inBuffer.getShort();
			if(len != xxLen){
				log.error(String.format("The picture length error, %s(head) != %s(picdata)", len, xxLen));
			}
			image.imageSize = inBuffer.get();
			image.zipRate = inBuffer.get();
			log.debug(String.format("Image info ch:%s, status:%s, imageSize:%s, zipRate:%s, len:%s, frames:%s",
					image.channel, image.imageStatus, image.imageSize, image.zipRate, len, count));
			image.waitingFrames = count -1;
			this.sendCommand((byte)0x21, new byte[]{});			
		}else if (image != null){
			int paddingLen = ASC100MX.unsignedShort(inBuffer);
			image.waitingFrames--;
			if(paddingLen == inBuffer.remaining()){
				log.debug(String.format("Image recieve frame:%s# size:%s, count:%s, remaining:%s", curFrame, inBuffer.remaining(), count, image.waitingFrames));				
				image.recieveData(curFrame, inBuffer);
			}else {
				log.error(String.format("The picture data error, %s(remaining) != %s(buffer)", paddingLen, inBuffer.remaining()));				
			}
			if(image.waitingFrames <= 0){
				int[] retry = image.getReTryFrames();
				image.waitingFrames = retry.length;			
				if(retry == null || retry.length == 0){
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
		String tmp = "";
		for(int i : frames) {
			tmp += " " + i;
		}
		log.debug("sendRetryFrame:" + tmp);
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
		this.image = null;
		try {
			sendCommand((byte)0x02, new byte[]{06});
		} catch (IOException e) {
			log.error(e.toString(), e);
		}
	}
	
	public void restart(){
		this.image = null;
		try {
			sendCommand((byte)0x03, new byte[]{05});
		} catch (IOException e) {
			log.error(e.toString(), e);
		}		
	}
	
	/**
	 * 点播实时图片。
	 */
	public void getRealImage(int channel){
		this.image = null;
		byte cmd = 0;
		switch(channel){
			case 1: cmd = 0x02; break;
			case 2: cmd = 0x03; break;
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
		
		public void debugRawData(ImageClientEvent event) {
			for(ImageClientListener l: ls){
				l.debugRawData(event);
			}			
		}
	};

}
