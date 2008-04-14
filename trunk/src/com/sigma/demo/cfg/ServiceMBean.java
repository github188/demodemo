package com.sigma.demo.cfg;

import java.util.*;

public class ServiceMBean {
	private String code;
	private String name;
	private String depends; 
	private Map<String,String> attrs = new HashMap<String,String>();
	
	public String getCode() {
		return code;
	}
	
	public void setCode(String code) {
		this.code = code;
	}
	
	public String getDepends() {
		return depends;
	}
	
	public void setDepends(String depends) {
		this.depends = depends;
	}
	
	public String getName() {
		return name;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	public Map<String,String> getAttribute(){
		return this.attrs;
	}
	
	public void setAttribute(String name, String value){
		this.attrs.put(name, value);
	}
	
}