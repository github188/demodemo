package com.sigma.demo.util;

import java.sql.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.log4j.Appender;
import org.apache.log4j.Layout;
import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;
import org.apache.log4j.spi.ErrorHandler;
import org.apache.log4j.spi.Filter;
import org.apache.log4j.spi.LoggingEvent;

public class DemoLogger implements Appender {
	private static final Logger log = LogManager.getLogger(DemoLogger.class);
	
	private Pattern FORMAT 		= Pattern.compile("\\[(\\w+)\\]\\{(\\w+)\\}(.*)",Pattern.CASE_INSENSITIVE);
	
	private String name;
	private String driver;
	private String url;
	private String user;
	private String password;
	private Connection conn = null; 
	private int demoServer = 0;
	private int maxLen = 255;
	//private
	private PreparedStatement pstm = null;
	private static String LOGSQL = "insert into dl_log(event_time,username,demo_server,action,description)" +
			" values(?,?,?,?,?)";
	
	public DemoLogger(String name, String driver,String url, String user, String password){
		this.name = name;
		this.driver = driver;
		this.url = url;
		this.user = user;
		this.password = password;
		//initLogger();
	}
	
	public void addFilter(Filter arg0){
	}

	public void clearFilters(){
	}

	public void close(){
		try {
			if (pstm != null) {
				pstm.close();
			}
		}catch (SQLException e) {
			log.error(e,e);
		}
		try {
			if (conn != null) {
				conn.commit();
				conn.close();
			}
		} catch (SQLException e) {
			log.error(e,e);
		}
	}

	/**
	 * Message formate. [action]{username}............;
	 */
	public void doAppend(LoggingEvent event){
		if(event.level.equals(org.apache.log4j.Priority.FATAL)){
			String[] info = parseMessage(event.getMessage().toString());
			boolean error = true;
			for(int i = 0; info != null && i < 3 && error; i++){
				try {
					pstm.setTimestamp(1, new Timestamp(event.timeStamp));
					pstm.setString(2, info[0]);
					pstm.setInt(3, demoServer);
					pstm.setString(4, info[1]);
					pstm.setString(5, info[2]);
					pstm.executeUpdate();
					conn.commit();
					error = false;
				} catch (SQLException e) {
					error = true;
					close();
					initLogger();
					log.error(e,e);
				}
			}
		}
	}
	private String[] parseMessage(String msg){
//		Pattern aa;
//		aa.
		String[] info = null; 
		Matcher format = FORMAT.matcher(msg);
		if(format.find()){
			info = new String[3];
			info[1] = format.group(1);
			info[0] = format.group(2);
			info[2] = format.group(3);
			if(info[2] != null && info[2].length() > maxLen){
				info[2] = info[2].trim().substring(0, maxLen);
			}
			
			info[0] = (info[0] == null) ? "SYS" : info[0].trim();
			info[1] = (info[1] == null) ? "SYS" : info[1].trim();
		}
		
		return info;
	}

	public ErrorHandler getErrorHandler(){
		return null;
	}

	public Filter getFilter(){
		return null;
	}

	public Layout getLayout(){
		return null;
	}

	public String getName(){
		return name;
	}

	public boolean requiresLayout(){
		return false;
	}

	public void setErrorHandler(ErrorHandler arg0){
	}

	public void setLayout(Layout arg0){
	}

	public void setName(String arg0){
		this.name = arg0;
	}

	public boolean initLogger(){
		boolean success = false;
		try {
			Class.forName(driver);
			conn = DriverManager.getConnection(url, user, password);
			conn.setAutoCommit(false);
			pstm = conn.prepareStatement(LOGSQL);
			pstm.setTimestamp(1, new Timestamp(System.currentTimeMillis()));
			pstm.setString(2, "SYSTEM");
			pstm.setInt(3, demoServer);
			pstm.setString(4, "INIT_LOG");
			pstm.setString(5, "Init logger successfully.");
			pstm.executeUpdate();
			conn.commit();
			success = true;
		}catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		catch (SQLException e) {
			e.printStackTrace();
		}
		return success;
	}
	
	public void setDemoServer(int id){
		this.demoServer = id; 
	}
	public void setMaxLength(int len){
		this.maxLen = len;
	}	
}
