package org.http.channel.server;

import java.io.FilterOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.Timer;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.fileupload.FileItemIterator;
import org.apache.commons.fileupload.FileItemStream;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.apache.commons.fileupload.util.Streams;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.http.channel.Version;
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
	private ThreadPoolExecutor threadPool = null;
	private Timer timer = new Timer();
	
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
		int core_thread_count = 5;
		threadPool = new ThreadPoolExecutor(
				core_thread_count,
				settings.getInt(Settings.MAX_ROUTE_THREAD_COUNT, 500),
				60, 
				TimeUnit.SECONDS, 
				new LinkedBlockingDeque<Runnable>(core_thread_count * 2)
				);
		
		startHTTPServer();
	}
	
	//private 
	
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
		
		if(client != null && client.activeClient() > 0){
			ProxySession s = newProxySession();
			Map<String, String> headers = new HashMap<String, String>();
			for(Enumeration<String> enums = request.getHeaderNames(); enums.hasMoreElements();){
				String name = enums.nextElement();
				String val = request.getHeader(name);
				headers.put(name, val);
			}
			s.method = request.getMethod();
			s.queryURL = request.getRequestURI();
			s.header = headers;
			if(request.getQueryString() != null){
				s.queryURL += "?" + request.getQueryString(); 
			}
			
			int len = 0;
			if(s.method.toLowerCase().equals("post")){
				byte[] tmpPostData = new byte[1024 * 64]; 
				InputStream in = request.getInputStream();
				if(in != null){
					for(int i = 0; i >= 0; ){
						i = in.read(tmpPostData, len, tmpPostData.length - len);
						if(i < 0 )break;
						len += i;
					}
					in.close();
					if(len > 0){
						s.content = new byte[len];
						System.arraycopy(tmpPostData, 0, s.content, 0, len);						
					}
				}
			}
			
			log.info(String.format("REQ:[%s] %s, %s, SID:%s", s.method, request.getPathInfo(), len, s.sid));			
			s.continuation =  ContinuationSupport.getContinuation(request, null); 
			
			//保存当前Session
			client.newSession(s);
			
			/**
			 * Servlet的方法已经会返回，但是Response的会话是一直保留。直到
			 * s.continuation.resume()， 被调用。
			 * 
			 * 这个其实是一个线程复用的设计模式。
			 */
			log.info("proxy session suspend.");
			s.continuation.setObject(response);
			s.continuation.suspend(180 * 1000);
		}else {
			response.setStatus(HttpServletResponse.SC_NOT_FOUND);
			response.setCharacterEncoding("utf8");
			response.getWriter().println("Not found active proxy client. <br/>" +
					"没有找到一个代理客户端来转发请求。<br/>" +
					"Server name:" + request.getServerName());
		}
	}
	
	/**
	 * 穿墙的第二步. 目的主机下载，用户的HTTP请求的协议数据头。在墙内去转发请求。
	 * @param request
	 * @param response
	 */
	public void forwardRequest(HttpServletRequest request, final HttpServletResponse response) throws IOException{
		//request.get
		//ProxySession s =
		//使用HTTP的分片方式，写数据数据流。
		ProxyClient client = this.getProxyClient(request, true);
		
		if(client != null){
			response.setHeader("Transfer-Encoding", "chunked");
			response.setStatus(HttpServletResponse.SC_OK);
			response.setContentType("application/octet-stream");
			//ZipOutputStream zip = new ZipOutputStream(response.getOutputStream());
			//zip.putNextEntry(new ZipEntry("command"));
			
			Continuation cons = ContinuationSupport.getContinuation(request, null); 			
			
			FilterOutputStream f = new FilterOutputStream(response.getOutputStream()){
				public void flush() throws IOException{
					super.flush();
					response.flushBuffer();
					//log.debug("flush command request.");
				}
			};
			
			ObjectOutputStream os = new ObjectOutputStream(f);
			//ObjectOutputStream
			
			/**
			 * 等待30分钟的HTTP请求。
			 */
			cons.setObject(os);
			log.info("new task tracker suspend.");
			response.flushBuffer();
			client.activeContinuation(cons);
			cons.suspend(300 * 1000);
		}else {
			log.info("Not found proxy clinet:" + request.getServerName());
		}
	}
	
	/**
	 * 穿墙的最后一步. 目的主机Post最终的响应数据。返回数据后，唤醒原始的HTTP连接。返回数据给用户。
	 * @param request
	 * @param response
	 */
	public void doneRequest(HttpServletRequest request, HttpServletResponse response) throws IOException{
		//ProxySession s =
    	boolean isMultipart = ServletFileUpload.isMultipartContent(request);
    	int status = 0;
    	int length = 0;
    	ProxyClient client = this.getProxyClient(request);
    	if(isMultipart && client != null){
    		ServletFileUpload upload = new ServletFileUpload();
    		ProxySession session = null;
    		HttpServletResponse proxyResponse = null;
    		try {
				FileItemIterator iter = upload.getItemIterator(request);
				while (iter.hasNext()) {
					FileItemStream item = iter.next();
				    if (item.isFormField()) {
				    	InputStream stream = item.openStream();
				    	if(item.getFieldName().equals("sid")){
				    		session = client.doneSession(Streams.asString(stream));
				    		if(session != null){
				    			proxyResponse = (HttpServletResponse)session.continuation.getObject();
				    			//log.debug("------------------------------");
				    		}else {
				    			break;
				    		}
				    	}else if(item.getFieldName().equals("status") && proxyResponse != null){
				    		status = Integer.parseInt(Streams.asString(stream));
				    		proxyResponse.setStatus(status);
				    	}else if(proxyResponse != null && !proxyResponse.isCommitted()){
				    		String name = item.getFieldName();
				    		String values = Streams.asString(stream);
				    		//log.debug(String.format("%s:%s", name, values));
				    		proxyResponse.setHeader(name,  values);
				    	}
				    	stream.close();
				    } else if(!item.getName().equals("")) {
				    	OutputStream os = proxyResponse.getOutputStream();
				    	InputStream in = item.openStream();
				    	byte[] buffer = new byte[64 * 1024];
				    	for(int len = 0; len >= 0; ){
				    		len = in.read(buffer);
				    		if(len > 0){
				    			os.write(buffer, 0, len);
				    			length += len;
				    		}
				    	}
				    	in.close();
				    	os.flush();
				    }
				}
			} catch (Exception e) {
				response.setHeader("upload_status", e.toString());
				log.error(e.toString(), e);
			} finally{
				if( session != null){
					log.info(String.format("[%s] %s %s, SID:%s", status, length, session.queryURL, session.sid));
					session.continuation.resume();
				}
			}
    	}else if(client == null){
    		response.getWriter().println("Invalid proxy client, for server:" + request.getServerName());
    	}else {
    		log.warn("The request is not a multpart content type.");
    	}
    	response.getWriter().println("OK");
	}
	
	public void status(HttpServletRequest request, HttpServletResponse response) throws IOException{
		StringBuffer status = new StringBuffer();
		
		status.append(String.format("=====%s %s %s=====\n", Version.getName(), Version.getVersion(), Version.getBuildDate()));
		ProxyClient client = this.getProxyClient(request);
		if(client != null){
			ArrayList<ProxySession> sessions = new ArrayList<ProxySession>(client.sessions.values());
			status.append(String.format("\n\n=====Active Session=====\n"));
			for(ProxySession s: sessions){
				status.append(s.toString() + "\n");
			}
			
			status.append(String.format("\n\n=====Blocking Session=====\n"));
			ArrayList<String> blocking = new ArrayList<String>(client.blocking);
			for(String s: blocking){
				status.append(s + "\n");
			}

			status.append(String.format("\n\n=====Waiting Session=====\n"));
			ArrayList<String> waiting = new ArrayList<String>(client.waiting);
			for(String s: waiting){
				status.append(s + "\n");
			}	
			
			status.append(String.format("\n\n=====Active internal client=====\n"));
			ArrayList<Continuation> clients = new ArrayList<Continuation>(client.clients);
			for(Continuation c: clients){
				status.append("Resumed:" +  c.isResumed() + ", pending:" + c.isPending() +  ", " + c.toString() + "\n");
			}
			
			status.append(String.format("\n\n=====Done Session=====\n"));
			for(ProxySession s: new ArrayList<ProxySession>(client.doneSession)){
				status.append(s.toString() + "\n");
			}
		}else {
			status.append(String.format("=====Active proxy client=====\n"));
			for(String key: proxyClients.keySet()){
				status.append(key + "\n");
			}
		}
		
		response.setContentType("text/plain");
		response.getWriter().write(status.toString());
	}
	
	/**
	 * 创建一个代理会话。
	 * @return
	 */
	private ProxySession newProxySession(){
		ProxySession s = new ProxySession();
		s.sid = "s" + nextSid();
		s.createTime = System.currentTimeMillis();
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
		return this.getProxyClient(request, false);
	}
	
	private ProxyClient getProxyClient(HttpServletRequest request, boolean autoCreate){
		String serverName = request.getServerName();
		if(serverName == null || 
		   serverName.indexOf(".0.") > 0 //127.0.0.1 10.0.2.2等地址
		  ){
			log.debug(String.format("Convert server name '%s' to 'default'", serverName));
			serverName = "default";
		}
		
		if(!this.proxyClients.containsKey(serverName) && autoCreate){
			log.info(String.format("create new proxy client for '%s'", serverName));
			this.proxyClients.put(serverName, new ProxyClient());
		}
		
		ProxyClient n = this.proxyClients.get(serverName);
		if(n == null){
			log.info(String.format("Not found proxy client for '%s'", serverName));
		}
		return n;
	}	
}
