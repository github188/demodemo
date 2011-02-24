package org.goku.socket;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;
import java.util.ArrayDeque;
import java.util.Queue;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.User;

/**
 * 用于当Socket有可读数据时，调用读操作，读一行有效的命令，再传给SocketServer处理。
 * 主要作用是为了非阻塞Socket操作的一个数据处理接口。
 * 
 * 另外保存一些和客户端相关的描述信息。
 * @author deon
 */
public class SocketClient implements SelectionHandler, Runnable {
	public static final int MODE_HTTP = 1;
	public static final int MODE_REALLPLY = 2;
	public static final int MODE_REPLAY = 3;
	
	public SocketChannel socket = null;
	
	/**
	 * 用户登录Session ID
	 */
	public String sessionId = null;
	
	/**
	 * 用户登录Session的用户名。
	 */
	public User loginUser = null;
	public String encoding = "unicode";
	public int connectionMode = MODE_HTTP;
	
	public FileReplayController replay = null;
	//public VideoDestination realPlay = null;
	//public int connectionMode = MODE_HTTP;
	
	protected SelectionKey selectionKey = null;	
	protected SimpleSocketServer server = null;
	private Log log = LogFactory.getLog("client.socket");
	
	protected ByteBuffer readBuffer = ByteBuffer.allocate(1024);
	protected StringBuffer curCmd = null;
	private String remoteIP = null;
	private Queue<ByteBuffer> writeQueue = new ArrayDeque<ByteBuffer>(10);

	public SocketClient(SocketChannel client, SimpleSocketServer server){
		this.socket = client;
		this.server = server;
		remoteIP = socket.socket().getRemoteSocketAddress().toString();
	}

	/**
	 * 有可读数据时，调用处理数据。
	 */
	public void run() {
		synchronized(this){
			try {
				if(this.selectionKey.isReadable()){
					this.read();
					if(this.selectionKey.isValid()){
						this.selectionKey.interestOps(selectionKey.interestOps() | SelectionKey.OP_READ);
						this.selectionKey.selector().wakeup();
					}
				}
				if(this.selectionKey.isValid() && this.selectionKey.isWritable()){
					this.writeBuffer();
				}
			} catch (Exception e) {
				log.error(e.toString(), e);
				this.selectionKey.cancel();
			}
		}
	}
	
	protected void writeBuffer() throws IOException{
		synchronized(this.writeQueue){
			ByteBuffer buffer = this.writeQueue.peek();
			while(buffer != null){
				this.socket.write(buffer);
				if(buffer.hasRemaining()){
					this.selectionKey.interestOps(selectionKey.interestOps() | SelectionKey.OP_WRITE);
					this.selectionKey.selector().wakeup();
					break;
				}else {
					this.writeQueue.remove();
					buffer = this.writeQueue.peek();
				}
			}
		}
		if(this.writeQueue.size() > 0){
			log.warn(String.format("The client too slow, Write buffer queue size:%s, to:%s", this.writeQueue.size(), toString()));
		}
	}
	
	/**
	 * 读Socket Buffer让后传给Socket服务器处理，每次读一行空行丢弃。
	 * @throws IOException 
	 */
	public void read() throws IOException{
		readBuffer.clear();
		int readLen = this.socket.read(readBuffer);
		if(readLen == -1){
			this.closeSocket();
		}else{
			readBuffer.flip();
			if(log.isDebugEnabled()){
				log.debug(String.format("Read data size:%s, from:%s", readBuffer.remaining(), toString()));
			}
			this.processBuffer(readBuffer);
		}
	}
	
	protected void write(byte[] src) throws IOException{
		ByteBuffer buffer = ByteBuffer.allocate(src.length);
		buffer.put(src);
		buffer.flip();
		this.write(buffer);
	}
	
	public void putWriteBuffer(ByteBuffer data){
		synchronized(this.writeQueue){
			this.writeQueue.offer(data);
			//如果当前Socket没有注册写操作.
			if((this.selectionKey.interestOps() & SelectionKey.OP_WRITE) == 0){
				this.selectionKey.interestOps(selectionKey.interestOps() | SelectionKey.OP_WRITE);
				this.selectionKey.selector().wakeup();				
			}
		}
	}
	
	protected void write(ByteBuffer src) throws IOException{
		//src.order(ByteOrder.BIG_ENDIAN);
		if(log.isDebugEnabled()){
			log.debug(String.format("write buffer size:%s, to:%s", src.remaining(), toString()));
		}
		synchronized(this.writeQueue){
			if(this.writeQueue.isEmpty()){
				this.socket.write(src);
				if(src.hasRemaining()){  //还有未写完的数据，放到队列等待Socket可写后再写。
					if(log.isDebugEnabled()){
						log.debug(String.format("Write blocking, insert data to buffer queue."));
					}
					this.writeQueue.offer(src);
					this.selectionKey.interestOps(selectionKey.interestOps() | SelectionKey.OP_WRITE);
					this.selectionKey.selector().wakeup();
				}
			}else {
				if(!this.writeQueue.offer(src)){
					log.warn("Write buffer queue full, client:" + this.toString());
				}else {
					if(log.isDebugEnabled()){
						log.debug(String.format("Insert data to buffer queue, size:%s, to:%s", src.remaining(), toString()));
					}
				}
			}
		}
	}
	
	/**
	 * 如果写缓存队列大于5个包，认为客户端太忙.
	 * @return
	 */
	public boolean writeBusy(){
		return this.writeQueue.size() > 5;
	}
	
	protected void processBuffer(ByteBuffer in){
		if(curCmd == null) curCmd = new StringBuffer();
		byte data = 0;
		while(in.hasRemaining()){
			data = in.get();
			if(data == '\n'){
				String cmd = this.curCmd.toString().trim();
				if(cmd.length() > 0){
					this.server.processClient(cmd, this);
				}
				curCmd = new StringBuffer();
			}else {
				curCmd.append((char)data);
			}
			//log.info("cmd:" + curCmd);
		}
		if(curCmd.length() > 0){
			log.debug("incomplete command in buffer:" + this.curCmd.toString());
		}
	}
	
	/**
	 * 由Selection线程调用。
	 * @param key
	 */
	public void setSelectionKey(SelectionKey key){
		this.selectionKey = key;
	}
	
	public void close(){
		this.closeSocket();
	}
	
	public void closeSocket(){
		log.info("Close socket, " + toString());
		if(this.selectionKey != null){
			this.selectionKey.cancel();
		}
		if(this.socket != null){
			try {
				this.socket.close();
			} catch (IOException e) {
				log.error(e.toString(), e);
			}
		}
	}
	
	public String toString(){
		String remoteIp = this.remoteIP;
		if(this.loginUser != null){
			remoteIp = loginUser.name + "@" + remoteIp;
		}
		return remoteIp;
	}
}
