package org.notebook.cache;

import java.io.Serializable;

public class NoteBook implements Serializable{

	private static final long serialVersionUID = -4458662555796010998L;
	public String name = "Deon的记事本";
	public String user = null; //System.getenv("USERNAME");
	public String endpoint = "http://deonwu84.appspot.com/note";
	public Category root = null;
	
	public String proxy = "";
	public String username = "";
	public String password = "";
	
	public NoteBook(){
		user = System.getenv("USERNAME");
		if(user == null) user = "user";
		name = "user" + "的记事本";
	}	
}