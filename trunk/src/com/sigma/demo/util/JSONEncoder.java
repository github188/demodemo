package com.sigma.demo.util;

import java.util.*;

import org.apache.commons.beanutils.BeanMap;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

public class JSONEncoder {
	
	@SuppressWarnings("unchecked")
	public JSONObject encodeObject(Object o) {
		JSONObject json = new JSONObject();
		BeanMap object = new BeanMap(o);
		for(Iterator iter = object.keyIterator(); iter.hasNext();) {
			String key = (String)iter.next();
			Object value = object.get(key);
			if(key.equalsIgnoreCase("class")){
				continue;
			}
			value = (value == null) ? "" : value;
			if(value.getClass().isArray()) {
				JSONArray array = new JSONArray();
				for(Object item :(Object[]) value) {
					array.add(encodeValue(item));
				}
				json.put(key, array);
			}else if(value instanceof Collection) {
				JSONArray array = new JSONArray();
				for(Object item : (Collection) value) {
					array.add(encodeValue(item));
				}
				json.put(key, array);
			}else {
				json.put(key, encodeValue(value));
			}
		}		
		return json;
	}
	
	private Object encodeValue(Object value) {
		//if(value == null) return "";
		//System.out.println(value.toString());
		if(value.getClass().isPrimitive() ||
				value.getClass().getName().startsWith("java.lang") ||
				value instanceof Date 
		) {
			return value;
		}else {
			return encodeObject(value);
		}
	}
}
