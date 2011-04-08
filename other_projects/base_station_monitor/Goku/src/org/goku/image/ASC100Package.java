package org.goku.image;

import java.nio.ByteBuffer;

public class ASC100Package {
	public static final int STATUS_INIT = 1;
	public static final int STATUS_CMD = 2;
	public static final int STATUS_LENGTH = 3;
	public static final int STATUS_DATA = 4;
	public static final int STATUS_CHECKSUM = 5;
	public static final int STATUS_END = 6;
	
	/**
	 * 当前包处理状态。
	 */
	public int status = STATUS_INIT;
	/**
	 * 协议包的命令字。
	 */
	public int cmd = 0;
	/**
	 * 数据包的长度。-1表示没有初始化
	 */		
	public int len = -1;
	public int checkSum = 0;
	public int bufferCheckSum = 0;
	
	public ByteBuffer inBuffer = ByteBuffer.allocate(64 * 1024);
	
	
	/**
	 * 当前是否正读到一个escaped;
	 */
	public boolean escaped = false;
	
	public void clear(){
		this.status = STATUS_INIT;
		this.cmd = 0;
		this.len = -1;
		checkSum = 0;
		bufferCheckSum = 0;
		inBuffer.clear();
	}
}
