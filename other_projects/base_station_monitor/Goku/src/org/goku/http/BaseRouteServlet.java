package org.goku.http;

import java.io.IOException;
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
	private static final long serialVersionUID = 1L;
	private static final Map<String, Method> handler = new HashMap<String, Method>();
	
	private Log log = LogFactory.getLog("http");

	protected void doGet(HttpServletRequest request, HttpServletResponse response) 
	throws ServletException, IOException {
    	doPost(request, response);
    }
    
    protected void doPost(HttpServletRequest request, HttpServletResponse response) 
	throws ServletException, IOException {
    	String action = request.getParameter("q");
		Method m = handler.get(action);
		log.debug("New http action:" + action);
		if(m == null && action != null){
			try {
				m = this.getClass().getMethod(action, new Class[]{HttpServletRequest.class, HttpServletResponse.class});
				handler.put(action, m);
			} catch (Exception e1) {
				//log.error(e1, e1);
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

}
