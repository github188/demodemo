package org.notebook.i18n;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.util.Enumeration;
import java.util.Properties;
import java.util.ResourceBundle;

public class SimpleResourceBound extends ResourceBundle {
	private Properties data = new Properties();
	public SimpleResourceBound(){		
		InputStream in = SimpleResourceBound.class.getClassLoader().
			getResourceAsStream(this.resource());
		try {
			Reader reader = new InputStreamReader(in, "utf-8");
			data.load(reader);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	private String resource(){
		String resource = "org/notebook/i18n/NoteBook_zh_CN.properties";
		if(!System.getProperty("user.language").equals("zh")){
			resource = "org/notebook/i18n/NoteBook_en.properties";
		}
		return resource;
	}
	

	@Override
	protected Object handleGetObject(String key) {
		//if(data)
		String value = (String)data.getProperty(key);
		return (value != null) ? value : key;
	}

	@Override
	public Enumeration<String> getKeys() {
		final Enumeration<Object> keys = data.keys();
		Enumeration<String> enums = new Enumeration<String>(){
			@Override
			public boolean hasMoreElements() {
				return keys.hasMoreElements();
			}
			@Override
			public String nextElement() {
				String k = keys.nextElement().toString();
				return k;
			}			
		};
		return enums;
	}
}
