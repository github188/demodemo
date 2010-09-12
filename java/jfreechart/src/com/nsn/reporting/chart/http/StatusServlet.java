package com.nsn.reporting.chart.http;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class StatusServlet extends HttpServlet{
	//private Map<String, String> param = new HashMap<String, String>();
	private Log log = LogFactory.getLog(StatusServlet.class);
	private SimpleHTTPServer service = null;
    public StatusServlet(){
    	service = SimpleHTTPServer.curInstance();
    }
    
    protected void doGet(HttpServletRequest request, HttpServletResponse response) 
    	throws ServletException, IOException {
    	this.doPost(request, response);
    }
    
    protected void doPost(HttpServletRequest request, HttpServletResponse response) 
	throws ServletException, IOException {
	    response.setContentType("text/plain");
	    response.setStatus(HttpServletResponse.SC_OK);
	    
	    int cached = SimpleHTTPServer.curInstance().cache.size();
	    response.getWriter().println("Cached Charts:" + cached);
	    
	    Runtime rt = Runtime.getRuntime();	    
	    PrintWriter w = response.getWriter();
	    w.println("availableProcessors:" + rt.availableProcessors());
	    w.println("freeMemory:" + byteToMega(rt.freeMemory()));
	    w.println("maxMemory:" + byteToMega(rt.maxMemory()));
	    w.println("totalMemory:" + byteToMega(rt.totalMemory()));
    }
    
    private String byteToMega(long x){
    	return (x / 1024.0 / 1024.0 ) + " M";
    }
}
