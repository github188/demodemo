package org.goku.db;

import java.sql.SQLException;
import java.util.Collection;
import java.util.Map;

import org.goku.core.model.BaseStation;
import org.goku.core.model.User;
import org.goku.settings.Settings;
import org.mortbay.log.Log;

/**
 * 数据访问接口，支持JDBC和HTTP的实现。转发服务器，如果不能直接访问数据库，需要实现
 * HTTP方式从控制服务器读取数据。
 * @author deon
 *
 */
public abstract class DataStorage {
	/**
	 * 根据主建加载一个对象，如果不存在。返回为null.
	 * @param obj
	 * @return 加载的对象，如果不存在，返回为null.
	 */	
	public abstract Object load(Class cls, String pk);
	
	/**
	 * 根据条件过滤对象。
	 */		
	public Collection list(Class cls, String filter, Object[] param){
		return null;
	}
	
	
	/**
	 * 保存一个数据对象，如果不存在，创建一个新对象，否则更新已存在对象。
	 * @param obj
	 * @return 返回true, 如果创建了新对象，否则为false;
	 */
	public abstract boolean save(Object obj);
	public abstract int execute_sql(String sql, Object[] param) throws SQLException;
	public abstract Collection<Map<String, Object>> query(String sql, Object[] param);
	
	public abstract boolean checkConnect();
	
	public static DataStorage create(Settings param){
		if(param.getString(Settings.DB_MASTER_DB, "").equals("dev_dummy")){
			return new DummyDataStorage(param);
		}else {
			return new JDBCDataStorage(param);
		}
	}
	
	
	public abstract Collection<BaseStation> listStation(User user);	
}
