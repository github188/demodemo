package org.goku.video.odip;

import java.io.IOException;

public interface VideoDestination {
	/**
	 * 判断接受数据类型，对于双码流的情况，客户端可能只需要接受底质量的码流。
	 * @param sourceType
	 */
	public boolean accept(int sourceType);
	
	/**
	 * 处理转发的数据，调用者保证接口的线程安全。只能有一个线程在调用写操作。
	 * @param data 
	 */	
	public void write(byte[] data) throws IOException;
	
	/**
	 * 关闭目地, 内部处理异常。
	 * @param data 
	 */
	public void close();
}
