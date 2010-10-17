package org.notebook.services;

import java.io.IOException;
import java.net.BindException;
import java.net.InetAddress;
import java.net.ServerSocket;

import org.notebook.gui.MainFrame;

public class SocketSingleInstance implements SingleInstance {
	private static final int SINGLE_INSTANCE_NETWORK_SOCKET = 44331;	
	private static ServerSocket socket = null;
	
	@Override
	public boolean checkRunning(MainFrame app) {
		try {
			socket = new ServerSocket(SINGLE_INSTANCE_NETWORK_SOCKET,0,
		    		InetAddress.getByAddress(new byte[] {127,0,0,1}));
			log.info("Setup Socket SingleInstance at port " + SINGLE_INSTANCE_NETWORK_SOCKET);
			//socket.accept()
			return false;
		}catch (BindException e) {
			return true;
		}catch (IOException e) {
			return true;
		}
	}
}
