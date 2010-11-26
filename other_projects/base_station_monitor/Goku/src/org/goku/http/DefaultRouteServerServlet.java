package org.goku.http;

import java.io.IOException;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.goku.route.RouteServer;

public class DefaultRouteServerServlet extends BaseRouteServlet{
	private RouteServer server = null;
	
	public void init(ServletConfig config){
		server = RouteServer.getInstance();
	}
	
	/**
	 * 实时监控，请求。将视频数据流，以HTTP下载的方式返回。建立一个长连接，异步输出
	 * 监控录像。
	 */
	public void real_play(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
    	response.getWriter().write("Welcome real_play!");      	
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
