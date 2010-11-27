package org.goku.http;

import java.io.IOException;
import java.io.Writer;

import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.route.VideoRouteServer;
import org.mortbay.util.ajax.Continuation;
import org.mortbay.util.ajax.ContinuationSupport;

public class DefaultRouteServerServlet extends BaseRouteServlet{
	private VideoRouteServer server = null;
	private Log log = LogFactory.getLog("http");
	
	private static Writer test = null;
	private static Continuation continuation = null;
	
	public void init(ServletConfig config){
		server = VideoRouteServer.getInstance();
	}
	
	/**
	 * 实时监控，请求。将视频数据流，以HTTP下载的方式返回。建立一个长连接，异步输出
	 * 监控录像。
	 */
	public void real_play(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
		response.setHeader("Transfer-Encoding", "chunked");
	    response.setContentType("text/plain");
	    response.setStatus(HttpServletResponse.SC_OK);
	    
	    //Transfer-Encoding: chunked
    	response.getWriter().write("Welcome real_play!");
    	
    	
    	log.info("start continuation....");
    	
    	continuation = ContinuationSupport.getContinuation(request, null);
    	
    	test = response.getWriter();
    	
    	test.write("waiting...\n");
    	test.flush();
    	response.flushBuffer();
    	
    	continuation.suspend(1000 * 1000);
    	
    	log.info("resumed thread...");
    	//response.getOutputStream().flush()
    	//response.getWriter().f
    }
	
	public void send_play(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
		response.getWriter().write("Welcome send_play!");
		
		String data = request.getParameter("data");
		
		if(data != null){
			try{
				log.info("send:" + data);
				test.write(data + "\n");
				test.flush();
			}catch(Exception e){
				log.error(e.toString(), e);
			}
			if(data.equals("close")){
				continuation.resume();
			}
		}
		
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
    	response.getWriter().write("Welcome route!");
    } 
	
}
