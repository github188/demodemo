package org.jvnet.hudson.hadoop.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;


public class DirectoryList extends BaseServlet{
	private Log log = LogFactory.getLog("hdfs.servlet");
	private static final long serialVersionUID = 1L;

	protected void doGet(HttpServletRequest request, HttpServletResponse response) 
	throws ServletException, IOException {
    	doPost(request, response);
    }
    
    protected void doPost(HttpServletRequest request, HttpServletResponse response) 
	throws ServletException, IOException {
    	String path = request.getPathInfo();
    	log.info("Request:" + path);
    	response.getWriter().write("welcome archiver.");
    }
    
    private void showDirectoryList(){
    	
    }
    
    private void serveFile(){
    	
    }
    
    private void notFound(){
    	
    }
}
