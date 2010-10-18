package org.notebook.io;

import java.io.IOException;

public class HttpResponse {
	public SimpleHttpClient connection = null;
	private String status = "";
	public int contentLength = -1;
	private int statusCode = 200;
	
	private String message = "";
	private String header = "";
	
	public int getResponseStatus(){
		return this.statusCode;
	}
	
	public String getResponseMessage(){
		return message;
	}
	
	public void setStatus(String status){
		this.status = status;
		this.statusCode = Integer.parseInt(status.split(" ", 3)[1]);
	}
	
	public void addMessage(String resp){
		if(this.message.length() > 0){
			this.message += "\n";
		}
		this.message += resp;		
	}
	
	public void addHeader(String head){
		if(this.header.length() > 0){
			this.header += "\n";
		}		
		this.header += head;
	}
	
	public String toString(){
		return this.status + "\n" + this.header + "\n" + this.message;
	}
	
	public void processHead(String head) throws IOException{
		
	}
}
