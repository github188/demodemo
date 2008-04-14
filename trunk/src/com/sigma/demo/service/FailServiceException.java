package com.sigma.demo.service;

public class FailServiceException extends Exception {
	private static final long serialVersionUID = 1223717291205771570L;
	
	public FailServiceException(String message){
		super(message);
	}
	
	public FailServiceException(String message, Throwable root){
		super(message, root);
	}

}
