package com.sigma.demo.event;

import com.sigma.demo.DemoEvent;

public class GraphEvent extends DemoEvent {
	private String serisName = null;
	private long xData = 0;
	private double yData = 0;
	public GraphEvent(String serisName, long xData, double yData){
		this.serisName = serisName;
		this.xData = xData;
		this.yData = yData;
	}
	
	public String getSerisName() {
		return serisName;
	}
	public long getXData() {
		return xData;
	}
	public double getYData() {
		return yData;
	}

}
