package org.http.channel.server;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.http.channel.settings.Settings;
import org.mortbay.jetty.Server;
import org.mortbay.jetty.servlet.ServletHandler;

public class ProxyServer {
	private Log log = LogFactory.getLog("gate");
	
	private Settings settings = null;
	public ProxyServer(Settings s){
		this.settings = s;
	}

	public void run(){
		startHTTPServer();
	}
	
	private void startHTTPServer(){
		int httpPort = settings.getInt(Settings.HTTP_PORT, -1);
		Server server = new Server(httpPort);
        ServletHandler handler = new ServletHandler();
        server.setHandler(handler);
        handler.addServletWithMapping("org.http.channel.server.servlet.CommandServlet", "/~/*");
        handler.addServletWithMapping("org.http.channel.server.servlet.ProxyServlet", "/*");
        try {
        	log.info("Start http server at " + httpPort);
			server.start();
			server.join();
		} catch (Exception e) {
			log.error(e.toString(), e);
		}
	}
}
