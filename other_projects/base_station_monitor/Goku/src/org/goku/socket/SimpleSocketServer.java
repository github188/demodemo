package org.goku.socket;

import java.io.IOException;
import java.nio.channels.SelectionKey;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class SimpleSocketServer implements Runnable {
	private Log log = LogFactory.getLog("server.socket");
	private SocketManager manager = null;
	private ServerSocketChannel serverChannel = null;
	protected SelectionKey selectionKey = null;
	
	private SocketHTTPAdaptor httpAdapter = null;
	private SocketVideoAdapter videoAdapter = null;
	private String servelt = null;
	
	/**
	 * 对象自身的一个引用，因为serverHandler内部需要使用。可能会引起对象无法内存回
	 * 收。SimpleSocketServer本身不应该被大量创建。
	 */
	private SimpleSocketServer server = null;
	
	public int listenPort = 0;
	
	public SimpleSocketServer(SocketManager manager, int nPort){
		this.manager = manager;
		this.listenPort = nPort;
		server = this;
	}

	@Override
	public void run() {
		//第一次运行初始化Server.
		if(selectionKey == null){
			httpAdapter = new SocketHTTPAdaptor(servelt);
			videoAdapter = new SocketVideoAdapter();
			serverChannel = manager.listen("0.0.0.0", this.listenPort, this);
		}else if(selectionKey.isAcceptable()){
			try {
				SocketChannel client = serverChannel.accept();
				log.debug("Accept client:" + client.socket().getRemoteSocketAddress());
				client.configureBlocking(false);
				client.socket().setTcpNoDelay(true);
				SocketClient handler = new SocketClient(client, server);
				
				manager.register(client, SelectionKey.OP_READ, handler);
				selectionKey.interestOps(SelectionKey.OP_ACCEPT);
				
				selectionKey.selector().wakeup();
			} catch (IOException e) {
				log.error(e.toString(), e);
			}
		}
	}
	
	/**
	 * 处理客户端的输入命令。
	 * @param data
	 * @param client
	 */
	public void processClient(String data, SocketClient client){
		if(log.isDebugEnabled()){
			log.debug(String.format("processing comand '%s' read from %s", data, client.toString()));
		}
		try{
			if(data.startsWith("cmd>")){
				this.httpAdapter.runCommand(data, client);
			}else if(data.startsWith("video>")){
				this.videoAdapter.runCommand(data, client);
			}else {
				if(client.connectionMode == SocketClient.MODE_HTTP){
					String error = String.format("Drop unkonw command:'%s', the valid prefix is 'cmd>' or 'video>'.", data);
					client.write(error.getBytes());
				}
			}
		}catch(IOException e){
			log.error(e.toString(), e);
			client.closeSocket();
		}
	}
	
	public void setServlet(String servlet){
		this.servelt = servlet;
	}
	
	/**
	 * 由Selection线程调用。
	 * @param key
	 */
	public void setSelectionKey(SelectionKey key){
		this.selectionKey = key;
	}
	
	public String toString(){
		return "SimpleSocketServer";
	}

}

