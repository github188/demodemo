package org.goku.master;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ThreadPoolExecutor;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class SocketVideoServer implements Runnable {
	private Log log = LogFactory.getLog("socket");
	ThreadPoolExecutor executor = null;
	private boolean isRunning = false;
	
	public SocketVideoServer(ThreadPoolExecutor executor){
		this.executor = executor;
	}

	@Override
	public void run() {
		//MasterVideoServer server = MasterVideoServer.getInstance();
		isRunning = true;
		log.info("start socket video server at port 8083");
		try {
			ServerSocket server = new ServerSocket(8083);
			while(isRunning){
				Socket client = server.accept();
				new VideoClient(client).start();		
			}
		} catch (Exception e) {
			log.error(e.toString(), e);
		}
		log.info("shutdown socket video server");
	}
	
	class VideoClient extends Thread{
		private Socket client = null;
		public VideoClient(Socket client){
			this.client = client;
		}

		@Override
		public void run() {
			log.info("New client, " + client.getInetAddress());
			MasterVideoServer server = MasterVideoServer.getInstance();
			try{
				BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream()));
				InputStream data = null; //new BufferedReader(new InputStreamReader(client.getInputStream()));
				String cmd = null;
				OutputStream out = client.getOutputStream();
				byte[] buffer = new byte[1024 * 4];
				while(true){
					cmd = in.readLine();
					if(cmd == null)break;
					cmd = cmd.trim();
					if(cmd.equals("")) continue;
					log.info("Read line from clinet:" + cmd);
					if(!cmd.equals("OK")){
						String file = server.settings.getString(cmd, null);
						if(file != null && new File(file).exists() ){
							if(data != null){
								data.close();
							}
							data = new FileInputStream(new File(file));
						}else if(file != null){
							log.warn("Not found file:" + file);
						}else {
							log.warn("Not found file by id " + cmd);
						}
					}
					if(data != null){
						int len = data.read(buffer);
						if(len > 0){
							out.write(buffer, 0, len);							
						}
						out.flush();
					}
				}
			}catch (Exception e) {
				log.error("Shutdown with error:" + e.toString());
			}
			log.error("shutdown client");
		}		
	}

}
