package org.jvnet.hudson.hadoop;

import java.io.File;
import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.ref.SoftReference;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
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
import org.bson.BSONObject;
import org.mortbay.jetty.Server;
import org.mortbay.jetty.servlet.ServletHandler;

import com.mongodb.BasicDBObject;
import com.mongodb.DB;
import com.mongodb.DBCollection;
import com.mongodb.DBCursor;
import com.mongodb.DBObject;
import com.mongodb.Mongo;
import com.mongodb.ServerAddress;
import com.mongodb.gridfs.GridFS;
import com.mongodb.gridfs.GridFSDBFile;

public class HDFSArchiver {
	private Log log = LogFactory.getLog("hdfs.archiver");
	public static final String version = "0.1.1";
	
	public static final String VERSION = "version";
	public static final String PREFIX = "prefix";
	public static final String HTTPPORT = "http_port";
	public static final String DBNAME = "dbname";
	public static final String GRIDFS = "fs";
	public static final String FILEQOUTA = "maxFileSize";
	public static final String DAILYQOUTA = "maxDailySize";
		
	public static double dailyQouta = 1024.0 * 1024 * 1024 * 100;
	public int httpPort = 8924;
	public String prefix = null;
	public String[] conns = null;
	public String defaultDB = "archiver";
	public File rootPath = null;
	
	public long fileQouta = 1024 * 1024 * 1024;
	
