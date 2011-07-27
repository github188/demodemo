package org.notebook.cache;

import java.io.File;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.MalformedURLException;
import java.net.URL;

import javax.jnlp.BasicService;
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
    
	public File getRootPath(){
		return new File(root.getFile());
	}    
	
	protected URL getPath(String id){
		try {
			return new URL(root.toString() + id);
		} catch (MalformedURLException e) {
			log.error(e, e);
			return null;
		}		
	}

	@Override
	protected OutputStream openOutputStream(String id) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	protected InputStream openInputStream(String id) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	protected URL locateResource(String id) {
		// TODO Auto-generated method stub
		return null;
	}
}
