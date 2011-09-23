package org.http.channel.server;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.http.channel.settings.Settings;
import org.mortbay.jetty.Server;
import org.mortbay.jetty.servlet.ServletHandler;

/**
 * 基于HTTP穿墙代理：
 * 
 * 墙外用户：        Gate-Server:   Gate-Client:         墙内主机  
 * 
 * 原始请求  --->    打包HTTP头        
 *                             <------下载原始请求
 *                                              ---->  目的请求
 *                                              <---   目的响应
 *                             <------上传响应数据
 *         <----    返回给用户
 * 
 * 墙内的主机始终，主动使用HTTP的方式，连接墙外的服务器。所有原始数据都是在 HTTP的
 * 数据段。最终实现穿墙的目的。
 * 
 * @author deon
 *
 */
public class ProxyServer {
	public static ProxyServer ins = null;
	private Log log = LogFactory.getLog("gate");
	
	private Settings settings = null;
	public ProxyServer(Settings s){
		this.settings = s;
		ins = this;
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
	
	/**
	 * 穿墙的第一步. 用户开始HTTP请求，把请求的协议头封装成数据包, 然后阻塞原始请求。 
	 * @param request
	 * @param response
	 */
	public void startRequest(HttpServletRequest request, HttpServletResponse response){
		//ProxySession s =  
	}
	
	/**
	 * 穿墙的第二步. 目的主机下载，用户的HTTP请求的协议数据头。在墙内去转发请求。
	 * @param request
	 * @param response
	 */
	public void forwardRequest(HttpServletRequest request, HttpServletResponse response){
		//ProxySession s =  
	}
	
	/**
	 * 穿墙的最后一步. 目的主机Post最终的响应数据。返回数据后，唤醒原始的HTTP连接。返回数据给用户。
	 * @param request
	 * @param response
	 */
	public void doneRequest(HttpServletRequest request, HttpServletResponse response){
		//ProxySession s =  
	}
}
