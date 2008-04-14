package com.sigma.demo.data;
/*
 * ---------------------------------------------------
 * $Id: XMLDemoData.java 785 2008-02-20 04:59:15Z dwu $
 * ---------------------------------------------------
 *
 */
import java.io.IOException;
import java.io.InputStream;
import java.io.Reader;

import org.apache.commons.digester.Digester;
import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

public class XMLDemoData extends DemoData {
	protected static final Logger log = LogManager.getLogger(XMLDemoData.class);
	
	String topology = null;
	
	public XMLDemoData(InputStream in) throws Exception {
		this(in,false);
	}
	
	public XMLDemoData(Reader reader) throws Exception{
		this(reader,false);
	}
	
	public XMLDemoData(InputStream in, boolean validate) throws Exception{
		try {
			Digester digester = initDigester(validate);
			digester.push(this);
			digester.parse(in);
			initDemoData();
		} catch (IOException e) {
			log.error(e,e);
		} 
	}
	
	public XMLDemoData(Reader reader, boolean validate) throws Exception{
		try {
			Digester digester = initDigester(validate);
			digester.push(this);
			digester.parse(reader);
			initDemoData();
		} catch (IOException e) {
			log.error(e,e);
		} 
	}
	
	
	private void initDemoData(){
		this.setFullName(this.getName());
	}
	
	private Digester initDigester(boolean validate){
		Digester digester = new Digester();
		digester.register("demo.dtd", Thread.currentThread().getContextClassLoader()
				.getResource("demo.dtd"));
		digester.setValidating(validate);
		
		digester.addSetProperties("demo");
		
		
		digester.addCallMethod("demo/brief", "setBrief", 0);
		digester.addCallMethod("demo/detail","setDescription", 0);
		digester.addCallMethod("demo/preparation","setPreparation", 0);
		digester.addCallMethod("demo/healthy","setHealthy", 0);
		digester.addCallMethod("demo/provision","setProvision", 0);
		

		digester.addObjectCreate("demo/series/ser","com.sigma.demo.data.XMLSeries");
		digester.addSetProperties("demo/series/ser");
		digester.addSetNext("demo/series/ser", "addSeries","com.sigma.demo.data.Series");
		
		//digester.addRule("demo/vars/var", new VarRule());
		
		//digester.addObjectCreate("demo/graphs/graph","com.sigma_rt.dl.data.Graph");
		digester.addRule("demo/graphs/graph", new GrpahType());
		digester.addSetProperties("demo/graphs/graph");
		//digester.addSetNext("demo/graphs/graph", "addGraph","com.sigma_rt.dl.data.Graph");

		digester.addObjectCreate("demo/consoles/console","com.sigma.demo.data.DemoConsole");
		digester.addSetProperties("demo/consoles/console");
		digester.addSetNext("demo/consoles/console", "addConsole","com.sigma.demo.data.DemoConsole");
		
		digester.addObjectCreate("demo/vncsvrs/vncsvr","com.sigma.demo.data.DemoVNC");
		digester.addSetProperties("demo/vncsvrs/vncsvr");
		digester.addSetNext("demo/vncsvrs/vncsvr", "addVnc","com.sigma.demo.data.DemoVNC");

		digester.addObjectCreate("demo/actions/action","com.sigma.demo.data.XMLDemoStep");
		digester.addSetProperties("demo/actions/action");
		digester.addCallMethod("demo/actions/action","setDescription", 0);
		digester.addSetNext("demo/actions/action", "addStep","com.sigma.demo.data.XMLDemoStep");
		
//		digester.addCallMethod("demo/steps/step/desc", "setDescription", 0);
//		
//		digester.addObjectCreate("demo/steps/step/action","com.sigma_rt.dl.data.Action");
//		digester.addSetProperties("demo/steps/step/action");
//		digester.addSetNext("demo/steps/step/action", "addAction","com.sigma_rt.dl.data.Action");
		
		
		return digester;
	}
	
	private class VarRule extends org.apache.commons.digester.Rule{
		 public void begin(java.lang.String namespace,
				 		   java.lang.String name,
				 		   org.xml.sax.Attributes attributes){
			 Digester d = getDigester();
			 Object top = d.peek();
			 if(top instanceof DemoData){
				 DemoData demo = (DemoData)top;
				 String _name = attributes.getValue("name");
				 String _value = attributes.getValue("value");
				 demo.addVariable(_name, _value);
			 }else{
				 log.warn("Not found DemoData object in degester top of stack.");
			 }
		 } 
	}
	
	private class GrpahType extends org.apache.commons.digester.Rule{
		 public void begin(java.lang.String namespace,
				 		   java.lang.String name,
				 		   org.xml.sax.Attributes attributes){
			 Digester d = getDigester();
			 String type = attributes.getValue("type");
			 if(type != null){
				 if(type.equals("pic")) {
					 d.push(new HistoryGraph());
				 }else if(type.equals("gta")) { //RealtimeGraph
					 RealtimeGraph gta = new XmlRealtimeGraph();
					 d.push(gta);
					 String style = attributes.getValue("style");
					 if(style != null){
						 if(style.equals("gsa")){
							 gta.setGraphStyle(GraphStyle.getStyleA());
						 }else if(style.equals("gsb")){
							 gta.setGraphStyle(GraphStyle.getStyleB());
						 }
					 }
				 }else {
					 log.warn("Not support graph type!");	 
				 }
			 }
		 }
		 public void end(java.lang.String namespace,
		 		   java.lang.String name){
			 Digester d = getDigester();
			 Graph g = (Graph) d.pop();
			 DemoData demo = (DemoData) d.peek();
			 demo.addGraph(g);
		 }
		 
	}
	

	public String getTopology() {
		return topology;
	}

	public void setTopology(String topology) {
		this.topology = topology;
		this.setTopologyURL(topology);
	}
	
}
