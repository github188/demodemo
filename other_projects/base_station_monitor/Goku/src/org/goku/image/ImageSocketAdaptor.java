package org.goku.image;

import java.io.IOException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.socket.SocketAdaptor;
import org.goku.socket.SocketClient;

public class ImageSocketAdaptor implements SocketAdaptor{
	private Log log = LogFactory.getLog("client.socket.image");
	public void runCommand(String command, SocketClient client) throws IOException{
		log.info("Read socket command:" + command);
	}
}
