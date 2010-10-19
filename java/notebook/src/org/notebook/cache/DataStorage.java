package org.notebook.cache;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public abstract class DataStorage{
	protected Log log = LogFactory.getLog("DataStorage");
	
	public NoteMessage load(String id){
		NoteMessage msg = (NoteMessage)this.loadObject(id);
		return msg;
	}
	public void save(NoteMessage msg){
		saveObject(msg.messageId, msg);		
	}
	public NoteBook loadNoteBook(){		
		NoteBook book = (NoteBook)this.loadObject("notebook");
		if(book != null){
			book.root.setRoot(book.root);
			book.root.restore();
			book.root.loader = this;
		}
		return book;
	}
	public void saveNoteBook(NoteBook book){
		saveObject("notebook", book);
		book.root.flush();
	}
	
	protected abstract void saveObject(String id, Object obj);
	protected abstract Object loadObject(String id);
}
