package org.goku.video.odip;

import static org.goku.video.odip.Constant.CTRL_VIDEO_START;
import static org.goku.video.odip.Constant.CTRL_VIDEO_STOP;

import java.io.IOException;
import java.net.ConnectException;
import java.net.NoRouteToHostException;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Queue;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.BaseStation;
import org.goku.core.model.MonitorChannel;
import org.goku.core.model.RouteRunningStatus;
import org.goku.socket.SelectionHandler;
import org.goku.socket.SocketManager;

/**
 * 监控客户端，处理于摄像头的交互。
 * @author deon
 */
public class MonitorClient implements Runnable, SelectionHandler{
	public BaseStation info = null;
	public Collection<MonitorClientListener> ls = Collections.synchronizedCollection(new ArrayList<MonitorClientListener>());
	
	public VideoRoute route = null;
	
	public RouteRunningStatus runningStatus = new RouteRunningStatus();
	/**
	 * 监控视频通道编号，从1开始。
	 */
	//public int channelId = 0;
	public String ipAddr = "";
	public int retryError = 0;
	
	protected Log log = null;
	/**
	 * 网络连接的操作对象，可以得到SocketChannel.
	 */
	protected SelectionKey selectionKey = null;
	protected SocketChannel socketChannel = null;
	protected SocketManager socketManager = null;
	protected ClientStatus status = null;
	
	/**
	 * 当前处理Client事件的对象，类似一个状态机。某一个时刻，只能有一个状态。
	 */
	protected ODIPHandler handler = null;
	private Queue<ByteBuffer> writeQueue = new ArrayDeque<ByteBuffer>(5);
	
	//设备进入写忙状态。
	private boolean isWriteBusy = false;
	
	public MonitorClient(BaseStation info, VideoRoute route, SocketManager socketManager){
		this.info = info;
		log = LogFactory.getLog("node." + info.uuid);
		
		this.route = route;
		this.route.setLogger(log);
		this.route.start(this);

		this.socketManager = socketManager;
		this.handler = new ODIPHandler(this);		
	}
	
	/** 
	 * @param selector
	 */
	public void connect() throws IOException{
		String[] address = this.info.locationId.split(":");
		if(address.length < 2){
			throw new IOException("Invalid location Id, <host>:<port>:<channel id>");
		}
		//this.channelId = Integer.parseInt(address[2]);
		this.ipAddr = address[0] + ":" + address[1];
		this.socketChannel = this.socketManager.connect(address[0],
				Integer.parseInt(address[1]),
				this);
		this.socketChannel.socket().setReceiveBufferSize(1024 * 64);
	}
	
	public void sendAlarmRequest(){
		this.handler.getAlarmStatus_A1(0);
	}
	
	/**
	 * 登录系统。
	 */
	public void login(){
		if(this.getClientStatus() == null){
			if(this.socketChannel == null || !this.socketChannel.isOpen()){
				try {
					this.connect();
					synchronized(this.socketChannel){
						try {
							this.socketChannel.wait(30 * 1000);
						} catch (InterruptedException e) {
						}
					}
				} catch (IOException e) {
					log.warn("Connection error, error:" + e.toString());
				}
			}
			
			//在多线程时，有可能在等待期间由其他线程完成了login操作。
			if(this.getClientStatus() == null &&  
					this.socketChannel != null &&
					this.socketChannel.isConnected()
					){
				this.handler.login("", "", true);
			}else if(this.getClientStatus() == null){
				log.debug("The socket channel isn't OK, can't login to client.");
			}
		}
	}
	
	/**
	 * 开启实时监控。
	 * @param player 收实时监控视频数据。
	 */
	public void realPlay(int channel){
		this.realPlay(channel, MonitorChannel.MAIN_VIDEO);
	}
	
	public void realPlay(int channel, int mode){
		this.login();
		if(this.getClientStatus() != null){
			MonitorChannel ch = info.getChannel(channel);
			if(ch == null){
				log.warn("Not found chanel id:" + channel);
			}else {
				this.handler.videoStreamControl(CTRL_VIDEO_START, channel, mode);
			}
		}else {
			log.warn("Can't open real play in disconnected client.");
		}		
	}
	
	public void openSound(){
		
	}
	
	/**
	 * 发送响应消息，避免被超时断开连接。
	 */
	public void ackActive(){
		this.handler.getAlarmStatus_A1(0);		
	}
	
	/**
	 * 查询回放记录。
	 */
	public void queryRecordFile(){
		
	}
	
	public void downloadByRecordFile(){
		
	}
	
	/**
	 * 当视频接受端为空时调用。发送关闭视频流的命令。
	 */
	public void videoDestinationEmpty(){
		for(MonitorChannel ch: info.getChannels()){
			this.handler.videoStreamControl(CTRL_VIDEO_STOP, ch.id, MonitorChannel.MAIN_VIDEO);
		}
	}

