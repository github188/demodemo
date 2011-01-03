package org.goku.master;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.URL;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.FileChannel.MapMode;
import java.nio.charset.Charset;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Collection;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.AlarmRecord;
import org.goku.core.model.BaseStation;
import org.goku.core.model.Location;
import org.goku.core.model.RouteServer;
import org.goku.core.model.SimpleCache;
import org.goku.core.model.SystemLog;
import org.goku.core.model.User;
import org.goku.db.QueryParameter;
import org.goku.db.QueryResult;
import org.goku.http.BaseRouteServlet;
import org.goku.http.HTTPRemoteClient;
import org.goku.http.HttpResponse;
import org.goku.http.SimpleHttpClient;
import org.json.simple.JSONValue;

public class MasterServerServlet extends BaseRouteServlet{
	protected DateFormat format= new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
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
		static_serve("org/goku/master/statics/index_home_page.txt", HTML, response);
	}
	
	public void help_doc(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		static_serve("org/goku/master/statics/help_doc.txt", TEXT, response);
	}	
	
	/**
	 * 返回基站列表 
	 */
	public void list_bs(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String sid = this.getStringParam(request, "sid", null);
		String mode = this.getStringParam(request, "mode", null);
		
		if(sid == null){
			response.getWriter().println("-2:Parameter error");
		}else {
			User userObj = (User)cache.get(sid);
			if(userObj == null){
				response.getWriter().println("1:Session is expired or logout");
			}else {
				Collection<BaseStation> list = server.storage.listStation(userObj);
				response.getWriter().println("0:Base station list$" + list.size());
				outputStationInfo(list, response.getWriter(), server.routeManager, mode);
				response.getWriter().println("");
			}
		}
	}
	
	/**
	 * 返回告警信息列表。 
	 */
	public void list_al(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String sid = this.getStringParam(request, "sid", null);
		
		if(sid == null){
			response.getWriter().println("-2:Parameter error");
		}else {
			User userObj = (User)cache.get(sid);
			if(userObj == null){
				response.getWriter().println("1:Session is expired or logout");
			}else {
				QueryParameter param = new QueryParameter();
				param.qsid = this.getStringParam(request, "qsid", null);
				param.limit = this.getIntParam(request, "limit", 100);
				param.offset = this.getIntParam(request, "offset", 0);
				param.order = this.getStringParam(request, "order", null);
				
				QueryResult alarms = server.storage.queryData(AlarmRecord.class, param);
				outputAlarmList(alarms, response.getWriter());
			}
		}
	}	
		
	/**
	 * 返回基站列表 
	 */
	public void add_route(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String port = this.getStringParam(request, "port", "8081");
		String socketPort = this.getStringParam(request, "socketPort", null);
		String groupName = this.getStringParam(request, "group", "default");
		
		String client = request.getRemoteHost();
		String url = "http://" + client + ":" + port;
		HTTPRemoteClient httpClient = new HTTPRemoteClient(url);
		if(httpClient.checkConnection()){
			log.info(String.format("add route:%s, group:%s, socket:%s", client + ":" + port, groupName, socketPort));
			RouteServer route = this.server.addRouteServer(client + ":" + port, groupName);
			route.socketPort = socketPort;			
			response.getWriter().println("0:Added route server");
		}else {
			response.getWriter().println("1:Failed to add route server");
		}
	}	

	/**
	 * 登录系统
	 */
	public void login(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String user = this.getStringParam(request, "user", null);
		String password = this.getStringParam(request, "password", null);
		
		String remoteAddr = request.getRemoteAddr();
		if(user == null || password == null){
			response.getWriter().println("-2:Parameter error");
		}else {
			User userObj = (User) server.storage.load(User.class, user);
			if(userObj != null){
				if(userObj.password != null && userObj.password.equals(password)){
					String key = md5(userObj.name + System.currentTimeMillis());
					cache.set(key, userObj, 60 * 30);
					request.setAttribute(SESSION_ID, key);
					request.setAttribute(SESSION_USER, userObj);
					SystemLog.saveLog(SystemLog.LOGIN_OK, user, "master", remoteAddr);
					response.getWriter().println("0:login ok$" + key);
				}else {
					SystemLog.saveLog(SystemLog.LOGIN_FAIL, user, "master", remoteAddr);
					response.getWriter().println("2:password error");
				}
			}else {
				SystemLog.saveLog(SystemLog.LOGIN_FAIL, user, "master", remoteAddr);
				response.getWriter().println("1:account not exist");
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
			User userObj = (User)cache.get(sid);
			if(userObj != null){
				String remoteAddr = request.getRemoteAddr();
				SystemLog.saveLog(SystemLog.LOGOUT, userObj.name, "master", remoteAddr);
			}
			response.getWriter().println("0:logout ok");
		}else {
			response.getWriter().println("-2:Parameter error");
		}
		
	}
	
	public void ping(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		response.getWriter().println("OK");
	}

	public void init_sql(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		static_serve("org/goku/master/statics/init_db_sql.txt", "text/plain", response);
	}
	
	public void settings_doc(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		static_serve("org/goku/master/statics/settings_doc.txt", "text/plain", response);
	}
	
	/**
	 * 将请求转发到视频转发服务器。
	 */
	public void start_record(HttpServletRequest request, HttpServletResponse response) 
	throws IOException, ServletException {
		forwardToRoute("start_record", request, response);
	}
	
	public void stop_record(HttpServletRequest request, HttpServletResponse response) 
	throws IOException, ServletException{
		forwardToRoute("stop_record", request, response);
	}
	
	public void rpc_add_location(HttpServletRequest request, HttpServletResponse response) 
	throws IOException{
		
	}
	
	public void rpc_add_bts(HttpServletRequest request, HttpServletResponse response) 
	throws IOException{
		
	}
	
	public void rpc_delete(HttpServletRequest request, HttpServletResponse response) 
	throws IOException{
		
	}
	
	public void rpc_list_bts(HttpServletRequest request, HttpServletResponse response) 
	throws IOException{
		response.setContentType("application/octet-stream");
        response.setCharacterEncoding("utf8");	
		String sid = this.getStringParam(request, "sid", null);
		Map<String, Object> data = new HashMap<String, Object>();		
		if(sid == null){
			data.put("status", "-2");
		}else {
			User userObj = (User)cache.get(sid);
			if(userObj == null){
				data.put("status", "1");
				response.getWriter().println("1:Session is expired or logout");
			}else {
				data.put("status", "0");
				Location node = server.storage.getRootLocation(userObj);
				data.put("data", node);
			}
		}
		JSONValue.writeJSONString(data, response.getWriter());
	}
	
	public void rpc_list_alarm(HttpServletRequest request, HttpServletResponse response) 
	throws IOException{
		response.setContentType("application/octet-stream");
        response.setCharacterEncoding("utf8");	
		String sid = this.getStringParam(request, "sid", null);
		
		Map<String, Object> data = new HashMap<String, Object>();
		if(sid == null){
			data.put("status", "-2");
		}else {
			User userObj = (User)cache.get(sid);
			if(userObj == null){
				data.put("status", "1");
			}else {
				data.put("status", "0");
				QueryParameter param = new QueryParameter();
				param.qsid = this.getStringParam(request, "qsid", null);
				param.limit = this.getIntParam(request, "limit", 100);
				param.offset = this.getIntParam(request, "offset", 0);
				param.order = this.getStringParam(request, "order", null);
				
				QueryResult alarms = server.storage.queryData(AlarmRecord.class, param);
				data.put("data", alarms);
			}
		}
		JSONValue.writeJSONString(data, response.getWriter());
	}	

	/**
	 * 将请求转发到视频转发服务器。
	 */	
	@SuppressWarnings("unchecked")
	protected void forwardToRoute(String action, HttpServletRequest request,
			HttpServletResponse response)throws ServletException, IOException {
		String uuid = this.getStringParam(request, "uuid", null);
		String sid = this.getStringParam(request, "sid", null);
		if(uuid == null){
			response.getWriter().println("-2:Parameter error");
		}else {
			BaseStation info = (BaseStation)server.storage.load(BaseStation.class, uuid);
			if(info == null){
				response.getWriter().println("1:BTS not found");
			}else {
				User userObj = (User)cache.get(sid);
				if(userObj == null){
					response.getWriter().println("1:Session is expired or logout");
				}else{
					if(info.routeServer != null && !info.equals("")){
						log.debug("Forward request to route:" + info.routeServer);
						SimpleHttpClient http = new SimpleHttpClient(new URL("http://" + info.routeServer));
						
						Map<String, String> param = new HashMap<String, String>();
						String name = null;
						for(Enumeration<String> enums = request.getParameterNames(); enums.hasMoreElements();){
							name = enums.nextElement();
							param.put(name, request.getParameter(name));
						}
						param.put("user", userObj.name);
						
						HttpResponse resp = http.post("/", param);
						response.getWriter().println(resp.getResponseMessage());
					}else {
						response.getWriter().println("9:BTS not connect to route server");
					}
				}
			}
		}
	}
	
	private void outputStationInfo(Collection<BaseStation> list, PrintWriter out, RouteServerManager rm, String mode){
		String data = null;
		for(BaseStation info: list){
			String routeAddr = info.routeServer;
			if(mode != null && mode.equals("socket")){
				RouteServer route = rm.getRouteReserver(info.routeServer);
				routeAddr = route.getConnectAddr(mode);
			}
			routeAddr = routeAddr == null ? "": routeAddr;
			
			data = info.uuid + "$" + info.devType + "$" +routeAddr + "$" + info.getStatus();
			out.println(data);
		}
	}
	
	private void outputAlarmList(QueryResult result, PrintWriter out){
		out.println(String.format("0:alarm list$%s$%s$%s", result.count, result.data.size(), result.sessionId));
		AlarmRecord alarm = null;
		String data = null;
		for(Iterator iter = result.data.iterator(); iter.hasNext();){
			alarm = (AlarmRecord)iter.next();
			data = String.format("%s$%s$%s$%s$%s$%s$%s", alarm.uuid, alarm.alarmType, alarm.baseStation,
					alarm.alarmStatus, alarm.getLevel(), 
					format.format(alarm.startTime),
					format.format(alarm.endTime));
			out.println(data);
		}
		out.println();
	}	
	
	//private String md5(String str)
	
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
