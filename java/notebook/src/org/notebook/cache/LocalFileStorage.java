package org.notebook.cache;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

public class LocalFileStorage extends DataStorage {
	private File root = null;

	public LocalFileStorage(File root){
		this.root = root;
		if(!this.root.isDirectory()){
			try{
				this.root.mkdirs();
			}catch(Exception e){
				log.error(e, e);
			}
		}
		log.info("Cache root:" + this.root.getAbsolutePath());
	}	
	
	protected Object loadObject(String id){
        File f = new File(root, id);
        ObjectInputStream si= null;
        Object obj = null;
        if(f.isFile()){
            try{
                si = new ObjectInputStream(new FileInputStream(f));
                obj = si.readObject();
                si.close();
                log.debug(String.format("load cache:%s", id));
            } catch (ClassNotFoundException e) {
                log.error(e, e.getCause());
            }catch(IOException e) {
                log.error(e, e.getCause());
            }
        }
        
        return obj;		
	}
	
	protected void saveObject(String id, Object obj){
        ObjectOutputStream so = null;
        File f = new File(root, id);
        try{
            so = new ObjectOutputStream(new FileOutputStream(f));
            so.writeObject(obj);
            so.close();
            log.debug(String.format("save cache:%s", id));
        }catch(IOException e ) {
                log.error(e, e.getCause());
        }		
	}	
}
