package com.sigma.demo.data;

public class XMLSeries extends Series {
	
	public String getxLabel() {
		return getXLabel();
	}
	public void setxLabel(String label) {
		super.setXLabel(label);
	}
	public String getyLabel() {
		return super.getYLabel();
	}
	public void setyLabel(String label) {
		super.setYLabel(label);
	}
	public long getyMax() {
		return super.getYMax();
	}
	public void setyMax(long max) {
		super.setYMax(max);
	}

}
