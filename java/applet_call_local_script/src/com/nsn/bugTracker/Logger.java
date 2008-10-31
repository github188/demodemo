package com.nsn.bugTracker;

public class Logger {
	public void debug(String msg){
		_msg(msg);
	}
	
	public void info(String msg){
		_msg(msg);
	}
	
	public void fatal(String msg){
		_msg(msg);
	}	
	
	public boolean isDebugEnabled(){
		return true;
	}
	
	public void error(String msg){
		_msg(msg);
	}	
	public void error(String msg, Throwable e){
		_msg(msg);
		e.printStackTrace();
	}	
	
	private void _msg(String msg){
		
		System.out.println(msg);		
	}
}
