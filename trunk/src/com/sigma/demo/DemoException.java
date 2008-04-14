package com.sigma.demo;

public class DemoException extends Exception {
	private static final long serialVersionUID = 1L;

	public DemoException(String message){
		super(message);
	}
	
	public DemoException(String message, Throwable root){
		super(message, root);
	}
}
