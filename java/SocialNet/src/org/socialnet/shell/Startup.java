package org.socialnet.shell;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.mortbay.jetty.Server;
import org.mortbay.jetty.servlet.ServletHandler;
import org.socialnet.core.JDBCDataLoader;
import org.socialnet.core.NodeHeap;
import org.socialnet.core.SessionManager;
import org.socialnet.core.SocialNet;


public class Startup {
	public static final String HTTP_PORT = "http_port";
	public static final String MAX_SESSION = "max_session";
	
	public static final String VERSION = "0.1.0";
	private static Log log = LogFactory.getLog(Startup.class);
	private String conf = "sns.conf";
	protected Properties settings = System.getProperties();

	public Startup(){
		try {
			InputStream is = this.getClass().getClassLoader().getResourceAsStream("org/socialnet/shell/global.conf");
			if(is != null){
				settings.load(is);
			}else {
				log.info("Not found default configuration!");
			}
		} catch (IOException e) {
			log.error(e, e.getCause());
		}
		
		File f = new File(this.conf);
		if(!f.isFile() && this.conf.equals("sns.conf")){
			try {
				OutputStream os = new FileOutputStream(f);
				byte[] buffer = new byte[1024 * 64];
				InputStream is = this.getClass().getClassLoader().getResourceAsStream("org/socialnet/shell/global.conf");
				int length = is.read(buffer);
				os.write(buffer, 0, length);
				os.close();
				log.info("write default config file:" + f.getAbsolutePath());
			}catch (IOException e) {
				log.error(e, e.getCause());
			}			
		}
	}

	public Startup(String conf){
		this();
		this.conf = conf;
		File f = new File(this.conf);
		if(f.isFile()){
			try {
				settings.load(new FileInputStream(f));
			} catch (FileNotFoundException e) {
				log.error(e, e.getCause());
			} catch (IOException e) {
				log.error(e, e.getCause());
			}
		}
	}
	
	public static void main(String[] args){
		Startup st = null;
		if(args.length == 0){
			st = new Startup();
		}else if (args.length == 1){
			if(args[0].equals("--version")){
				System.out.println("SocialNet " + VERSION);
				System.exit(0);
			}else if(args[0].equals("--help")){
				help();
			}else if(args[0].endsWith(".conf")){
				st = new Startup(args[0]);
			}else {
				help();
			}
		}else if(args.length > 1){
			help();
		}
		st.initDirectory();
		st.initLogging();
		st.initSocialNet();
		st.startHttpServer();
	}
	
	private void initLogging(){		
	}
	
	private void initDirectory(){
	}
	
	private void startHttpServer(){
		int httpPort = Integer.parseInt(settings.getProperty(HTTP_PORT, "8082"));
		
		Server server = new Server(httpPort);
		log.info("this is test info.");
		
		System.out.println("Listening HTTP port:" + httpPort);
		
        ServletHandler handler=new ServletHandler();
        server.setHandler(handler);
        handler.addServletWithMapping("org.socialnet.servlet.SearchServlet", "/search/*");
        try {
			server.start();
	        server.join();
	        System.out.println("Shutdown HTTP service..");
		} catch (Exception e) {
			System.err.println("Stop HTTP service, with error:" + e.toString());
		}		
	}
	
	private void initSocialNet(){
		JDBCDataLoader loader = new JDBCDataLoader(this.settings);
		
		int maxSession = Integer.parseInt(settings.getProperty(MAX_SESSION, "10"));
		SessionManager sm = new SessionManager(maxSession);
		NodeHeap pool = new NodeHeap(loader);
		pool.setMaxSession(maxSession);
		new SocialNet(sm, pool);
	}
	
	private static void help(){
		System.out.println("SocialNet.jar <config>");
		System.exit(0);
	}
}
