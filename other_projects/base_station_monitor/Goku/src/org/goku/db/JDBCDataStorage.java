package org.goku.db;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Types;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class JDBCDataStorage extends DataStorage {
	private Log log = LogFactory.getLog("db");
	
	private String host = null;
	private String username = null;
	private String password = null;
	private String jdbcUrl = null;
	
	public JDBCDataStorage(String host, String user, String password){
		this.jdbcUrl = String.format("jdbc:mysql://%s/goku", host);
	}
	
	public boolean checkConnect(){
		boolean isOk = false;
		try {
			Class.forName("com.mysql.jdbc.Driver").newInstance();
			Connection conn = DriverManager.getConnection(jdbcUrl, username,
					password);
			if(conn == null)throw new Exception("Failed to connect DB");
			conn.close();
			isOk = true;
		} catch (ClassNotFoundException ex) {
			log.error("Not found DB driver.");
		} catch (Exception ex){
			log.error(ex.toString());
		}
		return isOk;
	}
	
	public Collection<Object> list(Class cls, String filter){
		String sql = this.buildSelectSql(cls);
		sql += " where " + filter;
		
		Collection<Map<String, Object>> rowList = query(sql, new String[]{});		
		Collection<Object> result = new ArrayList<Object>();
		
		for(Map<String, Object> row: rowList){
			try {
				Object obj = cls.newInstance();
				for(String f: row.keySet()){
					try {
						cls.getField(f).set(obj, row.get(f));
					}catch (Exception ex){
						log.warn(String.format("Failed to set filed '%s' to class '%s'", f, cls.getName()));
					}
				}
			} catch (Exception e) {
				log.warn(e.toString());
			}
		}
		return result;
	}
	
	public Collection<Map<String, Object>> query(String sql, String[] param){
		sql = bindParam(sql, param);
		Connection conn = this.getConnection();
		Collection<Map<String, Object>> result = new ArrayList<Map<String, Object>>();
		if(conn == null)return result;
		
		Statement st = null;
		ResultSet rs = null;
	    try{
	    	log.debug("Query:" + sql);
	    	st = conn.createStatement();
	    	rs = st.executeQuery(sql);
	    	
	    	String[] names = null;
	    	int[] types = null;
	    	
	    	int count = rs.getMetaData().getColumnCount();
	    	ResultSetMetaData meta = rs.getMetaData();
	    	
	    	names = new String[count];
	    	types = new int[count];
	    	
	    	for(int i = 0; i < count; i++){
	    		names[i] = meta.getColumnName(i);
	    		types[i] = meta.getColumnType(i);
	    	}
	    	
	    	
	    	Map<String, Object> row = null;
	    	while(rs.next()){
	    		row = new HashMap<String, Object>();
	    		result.add(row);
	    		for(int i = 0; i < count; i++){
	    			Object value = null;
	    			switch(types[i]){
	    				case Types.DATE:
	    					value = rs.getDate(i);
	    					break;
	    				case Types.VARCHAR:
	    				case Types.CHAR:
	    				case Types.LONGNVARCHAR:
	    					value = rs.getString(i);
	    					break;
	    				case Types.INTEGER:
	    				case Types.TINYINT:
	    					value = rs.getInt(i);
	    					break;
	    				case Types.FLOAT:
	    					value = rs.getFloat(i);
	    			}
	    			row.put(names[i], value);
	    		}
	    	}
	    	
	    	conn.commit();
	    }
	    catch (SQLException ex){
	    	log.error(ex.toString(), ex);
	    }finally{
	    	if(rs != null){
	    		try {
					rs.close();
				} catch (SQLException e) {
				}
	    	}
	    	if(st != null){
	    		try {
					st.close();
				} catch (SQLException e) {
				}
	    	}
	    	if(conn != null){
	    		try {
					conn.close();
				} catch (SQLException e) {
				}
	    	}
	    }
		
		return result;
	}	
	

	@Override
	public int execute_sql(String sql, String[] param){
		sql = bindParam(sql, param);
		
		int updated = 0;
		log.debug("Execute sql:" + sql);
		Connection conn = this.getConnection();
		if(conn == null)return 0;
		Statement st = null;
	    try{
	    	st = conn.createStatement();
	    	updated = st.executeUpdate(sql);
	    	conn.commit();
	    }
	    catch (SQLException ex){
	    	try {
				conn.rollback();
			} catch (SQLException e) {
				log.error(e.toString());
			}
	    	log.error(ex.toString(), ex);
	    }finally{
	    	if(st != null){
	    		try {
					st.close();
				} catch (SQLException e) {
					log.error(e.toString());
				}
	    	}
	    	if(conn != null){
	    		try {
					conn.close();
				} catch (SQLException e) {
					log.error(e.toString());
				}
	    	}
	    }
	    
		return updated;
	}
	
	private String bindParam(String sql, String[] param){
		for(int i = 0; i < param.length; i++){
			sql = sql.replaceAll(String.format("\\$\\{%s\\}", i), 
								 param[i]);
		}		
		return sql;
	}
	
	private String buildSaveSql(Object obj){
		
		String[] fields = this.getORMFields(obj.getClass());
		String sql = "insert " + this.getTableName(obj.getClass()) +
		"(";
		for(int i = 0; i < fields.length -1; i++){
			sql += fields[i] + ",";
		}
		sql += fields[fields.length - 1] + ")";
		
		sql += "values(";
		
		for(int i = 0; i < fields.length -1; i++){
			sql += this.getSQLValue(obj, fields[i]) + ",";
		}		
		
		sql += this.getSQLValue(obj, fields[fields.length - 1]) + ")";
		
		return sql;
	}
	
	private String buildUpdateSql(Object obj){
		String[] fields = this.getORMFields(obj.getClass());
		String sql = "update " + this.getTableName(obj.getClass()) +		
		"set ";
		
		for(int i = 0; i < fields.length -1; i++){
			sql +=  fields[i] + "=" + this.getSQLValue(obj, fields[i]) + ",";
		}
		
		sql += fields[fields.length - 1] + "=" + this.getSQLValue(obj, fields[fields.length - 1]);
		
		sql += " where ";

		fields = this.getORMPkFields(obj.getClass());
		for(int i = 0; i < fields.length -1; i++){
			sql +=  fields[i] + "=" + this.getSQLValue(obj, fields[i]) + " and ";
		}
		
		sql += fields[fields.length - 1] + "=" + this.getSQLValue(obj, fields[fields.length - 1]);
		
		return sql;		
	}
	
	private String buildCheckExists(Object obj){
		String[] fields = this.getORMPkFields(obj.getClass());
		String sql = "select count(*) as have_row from " + this.getTableName(obj.getClass());
		sql += " where ";

		for(int i = 0; i < fields.length -1; i++){
			sql +=  fields[i] + "=" + this.getSQLValue(obj, fields[i]) + " and ";
		}
		
		sql += fields[fields.length - 1] + "=" + this.getSQLValue(obj, fields[fields.length - 1]);
		
		return sql;		
	}
	
	private String buildSelectSql(Class cls){
		String[] fields = this.getORMFields(cls);
		String sql = "select ";
		
		for(int i = 0; i < fields.length -1; i++){
			sql += fields[i] + ",";
		}
		
		sql += fields[fields.length - 1] + " from " +
			this.getTableName(cls);
		
		return sql;
	}
	
	private String getTableName(Class cls){
		try {
			return (String)cls.getField("ORM_TABLE").get(null);
		} catch (Exception e) {
			log.info(String.format("Not found ORM_TABLE field in class %s.", cls.getName()));
		}
		return null;
	}
	
	private String getSQLValue(Object obj, String field){
		try {
			Object val = obj.getClass().getField(field).get(obj);
			if(val == null){
				return "null";
			}else if(val instanceof String){
				return String.format("'%s'", val);
			}else if(val instanceof Date){
				return null;
			}else {
				return String.format("%s", val);
			}
		} catch (Exception e) {
			log.info(String.format("Not found %s field in class %s.", field, obj.getClass().getName()));
		}
		return "null";
	}	
	
	private String[] getORMFields(Class cls){
		try {
			return (String[])cls.getField("ORM_FIELDS").get(null);
		} catch (Exception e) {
			log.info(String.format("Not found ORM_FIELDS field in class %s.", cls.getName()));
		}
		return null;
	}	
	
	
	private String[] getORMPkFields(Class cls){
		try {
			return (String[])cls.getField("ORM_PK_FIELDS").get(null);
		} catch (Exception e) {
			log.info(String.format("Not found ORM_FIELDS field in class %s.", cls.getName()));
		}
		return null;
	}		
	
	private String getORMSql(Class cls, String name){
		String sql = null;
		try {
			sql = (String)cls.getField(name).get(null);
		} catch (Exception e) {
			log.info(String.format("Not found %s field in class %s.", name, cls.getName()));
		}
		return sql;
	}
	
	private Connection getConnection(){
		Connection conn = null;
		try {
			conn = DriverManager.getConnection(jdbcUrl, username,
					password);
		} catch (SQLException e) {
			log.equals(e.toString());
		}
		return conn;
	}

	@Override
	public boolean save(Object obj) {
		// TODO Auto-generated method stub
		return false;
	}
	
}
