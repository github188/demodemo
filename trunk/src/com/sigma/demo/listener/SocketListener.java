package com.sigma.demo.listener;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import com.sigma.demo.service.FailServiceException;
import com.sigma.demo.service.ThreadPoolSupportService;

public class SocketListener extends ThreadPoolSupportService implements SocketListenerMBean, ListenerService{
	
	private static final String DEFAULT_PORT = "8177";
	private SocketHandler handler = null;
	private ServerSocket server = null;
	private boolean isRunning = false;

	public void start() throws FailServiceException {
		this.initSockecServer(context.getStringParameter("port", DEFAULT_PORT));
		super.start();
	}

	private void initSockecServer(String ports) throws FailServiceException{
		for(String port : ports.split(",")){
			try{
				server = new ServerSocket(Integer.parseInt(port));
				log.info("Started Socket listener, Name:" + 
						this.getName() + 
						"\tIP:" + server.getLocalSocketAddress() +
						"\t Listen Port:" + server.getLocalPort());				
				break;
			}catch(java.net.BindException e){
				log.warn("The port '" + port +"' in use, try next port!");
				continue;
			}catch (IOException e){
				break;
			}
		}
		
		if(server == null){
			throw new FailServiceException("Can't bind the lisenter to port.");
		}
	}
	
	public void run() {
		try {
			isRunning = true;
			while (isRunning) {
				Socket client = server.accept();
				log.info("Accepted client:"
						+ client.getRemoteSocketAddress().toString());
				threadPool.execute(new NewSocketHandler(client));
			}
		}catch (IOException e) {
			if(!e.toString().endsWith("closed")){
				log.error(e.toString(), e);
			}else {
				log.info("Socket server closed!");
			}
		}
	}

	public void shutDown() throws FailServiceException {
		//log.debug("Listener Shuting down, " + this.toString());
		isRunning = false;
		try {
			server.close();
		} catch (IOException e) {
		}
	}

	public void setSocketHandler(SocketHandler handler) {
		this.handler = handler;
	}
	
	private class NewSocketHandler implements Runnable{
		Socket newSocket = null;
		NewSocketHandler(Socket socket){
			this.newSocket = socket;
		}
		public void run() {
			log.debug("Call socket handler...");
			if(newSocket == null){
				return;
			}
			if(handler != null){
				handler.connected(this.newSocket);
			}else {
				log.warn("Not found handler for socket.");
				try {
					this.newSocket.close();
				} catch (IOException e) {
					log.error(e,e);
				}
			}
		}
	}

	public String getBindAddress() {
		
		return (server != null) ? server.getInetAddress().getHostAddress() :
			"";
	}

	public int getListenPort() {
		
		return (server != null) ? server.getLocalPort() :
				-1;
	}

}
