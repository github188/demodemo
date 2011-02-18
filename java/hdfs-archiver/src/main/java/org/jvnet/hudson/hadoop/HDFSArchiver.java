package org.jvnet.hudson.hadoop;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.mortbay.jetty.Server;
import org.mortbay.jetty.servlet.ServletHandler;

import com.mongodb.DB;
import com.mongodb.Mongo;
import com.mongodb.ServerAddress;
import com.mongodb.gridfs.GridFS;
import com.mongodb.gridfs.GridFSInputFile;

public class HDFSArchiver {
	private Log log = LogFactory.getLog("hdfs.archiver");
	public int httpPort = 8924;
	public String[] conns = null;
	public String archiveRoot = "/archiver";
	public File rootPath = null;
	
	public GridFS fs = null;
		
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
	
	public boolean isConnected(){
		return fs != null;
	}
}
