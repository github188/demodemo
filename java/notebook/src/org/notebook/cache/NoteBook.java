package org.notebook.cache;

import java.io.Serializable;

public class NoteBook implements Serializable{

	private static final long serialVersionUID = -4458662555796010998L;
	public String name = "Deon的记事本";
	public String user = null; //System.getenv("USERNAME");
	public String password = null; //System.getenv("USERNAME");
	public String endpoint = "http://deonwu84.appspot.com/note";
	public Category root = null;
	
	public String proxy = "";
	public String username = "";
	public String proxypassword = "";
	public transient String authToken = "";
	public String authSID = "";
	
	public NoteBook(){
		user = defaultUser();
		if(user == null) user = "user";
		name = user + "的记事本";
	}
	
	protected String defaultUser(){
		try{
			return System.getenv("USERNAME");
		}catch(Exception e){
			return "unknown";
		}
	}
	
	public void setEndPoint(String url){
		this.endpoint = url;
	}
	
	public String getUser(){
		if(user.indexOf('@') > 0){
			return user.split("@")[0];
		}else {
			return this.user;
		}
	}
}
