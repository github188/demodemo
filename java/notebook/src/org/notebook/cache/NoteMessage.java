package org.notebook.cache;

import java.io.Serializable;
import java.util.Date;

public class NoteMessage implements Serializable{
	private static final long serialVersionUID = -4142745216950216687L;
	public String messageId;
	//public String name;
	public Date createDate = new Date();
	public Date lastUpdated = new Date();
	public String text = "";
	public String tags = "";
	public boolean isDirty = false;
	
	private transient Category owner = null;
	
	public NoteMessage(String id){
		this.messageId = id;
	}
	
	public void setText(String text){
		this.text = text;
	}
	
	public Category getCategory(){
		return this.owner;
	}
	
	public void setCategory(Category c){
		this.owner = c;
	}
}
