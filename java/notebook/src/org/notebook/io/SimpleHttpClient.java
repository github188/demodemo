package org.notebook.io;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.net.Socket;
import java.net.URL;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.Map;

import javax.net.ssl.SSLSocketFactory;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class SimpleHttpClient {
	private static Log log = LogFactory.getLog(SimpleHttpClient.class);
	private boolean hasProxy = false;
	private URL requestURL = null;
	private Socket socket = null;
	
	private HttpResponse response = null;
	
	private Map<String, String> header = new HashMap<String, String>();
	private byte[] body = null;
	
	private ByteArrayOutputStream buffer = new ByteArrayOutputStream(1024);
	private PrintWriter out = new PrintWriter(buffer);
	private InputStream in = null;
	//public static
	public static HttpResponse post(URL url, Map<String, String> param) throws IOException{
		return new SimpleHttpClient(url).post(param, new HashMap<String, String>());
	}
	
	public static HttpResponse post(URL url, Map<String, String> param, Map<String, String> head) throws IOException{
		return new SimpleHttpClient(url).post(param, head);
	}
	
	public HttpResponse get(String uri, Map<String, String> param) throws IOException{
		this.createSocket();
		this.createHttpRequest("GET");
		this.commit();
		this.processResponse();
		//this.close();
		return this.response;
	}
	
	public HttpResponse post(String uri, Map<String, String> param) throws IOException{
		this.createSocket();
		this.createHttpRequest("GET");
		this.commit();
		this.processResponse();
		//this.close();
		return this.response;
	}	
	
	private HttpResponse post(Map<String, String> param, Map<String, String> head) throws IOException{
		this.createSocket();
		this.createHttpRequest("POST");
		this.setHeader(head);
		this.buildBody(param);
		this.commit();	
		this.processResponse();
		//this.close();
		return this.response;
	}
	
	private void processResponse() throws IOException{
		response = new HttpResponse();
		response.connection = this;
		
		StringBuffer buffer = new StringBuffer();
		for(int data = in.read(); data >= 0; data= in.read()){
			if(data == '\n'){
				String header = buffer.toString();
				response.processHead(header);
			}else {
				buffer.append((char)data);
			}
		}

	}
	
	private SimpleHttpClient(URL url){
		this.requestURL = url;
		this.hasProxy = System.getProperty("http.proxyHost", null) != null;
	}
	
	private void createHttpRequest(String method){
		String requestLine = method;
		requestLine += " " + this.requestURL.getPath();
		requestLine += " HTTP/1.0";
		out.println(requestLine);
		out.println("Host: " + this.requestURL.getHost());
		//out.println("User-Agent: NoteBook/1.0 java1.6 client");
		out.println("User-Agent: Mozilla/5.0 (Windows;en-GB; rv:1.8.0.11) Gecko/20070312 Firefox/1.5.0.11");
		out.println("Accept: text/html;q=0.9,text/plain;q=0.8,*/*;q=0.5");
		out.println("Accept-Language: en-gb,en;q=0.5");
		out.println("Keep-Alive: 300");
		out.println("Connection: keep-alive");
		out.println("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7");
	}
	
	private void commit() throws IOException{
		out.println("Content-Type: application/x-www-form-urlencoded");
		out.println("Content-Length: " + this.body.length);
		out.println("");
		out.flush();
		buffer.write(this.body);
		out.close();
		log.info("commit http request...");
		log.info(buffer.toString());
		this.socket.getOutputStream().write(buffer.toByteArray());
		this.socket.getOutputStream().flush();
	}

	private void buildBody(Map<String, String> param) throws IOException{
		//this.header.putAll(head);
		String body = "";
		for(String name: param.keySet()){
			try {
				if(body.length() > 0) body += "&";
				body += name + "=" + URLEncoder.encode(param.get(name), "UTF-8");
			} catch (UnsupportedEncodingException e) {
				throw new IOException("encoding error, param key " + name);
			}
		}
		this.body = body.getBytes();
	}
	
	
	private void setHeader(Map<String, String> head){
		this.header.putAll(head);
	}
	//
	
	private void createSocket() throws IOException{
		if(this.socket == null || !this.socket.isConnected()){
			if(requestURL.getProtocol().equals("https")){
				this.createHTTPSSocket();
			}else {
				socket = new Socket(requestURL.getHost(), requestURL.getPort());
			}
			in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		}else {
			buffer = new ByteArrayOutputStream(1024);
		}
	}
	
	private Socket connectionToProxy() throws IOException{
		String host = System.getProperty("http.proxyHost");
		String proxyPort = System.getProperty("http.proxyPort", "80");
		Socket proxySocket = new Socket(host, Integer.parseInt(proxyPort));
		byte[] command = ("CONNECT "+ requestURL.getHost() +":443 HTTP/1.0\n\n").getBytes();
		log.info(new String(command));
		proxySocket.getOutputStream().write(command);
		proxySocket.getOutputStream().flush();
		
		BufferedReader proxyReader = new BufferedReader(new InputStreamReader(proxySocket.getInputStream()));
		for(int i = 0; i < 5; i++){
			log.info("proxy:" + proxyReader.readLine());
		}	
		return proxySocket;
	}
	
	private void createHTTPSSocket() throws IOException{
		int port = requestURL.getPort();
		port = port > 0 ? port : 443;
		
		log.info("connect to https:" + requestURL.getHost() + ", port:" + port);
		SSLSocketFactory sslsf = (SSLSocketFactory)SSLSocketFactory.getDefault();
		if(hasProxy){
			Socket proxySocket = connectionToProxy();
			socket = sslsf.createSocket(proxySocket, requestURL.getHost(), port, true);
		}else {
			socket = sslsf.createSocket(requestURL.getHost(), port);
		}
	}
		
	private void close() throws IOException{
		if(this.out != null){this.out.close();};
		if(this.in != null){this.in.close();};
		if(this.socket != null){this.socket.close();};
	};
	
	
	
}
