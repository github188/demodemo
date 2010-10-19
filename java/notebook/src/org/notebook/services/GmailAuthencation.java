package org.notebook.services;

import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.io.HttpResponse;
import org.notebook.io.SimpleHttpClient;

public class GmailAuthencation {
	private static Log log = LogFactory.getLog(GmailAuthencation.class);
	public String authToken = null;
	private String logintoken = null;
	private SimpleHttpClient client = null;
	
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
			param.put("service", "apps");
			param.put("source", "deonwu-deonwu84-5");
			if(logintoken != null && cpatcha != null && cpatcha.length() > 0){
				param.put("logintoken", logintoken);
				param.put("logincaptcha", cpatcha);
			}
			resp = client.post("/accounts/ClientLogin", param);
			log.info("Reponse:" + resp.toString() + "\n" + resp.getResponseMessage());
			String[] item = null;
			for(String line: resp.getResponseMessage().split("\n")){
				item = line.split("=", 2);
				log.info(item[0]);
				if(item[0].equals("Auth")){
					this.authToken = item[1];
					this.close();
					return true;
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
				}
			}
		} catch (Exception e) {
			log.info(e.toString());
			log.error(e, e);
		}
		return false;
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
		return imageIcon.getImage();
	}
	
	public static void main(String[] args){
		GmailAuthencation auth = new GmailAuthencation();
		log.info("start.....");
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
		log.info("done");
	}
}
