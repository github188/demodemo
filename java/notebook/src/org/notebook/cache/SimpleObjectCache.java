package org.notebook.cache;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class SimpleObjectCache{
	private static SimpleObjectCache ins = null;
	private File root = null;
	private Log log = null;
	public static SimpleObjectCache getInstance(){
		return ins;
	}
	
	public SimpleObjectCache(File root){
		ins = this;
		log = LogFactory.getLog(SimpleObjectCache.class);
		root = new File(root, ".notbook");
		if(!root.isDirectory()){
			try{
				root.mkdirs();
			}catch(Exception e){
				log.error(e, e);
			}
		}
	}
	public NoteMessage load(String id){
		return (NoteMessage)this.loadObject("notebook");
	}
	public void save(NoteMessage msg){
		saveObject(msg.messageId, msg);		
	}
	public NoteBook loadNoteBook(){		
		return (NoteBook)this.loadObject("notebook");
	}
	public void saveNoteBook(NoteBook book){
		saveObject("notebook", book);		
	}
	
	private Object loadObject(String id){
        File f = new File(root, id);
        ObjectInputStream si= null;
        Object obj = null;
        if(f.isFile()){
            try{
                si = new ObjectInputStream(new FileInputStream(f));
                obj = si.readObject();
                si.close();
                log.info(String.format("load cache:%s", id));
            } catch (ClassNotFoundException e) {
                    log.error(e, e.getCause());
            }catch(IOException e) {
                    log.error(e, e.getCause());
            }
        }
        
        return obj;		
	}
	
	private void saveObject(String id, Object obj){
        ObjectOutputStream so = null;
        File f = new File(root, id);
        try{
            so = new ObjectOutputStream(new FileOutputStream(f));
            so.writeObject(obj);
            so.close();
            log.info(String.format("save cache:%s", id));
        }catch(IOException e ) {
                log.error(e, e.getCause());
        }		
	}
}
