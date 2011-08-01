package org.notebook.services;

import org.notebook.cache.Configuration;
import org.notebook.cache.UpgradeFile;

public class TestDatabaseServices {
	
	public static void main(String[] args){
		Configuration conf = new Configuration();
		conf.param.put(Configuration.DB_DRIVER, "com.mysql.jdbc.Driver");
		conf.param.put(Configuration.DB_URL, "jdbc:mysql://127.0.0.1:3306/ftp_upgrade?useUnicode=true&characterEncoding=utf8");
		conf.param.put(Configuration.DB_USER, "root");
		conf.param.put(Configuration.DB_PASSWORD, "");
		
		DatabaseService db = new DatabaseService(conf);
		System.out.println(db.getLastVersion());
		System.out.println(db.getLastVersion());
		
		String old = db.getLastVersion();
		System.out.println(old);
		String n = db.getLastVersion();
		
		db.insertZipFile(n, "test.zip");
		db.insertZipFile(n, "test1.zip");
		n = db.getLastVersion();
		db.insertZipFile(n, "test.zip");
		db.insertZipFile(n, "test2.zip");
		
		for(UpgradeFile f: db.getUpgradeFile(old)){
			System.out.println("path:" + f.path);
		}
		
		System.out.println("done");
	}
}
