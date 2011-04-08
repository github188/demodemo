package org.goku.image;

import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.http.BaseRouteServlet;

public class ImageRouteServerServlet extends BaseRouteServlet {
	private ImageRouteServer server = null;
	private Log log = LogFactory.getLog("http");
	
	public void init(ServletConfig config){
		server = ImageRouteServer.getInstance();
	}	

	@Override
	protected void index_page(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		static_serve("org/goku/image/statics/help_doc.txt", "text/plain", response);

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
	
	/**
	 * 图片服务器，目前没有实现多个服务器，做负载调度。不需要删除操作。
	 */
	public void del_bs(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String uuid = this.getStringParam(request, "uuid", null);
		if(uuid == null){
			response.getWriter().println("-2:Parameter error");
		}else {
			response.getWriter().println("0:Disconnect BTS");
		}
	}
    
	/**
	 * 服务器的内部状态。
	 */
    public void status(HttpServletRequest request, HttpServletResponse response) 
	throws IOException {
    	DateFormat format= new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    	//response.getWriter().write("Welcome routing_status!");
    	String reset = this.getStringParam(request, "reset", "");
    	response.getWriter().println("Time:" + "OK");
    	//response.getWriter().println("Time:" + format.format(status.statusTime));
    	//response.getWriter().println("UCT:" + status.statusTime.getTime());
    }	

}