	public GridFS defaultFs = null;
	public  DBCollection qoutaCollection = null; 
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
		options.addOption(FILEQOUTA, true, "Max file size (MByte). e.g. 1024");
		options.addOption(DAILYQOUTA, true, "Max uploading size in 24hours (Byte). e.g. 102400");
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
		//long fileQouta = 0;
		//long Qouta = 0;
		if(addrs != null && addrs.length > 0){
			HDFSArchiver archiver = new HDFSArchiver(httpPort, addrs, ".");
			archiver.defaultDB = dbName;
			archiver.prefix = prefix;
			
			if(cmd.hasOption(FILEQOUTA)){
				archiver.fileQouta = Integer.parseInt(cmd.getOptionValue(FILEQOUTA)) * 1204 * 1024;
				
			}
			if(cmd.hasOption(DAILYQOUTA)){
				archiver.dailyQouta = Integer.parseInt(cmd.getOptionValue(DAILYQOUTA)) * 1024.0 * 1024.0;
			}
			
			archiver.start();
			System.out.println("Stopped.");
		}else {
			System.out.println("least one '-fs <ip:port>' is required.");
		}
		
	}
	
	public void start() throws IOException{
		log.info("defaultDB:" + defaultDB);
		log.info("prefix:" + prefix);
		log.info("max file size:" + this.fileQouta / 1024 / 1024 + " MB");
		log.info("max uploading size in 24 hours:"+ this.dailyQouta / 1024 / 1024 + " MB");
		connectHDFS();
		startHTTPServer();
	}
	
	private void connectHDFS(){
		List addrs = new ArrayList();
		for(String x : conns){
			log.info("MonogoDB addr:" + x);
			String[] addr = x.split(":");
			int port = Integer.parseInt(addr[1]);			
			try {
				addrs.add(new ServerAddress(addr[0], port));
			} catch (IOException e) {
				log.error(e.toString());
			}
		}
		try{
			Mongo mongo = new Mongo(addrs);	
			mongo.slaveOk();
			DB db = mongo.getDB(this.defaultDB); // new DB(mongo, "archive");
			defaultFs = new GridFS(db);
		}catch(Throwable e){
			log.error(e.toString());
		}
	}
	
	private void startHTTPServer(){
		Server server = new Server(httpPort);
        ServletHandler handler = new ServletHandler();
        server.setHandler(handler);
        handler.addServletWithMapping("org.jvnet.hudson.hadoop.servlet.UploadFile", this.prefix + "upload");
        handler.addServletWithMapping("org.jvnet.hudson.hadoop.servlet.DistributeLockService", this.prefix + "lock");
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
	
	public boolean archiveFile(String path, InputStream in, long size, String id) throws IOException{
		if(!isConnected()) return false;
		
		if(path.startsWith("/")) path = path.substring(1);
		log.info("archivePath:" + path);
		this.removeFile(path);
		
		Client client = this.getClientQouta(id);
		log.debug(String.format("Client uploading qouta, client:%s, qouta:%s MB", client.getString("_id"), 
				client.getQouta() / 1024 /1024));
		QoutaInputStream filterIn = new QoutaInputStream(in, (long)Math.min(client.getQouta(), 
																	  this.fileQouta));		
		if(path.indexOf('$') > 0){
			String[] t = path.split("\\$", 2);
			getGridFS(t[0]).createFile(filterIn, t[1]).save();
			//defaultFs.createFile(in, path);
		}else {
			defaultFs.createFile(filterIn, path).save();
		}
		log.debug(String.format("uploading file '%s' size:%s", path, filterIn.size));		
		client.reduceQouta(filterIn.size);
		this.updateClientQouta(client);
		
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
	
	public void removeFile(String path){
		if(!this.isConnected()) return;
		if(path.indexOf('$') > 0){
			String[] t = path.split("\\$", 2);
			getGridFS(t[0]).remove(t[1]);
		}else{
			defaultFs.remove(path);
		}				
	}
	
	public GridFSDBFile getFile(String path){
		if(!this.isConnected()) return null;
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
	
	public Client getClientQouta(String id){
		if(qoutaCollection == null){
			qoutaCollection = defaultFs.getDB().getCollection("client_qouta");
			qoutaCollection.setObjectClass(Client.class);
		}
		DBObject obj = qoutaCollection.findOne(new Client(id));
		Client client = null;
		if(obj != null){
			//log.debug("find client:" + obj.toString() + ", id:" + obj.get("_id") + ", qouta:" + obj.get("qouta") + ", update:" + obj.get("lastUpdated"));
			client = (Client)obj;
			//最后一次上传超过24小时，更新为24小时限制.
			if(System.currentTimeMillis() - client.getLastUpdateTime() > 3600 * 24 * 1000){
				client.put("qouta", this.dailyQouta);
			}
		}else {
			client = new Client(id, System.currentTimeMillis(), this.dailyQouta);
		}
		return client;
	}
	
	public void updateClientQouta(Client client){
		if(qoutaCollection == null){
			qoutaCollection = defaultFs.getDB().getCollection("client_qouta");
		}
		client.put("lastUpdated", System.currentTimeMillis());
		//log.debug("update client:" + client.toString() + ", id:" + client.get("_id") + ", qouta:" + 
		//		client.get("qouta") + ", update:" + client.get("lastUpdated"));
		qoutaCollection.update(new Client(client.getString("_id")), 
							   client, true, false);
	}
	
	public static class Client extends BasicDBObject{
		public Client(){}
		public Client(String id){
			this.put("_id", id);
		}
		public Client(String id, long updateTime, double qouta){
			this.put("_id", id);
			this.put("lastUpdated", updateTime);
			this.put("qouta", qouta);
		}
		
		public long getLastUpdateTime(){
			return this.getLong("lastUpdated");
		}
		
		public double getQouta(){
			//可能在Web接口重启后，修改了参数。已保留的Qouta比修改参数后的还大。
			//在两者间取一个小的值。
			return Math.min(this.getLong("qouta"), dailyQouta);
		}
		public void reduceQouta(long size){
			this.put("qouta", this.getQouta() - size);
		}
	} 
	
	class QoutaInputStream extends FilterInputStream{
		public long qouta = Long.MAX_VALUE, size = 0;		
		public QoutaInputStream(InputStream in, long qouta){
			super(in);
			this.qouta = qouta;
		}
		
		public int read() throws IOException{
			this.size++;
			this.checkQouta();
			return super.read();
		}
		
		public int read(byte[] b) throws IOException{
			int r = super.read(b);
			this.size += r;
			this.checkQouta();
			return r;
		}
		
		public int read(byte[] b, int off, int len) throws IOException{
			int r = super.read(b, off, len);
			this.size += r;
			this.checkQouta();
			return r;
		}		
		
		private void checkQouta() throws IOException{
			if(this.size > this.qouta) throw new IOException("violated the qouta limitation, size:" + this.qouta);
		}
	}
}
