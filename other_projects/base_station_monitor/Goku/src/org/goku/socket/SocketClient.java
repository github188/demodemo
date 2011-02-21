package org.goku.socket;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;

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
	
	protected void write(ByteBuffer src) throws IOException{
		//src.order(ByteOrder.BIG_ENDIAN);
		if(log.isDebugEnabled()){
			log.debug(String.format("Write data size:%s, to:%s", src.remaining(), toString()));
		}
		synchronized(this.socket){
			while(src.hasRemaining()){ //文档中说不保证所有数据被写完。
				this.socket.write(src);
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
				}				
			}
		}
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
