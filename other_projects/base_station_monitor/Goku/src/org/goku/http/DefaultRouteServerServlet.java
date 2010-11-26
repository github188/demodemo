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
    protected void real_play(HttpServletRequest request, HttpServletResponse response) 
	throws ServletException, IOException {
    	
    }
    
	
	/**
	 * 心跳检查。
	 */
    protected void heart_ok(HttpServletRequest request, HttpServletResponse response) 
	throws ServletException, IOException {
    	
    }
    
	/**
	 * 服务器的内部状态。
	 */
    protected void routing_status(HttpServletRequest request, HttpServletResponse response) 
	throws ServletException, IOException {
    	
    } 
	
}
