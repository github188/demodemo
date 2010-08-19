package com.nsn.reporting.chart.http;

import java.io.Serializable;

import org.jfree.chart.JFreeChart;

public class ChartData implements Serializable{
	private static final long serialVersionUID = -6477553450770664068L;
	public String sid = null;
	public String data = null;
	public transient JFreeChart chart = null;
	public String error = null;
	//public int width = 0, height = 0;
	//public float quality = 1;
	public String outExt = null;
	public byte[] rawData = null;
	public boolean updated = false; 
}
