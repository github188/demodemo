package org.notebook.services;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class GmailAuthencation {
	private static Log log = LogFactory.getLog(GmailAuthencation.class);
	public String authToken = null;
	private String logintoken = null;	
	
	public boolean login(String name, char[] passwd, String cpatcha, GmailAuthCallback callback){
		URL loginUrl = null;
		HttpURLConnection connection = null;
		OutputStream os = null;
		InputStream is = null;
		try {
			loginUrl = new URL("https://www.google.com/accounts/ClientLogin");
			connection = (HttpURLConnection)loginUrl.openConnection();
			connection.setDoOutput(true);
			connection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
			
			String postData = "accountType=HOSTED_OR_GOOGLE";
			postData += "&Email=" + name;
			postData += "&Passwd=" + new String(passwd);
			postData += "&service=apps";
			postData += "&source=deonwu-deonwu84-5";
			if(logintoken != null){
				postData += "&logintoken=" + logintoken;
				postData += "&logincaptcha=" + cpatcha;
			}
			os = connection.getOutputStream();
			os.write(postData.getBytes());
			os.close();
			connection.connect();
			//sun.net.www.protocol.https.HttpsURLConnectionImpl sx = null;
			//Object o = connection.getContent();
			//log.info("x:" + o.getClass());
			int status = connection.getResponseCode();
			log.info("response code:" + status);
			log.info("msg:" + connection.getResponseMessage());
			int length = connection.getContentLength();
			log.info("response length:" + length);
			log.info("response getContentType:" + connection.getContentType());
			//log.info("response getContent:" + connection.getContent(new Class[]{java.lang.String.class}));
			
			is = connection.getInputStream();
			BufferedReader reader = new BufferedReader(new InputStreamReader(is));
			
			String line = reader.readLine();
			String[] item = null;
			while(line != null){
				log.info(line);
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
				}
				line = reader.readLine();
			}			
		} catch (Exception e) {
			log.info(e.toString());
			log.error(e, e);
		}
		return false;
	}	
}
