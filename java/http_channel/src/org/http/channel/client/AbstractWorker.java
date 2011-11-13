package org.http.channel.client;

import java.io.IOException;
import java.io.InputStream;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.Cookie;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.http.channel.proxy.ProxySession;

public class AbstractWorker {
	protected Log log = LogFactory.getLog("gate");
	protected ProxySession request = null;
	protected URL remote = null;
	public AbstractWorker(ProxySession s, URL remote){
		this.request = s;
		this.remote = remote;
	}
	
	public void uploadResponse(InputStream in, Map<String, List<String>> header, int code) throws IOException{
		log.debug(String.format("Proxy return:%s, sid:%s", this.request.queryURL, this.request.sid));
		
		HTTPForm form = null;
		try {
			form = new HTTPForm(remote.toURI().resolve("/~/reponse").toURL());
		} catch (URISyntaxException e) {
			log.error(e.toString(), e);
			return;
		}
		form.setParameter("sid", this.request.sid);
		form.setParameter("status", code + "");
		
		
		String values = "";
		for(String k: header.keySet()){
			List<String> val = header.get(k);
			values = val.get(0);
			for(int i = 1; i < val.size(); i++){
				values += "," + val.get(i);
			}
			//log.info(String.format("%s->%s", k, values));
			if(k != null){
				form.setParameter(k, values);
			}
		}
		
		if(request.set_cookies != null){
			for(Cookie cookie: request.set_cookies){
				log.info(String.format("Set proxy cookie:%s=%s", cookie.getName(), cookie.getValue()));
				form.setParameter("cookie", String.format("%s;%s;%s;%s;%s", cookie.getName(), cookie.getValue(), cookie.getPath(),
						cookie.getMaxAge(), remote.getHost()));
			}
		}
		
		if(in != null){
			form.startFileStream("file0", "file", in);
		}
		String data = form.read();
		form.close();
		log.debug(String.format("Proxy done:%s, msg:%s", this.request.queryURL, data));
	}
	
	protected void forwardAuthRequest(ProxySession s) throws IOException{
		InputStream in = this.getClass().getClassLoader().getResourceAsStream("org/http/channel/client/redirect_user_login.html");
		Map<String, List<String>> header = new HashMap<String, List<String>>();
		List<String> hv = new ArrayList<String>();
		hv.add("failed");
		header.put("X-proxy-login", hv);		
		uploadResponse(in, header, 200);		
	}
	
	protected void forwardAuthOK(ProxySession s) throws IOException{
		InputStream in = this.getClass().getClassLoader().getResourceAsStream("org/http/channel/client/redirect_login_ok.html");
		Map<String, List<String>> header = new HashMap<String, List<String>>();
		List<String> hv = new ArrayList<String>();
		hv.add("ok");
		header.put("X-proxy-login", hv);
		uploadResponse(in, header, 200);	
	}
}