	public void close(){
		try {
			this.closeSocketChannel();
		} catch (IOException e) {
			log.error(e, e);
		}
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
		
		@Override
		public void loginOK(MonitorClientEvent event) {
			for(MonitorClientListener l: ls){
				l.loginOK(event);
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
						this.writeQueue.clear();
						this.isWriteBusy = false;
						this.eventProxy.connected(new MonitorClientEvent(this));
						this.selectionKey.interestOps(selectionKey.interestOps() | SelectionKey.OP_READ);
						this.selectionKey.selector().wakeup();
						this.retryError = 0;
					}catch(NoRouteToHostException conn){
						this.retryError++;
						this.socketChannel.close();
						this.selectionKey.cancel();
						this.eventProxy.timeout(new MonitorClientEvent(this));
					}catch(ConnectException conn){
						this.retryError++;
						this.socketChannel.close();
						this.selectionKey.cancel();
						this.eventProxy.timeout(new MonitorClientEvent(this));
					}
					synchronized(socketChannel){
						this.socketChannel.notifyAll();
					}
				}else if(this.selectionKey.isReadable()){
					this.read(socketChannel);
					if(this.selectionKey.isValid()){
						this.selectionKey.interestOps(selectionKey.interestOps() | SelectionKey.OP_READ);
						this.selectionKey.selector().wakeup();
					}
				}
				if(this.selectionKey.isValid() && this.selectionKey.isWritable() && this.writeQueue.size() > 0){
					this.writeBuffer();
				}
			} catch (Exception e) {
				log.error(e.toString(), e);				
				this.selectionKey.cancel();
				try {
					this.closeSocketChannel();
				} catch (IOException e1) {
					log.error(e1.toString(), e);
				}
				//this.eventProxy.writeIOException(new MonitorClientEvent(this));
			}
		}
	}
	
	/**
	 * 读Socket缓冲区并处理, 每次最多只处理一个ODIP协议报文，避免一个线程长时间被占
	 * 用。其他终端无法处理。
	 * 
	 * 每次处理一个报文，线程切换太多，导致性能低下／发送也有这个问题么？
	 * @param channel
	 * @throws IOException
	 */
	protected void read(SocketChannel channel) throws IOException{
		if(this.status != null){
			this.status.lastActiveTime = System.currentTimeMillis();
		}

		ByteBuffer buffer = null;
		int odipCount = 0;
		long st = System.currentTimeMillis();
		while(odipCount < 10){	//最多一次处理10个OIDP协议包就开始切换, 如果服务器没有性能问题，缓冲区应该低于3个ODIP包。
			odipCount++;
			buffer = handler.getDataBuffer(); //ByteBuffer.allocate(1024 * 64);
			int readLen = channel.read(buffer);
			if(readLen > 0){
				if(runningStatus.receiveData(readLen)){
					log.debug(String.format("The DVR '%s' read speed %1.3f Kb/s.", info.locationId, runningStatus.readKbs));
				}
			}
			if(readLen == -1){
				this.closeSocketChannel();
				break;
			}else if(!buffer.hasRemaining()){
				//如果当前缓冲区读满了，开始处理数据。
				this.handler.processData(this);
			}else{	//缓冲区没有读满
				break;
			}
		}
		
		if(odipCount > 5 || System.currentTimeMillis() - st > 5){
			log.warn(String.format("Route server too slow, have too many buffer waiting process. once process spend %s ms, %s odip in buffer.", 
					System.currentTimeMillis() - st, odipCount));
		}		
		/**
		 * 如果30秒没有任何写操作，发送一个告警查询，避免服务端超时断开。
		 */
		if(this.status != null){
			if(System.currentTimeMillis() - status.lastActionTime > 30 * 1000){
				this.ackActive();
			}
		}
	}
	
	protected void writeBuffer() throws IOException{
		synchronized(this.writeQueue){
			//用来计算timeOut.
			if(this.status != null){
				this.status.lastActionTime = System.currentTimeMillis();
			}

			ByteBuffer buffer = this.writeQueue.peek();
			while(buffer != null){
				this.socketChannel.write(buffer);
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
	}
	
	protected void write(ByteBuffer src, boolean sync) {
		log.debug("wirte to DVR:" + src.remaining());
		if(this.selectionKey == null || !this.selectionKey.isValid())return;
		if(this.writeQueue.size() > 10){
			//如果超过 1分钟 没有读到设备任何数据。设置超时。
			if(System.currentTimeMillis() - this.runningStatus.lastReadTime > 60 * 1000){
				this.eventProxy.timeout(new MonitorClientEvent(this));
			}else if(!this.isWriteBusy){ //第一次发现写忙。
				this.isWriteBusy = true;
				log.warn("DVR is too slow or disconnected.");
			}
		}else if(this.writeQueue.offer(src)){
			this.isWriteBusy = false;
			//如果当前Socket没有注册写操作.
			if(this.writeQueue.size() == 1 &&
			  (this.selectionKey.interestOps() & SelectionKey.OP_WRITE) != SelectionKey.OP_WRITE){
				this.selectionKey.interestOps(selectionKey.interestOps() | SelectionKey.OP_WRITE);
				this.selectionKey.selector().wakeup();
			}
			while(sync && src.remaining() > 0){
				synchronized(writeQueue){
					try {
						writeQueue.wait();
					} catch (InterruptedException e) {
					}
				}
			}
		}
		//log.info("qqq:size:" + this.writeQueue.size());
	}
	
	public void setClientStatus(ClientStatus status){
		this.status = status;
		if(status != null){
			this.status.lastActionTime = System.currentTimeMillis();
			this.status.lastActiveTime = System.currentTimeMillis();
		}
	}
	
	public ClientStatus getClientStatus(){
		return this.status;
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
				this.closeSocketChannel();
			}else {
				runningStatus.receiveData(readLen);
			}
		}
	}
	
	protected void write(byte[] src) throws IOException{
		ByteBuffer buffer = ByteBuffer.allocate(src.length);
		buffer.put(src);
		buffer.flip();
		this.write(buffer, false);
	}
	
	protected void closeSocketChannel() throws IOException{
		log.info("Close DVR socket channel.");
		if(this.selectionKey != null){
			this.selectionKey.cancel();
		}
		if(this.socketChannel != null){
			this.socketChannel.close();
		}
		this.socketChannel = null;
		
		//重置设备状态，需要重新登录。
		this.setClientStatus(null);
		this.writeQueue.clear();
		this.eventProxy.disconnected(new MonitorClientEvent(this));
	}
	
	//private InetSocketAddress get
	//new InetSocketAddress("127.0.0.1", 8080)
}
