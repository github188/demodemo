package org.notebook.cache;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.MalformedURLException;
import java.net.URL;

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
	
	public File getRootPath(){
		return root;
	}
	
	protected InputStream openInputStream(String id){
        File f = new File(root, id);
        InputStream si= null;
        if(f.isFile()){
            try{
                si = new FileInputStream(f);
                log.debug(String.format("load cache:%s, from:%s", id, f.getAbsoluteFile()));
            }catch(IOException e) {
                log.error(e, e.getCause());
            }
        }else {
        	si = this.getClass().getClassLoader().getResourceAsStream("org/notebook/cache/data/" + id);
        }
        return si;		
	}
	
	protected URL locateResource(String id){
		File f = new File(root, id);
		if(f.isFile()){
			try {
				return f.toURI().toURL();
			} catch (MalformedURLException e) {
				log.error(e.toString(), e);
				return null;
			}
		}else {
			return this.getClass().getClassLoader().getResource("org/notebook/cache/data/" + id);
		}		
	}
	
	protected OutputStream openOutputStream(String id){
        File f = new File(root, id);
        OutputStream  out = null;
        try{
        	out  = new FileOutputStream(f);
        	log.debug(String.format("save cache:%s, from:%s", id, f.getAbsoluteFile()));
        }catch(IOException e){
        	log.error(e, e.getCause());
        }        
        return out;
	}	

}
