package com.sigma.demo.data;

public class XMLDemoStep extends DemoStep {
	
	public void setPre(String pre){
		this.setPreAction(pre);
	}
	
	public void setFunc(String func){
		//this.setf
		super.setFunction(func);
	}
	
	public void setTopology(String topo){
		this.setTopologyURL(topo);
	}
}
