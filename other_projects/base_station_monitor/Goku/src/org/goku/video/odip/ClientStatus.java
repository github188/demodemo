package org.goku.video.odip;

/**
 * 协议里面用到的，摄像头状态信息。登录成功开始更新。
 * @author deon
 *
 */
public class ClientStatus {
	public int channelCount = 0; //通道数， ack_login W10
	
	/**
	 * 视频编码方式 ack_login W11
	 * 8 - MPEG4, 
	 * 9 - H.264
	 */
	public int videoType = 0;

	/**
	 * 设备类型 ack_login W12
	 */
	public int devType = 0;

	/**
	 * 设备类型 ack_login W16-W19
	 */	
	public int sessionId = 0;


	/**
	 * 视频制式 ack_login W28
	 */	
	public int devMode = 0;
	
}