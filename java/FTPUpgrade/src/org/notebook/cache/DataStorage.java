package org.notebook.cache;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.URL;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public abstract class DataStorage{
	protected Log log = LogFactory.getLog("storage");
	
	public DocumentDefine loadDocument(String name){
		InputStream in = openInputStream(name);
		DocumentDefine doc = new DocumentDefine();
		if(in != null){
			try{
				doc.load(new BufferedReader(new InputStreamReader(in)));
				doc.setImageURL(this.locateResource(doc.backGround));
			}catch(IOException e){
				log.error(e.toString(), e.getCause());
			}finally{
				try{
					in.close();
				}catch(Exception e){
					log.error(e.toString(), e.getCause());
				}
			}
		}
		return doc;
	}
	
	public void saveDocument(String name, DocumentDefine doc){
		OutputStreamWriter out = new OutputStreamWriter(openOutputStream(name));
		if(out != null){
			try{
				doc.save(out);
				out.flush();
			}catch(IOException e){
				log.error(e.toString(), e.getCause());
			}finally{
				try{
					out.close();
				}catch(Exception e){
					log.error(e.toString(), e.getCause());
				}
			}
		}
	}
	
	public abstract File getRootPath();	
	abstract protected URL locateResource(String id);	
	abstract protected OutputStream openOutputStream(String id);
	abstract protected InputStream openInputStream(String id);
}
