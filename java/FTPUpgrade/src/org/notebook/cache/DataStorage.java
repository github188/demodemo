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
		
	public abstract File getRootPath();	
	abstract protected URL locateResource(String id);	
	abstract protected OutputStream openOutputStream(String id);
	abstract protected InputStream openInputStream(String id);
}
