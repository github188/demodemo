package org.notebook.io;

import java.io.IOException;

public class HttpResponse {
	public SimpleHttpClient connection = null;
	private String status = "";
	public int contentLength = -1;
	private int statusCode = 200;
	private byte[] content = null;
	
	private String header = "";
	
	public int getResponseStatus(){
		return this.statusCode;
	}
	
	public String getResponseMessage(){
		return new String(content);
	}
	
	public byte[] getContent(){
		return content;
	}
	
	public void setContent(byte[] content){
		this.content = content;
	}
	
	public void setStatus(String status){
		this.status = status;
		this.statusCode = Integer.parseInt(status.split(" ", 3)[1]);
	}
	
	
	public void addHeader(String head){
		if(this.header.length() > 0){
			this.header += "\n";
		}		
		this.header += head;
	}
	
	public String toString(){
		return this.status + "\n" + this.header;
	}
}
