package org.goku.db;

import java.sql.SQLException;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.BaseStation;
import org.goku.core.model.User;
import org.goku.settings.Settings;

/**
 * 用于调试接口使用的，测试数据。
 * @author deon
 *
 */
public class DummyDataStorage extends DataStorage {
	private Log log = LogFactory.getLog("dummy");
	private Map<Class, Collection<Object>> objects = new HashMap<Class, Collection<Object>>();
	
	public DummyDataStorage(Settings settings){
		Collection<Object> xxx = new Vector<Object>();
		objects.put(User.class, xxx);
		
		User u = new User();
		u.name = "test1";
		u.password = "p1";		
		xxx.add(u);		
	}
	
	@Override
	public Object load(Class cls, String pk){
		Collection<Object> objList = objects.get(cls);
		if(cls.equals(User.class)){
			for(Object o: objList){
				User uu = (User)o;
				if(uu.name.equals(pk))return uu;
			}
		}
		
		return null;
	}	

	@Override
	public boolean save(Object obj) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public int execute_sql(String sql, Object[] param) throws SQLException {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public Collection<Map<String, Object>> query(String sql, Object[] param) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean checkConnect() {
		log.warn("Loading DummayDataStorage, It's only used for development.");
		
		log.warn("=================================================");
		log.warn("--------User----------");
		Collection<Object> objList = objects.get(User.class);
		for(Object o: objList){
			User uu = (User)o;
			log.info("name:" + uu.name + ", password:" + uu.password);
		}
		
		log.warn("=================================================");
		return true;
	}

	@Override
	public Collection<BaseStation> listStation(User user) {
		Collection<BaseStation> xxx = new Vector<BaseStation>();
		
		BaseStation bs = null; 
		bs = new BaseStation();
		bs.uuid = "0001";
		bs.connectionStatus = "01";
		
		bs.routeServer = "http://127.0.0.1:8080/xx";
		bs.devType = 1;
		xxx.add(bs);

		bs = new BaseStation();
		bs.uuid = "0002";
		bs.connectionStatus = "02";
		
		bs.routeServer = "http://127.0.0.1:8080/xx";
		bs.devType = 1;
		xxx.add(bs);
		
		return xxx;
	}

}
