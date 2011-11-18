package org.http.channel.client;

import javax.servlet.http.Cookie;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.http.channel.proxy.ProxySession;
import org.http.channel.settings.Settings;

public class AuthManager {
	private Log log = LogFactory.getLog("gate");
	public SimpleCache userCache = new SimpleCache();
	private AccountStorage[] storages = null; 
	private Settings settings = null;
	
	public AuthManager(Settings s){
		this.settings = s;
	}
	
	public boolean hasPermession(ProxySession session){
		String sid = this.getAuthSID(session);
		if(sid != null && userCache.get(sid, true) != null){
			return true;
		}else if(sid != null){
			Account a = retriveCookie(sid);
			if(a != null){
				userCache.set(sid, a, 60 * 30);
			}
		}
		return false;
	}
	
	public boolean login(ProxySession session, String username, String password){
		Account user = null;
		for(AccountStorage storage: storages){
			user = storage.authencate(username, password);
			if(user != null)break;
		}
		if(user != null){
			String sid = encodeUser(user);
			session.set_cookies = new Cookie[1];
			session.set_cookies[0] = new Cookie("ACSID", sid);
			session.set_cookies[0].setPath("/");
			session.set_cookies[0].setMaxAge(60 * 60 * 24 * 365);
			userCache.set(sid, user, 60 * 30);
		}
		return user != null;
	}
	
	public void load(){
		storages = new AccountStorage[1];
		storages[0] = new LADPUserStorage(settings);
	}
	
	private Account retriveCookie(String sid){
		return null;
	}
	
	private String encodeUser(Account account){
		return account.username;
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
