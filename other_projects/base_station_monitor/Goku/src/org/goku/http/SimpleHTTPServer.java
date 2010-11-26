package org.goku.http;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.settings.Settings;
import org.mortbay.jetty.Server;
import org.mortbay.jetty.servlet.ServletHandler;

public class SimpleHTTPServer implements Runnable{
	//private Settings settings = null;
	private Log log = LogFactory.getLog("http");
	
	private int httpPort = 0;
	private String servlet = "";
	private StartupListener lisener = null;
	private Server server = null;
	
	public SimpleHTTPServer(String bindHost,  int port){
		this.httpPort = port;
	}
	
	public void setServlet(String servlet){
		this.servlet = servlet;
	}
	
	public void addStartupListener(StartupListener l){
		this.lisener = l;
	}	


	@Override
	public void run() {
		server = new Server(httpPort);
        ServletHandler handler = new ServletHandler();
        server.setHandler(handler);
        handler.addServletWithMapping(servlet, "/*");
        try {
        	log.info("Start http server at " + httpPort);
			server.start();
			this.lisener.started();
		} catch (Exception e) {
			log.error(e.toString(), e);
		}
		
	}

}
