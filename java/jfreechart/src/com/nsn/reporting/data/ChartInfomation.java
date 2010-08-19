/**
* $ Name LastChangeRevision LastChangeDate LastChangeBy $
* $Id$
*/


package com.nsn.reporting.data;

import java.awt.Color;
import java.util.ArrayList;
import java.util.List;

import org.jfree.chart.ChartTheme;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.StandardChartTheme;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.data.RangeType;

public abstract class ChartInfomation {	
	public static final String STYLE_GROUP_STACK = "group_stack";
	public static final String STYLE_STACK = "stack";
	public static final String STYLE_PIE = "pie";
	public static final String STYLE_STACK_LINE = "stack_line";
	
	protected static final int STATE_INIT = 0;
	protected static final int STATE_HEADER = 1;
	protected static final int STATE_DATA = 2;
	protected static final int STATE_DONE = 3;
	
	
	public static final String HEADER_CHART = "Chart";
	public static final String HEADER_FIXEDLENGTH = "Fixed Length";
	public static final String HEADER_TITLE = "Title";
	public static final String HEADER_LABEL = "Label";
	public static final String HEADER_DATA = "Data";
	public static final String HEADER_OUTPUT = "Output";
	public static final String HEADER_SIZE = "Size";
	public static final String HEADER_QUALITY = "Quality";
	public static final String HEADER_GROUP = "Group";
	public static final String HEADER_SUBGROUP = "SubGroup";

	public static final String HEADER_COLORS = "Colors";
	
	/*
	 	the serials style.	 	
	 	example: Style:pass 
	*/
	public static final String HEADER_STYLE = "Style";
	protected static ChartTheme currentTheme = new StandardChartTheme("JFree");
	protected List<Color> serialColors = new ArrayList<Color>();
	protected List<AxisData> labels = new ArrayList<AxisData>();
	
	private String style = null;
	private int state = STATE_INIT;
	private int startNo = 0;
	private int dataCount = 0;
	

	private String size = "600x400";
	private String title = null;
	private String output = null;
	private int fixedLength = -1;
	private int dataLength = -1;
	private float quality = 1;
	
	public static class AxisData{
		public String label = "";
		public int startValue = 0;
		public int autoRange = 10;
		public int maxValue = -1;
		protected AxisData(String data){
			String[] axisData = data.split(":");
			label = axisData[0];
			if (axisData.length > 1){
				startValue = Integer.parseInt(axisData[1].trim());
			} 
			if (axisData.length > 2){
				autoRange = Integer.parseInt(axisData[2].trim());
			}
			if (axisData.length > 3){
				maxValue = Integer.parseInt(axisData[3].trim());
			}
		}
		public boolean equals(Object s){
			if (!(s instanceof AxisData)){
				return false;
			}
			AxisData o = (AxisData)s;
			return this.label.equals(o.label);
		}
		/**
		 * used to compatible old format that max value of second axis is 102. 
		 */
		public void setAsPercentAxis(){
			this.maxValue = 102;
		}
		
		public NumberAxis buildAxis(){
			NumberAxis valueAxis = new NumberAxis(this.label);
			if (this.startValue >= 0){
				valueAxis.setRangeType(RangeType.POSITIVE);
			}
			if (this.autoRange > 0){
				valueAxis.setAutoRangeMinimumSize(this.autoRange);
			}
			if (this.maxValue > 0){
				valueAxis.setRange(this.startValue, this.maxValue);
			}
			return valueAxis;
		}
	}	
	
	public float getQuality() {
		return quality;
	}

	public ChartInfomation(String style){
		this.style = style;
	}
	
	//private String name = null;
	
	public void parseData(String data, int lineNo) {
		boolean result = false;
		
		if(state == STATE_INIT){
			startNo = lineNo;
		}
		
		if(state == STATE_INIT || state == STATE_HEADER){
			if(data.indexOf(":") > 0){
				String[] headers = data.split(":", 2);
				result = this.processHeader(headers[0], headers[1]);
			}
		}else if(state == STATE_DATA){
			result = this.processData(data);
			this.dataCount++;
			
			if(this.dataCount == this.dataLength){
				this.state = STATE_DONE;
			}
		}else if(state == STATE_DONE) {
			echo("it's done to parse charting data.");
		}
		
		if(!result){
			echo(String.format("error line number:%s, line:%s", lineNo, data));
		}
		
	}
	
	public boolean isComplete(){
		return this.state == STATE_DONE;
	}
	
	public boolean processHeader(String name, String data){
		if(name.equals(HEADER_CHART)){
			this.state = STATE_HEADER;
		}else if(name.equals(HEADER_TITLE)){
			this.title = data;
		}else if (name.equals(HEADER_FIXEDLENGTH)){
			this.fixedLength = Integer.parseInt(data.trim());
		}else if (name.equals(HEADER_DATA)){
			this.dataLength = Integer.parseInt(data.trim());
			if(this.dataLength > 0){
				this.state = STATE_DATA;
			}else {
				this.state = STATE_DONE;
			}
			this.dataCount = 0;
		}else if (name.equals(HEADER_OUTPUT)){
			this.output = data;
		}else if (name.equals(HEADER_SIZE)){
			this.size = data;
		}else if (name.equals(HEADER_QUALITY)){
			this.quality = Float.parseFloat(data.trim());
		}else if (name.equals(HEADER_COLORS)){
			return parseColorHeader(data.trim());
		}else if (name.equals(HEADER_LABEL)){
			this.labels.clear();
			for(String s:data.split(",")){
				AxisData aixs = new AxisData(s.trim());
				this.labels.add(aixs);
				if (this.labels.size() > 2 && s.indexOf(":") < 0){
					aixs.setAsPercentAxis();
				}
			}
		}else {
			return false;
		}
		
		return true;
	}
		
	public abstract boolean processData(String data);
	
	public abstract JFreeChart createChart();
	
	private void echo(String s){
		System.out.println(s);
	}

	public String getStyle() {
		return style;
	}

	public String getTitle() {
		return title;
	}

	public String getOutput() {
		return output;
	}

	public String getSize() {
		return size;
	}

	public int getFixedLength() {
		return fixedLength;
	}

	public int getDataLength() {
		return dataLength;
	}
	
	private boolean parseColorHeader(String data){
		serialColors.clear();
		if(data == null) return false;
		for(String color: data.split("\\s+")){
			if(color.indexOf(",") == -1){
				serialColors.add(new Color(Integer.parseInt(color)));
			}else {
				String[] rgb = color.split(",");
				if(rgb.length != 3)
					throw new Error("Invalid RGB color:" + color);
				
				serialColors.add(new Color(Float.parseFloat(rgb[0]),
						Float.parseFloat(rgb[1]),
						Float.parseFloat(rgb[2])));
			}
		}
		
		return true;
	}
}
