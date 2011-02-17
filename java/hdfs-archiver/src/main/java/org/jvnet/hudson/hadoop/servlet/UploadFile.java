package org.jvnet.hudson.hadoop.servlet;

import java.io.IOException;
import java.io.InputStream;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileItemIterator;
import org.apache.commons.fileupload.FileUploadException;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.jvnet.hudson.hadoop.HDFSArchiver;

public class UploadFile extends BaseServlet{
	private Log log = LogFactory.getLog("hdfs.servlet");
	private static final long serialVersionUID = 1L;

	protected void doGet(HttpServletRequest request, HttpServletResponse response) 
	throws ServletException, IOException {
	    response.setContentType("text/html");
	    response.setCharacterEncoding("utf-8");
	    
	    this.outputStatic("gui_header.html", response.getWriter());
	    this.outputStatic("upload_form.html", response.getWriter());
	    this.outputStatic("gui_footer.html", response.getWriter());
    }
    
    protected void doPost(HttpServletRequest request, HttpServletResponse response) 
	throws ServletException, IOException {
    	//log.info("Request:" + path);
    	boolean isMultipart = ServletFileUpload.isMultipartContent(request);
    	if(isMultipart){
    		ServletFileUpload upload = new ServletFileUpload();
    		
    		String user = request.getParameter("user");
    		String dir = request.getParameter("dir");
    		String path = request.getParameter("path"); 
    		    		
    		try {
				FileItemIterator iter = upload.getItemIterator(request);
				while (iter.hasNext()) {
				    FileItem item = (FileItem) iter.next();
				    if (item.isFormField()) {
				    	if(item.getFieldName().equals("user")){
				    		user = item.getString();
				    	}else if(item.getFieldName().equals("dir")){
				    		dir = item.getString();
				    	}else if(item.getFieldName().equals("path")){
				    		path = item.getString();
				    	}
				    } else {
				        processUploadedFile(item, getArchivePath(user, dir, path));
				    }
				}
			} catch (FileUploadException e) {
				log.error(e.toString(), e);
			}
    	}
    	doGet(request, response);
    }
    
    protected void processUploadedFile(FileItem item, String path) throws IOException{
    	path += "/" + item.getName();
    	log.info("Archive to " + path + ", size:" + item.getSize());
    	InputStream ins = item.getInputStream();
    	HDFSArchiver.getArchiver().archiveFile(path, 
    			ins, item.getSize());
    	ins.close();
    }
    
    protected String getArchivePath(String user, String dir, String path){
    	String temp = "";
    	if(user != null && !user.trim().equals("")){
    		temp += "/" + user.trim();
    	}
    	if(dir != null && !dir.trim().equals("")){
    		temp += "/" + dir.trim();
    	}
    	if(path != null && !path.trim().equals("")){
    		temp += "/" + path;
    	}
    	return temp;    	
    }
}
