package org.jvnet.hudson.hadoop;

import java.io.DataOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.net.URISyntaxException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.mortbay.jetty.Server;
import org.mortbay.jetty.servlet.ServletHandler;

public class HDFSArchiver {
	private Log log = LogFactory.getLog("hdfs.archiver");
	public int httpPort = 8924;
	public URI hdfsURI = null;
	public URI harURI = null;
	public String archiveRoot = "/archiver";
	public File rootPath = null;
	
	public long harThreshold = 64 * 1024 * 1024;
	public FileSystem fs = null;
	public FileSystem harfs = null;	
	
	protected static HDFSArchiver ins = null;
	
	public HDFSArchiver(String httpPort, String hdfs, String local){
		rootPath = new File(local).getAbsoluteFile();
		try{
			this.httpPort = Integer.parseInt(httpPort);
		}catch(Exception e){
			log.error("Invalid http port:" + httpPort);
		}
		
		try {
			hdfsURI = new URI(hdfs);
			harURI = new URI(hdfs.replaceFirst("hdfs:", "har:"));
		} catch (URISyntaxException e) {
			log.error(e.toString(), e);
		}
		ins = this;
	}
	
	public static HDFSArchiver getArchiver(){
		return ins;
	}
	
	public static void main(String[] args) throws IOException{
		HDFSArchiver archiver = new HDFSArchiver("8924", args[1], ".");
		archiver.start();
	}
	
	public void start() throws IOException{
		connectHDFS();
		startHTTPServer();
	}
	
	private void connectHDFS() throws IOException{
        Configuration conf = new Configuration();
        //conf.set("fs.default.name", hdfsRoot);
        conf.set("dfs.data.dir",new File(rootPath, "data").getAbsolutePath());
        
        log.info("Connection to HDFS:" + hdfsURI.toString());
        log.info("Archive path:" + archiveRoot);
        
        fs = FileSystem.get(hdfsURI, conf);     
        Path root = new Path(archiveRoot);
        
        if(!fs.exists(root)){
        	log.info("Not found archive path:" + archiveRoot);
        	if(!fs.mkdirs(root)){
        		log.warn("Failed to create archive path.");
        	}
        }
        
        harfs = FileSystem.get(harURI, conf);
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
		} catch (Exception e) {
			log.error(e.toString(), e);
		}
	}
	
	public boolean archiveFile(String path, InputStream in, long size) throws IOException{
		if(!isConnected()) return false;
		
		Path archivePath = new Path(archiveRoot, path);
		
		if(fs.exists(archivePath)){
			fs.delete(archivePath, true);
		};
		if(harfs.exists(archivePath)){
			harfs.delete(archivePath, true);
		};
		
		DataOutputStream os = null;
		byte[] buffer = new byte[1024 * 64];
		if(size < this.harThreshold){
			os = harfs.create(archivePath, true, buffer.length);
		}else {
			os = fs.create(archivePath, true, buffer.length);
		}
		if(os != null){
			for(int len = in.read(buffer); len > 0;){
				os.write(buffer, 0, len);
				len = in.read(buffer);
			}
			os.flush();
			os.close();
		}else {
			return false;
		}
		
		return true;
	}
	
	public boolean isConnected(){
		return fs != null && harfs != null;
	}
}
