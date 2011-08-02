package org.notebook.services;

import java.io.IOException;
import java.net.BindException;
import java.net.InetAddress;
import java.net.ServerSocket;

import javax.swing.JFrame;

public class SocketSingleInstance implements SingleInstance {
	private static final int SINGLE_INSTANCE_NETWORK_SOCKET = 44332;	
	private static ServerSocket socket = null;
	
	@Override
	public boolean checkRunning(JFrame app) {
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
