package com.sigma.demo.event;

import com.sigma.demo.DemoEvent;

public class DMCallEvent extends DemoEvent {
	private String command = null;
	private String args = null;
	
	public DMCallEvent(String msg){
		int index = msg.indexOf(" ");
		command = msg.substring(0,index);
		args = msg.substring(index + 1);
	}
	
	public String getCommand(){
		return this.command;		
	}
	
	public String getArgs(){
		return this.args;
	}
}
