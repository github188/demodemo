package org.http.channel.server;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.zip.ZipOutputStream;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.http.channel.proxy.ProxyClient;
import org.http.channel.proxy.ProxySession;
import org.http.channel.settings.Settings;
import org.mortbay.jetty.Server;
import org.mortbay.jetty.servlet.ServletHandler;
import org.mortbay.util.ajax.Continuation;
import org.mortbay.util.ajax.ContinuationSupport;

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
	private long proxySessionId = 0;
	
	private Settings settings = null;
	
	/**
	 * 保存当前还在活动的Proxy会话。
	 */
	
	private Map<String, ProxyClient> proxyClients = new HashMap<String, ProxyClient>();
	
	//private Map<String, Queue<String>> newState = new HashMap<String, Queue<String>>();
	//private 
	
	public ProxyServer(Settings s){
		this.settings = s;
		ins = this;

		proxyClients.put("default", new ProxyClient());
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
	 * @throws IOException 
	 */
	public void startRequest(HttpServletRequest request, HttpServletResponse response) throws IOException{
		ProxyClient client = this.getProxyClient(request);
		
		ProxySession s = newProxySession();
		Map<String, String> headers = new HashMap<String, String>();
		log.info(String.format("New Request:%s, sid:%s", request.getPathInfo(), s.sid));
		for(Enumeration<String> enums = request.getHeaderNames(); enums.hasMoreElements();){
			String name = enums.nextElement();
			String val = request.getHeader(name);
			log.info(String.format("head:%s=>%s", name, val));
			headers.put(name, val);
		}
		s.method = request.getMethod();
		s.queryURL = request.getRequestURI();
		if(request.getQueryString() != null){
			s.queryURL += "?" + request.getQueryString(); 
		}
		log.info("query url:" + s.queryURL);		
		
		s.continuation =  ContinuationSupport.getContinuation(request, null); 
		
		//保存当前Session
		client.newSession(s);
		
		/**
		 * Servlet的方法已经会返回，但是Response的会话是一直保留。直到
		 * s.continuation.resume()， 被调用。
		 * 
		 * 这个其实是一个线程复用的设计模式。
		 */
		s.continuation.suspend(30 * 1000);
		s.continuation.setObject(response);
		log.info("proxy session suspend.");
	}
	
	/**
	 * 穿墙的第二步. 目的主机下载，用户的HTTP请求的协议数据头。在墙内去转发请求。
	 * @param request
	 * @param response
	 */
	public void forwardRequest(HttpServletRequest request, HttpServletResponse response) throws IOException{
		//request.get
		//ProxySession s =
		//使用HTTP的分片方式，写数据数据流。
		ProxyClient client = this.getProxyClient(request);
		
		if(client != null){
			response.setHeader("Transfer-Encoding", "chunked");
			response.setStatus(HttpServletResponse.SC_OK);
			response.setContentType("application/octet-stream");
			ObjectOutputStream os = new ObjectOutputStream(new ZipOutputStream(response.getOutputStream()));
			
			Continuation cons = ContinuationSupport.getContinuation(request, null); 
			
			/**
			 * 等待3分钟的HTTP请求。
			 */
			cons.suspend(180 * 1000);
			cons.setObject(os);
			log.info("new client suspend.");			
		}
	}
	
	/**
	 * 穿墙的最后一步. 目的主机Post最终的响应数据。返回数据后，唤醒原始的HTTP连接。返回数据给用户。
	 * @param request
	 * @param response
	 */
	public void doneRequest(HttpServletRequest request, HttpServletResponse response) throws IOException{
		//ProxySession s =  
	}
	
	/**
	 * 创建一个代理会话。
	 * @return
	 */
	private ProxySession newProxySession(){
		ProxySession s = new ProxySession();
		s.sid = "s" + nextSid();
		return s;
	}
	
	private synchronized long nextSid(){
		return this.proxySessionId++;
	}
	
	/**
	 * 根据HTTP请求，分析属于哪一个代理的用户。
	 * @return
	 */
	private ProxyClient getProxyClient(HttpServletRequest request){
		return this.proxyClients.get("default");
	}
}
