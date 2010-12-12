package org.goku.video.odip;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Iterator;
import java.util.concurrent.ThreadPoolExecutor;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * Video 路由器。负责转发Video数据。
 * @author deon
 *
 */
public class VideoRoute {
	private Log log = null;
	private ThreadPoolExecutor executor = null;
	private Collection<VideoDestination> destList = Collections.synchronizedCollection(new ArrayList<VideoDestination>());
	private MonitorClient client = null;
	
	public VideoRoute(ThreadPoolExecutor executor){
		this.executor = executor;
	}
	
	/**
	 * 使用Client的logger, 规范log输出。
	 * @param log
	 */
	public void setLogger(Log log){
		this.log = log;
	}
	
	public void start(MonitorClient client){
		this.client = client;
	}
	
	/**
	 * 处理缓存数据，读取到byte[]后，转发到不同的目的对象。复制到byte[]是为了避免，
	 * 目标节点数据处理太慢，导致数据还没有处理，就被源数据覆盖。
	 * 
	 * 如果写目的出现异常，关闭目的，并从转发列表中删除。
	 * @param source
	 * @param sourceType -- 源数据类型， 用于处理双码流情况。
	 */	
	public void route(ByteBuffer source, int sourceType){
		final byte[] data = new byte[source.limit()];
		source.get(data);
		synchronized(destList){
			VideoDestination dest = null;
			for(Iterator<VideoDestination> iter = destList.iterator(); iter.hasNext();){
				dest = iter.next();
				if(dest.isClosed()){
					iter.remove();
				}else if(dest.accept(sourceType)){
					executor.execute(new RoutingTask(dest, data));
				}
			}
		}
	}
	
	public void end(){
		synchronized(destList){
			for(VideoDestination dest: destList){
				dest.close();
			}
			destList.clear();
		}
	}
	
	public void addDestination(VideoDestination dest){
		if(!this.destList.contains(dest)){
			this.destList.add(dest);
			log.debug("Add video destination, dest=" + dest.toString());
		}
	}
	
	public void removeDestination(VideoDestination dest){
		if(this.destList.contains(dest)){
			this.destList.remove(dest);
			log.debug("Remove video destination, dest=" + dest.toString());
		}
		if(this.destList.size() <= 0){
			this.client.videoDestinationEmpty();
		}
	}
	
	public int destinationSize(){
		return this.destList.size();
	}
	
	class RoutingTask implements Runnable{
		private VideoDestination dest = null;
		private byte[] data = null;
		public RoutingTask(VideoDestination dest, byte[] data){
			this.dest = dest;
			this.data = data;
		}

		@Override
		public void run() {
			try{
				/*
				避免多个线程同时写一个目地, 如果出现一个很慢的目地，将导致线程阻塞
				知道线程池耗尽。
				
				可以考虑，如果使用信号量，如果在同一个目地上阻塞的线程过多。直接丢弃
				后面的包。
				*/
				synchronized(dest){
					dest.write(this.data);
				}
			}catch(Throwable e){
				log.warn("Routting error:", e);
				removeDestination(this.dest);
				dest.close();				
			}
		}
	}
}
