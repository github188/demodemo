package org.goku.http;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public abstract class BaseRouteServlet extends HttpServlet{
	public static final String TEXT = "text/plain";
	
	private static final long serialVersionUID = 1L;
	private static final Map<String, Method> handler = new HashMap<String, Method>();
	
	private Log log = LogFactory.getLog("http");

	protected void doGet(HttpServletRequest request, HttpServletResponse response) 
	throws ServletException, IOException {
    	doPost(request, response);
    }
    
    protected void doPost(HttpServletRequest request, HttpServletResponse response) 
	throws ServletException, IOException {
    	response.setCharacterEncoding("utf-8");
    	String action = request.getParameter("q");
		Method m = handler.get(action);
		log.debug("New http action:" + action);
		if(m == null && action != null){
			try {
				m = this.getClass().getMethod(action, new Class[]{HttpServletRequest.class, HttpServletResponse.class});
				handler.put(action, m);
			} catch (Exception e1) {
				log.error(e1, e1);
			}
		}
		if(m == null){
			index_page(request, response);
		}else {
			try {
				m.invoke(this, new Object[]{request, response});
			} catch (Exception e1) {
				log.error(e1, e1);
			}
		}    	
    }
    
    protected abstract void index_page(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException;
    
    protected void static_serve(String path, String mimeType, HttpServletResponse response) throws IOException{
	    response.setContentType(mimeType == null ? TEXT : mimeType);
	    response.setCharacterEncoding("utf-8");
	    InputStream ins = this.getClass().getClassLoader().getResourceAsStream(path);
	    byte[] buffer = new byte[64 * 1024];
	    if(ins == null){
	    	//response.setStatus(HttpServletResponse.SC_NOT_FOUND);
	    	response.getWriter().write("Not found:" + path);
	    }else {
	    	for(int len = ins.read(buffer); len > 0; ){
	    		response.getOutputStream().write(buffer, 0, len);
	    		len = ins.read(buffer);
	    	}	    	
	    }
    }
    
    protected String getStringParam(HttpServletRequest request, String name, String def){
    	String val = request.getParameter(name);
    	if(val == null || val.trim().equals("")){
    		val = def;
    	}
    	return val;
    }
    
    protected int getIntParam(HttpServletRequest request, String name, int def){
    	String val = request.getParameter(name);
    	int intVal = def;
    	if(val != null && !val.trim().equals("")){
    		intVal = Integer.parseInt(val);
    	}
    	return intVal;
    }
}
