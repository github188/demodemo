package org.goku.core.model;

import java.io.IOException;
import java.io.Writer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.goku.db.DataStorage;
import org.json.simple.JSONStreamAware;
import org.json.simple.JSONValue;

public class Location implements JSONStreamAware{
	public static final String ORM_TABLE = "location";
	public static final String[] ORM_FIELDS = new String[]{"uuid", "name",
		"parent", };
	public static final String[] ORM_PK_FIELDS = new String[]{"uuid"};
	
	public String uuid = null;	
	public String name = null;
	public String parent = null;
	
	public List<Location> children = new ArrayList<Location>();
	public List<BaseStation> listBTS = new ArrayList<BaseStation>();
	
	public void load(DataStorage storage, User user){
		
	}
	
	@Override
	public void writeJSONString(Writer out) throws IOException {
        Map<String, Object> obj = new HashMap<String, Object>();
        obj.put("uuid", uuid);
        obj.put("name", name);
        obj.put("children", children);
        obj.put("listBTS", listBTS);
        JSONValue.writeJSONString(obj, out);
	}
}
