package org.goku.master;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.FileChannel.MapMode;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.http.BaseRouteServlet;

public class MasterServerServlet extends BaseRouteServlet{
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
	    response.setStatus(HttpServletResponse.SC_OK);
	    
	    String range = request.getHeader("Range");
	    long start = 0, end = Long.MAX_VALUE;
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
	    start = Math.min(start, end);
	    
	    log.info(String.format("Start replay video, mime:%s, Range bytes=%s-%s, file:%s", 
				   mime, start, end, file));
	    
	    FileChannel channel = new FileInputStream(file).getChannel();
	    MappedByteBuffer buffer = channel.map(MapMode.READ_ONLY, start, end - start);
	    
	    byte[] byteBuffer  = new byte[64 * 1024];
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
	    
	    log.debug("Done replay.");
	}
	
	@Override
	protected void index_page(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		response.getWriter().write("Welcome master server!");
	}
	
	/**
	 * 换回基站列表 
	 */
	protected void list_station(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		//response.getWriter().write("Welcome master server!");
		//"select uuid, manageServer, devType, connectionStatus, alarmStatus " +
		//"from base_station"
	}

	/**
	 * 登录系统
	 */
	protected void login(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		response.getWriter().write("Welcome master server!");
	}
}
