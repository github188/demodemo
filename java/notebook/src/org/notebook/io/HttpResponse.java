package org.notebook.io;

import java.util.HashMap;
import java.util.Map;

public class HttpResponse {
	public SimpleHttpClient connection = null;
	private String status = "";
	public int contentLength = -1;
	private int statusCode = 200;
	private byte[] content = null;
	private Map<String, String> cookies = new HashMap<String, String>();
	
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
	
	public String getCookie(String name){
		return this.cookies.get(name);
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
		if(head.startsWith("Set-Cookie:")){
			processCookies(head);
		}
	}
	
	protected void processCookies(String header){
		header = header.replaceAll("Set-Cookie:", "");
		String cookie = header.split(";", 2)[0];
		String term[] = cookie.split("=", 2);
		cookies.put(term[0].trim(), term[1].trim());
	}
	
	public String toString(){
		return this.status + "\n" + this.header;
	}
}
