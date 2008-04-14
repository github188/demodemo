package com.sigma.web;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;


public class UrlMapping {
	protected static final Logger log = LogManager.getLogger(UrlMapping.class);
	//String
	private Pattern mappingPattern = null;
	private String paramPattern = null;
	private Command handler = null;
	
	public UrlMapping(String pattern, Class cl){
		if(pattern != null){
			String[] cfg = pattern.split(":");			
			if(cfg[0].startsWith("/")){
				cfg[0] = "^" + cfg[0];
			}
			mappingPattern = Pattern.compile(cfg[0], Pattern.CASE_INSENSITIVE);
			log.info("New UrlMapping:" + mappingPattern.pattern() + ", Class:" + cl.toString());
			if(cfg.length > 1){
				paramPattern = cfg[1];
			}
		}
		try {
			handler = (Command)cl.newInstance();
		}catch (InstantiationException e) {
			log.error(e,e);
		}catch (IllegalAccessException e) {
			log.error(e,e);
		}

	}
	
	public boolean matched(String url){
		boolean matched = false;
		if(url != null && mappingPattern != null){
			log.debug("match pattern:" + mappingPattern.pattern() + ", url:" + url);
			Matcher m = mappingPattern.matcher(url);
			matched = m.find();
			if(matched && log.isDebugEnabled()){
				log.debug("matched pattern:" + mappingPattern.pattern() + ", url:" + url);
			}
		}
		return matched;
	}		
	
	public Map<String,String> parseUrlParameter(String url){
		Matcher m = mappingPattern.matcher(url);
		Map<String,String> params = null;
		
		if(paramPattern != null && m.find()){
			params = new HashMap<String,String>();
			String _tmp = paramPattern;
			for(int i = 0; i <= m.groupCount(); i++){
				String v = m.group(i);
				v = (v == null) ? "" : v; 
				_tmp = _tmp.replaceAll("\\$\\{" + i +"\\}", v);
			}
			if(log.isDebugEnabled()){
				log.debug("Url Parameter:" + _tmp);
			}
			for(String kv : _tmp.split(",")){
				log.debug("build init urlParam:" + kv);
				int i = kv.indexOf("=");
				if(i > 0){
					params.put(kv.substring(0,i), kv.substring(i + 1));
				}
			}			
		}
		
		return params;
	}
	
	public Command getURLHandler(){
		return handler;
	}
}
