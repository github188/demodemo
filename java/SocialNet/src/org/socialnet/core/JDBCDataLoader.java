package org.socialnet.core;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class JDBCDataLoader implements DataSource {
	public static final String RELATION_SQL = "sql_relation";
	public static final String ID_EXIST_SQL = "sql_check_id_exist";
	public static final String DB_DRIVER = "db_driver";
	public static final String DB_URL = "db_url";
	public static final String DB_USERNAME = "db_username";
	public static final String DB_PASSWORD = "db_password";
	
	private Log log = LogFactory.getLog(JDBCDataLoader.class);
	
	private Properties settings;
	private Connection conn = null;
	
	protected String sql_relation = null;
	protected String sql_check_id = null;
	
	public JDBCDataLoader(Properties settings) {
		this.settings = settings;
		initDBConnection();
	}

	@Override
	public Collection<Relation> loadEdges(int nodeId) {
		ResultSet rs = null;
		PreparedStatement pstm = null;
		ArrayList<Relation> result = new ArrayList<Relation>(100);
		int friend, role;
		float weight;
	    try
	    {
	    	if(conn == null) conn = this.getConnection();
	    	pstm = conn.prepareStatement(sql_relation,
	    			 ResultSet.TYPE_FORWARD_ONLY,
	    			 ResultSet.CONCUR_READ_ONLY
	    			 );
	    	pstm.setInt(1, nodeId);
	    	rs = pstm.executeQuery();
	    	
	    	rs.setFetchSize(100);
			while (rs.next()){
				friend = rs.getInt("friend");
				role = rs.getInt("relation");
				weight = rs.getFloat("weight");
				result.add(new Relation(friend, role, weight));
			}
	    }catch (SQLException ex){
	      log.error(ex);
	      conn = null;
	    }finally{
	    	if(rs != null){try {
				rs.close();
			} catch (SQLException e) {
				log.error(e);				
			}}
	    	if(pstm != null){try {
				pstm.close();
			} catch (SQLException e) {
				log.error(e);
			}}
	    }
	    result.trimToSize();
		return result;
	}

	@Override
	public boolean saveEdges(int start, int end) {
		throw new Error("not implemented.");
	}

	@Override
	public boolean removeEdges(int start, int end) {
		throw new Error("not implemented.");
	}

	@Override
	public boolean isExist(int nodeId) {
		PreparedStatement pstm = null;
		ResultSet rs = null;
		boolean exits = false;
	    try
	    {
	    	if(conn == null) conn = this.getConnection();
			if(sql_check_id == null || "".equals(sql_check_id.trim())){
				return true;
			}	    	
	    	pstm = conn.prepareStatement(sql_check_id,
	    			 	ResultSet.TYPE_FORWARD_ONLY,
	    			 	ResultSet.CONCUR_READ_ONLY
	    			 );
	    	pstm.setInt(1, nodeId);
	    	rs = pstm.executeQuery();	    	
	    	rs.setFetchSize(1);
	    	if (rs.next()) exits = true;
	    }catch (SQLException ex){
	      log.error(ex);
	      conn = null;
	    }finally{
	    	if(rs != null){try {
				rs.close();
			} catch (SQLException e) {
				log.error(e);				
			}}
	    	if(pstm != null){try {
				pstm.close();
			} catch (SQLException e) {
				log.error(e);
			}}
	    }
		return exits;
	}
	
	private void initDBConnection(){
		String driver = settings.getProperty(DB_DRIVER).trim();
		sql_relation = settings.getProperty(RELATION_SQL);
		sql_check_id = settings.getProperty(ID_EXIST_SQL);
		log.info("db_driver:" + driver);
		try {
			Class.forName(driver);
			this.getConnection();
			log.info("sql_relation:" + sql_relation);
			log.info("sql_check_id:" + sql_check_id);			
			this.isExist(0);
			if(conn == null) System.exit(-1); //SQL错误，conn被置为空。
			this.loadEdges(0);
			if(conn == null) System.exit(-1); //SQL错误，conn被置为空。
		} catch (ClassNotFoundException e) {
			log.error(e, e.getCause());
			System.exit(-1);
		} catch(SQLException e){
			log.error(e, e.getCause());
			System.exit(-1);
		}
	}
	
	private Connection getConnection() throws SQLException{
		String url = this.settings.getProperty(DB_URL);
		String username = this.settings.getProperty(DB_USERNAME);
		String password = this.settings.getProperty(DB_PASSWORD);
		log.info("db_url:" + url);
		log.info("username:" + username);
		log.info("password:" + password);
		return DriverManager.getConnection(url, username, password);		
	}

}
