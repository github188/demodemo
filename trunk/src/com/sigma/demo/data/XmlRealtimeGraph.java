package com.sigma.demo.data;

public class XmlRealtimeGraph extends RealtimeGraph{
	private String series;

	public String getSeries() {
		return series;
	}

	public void setSeries(String series) {
		this.series = series;
		if(series != null && !"".equals(series)){
			for(String s: series.split(",")){
				this.addSeries(s);
			}
		}
	}
}
