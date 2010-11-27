package org.goku.odip;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.concurrent.ThreadPoolExecutor;

/**
 * Video 路由器。负责转发Video数据。
 * @author deon
 *
 */
public class VideoRoute {
	private ThreadPoolExecutor executor = null;
	private Collection<VideoDestination> destList = Collections.synchronizedCollection(new ArrayList<VideoDestination>());
	
	public VideoRoute(ThreadPoolExecutor executor){
		this.executor = executor;
	}
	
	public void start(){
		
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
			for(VideoDestination dest: destList){
				if(dest.accept(sourceType)){
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
		this.destList.add(dest);		
	}
	
	public void removeDestination(VideoDestination dest){
		this.destList.remove(dest);
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
				dest.write(this.data);
			}catch(Exception e){
				dest.close();
				removeDestination(this.dest);
			}
		}
		
	}
}
