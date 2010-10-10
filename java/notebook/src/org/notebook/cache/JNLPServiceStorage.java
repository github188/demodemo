package org.notebook.cache;

import java.io.ByteArrayOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.MalformedURLException;
import java.net.URL;

import javax.jnlp.BasicService;
import javax.jnlp.FileContents;
import javax.jnlp.PersistenceService;
import javax.jnlp.ServiceManager;
import javax.jnlp.UnavailableServiceException;

public class JNLPServiceStorage extends DataStorage{
    private PersistenceService ps = null;
    private BasicService bs = null;
    private URL root = null;
    
    public JNLPServiceStorage(){
        try { 
            ps = (PersistenceService)ServiceManager.lookup("javax.jnlp.PersistenceService"); 
            bs = (BasicService)ServiceManager.lookup("javax.jnlp.BasicService");
        	root = bs.getCodeBase();
        	log.error("JNLP Root:" + root.toString());
        } catch (UnavailableServiceException e) { 
            ps = null; 
            bs = null;
            log.error("failed to init JNLP service");
        }
    }

	@Override
	protected Object loadObject(String id) {
		log.info(String.format("loading:%s", id));
		URL path = getPath(id);
		ObjectInputStream si = null;
		Object obj = null;
		if(path != null){
			try {
				FileContents fc = ps.get(path);
                si = new ObjectInputStream(fc.getInputStream());
                obj = si.readObject();
                si.close();
                log.info(String.format("load cache:%s", id));
			} catch(FileNotFoundException e){
				log.info(String.format("Not found:%s", id));
			} catch (Exception e) {
				log.error("Error load:" + id, e);
			} finally{
				if(si != null){
					try {
						si.close();
					} catch (IOException e) {
						log.error(e, e);
					}
				}
			}
		}
		return obj;
	}

	@Override
	protected void saveObject(String id, Object obj) {
		log.info(String.format("saving:%s", id));
		URL path = getPath(id);
		if(path != null){
			ByteArrayOutputStream buffer = new ByteArrayOutputStream(100 * 1024);
			ObjectOutputStream so = null;
	        try {
	        	so = new ObjectOutputStream(buffer);
				so.writeObject(obj);
				so.close();
				
				try{
					ps.delete(path);
				}catch (IOException e) {					
				}
				
		        long allocSize = ps.create(path, buffer.size()); 
		        if(allocSize >= buffer.size()){
			        FileContents fc = ps.get(path); 
			        OutputStream os = fc.getOutputStream(true); 
			        os.write(buffer.toByteArray()); 
			        os.close();
			        log.info(String.format("save cache:%s", id));
		        } else {
		        	log.info("failed to alloc size, alloced size:" + allocSize);
		        }
			} catch (IOException e) {
				log.error("Error saveObject:" + id, e);
				return;
			}
		}
	}
	
	protected URL getPath(String id){
		try {
			return new URL(root.toString() + id);
		} catch (MalformedURLException e) {
			log.error(e, e);
			return null;
		}		
	}
}
