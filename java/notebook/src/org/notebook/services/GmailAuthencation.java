package org.notebook.services;

import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.io.ClientHttpRequest;
import org.notebook.io.HttpResponse;
import org.notebook.io.SimpleHttpClient;

public class GmailAuthencation {
	private Log log = LogFactory.getLog("GmailAuthencation");
	public String authToken = null;
	public String authSID = null;
	private String logintoken = null;
	private SimpleHttpClient client = null;
	private Map<String, String> errors = new HashMap<String, String>();
	
	public GmailAuthencation(){
		errors.put("CaptchaRequired", "请输入正确的验证码.");
		errors.put("BadAuthentication", "用户名或密码错误.");
		errors.put("AccountDisabled", "用户名禁用.");
	}
	
	public boolean login(String name, char[] passwd, String cpatcha, GmailAuthCallback callback){
		authToken = null;		
		HttpResponse resp = null;
		try {
			if(client == null){
				client = new SimpleHttpClient(new URL("https://www.google.com/"));
			}			
			Map<String, String> param = new HashMap<String, String>();
			param.put("accountType", "HOSTED_OR_GOOGLE");
			param.put("Email", name);
			param.put("Passwd", new String(passwd));
			param.put("service", "ah");
			param.put("source", "deonwu-deonwu84-5");
			if(logintoken != null && cpatcha != null && cpatcha.length() > 0){
				param.put("logintoken", logintoken);
				param.put("logincaptcha", cpatcha);
			}
			resp = client.post("/accounts/ClientLogin", param);
			log.info("Reponse:" + resp.toString() + "\n" + resp.getResponseMessage());
			String[] item = null;
			boolean loginOk = false;
			for(String line: resp.getResponseMessage().split("\n")){
				item = line.split("=", 2);
				if(item[0].equals("Auth")){
					this.authToken = item[1];
					processSubAuth(item[1]);
					loginOk = true;
				}else if(item[0].equals("CaptchaToken")){
					this.logintoken = item[1];
				}else if(item[0].equals("CaptchaUrl")){
					if(callback != null){
						callback.showCaptcha(getCaptchaImage("/accounts/" + item[1]));
					}
				}else if(item[0].equals("Error")){
					if(callback != null){
						callback.error(item[1]);
					}
				}else if(item[0].equals("SID")){
					this.authSID = item[1];					
				}
			}
		} catch (Exception e) {
			log.info(e.toString());
			log.error(e, e);
		}
		return false;
	}
	
	private void processSubAuth2(String token) throws IOException{
		HttpResponse resp = null;
		SimpleHttpClient httpClient = new SimpleHttpClient(new URL("http://deonwu84.appspot.com"));
		
		//URL url = new URL("http://deonwu84.appspot.com/_ah/login?auth=" + token);
		
		log.info("login:/_ah/login?auth=" + token);
		resp = httpClient.get("http://deonwu84.appspot.com/_ah/login?auth=" + token, new HashMap<String, String>());		
		//Object obj = conn.getContent();
		log.info("status:" + resp.getResponseStatus());
		//log.info(":" + resp.toString());
		log.info("Auth Cookie:"); 
	}	
	
	private void processSubAuth(String token) throws IOException{
		/*
		URL url = new URL("http://deonwu84.appspot.com/_ah/login?auth=" + token);
		log.info(url.toString());
		HttpURLConnection conn = (HttpURLConnection)url.openConnection();
		log.info("status:" + conn.getResponseCode());
		log.info("status:" + conn.getResponseMessage());
		//log.info(":" + resp.toString());
		log.info("Auth Cookie:");
		*/ 
		Map<String, String> param = new HashMap<String, String>();
		param.put("auth", token);
		InputStream xx = ClientHttpRequest.post(new URL("http://deonwu84.appspot.com/_ah/login"),
				param);
		BufferedReader reader = new BufferedReader(new InputStreamReader(xx));
		for(String l = reader.readLine(); l != null; l = reader.readLine()){
			log.info("msg:" + l);
		}
	}
	
	public String getErrorMsg(String key){
		String msg = errors.get(key);
		if(msg == null){
			msg = "未知错误";
		}
		return msg;
	}
	
	public void close(){
		try{
			if(this.client != null){
				this.client.close();
			}
		}catch (Exception e) {
			log.info(e.toString());
			log.error(e, e);
		}finally{
			this.client = null;
		}
	}
	
	protected Image getCaptchaImage(String uri) throws IOException{
		HttpResponse resp = null;
		resp = client.get(uri, new HashMap<String, String>());
		ImageIcon imageIcon = new ImageIcon(resp.getContent());
		log.info("Got captcha image:...");
		return imageIcon.getImage().getScaledInstance(200, 70, Image.SCALE_SMOOTH);
	}
	
	public static void main(String[] args){
		GmailAuthencation auth = new GmailAuthencation();
		System.setProperty("https.proxyHost", "10.144.1.10");
		System.setProperty("https.proxyPort", "8080");		
		
		System.setProperty("http.proxyHost", "10.144.1.10");
		System.setProperty("http.proxyPort", "8080");		
		auth.login("deonwu@gmail.com", "123".toCharArray(), null, 
				new GmailAuthCallback(){
					public void showCaptcha(Image image) {
						BufferedImage buffer = new BufferedImage(
								image.getWidth(null),
								image.getHeight(null),
								BufferedImage.TYPE_BYTE_BINARY );
						buffer.getGraphics().drawImage(image, 0, 0, null);
						try{
							String path = "C:/USERS/_work/workspace_pydev/notebook/xx.jpg";
							System.out.println(path);
							ImageIO.write(buffer, "jpg", new File(path));
						}catch(Exception e){
							e.printStackTrace();
						}
					}
					@Override
					public void error(String message) {
						
					}
				}
		);
	}
}
