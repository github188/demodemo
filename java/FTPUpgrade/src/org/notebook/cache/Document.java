package org.notebook.cache;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

public class Document implements Serializable, Cloneable{
	private Map<String, String> data = new HashMap<String, String>();
	
	public String get(String name){
		return data.get(name);
	}
	
	public void put(String name, String value){
		this.data.put(name, value);
	}
	
	public Document copy(){
		Document nDoc = new Document();
		nDoc.data.putAll(data);
		return nDoc;
	}
}
