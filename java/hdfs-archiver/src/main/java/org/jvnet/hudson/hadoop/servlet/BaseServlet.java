package org.jvnet.hudson.hadoop.servlet;

import java.io.IOException;
import java.io.InputStream;
import java.io.Writer;

import javax.servlet.http.HttpServlet;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class BaseServlet extends HttpServlet{
	private Log log = LogFactory.getLog("hdfs.servlet");
	private static final long serialVersionUID = 1L;
	
	protected void outputStatic(String name, Writer out) throws IOException{
		String path = "org/jvnet/hudson/hadoop/servlet/static/" + name;
	    InputStream ins = this.getClass().getClassLoader().getResourceAsStream(path);
	    byte[] buffer = new byte[128 * 1024];
	    if(ins != null){
	    	int len = ins.read(buffer);
	    	out.write(new String(buffer, 0, len, "utf8"));
	    }
	}
}
