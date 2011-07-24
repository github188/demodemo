package org.notebook.io;

public class ClientException extends Exception {
	public static final String AUTH_ERROR = "AUTH_ERROR";
	public ClientException(String error){
		super(error);
	}
}
