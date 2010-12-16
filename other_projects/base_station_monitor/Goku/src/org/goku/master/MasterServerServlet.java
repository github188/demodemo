package org.goku.master;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.FileChannel.MapMode;
import java.nio.charset.Charset;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Collection;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.BaseStation;
import org.goku.core.model.SimpleCache;
import org.goku.core.model.User;
import org.goku.http.BaseRouteServlet;
import org.goku.http.HTTPRemoteClient;

public class MasterServerServlet extends BaseRouteServlet{
	private static SimpleCache cache = new SimpleCache();
	private MasterVideoServer server = null;
	private Log log = LogFactory.getLog("http");
	
	public void init(ServletConfig config){
		server = MasterVideoServer.getInstance();
	}	

	public void replay(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String id = request.getParameter("id");
		if(id == null){
			response.getWriter().write("Parameter list 'id=<PK>', 'mime=text/plain'");
		}else {
			String file = server.settings.getString(id, null);
			if(file != null && new File(file).exists() ){
				_play(file, request, response);		
			}else if(file != null){
				response.getWriter().write("Not found file:" + file);
			}else {
				response.getWriter().write("Not found file by id " + id);
			}
		}
		
	}
	
	private void _play(String file, HttpServletRequest request, 
					HttpServletResponse response) throws ServletException, IOException{
		response.setHeader("Transfer-Encoding", "chunked");
		
		String mime = request.getParameter("mime");
		mime = mime == null ? "application/octet-stream" : mime;		
	    response.setContentType(mime);
	    response.setStatus(HttpServletResponse.SC_PARTIAL_CONTENT);
	    
	    String range = request.getHeader("Range");
	    long start = 0, end = Integer.MAX_VALUE;
	    if(range != null){
	    	log.debug("Request range:" + range);
	    	String[] ranges = range.split("=", 2)[1].split("-", 2);
	    	try{
	    		start = Integer.parseInt(ranges[0].trim());
	    	}catch(Exception e){}
	    	try{
	    		end = Integer.parseInt(ranges[1].trim());
	    	}catch(Exception e){}
	    }
	    	    
	    long fileSize = new File(file).length();
	    end = Math.min(end, fileSize);
	    //end = Math.min(end, start + 1024 * 1024);
	    start = Math.min(start, end);
	    
	    log.info(String.format("Start replay video, mime:%s, Range bytes=%s-%s, file:%s", 
				   mime, start, end, file));
	    
	    response.setHeader("Content-Length", (end - start) + "");
	    response.setHeader("Content-Range", String.format("bytes %s-%s/%s", start, end, fileSize));
	    
	    FileChannel channel = new FileInputStream(file).getChannel();
	    MappedByteBuffer buffer = channel.map(MapMode.READ_ONLY, start, end - start);
	    
	    byte[] byteBuffer  = new byte[1024 * 640];
	    for(int remain = 0; buffer.hasRemaining();){
	    	//如果剩余数据大于Buffer直接发送整个Buffer, 否则只发送剩余数据。
	    	remain = buffer.remaining();
	    	if(remain > byteBuffer.length){
	    		buffer.get(byteBuffer);
	    		response.getOutputStream().write(byteBuffer);
	    	}else {
	    		buffer.get(byteBuffer, 0, remain);
	    		response.getOutputStream().write(byteBuffer, 0, remain);
	    	}
	    	response.flushBuffer();
	    }
	    
	    channel.close();
	    
	    log.debug("Done replay.");
	}
	
	@Override
	protected void index_page(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		//response.getWriter().write("Welcome master server!");
		static_serve("org/goku/master/help_doc.txt", "text/plain", response);
	}
	
	/**
	 * 返回基站列表 
	 */
	public void list_bs(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String sid = this.getStringParam(request, "sid", null);
		
		if(sid == null){
			response.getWriter().println("-2:参数错误");
		}else {
			User userObj = (User)cache.get(sid);
			if(userObj == null){
				response.getWriter().println("1:会话过期或已注销");
			}else {
				Collection<BaseStation> list = server.storage.listStation(userObj);
				response.getWriter().println("0:基站信息列表$" + list.size());
				outputStationInfo(list, response.getWriter());
				response.getWriter().println("");
			}
		}
	}
		
	/**
	 * 返回基站列表 
	 */
	public void add_route(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String port = this.getStringParam(request, "port", "8081");
		String groupName = this.getStringParam(request, "group", "default");
		
		String client = request.getRemoteHost();
		String url = "http://" + client + ":" + port;
		HTTPRemoteClient httpClient = new HTTPRemoteClient(url);
		if(httpClient.checkConnection()){
			log.info(String.format("add route:%s, group:%s", client + ":" + port, groupName));
			this.server.addRouteServer(client + ":" + port, groupName);
			
			response.getWriter().println("0:添加Route成功");
		}else {
			response.getWriter().println("1:连接Route失败");
		}
	}	

	/**
	 * 登录系统
	 */
	public void login(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String user = this.getStringParam(request, "user", null);
		String password = this.getStringParam(request, "password", null);

		if(user == null || password == null){
			response.getWriter().println("-2:参数错误");
		}else {
			User userObj = (User) server.storage.load(User.class, user);
			if(userObj != null){
				if(userObj.password != null && userObj.password.equals(password)){
					String key = md5(userObj.name + System.currentTimeMillis());
					cache.set(key, userObj, 60 * 30);
					request.setAttribute(SESSION_ID, key);
					request.setAttribute(SESSION_USER, userObj);
					response.getWriter().println("0:登录成功$" + key);
				}else {
					response.getWriter().println("2:密码错误");
				}
			}else {
				response.getWriter().println("1:帐号不存在");
			}
		}
	}
	
	public void logout(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String sid = this.getStringParam(request, "sid", null);
		
		if(sid != null){
			cache.remove(sid);
			request.setAttribute(SESSION_ID, null);
			request.setAttribute(SESSION_USER, null);			
			response.getWriter().println("0:注销成功");
		}else {
			response.getWriter().println("-2:参数错误");
		}
		
	}
	
	public void ping(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		response.getWriter().println("OK");
	}
	/**
	 * 登录系统
	 */
	public void init_sql(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		static_serve("org/goku/master/init_db_sql.txt", "text/plain", response);
	}
	
	private void outputStationInfo(Collection<BaseStation> list, PrintWriter out){
		String data = null;
		for(BaseStation info: list){
			data = info.uuid + "$" + info.devType + "$" + info.routeServer + "$" + info.getStatus();
			out.println(data);
		}
	}
	
	private String md5(String str){
	    MessageDigest messageDigest = null;
		try {
			messageDigest = MessageDigest.getInstance("MD5");
		} catch (NoSuchAlgorithmException e1) {
		}
		if(messageDigest != null){
			messageDigest.reset();
			messageDigest.update(str.getBytes(Charset.forName("UTF8")));
			final byte[] resultByte = messageDigest.digest();
		    String result = "";
		    for(byte e: resultByte){
		    	result += String.format("%x", e);
		    }
		    return result;			
		}
		return "n/a";
	}
}
