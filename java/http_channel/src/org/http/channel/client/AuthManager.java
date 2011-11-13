package org.http.channel.client;

import javax.servlet.http.Cookie;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.http.channel.proxy.ProxySession;

public class AuthManager {
	private Log log = LogFactory.getLog("gate");
	public boolean hasPermession(ProxySession session){
		String sid = this.getAuthSID(session);
		log.info("Current sid:" + sid);		
		return sid != null && sid.length() > 1;
	}
	
	public boolean login(ProxySession session, String username, String password){
		session.set_cookies = new Cookie[1];
		session.set_cookies[0] = new Cookie("ACSID", "12345678");
		session.set_cookies[0].setPath("/");
		session.set_cookies[0].setMaxAge(60 * 60 * 24 * 365);
		return true;
	}
	
	private String getAuthSID(ProxySession session){
		String value = session.header.get("Cookie");	
		return this.parseCookie(value, "ACSID");
	}
	
	private String parseCookie(String cookie, String key){		
		if(cookie == null)return null;
		int start = cookie.indexOf(key);
		int end = Math.max(cookie.indexOf(";", start), cookie.length());
		
		return cookie.substring(start + key.length() + 1, end);
	}
}
