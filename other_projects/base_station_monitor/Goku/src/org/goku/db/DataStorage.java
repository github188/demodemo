package org.goku.db;

import java.sql.SQLException;
import java.util.Collection;
import org.goku.settings.Settings;

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
	public Object load(Class cls, String pk){
		return null;
	}
	
	/**
	 * 根据条件过滤对象。
	 */		
	public Collection list(Class cls, String filter){
		return null;
	}
	
	/**
	 * 保存一个数据对象，如果不存在，创建一个新对象，否则更新已存在对象。
	 * @param obj
	 * @return 返回true, 如果创建了新对象，否则为false;
	 */
	public abstract boolean save(Object obj);
	public abstract int execute_sql(String sql, String[] param) throws SQLException;
	
	public static DataStorage create(Settings param){
		return null;
	}
	
}
