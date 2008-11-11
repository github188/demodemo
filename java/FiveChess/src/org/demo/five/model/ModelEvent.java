package org.demo.five.model;


/**
 * 数据变化时触发。
 * @author deon
 */
public class ModelEvent {
	public int ADD = 0;
	public int REMOVE = 1;
	
	private int action;
	private int x;
	private int y;
	private int data;
	public ModelEvent(int action, int x, int y, int data){
		this.action = action;
		this.x = x;
		this.y = y;
		this.data = data;
	}
	
	public int getAction() {
		return action;
	}

	public int getData() {
		return data;
	}

	public int getX() {
		return x;
	}

	public int getY() {
		return y;
	}

}
