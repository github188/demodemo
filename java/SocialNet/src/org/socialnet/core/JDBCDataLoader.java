package org.socialnet.core;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Comparator;
import java.util.Iterator;
import java.util.Properties;
import java.util.TreeSet;

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
	
	protected Collection<String> relation_sql_list = null;
	//protected String sql_check_id = null;
	
	public JDBCDataLoader(Properties settings) {
		this.settings = settings;
		initDBConnection();
	}

	@Override
	public Collection<Relation> loadEdges(int nodeId) {
		Collection<Relation> sortedSet = new TreeSet<Relation>(new Comparator<Relation>(){
			@Override
			public int compare(Relation o1, Relation o2) {
				if(o1.end_id == o2.end_id && o1.weight == o2.weight){
					return 0;					 
				}else {
					return o1.weight > o2.weight ? 1 : -1;
				}
			}
		});		
		for(String sql: relation_sql_list){
			loadRealtion(nodeId, sql, sortedSet);
			if(conn == null) break; //查询错误. 
		}
		Collection<Relation> result = new ArrayList<Relation>(sortedSet.size());
		result.addAll(sortedSet);
	    RunStatus.db_query_count++;
		return result;
	}
	
	private void loadRealtion(int nodeId, String sql, Collection<Relation> result){
		ResultSet rs = null;
		PreparedStatement pstm = null;
		int friend;
		float weight;
		String desc;
	    try
	    {
	    	if(conn == null) conn = this.getConnection();
	    	pstm = conn.prepareStatement(sql,
	    			 ResultSet.TYPE_FORWARD_ONLY,
	    			 ResultSet.CONCUR_READ_ONLY
	    			 );
	    	pstm.setInt(1, nodeId);
	    	rs = pstm.executeQuery();
	    	
	    	rs.setFetchSize(100);
			while (rs.next()){
				friend = rs.getInt("friend");
				desc = rs.getString("desc");
				weight = rs.getFloat("weight");
				result.add(new Relation(friend, weight, desc));
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
		return true;
	}
	
	private void initDBConnection(){
		String driver = settings.getProperty(DB_DRIVER).trim();
		log.info("db_driver:" + driver);
		try {
			Class.forName(driver);
			this.getConnection();
			this.relation_sql_list = getRelationSQL(settings);
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
	
	private Collection<String> getRelationSQL(Properties settings){
		Collection<String> sqls = new ArrayList<String>();
		for(Iterator iter = settings.keySet().iterator();
			iter.hasNext();){
			String obj = iter.next().toString();
			if(obj.startsWith("sql_relation")){
				sqls.add(settings.getProperty(obj));
				log.info("relation sql:" + settings.getProperty(obj));
			}
		}
		return sqls;
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
