package org.goku.odip;

import java.io.IOException;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.nio.channels.spi.SelectorProvider;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Iterator;
import java.util.concurrent.ThreadPoolExecutor;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * NIO Selecting 线程。
 * @author deon
 */
public class ChannelSelector implements Runnable{
	private Log log = LogFactory.getLog("main");
	private Selector selector = null;
	private ThreadPoolExecutor threadPool = null;
	private boolean isRunning = true;
	
	private Collection<ChangeRequest> paddings = new ArrayList<ChangeRequest>();//#this.pendingData
	
	public ChannelSelector(ThreadPoolExecutor threadPool) throws IOException{
		selector = SelectorProvider.provider().openSelector(); 		
		this.threadPool = threadPool;
		//threadPool.execute(this);
	}
	
	/**
	 * 不同的线程注册Selector会被阻塞。先保存注册结果，由selecting线程完成操作。
	 * @param channel
	 * @param ops
	 * @param att
	 * @return
	 * @throws ClosedChannelException
	 */
	public void register(SocketChannel channel, int ops, Object att) 
		throws ClosedChannelException{
		//SelectionKey key = channel.register(this.selector, ops, att);		
		synchronized(paddings){
			this.paddings.add(new ChangeRequest(channel, ops, att));
			this.selector.wakeup();
		}
	}

	@Override
	public void run() {		
		log.info("The ChannelSelector is started..");		
		while(isRunning){
			try{
				synchronized(paddings){
					for(ChangeRequest req: this.paddings){
						SelectionKey key = req.channel.register(this.selector, req.ops, req.att);
						if(req.att instanceof MonitorClient){
							((MonitorClient)req.att).setSelectionKey(key);
						}
					}
					this.paddings.clear();
				}
					
				this.selector.select();
				Iterator<SelectionKey> selectedKeys = this.selector.selectedKeys().iterator();
				while (selectedKeys.hasNext()) {
					SelectionKey key = selectedKeys.next();
					selectedKeys.remove();
					if(!key.isValid()){
						continue;
					}
					/*
					 * 避免当前Key再次被select,导致多一个线程同时处理相同的collection.
					 * 数据处理完成后，需要重新设置需要监控的操作。
					 */
					key.interestOps(0);
					
					Object att = key.attachment();					
					if(att != null && att instanceof Runnable){
						this.threadPool.execute((Runnable)att);
					}else {
						log.warn("The selection key have not runnable attachment.");
						key.cancel();
					}
				}
			}catch(IOException e){
				log.error(e.toString(), e);
			}
		}
		log.info("The ChannelSelector is shutting down..");
	}
	
	
	public void shutdown(){
		this.isRunning = false;
		this.selector.wakeup();		
	}
	
	class ChangeRequest{
		SocketChannel channel = null;
		int ops = 0;
		Object att = null;
		public ChangeRequest(SocketChannel channel, int ops, Object att){
			this.channel = channel;
			this.ops = ops;
			this.att = att;
		}
	}

}
