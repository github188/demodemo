package org.goku.master;

import java.io.IOException;

/**
 * 中心控制服务器的客户端，通过HTTP和服务端交互。
 * @author deon
 *
 */
public class MasterClient {
	
	public MasterClient(String url){
		
	}
	
	/**
	 * 检查中心服务器的，是否在运行。
	 */	
	public boolean checkConnection(){
		return true;
	}
	
	/**
	 * 通知中心服务器，转发服务器已启动。
	 * @param host 转发服务器的名称。
	 * @param port 转发服务器的HTTP接口端口。
	 */
	public void startUp(String host, int port){
		
	}

	/**
	 * 通知中心服务器，转发服务器正在关闭。
	 * @param host 转发服务器的名称。
	 * @param port 转发服务器的HTTP接口端口。
	 */	
	public void shutDown(String host, int port){
		
	}
}
