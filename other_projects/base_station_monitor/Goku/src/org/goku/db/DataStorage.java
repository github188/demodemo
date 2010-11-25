package org.goku.db;

import java.util.Collection;
import org.goku.settings.Settings;

/**
 * 数据访问接口，支持JDBC和HTTP的实现。转发服务器，如果不能直接访问数据库，需要实现
 * HTTP方式从控制服务器读取数据。
 * @author deon
 *
 */
public abstract class DataStorage {
	public Object load(Class cls, String pk){
		return null;
	}
	
	public Collection list(Class cls, String filter){
		return null;
	}
	
	public static DataStorage create(Settings param){
		return null;
	}
}
