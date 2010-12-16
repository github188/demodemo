package org.goku.socket;

import java.io.IOException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 处理Socket的Video相关的命令。
 * 
 * 命令格式：
 * 
 * video>real?uuid=1111
 * video>replay?uuid=1111
 * video>seek?uuid=1111
 * video>ok
 * 
 * @author deon
 */
public class SocketVideoAdapter {
	private Log log = LogFactory.getLog("client.socket.video");
	
	public void runCommand(String command, SocketClient client)  throws IOException{
		
	}
}