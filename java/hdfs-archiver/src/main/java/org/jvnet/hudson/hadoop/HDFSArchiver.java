package org.jvnet.hudson.hadoop;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.lang.ref.SoftReference;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.zip.ZipFile;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.mortbay.jetty.Server;
import org.mortbay.jetty.servlet.ServletHandler;

import com.mongodb.BasicDBObject;
import com.mongodb.DB;
import com.mongodb.DBCursor;
import com.mongodb.DBObject;
import com.mongodb.Mongo;
import com.mongodb.ServerAddress;
import com.mongodb.gridfs.GridFS;
import com.mongodb.gridfs.GridFSDBFile;
import com.mongodb.gridfs.GridFSInputFile;

public class HDFSArchiver {
	private Log log = LogFactory.getLog("hdfs.archiver");
	public int httpPort = 8924;
	public String[] conns = null;
	public String archiveRoot = "/archiver";
	public File rootPath = null;
	
	public GridFS fs = null;
	public Map<String, SoftReference<ZipFileWrapper>> zipCache = new HashMap<String, SoftReference<ZipFileWrapper>>();//cache.put(sid, new SoftReference<ChartData>(chart));
		
	protected static HDFSArchiver ins = null;
	
	public HDFSArchiver(String httpPort, String[] conn, String local){
		rootPath = new File(local).getAbsoluteFile();
		try{
			this.httpPort = Integer.parseInt(httpPort);
		}catch(Exception e){
			log.error("Invalid http port:" + httpPort);
		}
		
		this.conns = conn;
		ins = this;
	}
	
	public static HDFSArchiver getArchiver(){
		return ins;
	}
	
	public static void main(String[] args) throws IOException{
		if(args.length == 1){
			HDFSArchiver archiver = new HDFSArchiver("8924", args, ".");
			archiver.start();
		}else {
			System.out.println("Usage:java HDFSArchiver <HDFS namenode>");
		}
	}
	
	public void start() throws IOException{
		connectHDFS();
		startHTTPServer();
	}
	
	private void connectHDFS() throws IOException{
		List addrs = new ArrayList();
		for(String x : conns){
			log.info("DB conn:" + x);
			String[] addr = x.split(":");
			int port = Integer.parseInt(addr[1]);			
			addrs.add(new ServerAddress(addr[0], port));
		}
		Mongo mongo = new Mongo(addrs);	
		mongo.slaveOk();
		DB db = mongo.getDB("archive"); // new DB(mongo, "archive");
		fs = new GridFS(db);
	}
	
	private void startHTTPServer(){
		Server server = new Server(httpPort);
        ServletHandler handler = new ServletHandler();
        server.setHandler(handler);
        handler.addServletWithMapping("org.jvnet.hudson.hadoop.servlet.UploadFile", "/upload");
        handler.addServletWithMapping("org.jvnet.hudson.hadoop.servlet.DirectoryList", "/*");
        try {
        	log.info("Start http server at " + httpPort);
			server.start();
			server.join();
		} catch (Exception e) {
			log.error(e.toString(), e);
		}
	}
	
	public boolean archiveFile(String path, InputStream in, long size) throws IOException{
		if(!isConnected()) return false;
		
		if(path.startsWith("/")) path = path.substring(1);
		log.info("archivePath:" + path);
		
		GridFSInputFile file = fs.createFile(in, path);
		file.save();

		return true;
	}
	
	public List searchFile(String path, int offset, int limit){
		DBCursor cursor = null;
		if(path != null && !"".equals(path.trim())){
			DBObject f = new BasicDBObject();
			path = path.replace("*", ".*");
			f.put("filename", java.util.regex.Pattern.compile("^" + path));
			cursor = fs.getFileList(f);
		}else {
			cursor = fs.getFileList();
		}
		List result = cursor.skip(offset).limit(limit).toArray();
		cursor.close();
		return result;
	}
	
	public GridFSDBFile getFile(String path){
		return fs.findOne(path);
	}
	
	public ZipFile getCachedZip(String path){
		SoftReference<ZipFileWrapper> ref = zipCache.get(path);
		
		if(ref != null && ref.get() == null){
			System.gc();
			Collection<String> conn = new ArrayList<String>();
			conn.addAll(zipCache.keySet());
			for(String k : conn){
				if(zipCache.get(k).get() == null){
					zipCache.remove(k);
				}
			}
		}
		
		if(ref != null && 
		   ref.get() != null && 
		   ref.get().rawFile.isFile()
		   ){
			log.debug("hit zip cache:" + path + "-->" + ref.get().rawFile.getAbsolutePath());
			return ref.get().file;
		}else {
			GridFSDBFile file = this.getFile(path);
			if(file != null){
				ZipFileWrapper cache = new ZipFileWrapper();
				try {
					cache.rawFile = File.createTempFile("archive", "tmp");
					log.debug("cache zip file:" + path + "-->" + cache.rawFile.getAbsolutePath());
					file.writeTo(cache.rawFile);
					cache.file = new ZipFile(cache.rawFile, ZipFile.OPEN_READ);
					zipCache.put(path, new SoftReference<ZipFileWrapper>(cache));
					return cache.file;
				} catch (IOException e) {
					log.error(e.toString(), e);
				}
			}
		}
		return null;
	}
	
	public boolean isConnected(){
		return fs != null;
	}
	
	class ZipFileWrapper{
		ZipFile file = null;
		File rawFile = null; 
		protected void finalize() throws Throwable {
		    try {
		    	if(file != null){
		    		file.close();
		    	}
		    	if(rawFile != null){
		    		rawFile.delete();
		    	}
		    } finally {
		        super.finalize();
		    }
		}
	}
}
