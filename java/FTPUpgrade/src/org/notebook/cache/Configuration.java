package org.notebook.cache;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.prefs.BackingStoreException;
import java.util.prefs.Preferences;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class Configuration {
	protected Log log = LogFactory.getLog("cfg");
	private static String FTP_HOST = "ftp_host";
	private static String FTP_USER = "ftp_user";
	private static String FTP_PASSWORD = "ftp_password";
	private static String FTP_ROOT_DIR = "ftp_root_dir";
	private static String FTP_ZIP_DIR = "ftp_zip_dir";
	
	private Map<String, String> param = new HashMap<String, String>();
	
	public String getHost(){
		return param.get(FTP_HOST);
	}
	
	public String getRootPath(){
		return param.get(FTP_ROOT_DIR);
	}
	
	public String getUsername(){
		return param.get(FTP_USER);
	}
	
	public String getPassword(){
		return param.get(FTP_PASSWORD);
	}
	
	public void load(File path){
		Properties p = System.getProperties();
		InputStream in = null;
		try {
			if(path.isFile()){
				log.debug("Loading " + path.getAbsolutePath());
				in = new FileInputStream(path);
				p.load(in);
			}
		} catch (IOException e) {
			log.error(e.toString(), e);
		}finally{
			if(in != null){
				try {
					in.close();
				} catch (IOException e) {
					log.error(e.toString(), e);
				}
			}
		}
		
		param.put(FTP_HOST, p.getProperty(FTP_HOST, "127.0.0.1"));
		param.put(FTP_USER, p.getProperty(FTP_USER, "root"));
		param.put(FTP_PASSWORD, p.getProperty(FTP_PASSWORD, ""));
		param.put(FTP_ROOT_DIR, p.getProperty(FTP_ROOT_DIR, "/"));
		param.put(FTP_ZIP_DIR, p.getProperty(FTP_ZIP_DIR, "/zip"));		
	}
	
	public void loadRegistry(){
		Preferences perf = Preferences.userRoot().node("ftp_upgrade");
		log.debug("path:" + perf.absolutePath());
		param.put(FTP_HOST, perf.get(FTP_HOST, getHost()));
		param.put(FTP_USER, perf.get(FTP_USER, getUsername()));
		param.put(FTP_PASSWORD, perf.get(FTP_PASSWORD, getPassword()));
		param.put(FTP_ROOT_DIR, perf.get(FTP_ROOT_DIR, getRootPath()));
	}
	
	public void saveRegistry(){
		Preferences perf = Preferences.userRoot().node("ftp_upgrade");
		perf.put(FTP_HOST, getHost());
		perf.put(FTP_USER, getUsername());
		perf.put(FTP_PASSWORD, getPassword());
		perf.put(FTP_ROOT_DIR, getRootPath());
		try {
			perf.flush();
			log.debug("save registory:" + perf.absolutePath());
		} catch (BackingStoreException e) {
			log.error(e.toString(), e);
		}
	}
}
