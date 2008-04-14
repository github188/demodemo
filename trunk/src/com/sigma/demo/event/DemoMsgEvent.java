package com.sigma.demo.event;

import com.sigma.demo.DemoEvent;

public class DemoMsgEvent extends DemoEvent {
	private String level = null;
	private String message = null;

	public DemoMsgEvent(String msg, String level){
		this.level = level;
		this.message = msg;		
	}
	
	public String getType(){
		return this.level;
	}
	
	public String getMessage(){
		return this.message;
	}
}

