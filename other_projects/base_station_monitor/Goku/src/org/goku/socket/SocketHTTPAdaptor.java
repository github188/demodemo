package org.goku.socket;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 把字符串，封装成为一个Sevlet API调用Sevelet来处理，统一HTTP和Socket的服务
 * 端处理方式。
 * 
 * 命令格式：
 * 
 * cmd>login?uuid=123&xxx=bbb
 */

public class SocketHTTPAdaptor {
	private Log log = LogFactory.getLog("client.socket.http");
	private String servlet = null;
	
	public SocketHTTPAdaptor(String servlet){
		
	}
	
	public void runCommand(String command, SocketClient client){
		log.info("run command:" + command);		
	}

}
