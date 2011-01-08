package org.goku.video;

import java.io.IOException;
import java.io.OutputStream;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.RouteRunningStatus;
import org.goku.http.BaseRouteServlet;
import org.goku.video.odip.MonitorClient;
import org.goku.video.odip.VideoDestination;
import org.mortbay.util.ajax.Continuation;
import org.mortbay.util.ajax.ContinuationSupport;

/**
 * HTTP交互接口
 * 
 * @author deon
 */
public class DefaultRouteServerServlet extends BaseRouteServlet{
	private VideoRouteServer server = null;
	private Log log = LogFactory.getLog("http");
	
	public void init(ServletConfig config){
		server = VideoRouteServer.getInstance();
	}

	public void real_play(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
		String uuid = this.getStringParam(request, "uuid", null);
	    MonitorClient client = null;
		if(uuid == null){
			response.getWriter().println("-2:Parameter error");
		}else {
		    client = server.getMonitorClient(uuid);
		    if(client == null){
		    	response.getWriter().println("1:BTS not found");
		    }else if(client.getClientStatus() == null){
		    	response.getWriter().println("2:BTS disconnected");
		    }else {
		    	client.realPlay();
		    	response.getWriter().println("0:Video request OK");
		    }
		}
	}
	
	/**
	 * 开始视频录像。
	 * @param request
	 * @param response
	 * @throws IOException
	 */
	public void start_record(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
		String uuid = this.getStringParam(request, "uuid", null);
		String user = this.getStringParam(request, "user", null);
	    MonitorClient client = null;
		if(uuid == null || user == null){
			response.getWriter().println("-2:Parameter error");
		}else {
		    client = server.getMonitorClient(uuid);
		    if(client == null){
		    	response.getWriter().println("1:BTS not found");
		    }else if(client.getClientStatus() == null){
		    	response.getWriter().println("2:BTS disconnected");
		    }else {
		    	String sid = server.recordManager.startManualRecord(client, user);
		    	client.realPlay();
		    	response.getWriter().println("0:Start video recording$" + sid);
		    }
		}
	}
	
	/**
	 * 开启设备代理。
	 * @param request
	 * @param response
	 * @throws IOException
	 */
	public void create_proxy(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
		String uuid = this.getStringParam(request, "uuid", null);
	    MonitorClient client = null;
		if(uuid == null){
			response.getWriter().println("-2:Parameter error");
		}else {
		    client = server.getMonitorClient(uuid);
		    if(client == null){
		    	response.getWriter().println("1:BTS not found");
		    }else {
		    	int port = server.proxyServer.createProxy(client.ipAddr);
		    	if (port > 0){
		    		response.getWriter().println("0:create proxy ok");
		    		response.getWriter().println(request.getLocalAddr() + ":" + port);
		    	}else {
		    		response.getWriter().println("2:Not found free port");		    		
		    	}
		    }
		}
	}	
	
	/**
	 * 停止录像。
	 * @param request
	 * @param response
	 * @throws IOException
	 */
	public void stop_record(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
		String uuid = this.getStringParam(request, "uuid", null);
		if(uuid == null){
			response.getWriter().println("-2:Parameter error");
		}else {
			server.recordManager.stoptRecord(uuid);
		    response.getWriter().println("0:Stop video record");
		}
	}
	
	/**
	 *视频传输通道。
	 */
	public void video(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
	    String uuid = request.getParameter("uuid");
	    MonitorClient client = null;
	    if(uuid != null){
	    	client = server.getMonitorClient(uuid);
	    }
	    
	    if(client != null){
			response.setHeader("Transfer-Encoding", "chunked");
		    response.setContentType("application/octet-stream");
			//response.setContentType("video/h264");			
		    response.setStatus(HttpServletResponse.SC_OK);
		    Continuation continuation = ContinuationSupport.getContinuation(request, null);
		    response.flushBuffer();
		    
		    RealPlayRouting callback = new RealPlayRouting(continuation, 
		    		response.getOutputStream(), 
		    		request.getRemoteHost());
		    client.route.addDestination(callback);
		    //suspend 365 days
		    continuation.suspend(1000 * 60 * 60 * 365);
		    //suspend timeout.
		    callback.close();
	    }else {
	    	response.getWriter().write("Not found client by uuid:" + uuid); 
	    }
    }
	
	public void send_play(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
		response.getWriter().write("Welcome send_play!");
		
		String data = request.getParameter("data");
		
    }
    
	
	public void ping(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		response.getWriter().println("OK");
	}
	
	public void info(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		response.getWriter().println(this.server.groupName + "$" + this.server.socketServer.listenPort);
	}
	
	public void add_bs(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String uuid = this.getStringParam(request, "uuid", null);
		if(uuid == null){
			response.getWriter().println("-2:Parameter error");
		}else {
			if(server.addMonitorClient(uuid)){
				response.getWriter().println("0:Added BTS");
			}else {
				response.getWriter().println("1:Failed to add BTS");
			}
		}
	}
	
	public void del_bs(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String uuid = this.getStringParam(request, "uuid", null);
		if(uuid == null){
			response.getWriter().println("-2:Parameter error");
		}else {
			MonitorClient mc = server.getMonitorClient("uuid");
			if(mc == null){
				response.getWriter().println("1:BTS not found");
			}else if(mc.route.destinationSize() > 0){
				response.getWriter().println("2:BTS using by user");
			}else {
				server.removeMonitorClient(mc);
				response.getWriter().println("0:Disconnect BTS");
			}
		}
	}
    
	/**
	 * 服务器的内部状态。
	 */
    public void status(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
    	DateFormat format= new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
    	//response.getWriter().write("Welcome routing_status!");
    	String reset = this.getStringParam(request, "reset", "");
    	RouteRunningStatus status = server.getStatus(runningStatus, reset.equals("Y"));
    	response.getWriter().println("Time:" + format.format(status.statusTime));
    	//response.getWriter().println("UCT:" + status.statusTime.getTime());
    	response.getWriter().println("UCT:" + status.statusTime.getTime()/1000);
    	response.getWriter().println("recvData:" + status.receiveData);
    	response.getWriter().println("sentData:" + status.sendData);
    	response.getWriter().println("activeVideo:" + status.activeVideo);
    	response.getWriter().println("connectVideo:" + status.connectVideo);
    	response.getWriter().println("allVideo:" + status.allVideo);
    	response.getWriter().println("clientRequestCount:" + status.clientRequestCount);
    }

    protected void index_page(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
    	static_serve("org/goku/video/help_doc.txt", "text/plain", response);
    }
	
    class RealPlayRouting implements VideoDestination{
    	private OutputStream os = null;
    	private Continuation continuation = null;
    	private boolean running = true;
    	
    	private String remoteIp = null;
    	public RealPlayRouting(Continuation continuation, OutputStream os, String ip){
    		this.continuation = continuation;
    		this.os = os;
    		//用来输出Log.
    		this.remoteIp = ip;
    	}

		@Override
		public boolean accept(int sourceType) {
			return true;
		}
		
		public boolean isClosed(){
			return !this.running;
		}		

		@Override
		public void write(byte[] data) throws IOException {
			if(!this.running) throw new IOException("Destination closed.");
			this.os.write(data);
			os.flush();
		}

		@Override
		public void close() {
			if(this.running){
				this.running = false;
				continuation.resume();
			}
		}
		
		public String toString(){
			return String.format("HTTP<%s>", this.remoteIp);
		}
    	
    }
}
