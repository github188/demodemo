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
	public transient boolean isDirty = false;
	
	private transient Category owner = null;
	
	public NoteMessage(String id){
		this.messageId = id;
	}
	
	public void setText(String text){
		this.text = text;
		if(owner != null){
			owner.setLastUpdate();
		}
		this.lastUpdated = new Date();
	}
	
	public Category getCategory(){
		return this.owner;
	}
	
	public void setCategory(Category c){
		this.owner = c;
	}
	
	public String getText(){
		if (text == null){
			text = "";
		}
		return text;
	}
	
	public Date getLastUpdate(){
		if(this.lastUpdated == null){
			this.lastUpdated = new Date();
		}
		return this.lastUpdated;
	}
	
	public void update(NoteMessage note){
		if(note != null){
			this.text = note.getText();
		}
	}
}
