package org.jvnet.hudson.hadoop.servlet;

import java.io.IOException;
import java.io.InputStream;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileItemIterator;
import org.apache.commons.fileupload.FileItemStream;
import org.apache.commons.fileupload.FileUploadException;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.apache.commons.fileupload.util.Streams;
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
					FileItemStream item = iter.next();
				    if (item.isFormField()) {
				    	InputStream stream = item.openStream();
				    	if(item.getFieldName().equals("user")){
				    		user = Streams.asString(stream);
				    	}else if(item.getFieldName().equals("dir")){
				    		dir = Streams.asString(stream);
				    	}else if(item.getFieldName().equals("path")){
				    		path = Streams.asString(stream);
				    	}
				    	stream.close();
				    } else {
				        processUploadedFile(item, getArchivePath(user, dir, path));
				    }
				}
			} catch (FileUploadException e) {
				log.error(e.toString(), e);
			}
    	}else {
    		log.warn("The request is not a multpart content type.");
    	}
    	doGet(request, response);
    }
    
    protected void processUploadedFile(FileItemStream item, String path) throws IOException{
    	path += "/" + item.getName();
    	int size = 0;
    	try{
    		String length = item.getHeaders().getHeader("Content-Length");
    		size = Integer.parseInt(length);
    	}catch(Exception e){
    	}
    	//item.g
    	log.info("Archive to " + path + ", size:" + size);
    	//Content-Length
    	InputStream ins = item.openStream();
    	HDFSArchiver.getArchiver().archiveFile(path, 
    			ins, size);
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
