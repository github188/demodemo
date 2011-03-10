package org.goku.video.odip;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SelectableChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;
import java.util.ArrayDeque;
import java.util.Queue;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.MonitorChannel;
import org.goku.socket.ChannelHandler;
import org.goku.socket.NIOSocketChannel;
import org.goku.socket.SelectionHandler;

public class VideoChannel implements Runnable, SelectionHandler, NIOSocketChannel{
	
	private Log log = null;
	/**
	 * 网络连接的操作对象，可以得到SocketChannel.
	 */
	protected SelectionKey selectionKey = null;
	//protected SocketChannel socketChannel = null;	
	public MonitorClient client = null;
	private Queue<ByteBuffer> writeQueue = new ArrayDeque<ByteBuffer>(5);	
	protected ODIPHandler handler = null;	
	
	private long lastBenckTime = 0, readSize = 0;
	//private long startDropTime = 0;
	private double videoSpeed = 0;
	private long lastActiveTime = 0;
	
	//摄像头通道号.
	public MonitorChannel channel = null;
	private Runnable startUp = null;
	
	public VideoChannel(MonitorClient client, MonitorChannel channel, Runnable startUp){
		this.client = client;
		this.log = LogFactory.getLog("node." + client.info.uuid + ".ch" + channel.id);
		this.channel = channel;
		this.handler = new ODIPHandler(client, this);
		this.handler.isVideoChannel = true;
		this.startUp = startUp;
	}

	@Override
	public void run() {
		synchronized(this){
			try {
				if(this.selectionKey.isConnectable()){
					try{
						((SocketChannel)selectionKey.channel()).finishConnect();
						this.selectionKey.interestOps(selectionKey.interestOps() | SelectionKey.OP_READ);
						this.writeQueue.clear();
						this.initVideoAuth();
						this.startUp.run();
					}catch(IOException conn){
						log.warn("Failed to create video channel:" + conn.toString());
						selectionKey.channel().close();
						this.selectionKey.cancel();
					}
				}else if(this.selectionKey.isReadable()){
					this.read((SocketChannel)selectionKey.channel());
					if(this.selectionKey.isValid()){
						this.selectionKey.interestOps(selectionKey.interestOps() | SelectionKey.OP_READ);
						this.selectionKey.selector().wakeup();
					}
				}
				if(this.selectionKey.isValid() && this.selectionKey.isWritable() && this.writeQueue.size() > 0){
					this.writeBuffer((SocketChannel)selectionKey.channel());
				}
			} catch (Exception e) {
				log.error(e.toString(), e);				
				this.selectionKey.cancel();
			}
		}
	}
	
	private synchronized void initVideoAuth(){
		this.handler.videoStreamAuth(this.channel.id);
		this.notifyAll();
	}
	
	/**
	 * 读Socket缓冲区并处理
	 * @param channel
	 * @throws IOException
	 */
	protected void read(SocketChannel channel) throws IOException{
		ByteBuffer buffer = null;
		int odipCount = 0;
		long st = System.currentTimeMillis();
		while(odipCount < 10){	//最多一次处理10个OIDP协议包就开始切换, 如果服务器没有性能问题，缓冲区应该低于3个ODIP包。
			odipCount++;
			buffer = handler.getDataBuffer(); //ByteBuffer.allocate(1024 * 64);
			int readLen = channel.read(buffer);
			if(readLen > 0){
				this.readSize += readLen;
				if(System.currentTimeMillis() - this.lastBenckTime > 1000){
					videoSpeed = this.readSize * 1.0 / (System.currentTimeMillis() - this.lastBenckTime);
					this.readSize = 0;
					this.lastBenckTime = System.currentTimeMillis();
					
					if(log.isDebugEnabled()){
						log.debug(String.format("The video channel '%s' read speed %1.3f Kb/s.", this.toString(), this.videoSpeed));
					}
				}
			}else if(readLen == -1){
				this.closeSocketChannel();
				break;
			}else if(!buffer.hasRemaining()){
				//如果当前缓冲区读满了，开始处理数据。
				this.handler.processData();
			}else{	//缓冲区没有读满
				break;
			}
		}
		
		if(odipCount > 5 || System.currentTimeMillis() - st > 5){
			log.warn(String.format("The Video process too slow, once process spend %s ms, %s video in buffer.", 
					System.currentTimeMillis() - st, odipCount));
		}
		
		/**
		 * 如果30秒没有任何写操作，发送一个告警查询，避免服务端超时断开。
		 */
		if(System.currentTimeMillis() - lastActiveTime > 30 * 1000){
			this.ackActive();
		}
	}	
	
	public void ackActive(){
		
	}
	
	protected void writeBuffer(SocketChannel channel) throws IOException{
		synchronized(this.writeQueue){
			//用来计算timeOut.
			this.lastActiveTime = System.currentTimeMillis();

			ByteBuffer buffer = this.writeQueue.peek();
			while(buffer != null){
				log.debug("wirte to DVR Video channel:" + buffer.remaining());
				channel.write(buffer);
				if(buffer.hasRemaining()){
					this.selectionKey.interestOps(selectionKey.interestOps() | SelectionKey.OP_WRITE);
					this.selectionKey.selector().wakeup();
					break;
				}else {
					this.writeQueue.remove();
					buffer = this.writeQueue.peek();
				}
			}
			this.writeQueue.notifyAll();
		}
		//
		synchronized(channel){
			channel.notifyAll();
		}
	}	
	
	public void write(ByteBuffer src, boolean sync) {
		if(this.selectionKey == null || !this.selectionKey.isValid())return;
		//log.debug("wirte to DVR Video channel:" + src.remaining());
		if(this.writeQueue.size() > 10){
			try {
				this.closeSocketChannel();
			} catch (IOException e) {
			}
		}else if(this.writeQueue.offer(src)){
			//如果当前Socket没有注册写操作.
			if(this.writeQueue.size() == 1 &&
			  (this.selectionKey.interestOps() & SelectionKey.OP_WRITE) != SelectionKey.OP_WRITE){
				this.selectionKey.interestOps(selectionKey.interestOps() | SelectionKey.OP_WRITE);
				this.selectionKey.selector().wakeup();
			}
		}
	}	

	@Override
	public void setSelectionKey(SelectionKey key) {
		this.selectionKey = key;
	}
	
	public void closeSocketChannel() throws IOException{
		log.info("Close " + this.toString());
		if(this.selectionKey != null){
			this.selectionKey.channel().close();
			this.selectionKey.cancel();
		}
		if(this.channel != null){
			this.channel.videoChannel = null;
		}
	}

	@Override
	public void read(ByteBuffer buffer) throws IOException {
		if(this.selectionKey.isReadable()){
			int readLen = ((SocketChannel)this.selectionKey.channel()).read(buffer);
			if(readLen == -1){
				this.closeSocketChannel();
			}else {
				this.readSize += readLen;
			}
		}
	}

	public String toString(){
		return String.format("Video channel %s<%s>.", this.client.info.uuid, this.channel.id);
	};	
}
