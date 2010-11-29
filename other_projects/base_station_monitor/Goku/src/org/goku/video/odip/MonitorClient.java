package org.goku.video.odip;

import java.io.IOException;
import java.net.ConnectException;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.BaseStation;


/**
 * 监控客户端，处理于摄像头的交互。
 * @author deon
 */
public class MonitorClient implements Runnable{
	public BaseStation info = null;
	public Collection<MonitorClientListener> ls = Collections.synchronizedCollection(new ArrayList<MonitorClientListener>());
	
	public VideoRoute route = null;
	
	protected Log log = null; 
	/**
	 * 网络连接的操作对象，可以得到SocketChannel.
	 */
	protected SelectionKey selectionKey = null;
	protected SocketChannel socketChannel = null;
	protected SocketManager socketManager = null;
	
	/**
	 * 当前处理Client事件的对象，类似一个状态机。某一个时刻，只能有一个状态。
	 */
	protected ODIPHandler handler = null;
	
	public MonitorClient(BaseStation info, VideoRoute route, SocketManager socketManager){
		this.info = info;
		log = LogFactory.getLog("node." + info.uuid);
		
		this.route = route;
		this.socketManager = socketManager;
		this.handler = new ODIPHandler(this);
	}
	
	/** 
	 * @param selector
	 */
	public void connect() throws IOException{
		String[] address = this.info.locationId.split(":");
		this.socketChannel = this.socketManager.connect(address[0],
				Integer.parseInt(address[1]),
				this);
	}
	
	public void checkAlarm(){
		
	}
	
	public void login(){
		
	}
	
	public void realPlay(){
		
	}
	
	public void openSound(){
		
	}
	
	/**
	 * 查询回放记录。
	 */
	public void queryRecordFile(){
		
	}
	
	public void downloadByRecordFile(){
		
	}


	public void close(){
		
	}
	
	public void addListener(MonitorClientListener l){
		if(! this.ls.contains(l)){
			this.ls.add(l);
		}
	}
	
	public void removeListener(MonitorClientListener l){
		if(this.ls.contains(l)){
			this.ls.remove(l);
		}
	}
	
	public MonitorClientListener eventProxy = new MonitorClientListener(){

		@Override
		public void connected(MonitorClientEvent event) {
			for(MonitorClientListener l: ls){
				l.connected(event);
			}
		}

		@Override
		public void disconnected(MonitorClientEvent event) {
			for(MonitorClientListener l: ls){
				l.disconnected(event);
			}
		}
		
		@Override
		public void timeout(MonitorClientEvent event) {
			for(MonitorClientListener l: ls){
				l.timeout(event);
			}
		}
		
		@Override
		public void alarm(MonitorClientEvent event) {
			for(MonitorClientListener l: ls){
				l.alarm(event);
			}
		}	
		
		@Override
		public void writeIOException(MonitorClientEvent event) {
			for(MonitorClientListener l: ls){
				l.writeIOException(event);
			}
		}		
		
		@Override
		public void loginError(MonitorClientEvent event) {
			for(MonitorClientListener l: ls){
				l.loginError(event);
			}
		}				
	};
	
	/**
	 * 由Selection线程调用。
	 * @param key
	 */
	public void setSelectionKey(SelectionKey key){
		this.selectionKey = key;
	}
	
	/**
	 * 有需要处理的事件时，Selector使用一个后台线程调用run方法，处理当前的事件。
	 * 
	 * Run只有一个线程会运行，当前数据处理完成后，需要修改interestOps，重新触发
	 * select.
	 */
	@Override
	public void run() {
		synchronized(this){
			try {
				//log.info("connected:" + socketChannel.isConnected() + ", " + Thread.currentThread().getId());
				if(this.selectionKey.isConnectable()){
					//log.info("xxx2:" + new Date());
					try{
						socketChannel.finishConnect();
						this.eventProxy.connected(new MonitorClientEvent(this));
						this.selectionKey.interestOps(SelectionKey.OP_READ);
						this.selectionKey.selector().wakeup();
					}catch(ConnectException conn){
						this.selectionKey.cancel();
						this.eventProxy.timeout(new MonitorClientEvent(this));
					}
					//log.info("Change select ops as READ.");
				}else if(this.selectionKey.isReadable()){
					this.read(socketChannel);
					if(this.selectionKey.isValid()){
						this.selectionKey.interestOps(SelectionKey.OP_READ);
						this.selectionKey.selector().wakeup();
					}
				}
			} catch (Exception e) {
				log.error(e.toString(), e);
				this.selectionKey.cancel();
			}
		}
	}
	
	/**
	 * 读Socket缓冲区并处理, 每次最多只处理一个ODIP协议报文，避免一个线程长时间被占
	 * 用。其他终端无法处理。
	 * @param channel
	 * @throws IOException
	 */
	protected void read(SocketChannel channel) throws IOException{
		ByteBuffer buffer = null;
		buffer = handler.getDataBuffer(); //ByteBuffer.allocate(1024 * 64);
		int readLen = channel.read(buffer);
		if(readLen == -1){
			this.readChannelClosed();
		}else if(!buffer.hasRemaining()){
			//如果当前缓冲区读满了，开始处理数据。
			this.handler.processData(this);
			//处理完成后，重新开始读协议头。
			this.handler.resetBuffer();
		}
	}
	
	/**
	 * 在ODIPHandler里，可能需要根据协议头，读些数据。
	 * @param buffer
	 * @throws IOException
	 */
	public void read(ByteBuffer buffer) throws IOException{
		if(this.selectionKey.isReadable()){
			int readLen = this.socketChannel.read(buffer);
			if(readLen == -1){
				this.readChannelClosed();
			}
		}
	}
	
	protected void write(byte[] src) throws IOException{
		ByteBuffer buffer = ByteBuffer.allocate(src.length);
		buffer.put(src);
		buffer.flip();
		this.socketChannel.write(buffer);
	}
	
	protected void write(ByteBuffer src){
		/**
		 * 需要一个flush操作么？
		 */
		try{
			this.socketChannel.write(src);
		}catch(IOException e){
			this.eventProxy.writeIOException(new MonitorClientEvent(this));
		}
	}
	
	protected void readChannelClosed() throws IOException{
		log.debug("Close connection by channel read -1.");
		this.selectionKey.cancel();
		this.socketChannel.close();
		
		this.eventProxy.disconnected(new MonitorClientEvent(this));
	}
	
	//private InetSocketAddress get
	//new InetSocketAddress("127.0.0.1", 8080)
}
