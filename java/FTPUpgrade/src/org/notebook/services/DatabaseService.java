package org.notebook.services;

import java.net.URL;
import java.net.URLClassLoader;
import java.sql.Connection;
import java.sql.Driver;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.Configuration;
import org.notebook.cache.UpgradeFile;

public class DatabaseService {
	private Configuration config = null;
	protected Log log = LogFactory.getLog("db");
	
	//jdbc_dirver
	//jdbc_url: 192.168.1.2:3360/
	//jdbc_user
	//jdbc_password
	//Class.forName("oracle.jdbc.driver.OracleDriver");
	//jdbc:oracle:thin:@//server.local:1521/prod
	//private Class driver = null;
	private Object driver = null;
	
	public DatabaseService(Configuration conf){
		this.config = conf;
	}
	
	public String getNewVersion(){
		Connection conn = null;
		PreparedStatement pstm = null;
		ResultSet rs = null;
		float curVer = 0;
		try {
			conn = getConnection();
			pstm = conn.prepareStatement("insert into tb_version(VERSION, INSERT_DATE)" +
					    				 "values(?, ?)");
			rs = pstm.executeQuery("select max(VERSION) as lastVersion from tb_version");
			if(rs.next()){
				curVer = rs.getFloat("lastVersion");
			}else {
				curVer = 1.0f;
			}
			curVer = curVer + 1;
			pstm.setFloat(1, curVer);
			//pstm.setDate(2, new Date(System.currentTimeMillis()));
			//pstm.setTime(2, new Time(System.currentTimeMillis()));
			pstm.setTimestamp(2, new Timestamp(System.currentTimeMillis()));
			pstm.execute();
			conn.commit();
		} catch (SQLException e) {
			try {
				if(conn != null)conn.rollback();
			} catch (SQLException e1) {}
			log.error(e.toString(), e);
		}finally{
			try{
				if(rs != null)rs.close();
			}catch(SQLException e){};
			try {
				if(pstm != null) pstm.close();
			} catch (SQLException e) {}
			try {
				if(conn != null) conn.close();
			} catch (SQLException e) {}
		}
		
		return String.format("%1.2f", curVer);
	}
	
	public void insertZipFile(String version, String path){
		Connection conn = null;
		Statement stm = null;
		try {
			conn = getConnection();
			stm = conn.createStatement();
			stm.execute("insert into tb_upload_file(VERSION, FILENAME)" +
					    String.format("values(%s, '%s')", version, path));
			conn.commit();
		} catch (SQLException e) {
			try {
				if(conn != null)conn.rollback();
			} catch (SQLException e1) {}
			log.error(e.toString(), e);
		}finally{
			if(stm != null)
				try {
					stm.close();
				} catch (SQLException e) {}
			if(conn != null)
				try {
					conn.close();
				} catch (SQLException e) {}
		}
	}
	
	public boolean testConnection(){
		Connection conn = null;
		PreparedStatement pstm = null;
		ResultSet rs = null;
		boolean isOk = false;
		//清空缓存从新加载驱动。
		driver = null;
		try {
			conn = getConnection();
			pstm = conn.prepareStatement("select distinct FILENAME from tb_upload_file where version>1");
			rs = pstm.executeQuery();
			isOk = true;
		} catch (SQLException e) {
			log.error(e.toString(), e);
		}finally{
			try{
				if(rs != null)rs.close();
			}catch(SQLException e){};
			try {
				if(pstm != null) pstm.close();
			} catch (SQLException e) {}
			try {
				if(conn != null) conn.close();
			} catch (SQLException e) {}
		}		
		return isOk;		
	}
	
	public Collection<UpgradeFile> getUpgradeFile(String version){
		Connection conn = null;
		PreparedStatement pstm = null;
		ResultSet rs = null;
		float curVer = 0;
		Collection<UpgradeFile> files = new ArrayList<UpgradeFile>();
		UpgradeFile file = null;
		try {
			conn = getConnection();
			pstm = conn.prepareStatement("select distinct FILENAME from tb_upload_file where version>" + version);
			rs = pstm.executeQuery();
			while(rs.next()){
				file = new UpgradeFile();
				file.path = rs.getString("FILENAME");
				files.add(file);
			}
		} catch (SQLException e) {
			log.error(e.toString(), e);
		}finally{
			try{
				if(rs != null)rs.close();
			}catch(SQLException e){};
			try {
				if(pstm != null) pstm.close();
			} catch (SQLException e) {}
			try {
				if(conn != null) conn.close();
			} catch (SQLException e) {}
		}
		
		return files;
	}
	
	/**
	 * jdbc:mysql://%s?useUnicode=true&characterEncoding=utf8
	 * @return
	 * @throws SQLException 
	 */
	public Connection getConnection() throws SQLException{		
		/**
		 * 如果驱动没有加载，使用类加载器找到数据库驱动。
		 */
		if(driver == null){
			URL curJar = DatabaseService.class.getProtectionDomain().getCodeSource().getLocation();
			try {
				URL driverURL = curJar.toURI().resolve("jdbc-driver.jar").toURL();
				log.debug("JDBC url:" + driverURL.toString());
				System.setProperty("java.class.path", driverURL.toString());
				URLClassLoader cl = new URLClassLoader(new URL[]{driverURL, }, DatabaseService.class.getClassLoader());
				Class driverClass = cl.loadClass(config.param.get(Configuration.DB_DRIVER));
				driver = driverClass.newInstance();
				if(driver instanceof Driver){
					log.debug("registoer driver:" + driver);
					DriverManager.registerDriver((Driver)driver);
				}
			} catch (Exception e) {
				log.error(e.toString(), e);
				driver = new Object();
			}
		}
		
		Connection conn = null;
		if(driver instanceof Driver){
			Properties prop = new Properties();
			prop.put("user", config.param.get(Configuration.DB_USER));
			prop.put("password", config.param.get(Configuration.DB_PASSWORD));
			conn = ((Driver) driver).connect(config.param.get(Configuration.DB_URL), prop);
		}else {
			conn = DriverManager.getConnection(config.param.get(Configuration.DB_URL), 
							config.param.get(Configuration.DB_USER),
							config.param.get(Configuration.DB_PASSWORD));
		}
		conn.setAutoCommit(false);
		return conn;
	}
}
