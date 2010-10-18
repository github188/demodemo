package org.notebook.services;

import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.io.HttpResponse;
import org.notebook.io.SimpleHttpClient;

public class GmailAuthencation {
	private static Log log = LogFactory.getLog(GmailAuthencation.class);
	public String authToken = null;
	private String logintoken = null;	
	
	public boolean login(String name, char[] passwd, String cpatcha, GmailAuthCallback callback){
		URL loginUrl = null;
		//HttpURLConnection connection = null;
		//OutputStream os = null;
		//InputStream is = null;
		HttpResponse resp = null;
		try {
			loginUrl = new URL("https://www.google.com/accounts/ClientLogin");
			
			Map<String, String> param = new HashMap<String, String>();
			param.put("accountType", "HOSTED_OR_GOOGLE");
			param.put("Email", name);
			param.put("Passwd", new String(passwd));
			param.put("service", "apps");
			param.put("source", "deonwu-deonwu84-5");
			if(logintoken != null){
				param.put("logintoken", logintoken);
				param.put("logincaptcha", cpatcha);
			}			
			resp = SimpleHttpClient.post(loginUrl, param);
			log.info("Reponse:" + resp.toString());
			String[] item = null;
			for(String line: resp.getResponseMessage().split("\n")){
				item = line.split("=", 2);
				log.info(item[0]);
				if(item[0].equals("Auth")){
					log.info("OK");
					this.authToken = item[1];
					return true;
				}else if(item[0].equals("CaptchaToken")){
					this.logintoken = item[1];
				}else if(item[0].equals("CaptchaUrl")){
					if(callback != null){
						callback.showCaptcha(new URL("https://www.google.com/accounts/" + item[1]));
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
	
	public static void main(String[] args){
		GmailAuthencation auth = new GmailAuthencation();
		log.info("start.....");
		System.setProperty("https.proxyHost", "10.144.1.10");
		System.setProperty("https.proxyPort", "8080");		
		
		System.setProperty("http.proxyHost", "10.144.1.10");
		System.setProperty("http.proxyPort", "8080");		
		auth.login("deonwu@gmail.com", "123".toCharArray(), "", null);
		log.info("done");
	}
}
