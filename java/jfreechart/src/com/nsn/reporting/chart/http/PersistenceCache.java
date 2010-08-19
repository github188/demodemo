package com.nsn.reporting.chart.http;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class PersistenceCache {
	private Log log = LogFactory.getLog(PersistenceCache.class);
	private File cacheRoot = null;
	public PersistenceCache(File root){
		cacheRoot = root;
		if(!cacheRoot.exists()){
			cacheRoot.mkdirs();
		}
		log.debug("create cache:" + root.getAbsolutePath());
	}
	public PersistenceCache(String root){
		cacheRoot = new File(root);
		if(!cacheRoot.exists()){
			cacheRoot.mkdirs();
		}
		log.debug("create cache:" + cacheRoot.getAbsolutePath());
	}
	
	public void save(ChartData data){
		ObjectOutputStream so = null;
		File f = getCacheFile(data.sid);
		try{
			so = new ObjectOutputStream(new FileOutputStream(f));
			so.writeObject(data);
			so.close();
			log.info(String.format("save cache:%s", data.sid));
		}catch(IOException e ) {
			log.error(e, e.getCause());
		}
	}
	
	public ChartData load(String sid){
		File f = getCacheFile(sid);
		ChartData data = null;
		ObjectInputStream si= null;
		if(f.isFile()){
			try{
				si = new ObjectInputStream(new FileInputStream(f));
				data = (ChartData)si.readObject();
				//if the updated is false, the HTTP request will blocked.
				data.updated = true;
				si.close();
				log.info(String.format("load cache:%s", sid));
			} catch (ClassNotFoundException e) {
				log.error(e, e.getCause());
			}catch(IOException e) {
				log.error(e, e.getCause());
			}
		}
		
		return data;
	}
	
	private File getCacheFile(String sid){
		int dir_name_length = 2;
		
		File path = cacheRoot;
		int endIndex = sid.length();
		for(int i = 0; i < 5; i++){
			if(endIndex - dir_name_length < 0){break;}
			path = new File(path, sid.substring(endIndex - dir_name_length, endIndex));
			endIndex -= dir_name_length;
		}
		if(!path.exists()){
			if(!path.mkdirs()){
				log.error("Failed to create directory:" + path.getAbsolutePath());
			}
		}
		
		log.debug(String.format("mapping sid=%s to path=%s", sid, path.getAbsoluteFile()));
		return new File(path, sid);
	}
}
