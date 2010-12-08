package org.goku.video;

import java.io.IOException;
import java.io.OutputStream;
import java.io.Writer;

import javax.servlet.ServletConfig;
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
	
	/**
	 * 实时监控，请求。将视频数据流，以HTTP下载的方式返回。建立一个长连接，异步输出
	 * 监控录像。
	 */
	public void real_play(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
	    String uuid = request.getParameter("bid");
	    MonitorClient client = null;
	    if(uuid != null){
	    	client = server.getMonitorClient(uuid);
	    }
	    
	    if(client != null){
			response.setHeader("Transfer-Encoding", "chunked");
		    response.setContentType("application/octet-stream");
		    response.setStatus(HttpServletResponse.SC_OK);
		    Continuation continuation = ContinuationSupport.getContinuation(request, null);
		    response.flushBuffer();
		    
		    RealPlayRouting callback = new RealPlayRouting(continuation, 
		    		response.getOutputStream(), 
		    		request.getRemoteHost());
		    client.realPlay(callback);		    
		    //suspend 1 hour
		    continuation.suspend(1000 * 60 * 60);
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
    
	
	/**
	 * 心跳检查。
	 */
    public void heart_ok(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
    	response.getWriter().write("Welcome heart_ok!");      	
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
