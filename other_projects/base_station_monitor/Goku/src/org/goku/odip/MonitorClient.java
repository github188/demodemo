package org.goku.odip;

import java.io.IOException;
import java.net.InetSocketAddress;
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
	
	/**
	 * 当前处理Client事件的对象，类似一个状态机。某一个时刻，只能有一个状态。
	 */
	protected Runnable handler = null;
	
	public MonitorClient(BaseStation info){
		log = LogFactory.getLog("node." + info.uuid);
	}
	
	/**
	 * 连接客户端，后注册到ChannelSelector，如果有可读／数据由Selector触发一个事件。
	 * @param selector
	 */
	public void connect(ChannelSelector selector) throws IOException{
		SocketChannel socketChannel = SocketChannel.open();
		socketChannel.configureBlocking(false);
		socketChannel.connect(new InetSocketAddress("127.0.0.1", 8080));		
		selectionKey = selector.register(socketChannel, SelectionKey.OP_CONNECT, this);
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
		public void alarm(MonitorClientEvent event) {
			for(MonitorClientListener l: ls){
				l.alarm(event);
			}
		}
	};
	
	/**
	 * 有需要处理的事件时，Selector使用一个后台线程调用run方法，处理当前的事件。
	 */
	@Override
	public void run() {
		// TODO Auto-generated method stub
		
	}
}
