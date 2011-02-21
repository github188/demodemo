package org.jvnet.hudson.hadoop.servlet;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.jvnet.hudson.hadoop.HDFSArchiver;

import com.mongodb.gridfs.GridFSDBFile;


public class DirectoryList extends BaseServlet{
	private Log log = LogFactory.getLog("hdfs.servlet");
	private static final long serialVersionUID = 1L;
	private static Map<String, String> mime = new HashMap<String, String>();
	static{
		mime.put("html", "text/html");
		mime.put("htm", "text/html");
		mime.put("js", "application/x-javascript");
		mime.put("css", "text/css");
		mime.put("gif", "image/gif");
		mime.put("jpeg", "image/jpeg");
		mime.put("jpg", "image/jpeg");		
		mime.put("xml", "text/xml");
		mime.put("txt", "text/plain");
		mime.put("log", "text/plain");
	}

	protected void doGet(HttpServletRequest request, HttpServletResponse response)
	throws ServletException, IOException {
    	doPost(request, response);
    }
    
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
	throws ServletException, IOException {
    	String path = request.getPathInfo();
    	log.info("Request:" + path);
    	if(path == null) path = "/";
    	path = path.substring(1);
    	if(path.indexOf("!") > 0){
    		this.viewZipFile(path, request, response);
    	}else if(isQuery(request, path)){
    		showDirectoryList(path, request, response);
    	}else {
    		serveFile(path, request, response);
    	}
    	//response.getWriter().write("welcome archiver.");
    }
    
    private boolean isQuery(HttpServletRequest request, String path){
    	boolean query = false;
    	if(request.getParameter("q") != null && request.getParameter("limit") != null){
    		query = true;
    	}else if(path.endsWith("/") ||  //查询一个目录
    			path.length() == 0 ||   //根路径
    			path.indexOf('*') > 0 //路径里有通篇符
    			//path.indexOf('!') > 0 //浏览压缩文件。
    			){
    		query = true;
    	}
    	return query;
    }
    
    private void showDirectoryList(String path, 
    		HttpServletRequest request,
    		HttpServletResponse response) throws IOException{
    	response.setContentType("text/html");
    	response.setCharacterEncoding("utf8");
    	int offset = 0;
    	int limit = 100;
    	try{
    		offset = Integer.parseInt(request.getParameter("offset"));
    	}catch(Exception e){};
    	try{
    		limit = Integer.parseInt(request.getParameter("limit"));
    	}catch(Exception e){};
    	if(request.getParameter("q") != null){
    		path = request.getParameter("q");
    	}
    	
    	List fileList = HDFSArchiver.getArchiver().searchFile(path, offset, limit);
    	
    	log.info("find file count:" + fileList.size() + ", path:" + path + ", offset:" + offset + ", limit:" + limit);
    	HashMap<String, Object> context = new HashMap<String, Object>();
    	context.put("path", path);
    	context.put("offset", offset);
    	context.put("limit", limit);
    	
    	context.put("fileList", fileList);
    	this.outputStatic("gui_header.html", response.getWriter());
    	this.renderTemplate("dir_list.html", context, response.getWriter());
    	this.outputStatic("gui_footer.html", response.getWriter());
    }
    
    private void serveFile(String path, 
    		HttpServletRequest request,
    		HttpServletResponse response) throws IOException{
    	
    	GridFSDBFile file = HDFSArchiver.getArchiver().getFile(path);
    	if(file != null){
    		String content = file.getContentType();
    		if(content == null){
    			content = getContentType(path);
    		}
    		response.setContentType(content);
    		log.debug("Response file:" + path + ", content:" + content);
    		file.writeTo(response.getOutputStream());
    	}else {
    		log.debug("Not found file:" + path);
    		response.setStatus(HttpServletResponse.SC_NOT_FOUND);
    		response.getWriter().write("Not found file:" + path);
    	}
    }
    
    private String getContentType(String path){
    	String[] xx = path.split("\\.");
    	String type = mime.get(xx[xx.length-1]);
    	if(type == null)type = "application/octet-stream";
    	return type;
    }
    
    private void viewZipFile(String path, HttpServletRequest request,
    						 HttpServletResponse response) throws IOException{
    	String[] uri = path.split("!", 2);
    	String internalPath = uri[1].substring(1);
    	ZipFile zipFile = HDFSArchiver.getArchiver().getCachedZip(uri[0]);
    	request.setAttribute("zipFile", uri[0]);
    	if(zipFile == null){
    		log.debug("Not found file:" + path);
    		response.setStatus(HttpServletResponse.SC_NOT_FOUND);
    		response.getWriter().write("Not found file:" + path);    		
    	}else if(uri[1].endsWith("/")){
    		listZipFile(zipFile, internalPath, request, response);
    	}else {
    		ZipEntry entry = zipFile.getEntry(internalPath);
    		if(entry != null){
    			response.setContentType(getContentType(internalPath));
    			InputStream in = zipFile.getInputStream(entry);
    			if(in != null){
    				byte[] buffer = new byte[1024 * 64];
    				for(int len = in.read(buffer); len > 0;){
    					response.getOutputStream().write(buffer, 0, len);
    					len = in.read(buffer);
    				}
    				in.close();
    			}else {
            		response.setStatus(HttpServletResponse.SC_NOT_FOUND);
            		response.getWriter().write("Failed to open file:" + internalPath);
    			}
    		}else {
        		log.debug("Not found file:" + internalPath);
        		response.setStatus(HttpServletResponse.SC_NOT_FOUND);
        		response.getWriter().write("Not found file:" + internalPath);
        	}
    	}
    }

    private void listZipFile(ZipFile zipFile, String path, HttpServletRequest request,
			 HttpServletResponse response) throws IOException{
    	response.setContentType("text/html");
    	response.setCharacterEncoding("utf8");
    	
    	List<ZipEntry> list = new ArrayList<ZipEntry>();
    	ZipEntry entry = null;
    	for(Enumeration enumeration = zipFile.entries(); enumeration.hasMoreElements();){
    		entry = (ZipEntry)enumeration.nextElement();
    		//log.info("zip entry:" + entry.getName());
    		if(!entry.getName().startsWith(path)) continue;
    		String name = entry.getName(); 
    		if(name.endsWith("/"))name = name.substring(0, name.length() -1);
    		if(name.indexOf('/', path.length()) > 0) continue;
    		list.add(entry);
    	}
    	
    	log.info("list file count:" + list.size() + ", path:" + path);
    	HashMap<String, Object> context = new HashMap<String, Object>();
    	context.put("path", path);    	
    	context.put("fileList", list);
    	context.put("zipFile", request.getAttribute("zipFile"));
    	this.outputStatic("gui_header.html", response.getWriter());
    	this.renderTemplate("zip_dir_list.html", context, response.getWriter());
    	this.outputStatic("gui_footer.html", response.getWriter());    	
    }
}
