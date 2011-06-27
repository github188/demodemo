package org.notebook.cache;

import java.io.Serializable;

public class InputDataField implements Serializable, Cloneable{
	public String type = "";
	public String name = "";
	public int x = 0;
	public int y = 0;
	public int w = 100;
	public int h = 20;
	public int tabIndex = 0;
	
	public InputDataField(String data){
		this.parseData(data);
	}
	
	private void parseData(String data){
		String[] d = data.split("\\$");
		for(int i = 0; i < d.length; i++){
			switch(i){
				case 0: type = d[i].trim(); break;
				case 1: tabIndex = Integer.parseInt(d[i].trim()); break;
				case 2: name = d[i].trim(); break;
				case 3: x = Integer.parseInt(d[i].trim()); break;
				case 4: y = Integer.parseInt(d[i].trim()); break;
				case 5: w = Integer.parseInt(d[i].trim()); break;
				case 6: h = Integer.parseInt(d[i].trim()); break;
			}			
		}
	}
	
	public String getConfigData(){
		return String.format("%s$%s$%s$%s$%s$%s$%s", 
				type, tabIndex, name, x, y, w, h);
	}
}
