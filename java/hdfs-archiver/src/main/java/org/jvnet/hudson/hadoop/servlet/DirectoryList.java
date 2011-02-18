package org.jvnet.hudson.hadoop.servlet;

import java.io.IOException;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.jvnet.hudson.hadoop.HDFSArchiver;


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
    	path = path.substring(1);
    	log.info("Request:" + path);
    	int offset = 0;
    	int limit = 100;
    	try{
    		offset = Integer.parseInt(request.getParameter("offset"));
    	}catch(Exception e){};
    	try{
    		limit = Integer.parseInt(request.getParameter("limit"));
    	}catch(Exception e){};
    	
    	List fileList = HDFSArchiver.getArchiver().searchFile(path, offset, limit);
    	//response.getWriter().write("welcome archiver.");
    }
    
    private void showDirectoryList(){
    	//DBObject obj = new BasicDBObject();
    }
    
    private void serveFile(){
    	
    }
    
    private void notFound(){
    	
    }
}
