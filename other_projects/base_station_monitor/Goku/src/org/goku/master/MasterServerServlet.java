package org.goku.master;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

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
			response.getWriter().write("Parameter list 'id=<PK>', 'mime=text/plain', 'len=128', 'delay=2'.");
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
		mime = mime == null ? "text/plain" : mime;		
	    response.setContentType(mime);
	    response.setStatus(HttpServletResponse.SC_OK);
	    
	    
	    int len = 128;
	    try{
	    	len = Integer.parseInt(request.getParameter("len"));
	    }catch(Exception e){};
	    
	    int delay = 2;
	    try{
	    	delay = Integer.parseInt(request.getParameter("delay"));
	    }catch(Exception e){};	    
	    
	    log.info("Start replay mime:" + mime + ", len:" + len + ", delay:" + delay + ", file:" + file);
	    
	    byte[] buffer = new byte[len];
	    
	    InputStream in = new FileInputStream(file);
	    
	    int readLen = in.read(buffer);
	    
	    while(readLen > 0){
	    	response.getOutputStream().write(buffer, 0, readLen);
	    	response.flushBuffer();
	    	log.info("sleep " + delay + " seconds...");
	    	try {
				Thread.sleep(delay * 1000);
			} catch (InterruptedException e) {
				log.error(e.toString(), e);
			}
			readLen = in.read(buffer);
	    }
	    
	    log.info("done replay");
	}
	
	@Override
	protected void index_page(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		response.getWriter().write("Welcome master server!");
	}

}
