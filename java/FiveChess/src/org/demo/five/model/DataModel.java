package org.demo.five.model;

import java.util.*;

public class DataModel {
	protected byte[][] data = new byte[18][18];
	private Collection<ModelListener> listener = new Vector<ModelListener>();
	public DataModel(){
		
	}
	
	public void addListener(ModelListener l){
		this.listener.add(l);		
	}
	
	protected void fireEvent(ModelEvent e){
		for (ModelListener l : this.listener){
			l.update(e);
		}
	}
}
