package com.sigma.demo.cfg;

import java.io.IOException;
import java.io.InputStream;
import java.io.Reader;
import java.util.Collection;
import java.util.Vector;

import org.apache.commons.digester.Digester;
import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;
import org.xml.sax.SAXException;

public class DemoServerConfig {
	protected static final Logger log = LogManager.getLogger(DemoServerConfig.class);
	
	private String name;
	private String config;
	
	private Collection<ServiceMBean> services = new Vector<ServiceMBean>();
	
	public DemoServerConfig(InputStream in){
		try {
			Digester digester = initDigester();
			digester.push(this);
			digester.parse(in);
			//initDemoData();
		} catch (IOException e) {
			log.error(e,e);
		} catch (SAXException e) {
			log.error(e,e);
		}
	}
	
	public DemoServerConfig(Reader reader){
		try {
			Digester digester = initDigester();
			digester.push(this);
			digester.parse(reader);
			//initDemoData();
		} catch (IOException e) {
			log.error(e,e);
		} catch (SAXException e) {
			log.error(e,e);
		}
	}
	
	 

	
	public void addServiceMBean(ServiceMBean service){
		services.add(service);
	}
	
	public Collection<ServiceMBean> getServiceMBean(){
		return services;
	}
	
	private Digester initDigester(){
		Digester digester = new Digester();
		digester.addSetProperties("server");
		
		digester.addObjectCreate("server/mbean","com.sigma.demo.cfg.ServiceMBean");
		digester.addSetProperties("server/mbean");
		digester.addSetNext("server/mbean", "addServiceMBean","com.sigma.demo.cfg.ServiceMBean");
		
		digester.addRule("server/mbean/attribute", new AttributeRule());
		
		return digester;
	}
	
	private class AttributeRule extends org.apache.commons.digester.Rule{
		ServiceMBean mbean = null;
		String name = null;
		 public void begin(java.lang.String namespace,
				 		   java.lang.String name,
				 		   org.xml.sax.Attributes attributes){
			 Digester d = getDigester();
			 Object top = d.peek();
			 if(top instanceof ServiceMBean){
				 mbean = (ServiceMBean)top;
				 this.name = attributes.getValue("name");
			 }else {
				 log.warn("Not found ServiceMBean object in degester top of stack.");
			 }
		 }
		 public void body(String namsspance,String name, String body){
			 if(mbean != null && name != null && body != null){
				 mbean.setAttribute(this.name, body.trim());
			 }
		 }
		 
		 public void end(String namsspance,String name){
			 mbean = null;
			 name = null;
		 }
	}

	public String getConfig() {
		return config;
	}

	public void setConfig(String config) {
		this.config = config;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}	
}
