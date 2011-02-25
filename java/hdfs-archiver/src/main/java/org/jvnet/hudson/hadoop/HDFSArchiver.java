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

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;
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
	public static final String version = "0.1.1";
	
	public static final String VERSION = "version";
	public static final String PREFIX = "prefix";
	public static final String HTTPPORT = "http_port";
	public static final String DBNAME = "dbname";
	public static final String GRIDFS = "fs";
		
	public int httpPort = 8924;
	public String prefix = null;
	public String[] conns = null;
	public String defaultDB = "archiver";
	public File rootPath = null;
	
	public GridFS defaultFs = null;
	public Map<String, GridFS> fsCache = new HashMap<String, GridFS>();
	
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
		Options options = new Options();
		options.addOption(VERSION, false, "show version.");
		options.addOption(PREFIX, true, "the prefix of HTTP service.");
		options.addOption(HTTPPORT, true, "http listen port.");
		options.addOption(DBNAME, true, "default db name");
		options.addOption(GRIDFS, true, "Mongodb server set address. e.g. 127.0.0.1:2017");
		options.getOption(GRIDFS).setArgs(Option.UNLIMITED_VALUES);
		
		final String usage = "GridFS [options]";
		CommandLine cmd = null;
		
		try{
			CommandLineParser parser = new PosixParser();
			cmd = parser.parse(options, args);			
		}catch(ParseException e){
			HelpFormatter formatter = new HelpFormatter();
			formatter.printHelp(usage, options);
			System.exit(-1);
		}
		
		if(cmd.hasOption(VERSION)){
			System.out.println("GridFS " + version);
			return;
		}
		
		String prefix = cmd.getOptionValue(PREFIX, "/");
		String httpPort = cmd.getOptionValue(HTTPPORT, "8924");
		String dbName = cmd.getOptionValue(DBNAME, "archive");
		String[] addrs = cmd.getOptionValues(GRIDFS);
		
		if(addrs != null && addrs.length > 0){
			HDFSArchiver archiver = new HDFSArchiver(httpPort, addrs, ".");
			archiver.defaultDB = dbName;
			archiver.prefix = prefix;
			archiver.start();
			System.out.println("Stopped.");
		}else {
			System.out.println("least one '-fs <ip:port>' is required.");
		}
		
	}
	
	public void start() throws IOException{
		log.info("defaultDB:" + defaultDB);
		log.info("prefix:" + prefix);
		connectHDFS();
		startHTTPServer();
	}
	
	private void connectHDFS() throws IOException{
		List addrs = new ArrayList();
		for(String x : conns){
			log.info("MonogoDB addr:" + x);
			String[] addr = x.split(":");
			int port = Integer.parseInt(addr[1]);			
			addrs.add(new ServerAddress(addr[0], port));
		}
		Mongo mongo = new Mongo(addrs);	
		mongo.slaveOk();
		DB db = mongo.getDB(this.defaultDB); // new DB(mongo, "archive");
		defaultFs = new GridFS(db);
	}
	
	private void startHTTPServer(){
		Server server = new Server(httpPort);
        ServletHandler handler = new ServletHandler();
        server.setHandler(handler);
        handler.addServletWithMapping("org.jvnet.hudson.hadoop.servlet.UploadFile", this.prefix + "upload");
        handler.addServletWithMapping("org.jvnet.hudson.hadoop.servlet.DirectoryList", this.prefix + "*");
        if(this.prefix.length() > 1){
        	handler.addServletWithMapping("org.jvnet.hudson.hadoop.servlet.WelcomeIndex", "/*");
        }
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
		
		if(path.indexOf('$') > 0){
			String[] t = path.split("\\$", 2);
			getGridFS(t[0]).createFile(in, t[1]).save();
			//defaultFs.createFile(in, path);
		}else {
			defaultFs.createFile(in, path).save();
		}
		return true;
	}
	
	public List searchFile(String path, int offset, int limit){
		DBCursor cursor = null;
		GridFS fs = defaultFs;
		if(path.indexOf('$') > 0){
			String[] t = path.split("\\$", 2);
			fs = getGridFS(t[0]);
			path = t[1];
		}
		
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
		if(path.indexOf('$') > 0){
			String[] t = path.split("\\$", 2);
			return getGridFS(t[0]).findOne(t[1]);
		}else{		
			return defaultFs.findOne(path);
		}
	}
	
	private GridFS getGridFS(String name){
		name = name.replace('/', '_');
		GridFS fs = fsCache.get(name);
		if(fs == null){
			fs = new GridFS(defaultFs.getDB().getMongo().getDB(name));
		}
		fsCache.put(name, fs);
		return fs;
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
					cache.rawFile.deleteOnExit();
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
		return defaultFs != null;
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
