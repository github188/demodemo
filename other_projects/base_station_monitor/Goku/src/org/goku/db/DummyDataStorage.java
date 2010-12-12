package org.goku.db;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStreamReader;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.BaseStation;
import org.goku.core.model.RouteServer;
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
		u.password = "pass";		
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
		}else if(cls.equals(BaseStation.class)){
			for(Object o: objList){
				BaseStation uu = (BaseStation)o;
				if(uu.uuid.equals(pk)) return uu;
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
		return new ArrayList<Map<String, Object>>();
	}

	@Override
	public boolean checkConnect() {
		
		log.warn("Loading DummayDataStorage, It's only used for development.");
		loadStanloneDB();
		
		log.warn("=================================================");
		log.warn("--------User----------");
		Collection<Object> objList = objects.get(User.class);
		for(Object o: objList){
			User uu = (User)o;
			log.info("name:" + uu.name + ", password:" + uu.password);
		}
		
		log.warn("--------BaseStation----------");
		objList = objects.get(BaseStation.class);
		for(Object o: objList){
			BaseStation uu = (BaseStation)o;
			log.info("name:" + uu.uuid + ", location:" + uu.locationId);
		}
		
		log.warn("=================================================");
		return true;
	}
	
	private void loadStanloneDB(){
		File file = new File("standlone.db");
		if(file.isFile()){
			log.info("Loading db file:" + file.getAbsolutePath());
			try {
				BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
				
				Collection<Object> bsList = new Vector<Object>();
				objects.put(BaseStation.class, bsList);

				for(String line = reader.readLine(); line != null;){
					line = line.trim();
					if(line.length() == 0)continue;
					log.debug("Read line:" + line);
					if(line.startsWith("BS:")){
						line = line.split(":", 2)[1];
						BaseStation bs = new BaseStation();
						String[] bsinfo = line.split("\\$", 4);
						log.debug("Bsinfo:" + bsinfo[0]);
						bs.uuid = bsinfo[0];
						bs.devType = Integer.parseInt(bsinfo[1]);
						bs.groupName = bsinfo[2];
						bs.locationId = bsinfo[3];
						bsList.add(bs);
					}
					
					line = reader.readLine();
				}
			} catch (Exception e) {
				log.error(e);
			}
		}		
	}

	@Override
	public Collection<BaseStation> listStation(User user) {
		//Collection<BaseStation> xxx = new Vector<BaseStation>();
		Collection xxx = objects.get(BaseStation.class);
		
		return xxx;
	}

	@Override
	public Collection<BaseStation> listStation(RouteServer route) {
		// TODO Auto-generated method stub
		return new ArrayList<BaseStation>();
	}

	@Override
	public Collection<BaseStation> listDeadStation(String group) {
		Collection xxx = objects.get(BaseStation.class);
		
		return xxx;		
		//return null;
	}

	@Override
	public void removeRouteServer(RouteServer route) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public boolean save(Object obj, String[] fields) {
		// TODO Auto-generated method stub
		return false;
	}

}
