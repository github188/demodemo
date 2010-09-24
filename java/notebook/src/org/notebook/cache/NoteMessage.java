package org.notebook.cache;

import java.io.Serializable;
import java.util.Date;

public class NoteMessage implements Serializable{
	private static final long serialVersionUID = -4142745216950216687L;
	public String messageId;
	//public String name;
	public Date createDate = new Date();
	public Date lastUpdated;
	public String text;
	public String tags;
	
	public NoteMessage(String id){
		this.messageId = id;
	}
}
