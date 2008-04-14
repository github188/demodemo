package com.sigma.demo.util;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

public class Parameter {
	private Map<String,String> attrs = new HashMap<String,String>();
	
	public Parameter(){
		
	}
	
	public Parameter(Map<String,String> values){
		if(values != null){
			attrs.putAll(values);
		}
	}
	
	public int getIntParameter(String name, int def){
		String value = getStringParameter(name, null);
		if(value != null){
			try{
				def = Integer.parseInt(value);
			}catch(NumberFormatException e){}
		}
		return def;
	}
	
	public String getStringParameter(String name, String def){
		String value = attrs.get(name);
		if(value == null || "".equals(value)){
			value = def;
		}
		
		return value;
	}
	
	public boolean getBooleanParameter(String name, boolean def){
		String value = getStringParameter(name, null);
		if(value != null ){
			try{
				def = Boolean.parseBoolean(value);
			}catch(Error e){}
		}
		return def;
	}
	
	public Set<String> getKeySet() {
		return attrs.keySet();
	}

}
