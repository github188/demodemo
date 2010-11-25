package org.goku.odip;

import java.io.IOException;
import java.net.ConnectException;
import java.net.InetSocketAddress;
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
	protected Log log = null; 
	private BaseStation info = null;
	public Collection<MonitorClientListener> ls = Collections.synchronizedCollection(new ArrayList<MonitorClientListener>());
	
	/**
	 * 网络连接的操作对象，可以得到SocketChannel.
	 */
	protected SelectionKey selectionKey = null;
	protected SocketChannel socketChannel = null;
	
	/**
	 * 当前处理Client事件的对象，类似一个状态机。某一个时刻，只能有一个状态。
	 */
	protected ODIPHandler handler = null;
	
	public MonitorClient(BaseStation info){
		this.info = info;
		log = LogFactory.getLog("node." + info.uuid);		
	}
	
	/**
	 * 连接客户端，后注册到ChannelSelector，如果有可读／数据由Selector触发一个事件。
	 * @param selector
	 */
	public void connect(ChannelSelector selector) throws IOException{		
		String[] address = this.info.locationId.split(":");
		socketChannel = SocketChannel.open();
		socketChannel.socket().setSoTimeout(5 * 1000);
		socketChannel.configureBlocking(false);
		socketChannel.connect(new InetSocketAddress(address[0], Integer.parseInt(address[1])));		
		log.info("connecting to " + address[0] + ":" + address[1]);		
		selector.register(socketChannel, SelectionKey.OP_CONNECT, this);
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
		this.ls.add(l);
	}
	
	public void removeListener(MonitorClientListener l){
		this.ls.remove(l);
	}		
	
	protected MonitorClientListener eventProxy = new MonitorClientListener(){

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
		} catch (IOException e) {
			log.error(e.toString(), e);
			this.selectionKey.cancel();
		}
	}
	
	/**
	 * 读Socket缓冲区，并处理所有数据。
	 * @param channel
	 * @throws IOException
	 */
	protected void read(SocketChannel channel) throws IOException{
		ByteBuffer buffer = null;
		while(true){
			buffer = handler.getDataBuffer(); //ByteBuffer.allocate(1024 * 64);
			int readLen = channel.read(buffer);
			if(readLen == -1){
				this.readChannelClosed();
				break;
			}else if(buffer.hasRemaining()){ //未读满缓冲区，当前数据处理完成。
				break;
			}else {
				//如果当前缓冲区读满了，开始处理数据。
				this.handler.processData(this);
				//处理完成后，重新开始读协议头。
				this.handler.resetBuffer();
			}
		}
	}
	
	public void read(ByteBuffer buffer) throws IOException{
		if(this.selectionKey.isReadable()){
			int readLen = this.socketChannel.read(buffer);
			if(readLen == -1){
				this.readChannelClosed();
			}
		}
	}
	
	protected void write(byte[] src) throws IOException{
		ByteBuffer buffer = ByteBuffer.allocate(1024 * 64);
		buffer.put(src);
		buffer.flip();
		this.socketChannel.write(buffer);
	}
	
	protected void write(ByteBuffer src) throws IOException{
		this.socketChannel.write(src);
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
