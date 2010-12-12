package org.goku.video;

import java.io.IOException;
import java.io.OutputStream;
import java.io.Writer;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
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
			response.getWriter().println("-2:参数错误");
		}else {
		    client = server.getMonitorClient(uuid);
		    if(client == null){
		    	response.getWriter().println("1:基站不存在");
		    }else if(client.getClientStatus() == null){
		    	response.getWriter().println("2:未建立连接");
		    }else {
		    	client.realPlay();
		    	response.getWriter().println("0:请求发送成功");
		    }
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
	
	public void add_bs(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String uuid = this.getStringParam(request, "uuid", null);
		if(uuid == null){
			response.getWriter().println("-2:参数错误");
		}else {
			if(server.addMonitorClient(uuid)){
				response.getWriter().println("0:添加成功");
			}else {
				response.getWriter().println("1:添加失败");
			}
		}
	}
	
	public void del_bs(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String uuid = this.getStringParam(request, "uuid", null);
		if(uuid == null){
			response.getWriter().println("-2:参数错误");
		}else {
			MonitorClient mc = server.getMonitorClient("uuid");
			if(mc == null){
				response.getWriter().println("1:不存在基站");
			}else if(mc.route.destinationSize() > 0){
				response.getWriter().println("2:有连接的用户");
			}else {
				server.removeMonitorClient(mc);
				response.getWriter().println("0:停止成功");
			}
		}
	}
    
	/**
	 * 服务器的内部状态。
	 */
    public void routing_status(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
    	response.getWriter().write("Welcome routing_status!");    	
    } 

    protected void index_page(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
    	response.getWriter().write("Welcome video router!");
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
