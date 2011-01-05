package org.goku.socket.proxy;

import java.io.IOException;
import java.nio.channels.ServerSocketChannel;
import java.util.ArrayDeque;
import java.util.HashMap;
import java.util.Map;
import java.util.Queue;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.socket.SocketManager;

public class SocketProxyServer {
	private Log log = LogFactory.getLog("proxy.server");
	private SocketManager manager = null;	
	private Queue<Integer> portPool = null;
	private Map<String, ListenProxy> proxyMapping = new HashMap<String, ListenProxy>();
	
	public SocketProxyServer(SocketManager manager, int startPort, int endPort){
		this.manager = manager;
		
		portPool = new ArrayDeque<Integer>(endPort - startPort + 1);
		for(int i = startPort; i <= endPort; i++){
			portPool.add(i);
		}
	}
	
	/**
	 * 创建一个新的代理，返回代理的连接端口号。
	 * @param dest
	 * @return
	 */
	public int createProxy(String dest) throws IOException{
		int proxyPort = this.getProxyPort();
		ListenProxy proxy = null;
		if(proxyPort > 0){
			proxy = new ListenProxy(manager, dest);
			manager.listen("0.0.0.0", proxyPort, proxy);
			this.proxyMapping.put(dest, proxy);
		}
		return proxyPort;
	}
	
	/**
	 * 根据目地地址关闭代理。
	 * @param dest
	 */
	public void releaseProxy(String dest){
		
	}
	
	/**
	 * 根据端口关闭代理。
	 * @param dest
	 */	
	public void releaseProxy(int port){
		
	}
	
	protected int getProxyPort(){
		int port = -1;
		synchronized(portPool){
			if(portPool.size() > 0){
				port = portPool.poll();
			}
		}
		return port;
	}

}
