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
	public final static String FTP_HOST = "ftp_host";
	public final static String FTP_USER = "ftp_user";
	public final static String FTP_PASSWORD = "ftp_password";
	public final static String FTP_ROOT_DIR = "ftp_root_dir";
	public final static String FTP_ZIP_DIR = "ftp_zip_dir";
	public final static String LOCAL_VERSION = "local_version";
	
	public final static String DB_DRIVER = "db_driver";
	public final static String DB_URL = "db_url";
	public final static String DB_USER = "db_user";
	public final static String DB_PASSWORD = "db_password";
	public final String[] PARAMS = {FTP_HOST, FTP_USER, FTP_PASSWORD, FTP_ROOT_DIR, FTP_ZIP_DIR,
			DB_DRIVER, DB_URL, DB_USER, DB_PASSWORD
			};
	
	public Map<String, String> param = new HashMap<String, String>();
	
	public Configuration(){
		param.put(FTP_HOST, "127.0.0.1");
		param.put(FTP_USER, "root");
		param.put(FTP_PASSWORD, "");
		param.put(FTP_ROOT_DIR, "/");
		param.put(FTP_ZIP_DIR, "/zip");
		param.put(DB_DRIVER, "oracle.jdbc.driver.OracleDriver");
		param.put(DB_URL, "jdbc:oracle:thin:127.0.0.1:1521/prod");
		param.put(DB_USER, "user");
		param.put(DB_PASSWORD, "");
		
		param.put(LOCAL_VERSION, "0.1");
	}
	
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
	
	public String getLocalVersion(){
		return param.get(LOCAL_VERSION);
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
		
		for(String name: PARAMS){
			if(p.getProperty(name) != null){
				param.put(name, p.getProperty(name));
			}
		}
	}
	
	public void loadRegistry(){
		Preferences perf = Preferences.userRoot().node("ftp_upgrade");
		//log.debug("path:" + perf.absolutePath());
		for(String name: PARAMS){
			if(perf.get(name, null) != null){
				param.put(name, perf.get(name, null));
			}
		}
	}
	
	public void saveRegistry(){
		Preferences perf = Preferences.userRoot().node("ftp_upgrade");
		for(String name: PARAMS){
			perf.put(name, param.get(name));
		}
		try {
			perf.flush();
			log.debug("save registory:" + perf.absolutePath());
		} catch (BackingStoreException e) {
			log.error(e.toString(), e);
		}
	}
}
