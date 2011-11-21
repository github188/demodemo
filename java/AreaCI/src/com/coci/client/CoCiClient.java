package com.coci.client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ConnectException;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.http.Header;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.conn.ConnectTimeoutException;
import org.apache.http.cookie.Cookie;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.impl.cookie.BasicClientCookie;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

import android.content.ContentValues;
import android.util.Log;

import com.coci.provider.AreaCI;
import com.coci.provider.AreaCI.TaskInfo;

public class CoCiClient {
	private final static String TAG = "areaci.http";
	private DefaultHttpClient client = null;
	private URI endpoint = null;
	public boolean isConnected = false;
	public boolean isLoginOk = true;
	public BasicClientCookie acsid = null;
	
	public CoCiClient(){
		 client = new DefaultHttpClient();
		 acsid = new BasicClientCookie("ACSID", "");
		 acsid.setExpiryDate(new Date(System.currentTimeMillis() + 365 * 1000 * 60 * 60 * 24));
	}
	
	public boolean connect(URI server){
		this.endpoint = server;
		return true;
	}
	
	//更新会话Cookie
	public void updateAuthSID(String sid){
		this.acsid.setValue(sid);
		client.getCookieStore().addCookie(acsid);
	}
	
	public boolean connect(String endpoint, String proxy){
		try {
			this.endpoint = new URI(endpoint);
			acsid.setDomain(this.endpoint.getHost());
			acsid.setPath("/");
			client.getCookieStore().addCookie(acsid);
			//The connection is OK if get status info from AreaCI.
			getRPCData("status_info", null);
		} catch (URISyntaxException e) {
			Log.e(TAG, e.toString());
		}
		
		return isConnected;
	}
	
	public boolean login(String username, String password){
		Map<String, String> param = new HashMap<String, String>();		
		this.isLoginOk = false;
		param.put("username", username);
		param.put("password", password);		
		getRPCData("/~/user_login", param);
		if(isLoginOk){
			for(Cookie c:client.getCookieStore().getCookies()){
				if(!c.getName().equals("ACSID")){
					continue;
				}else {
					this.acsid.setValue(c.getValue());
					Log.d(TAG, "ACSID:" + this.acsid.getValue());
					break;
				}
			}
		}
		return isLoginOk;
	}
	
	public List<ContentValues> updatedTask(long lastTime, long limit) throws JSONException{		
		List<ContentValues> data = null;
		Map<String, String> param = new HashMap<String, String>();
		param.put("last_time", lastTime + "");
		param.put("limit", limit + "");	
		Object obj = getRPCData("updated_task", param);
		try{
			if(obj != null){
				JSONObject json = (JSONObject)obj;
				if(json.getString("status").equals("ok")){
					JSONArray _tmp = json.getJSONArray("data");
					if(_tmp != null){
						data = new ArrayList<ContentValues>(_tmp.length());
						for(int i = 0; i < _tmp.length(); i++){
							data.add(jsonToContentValues(_tmp.getJSONObject(i),
									AreaCI.DB_COLUMNS.get(TaskInfo.DB_TABLE_NAME)));
						}
					}
				}
			}
		}catch(JSONException e){
			Log.e(TAG, e.toString());
		}
		return data;
	}
	
	private ContentValues jsonToContentValues(JSONObject json, Set<String> fields){
		ContentValues v = new ContentValues();
		for(Iterator<String> iter = json.keys(); iter.hasNext(); ){
			String key = iter.next();
			if(!fields.contains(key))continue;
			try {
				v.put(key, json.getString(key));
			} catch (JSONException e) {
				Log.e(TAG, e.toString());
			}
		}
		return v;
	}
	
	private void saveAccessKey(String key){
		//this.accessKey = sessionHeader.getValue();
	}
	
	private Object getRPCData(String api, Map<String, String> param){
		HttpResponse response = null;
		BufferedReader in = null;
		JSONObject result = null;
		List<NameValuePair> nameValuePairs = null;
		Header authHeader = null;
		
		HttpPost request = new HttpPost(endpoint.resolve(api));
        
        StringBuffer query = new StringBuffer(endpoint.resolve(api).toString() + "?");        
        if(param != null){
        	nameValuePairs = new ArrayList<NameValuePair>(param.size());
	        for(Entry<String, String> item : param.entrySet()){
	        	query.append("&" + item.getKey() + "=" + item.getValue());
	        	nameValuePairs.add(new BasicNameValuePair(item.getKey(), item.getValue()));
	        }
        }
        Log.d(TAG, "Request:" + query.toString());
        try {
        	//request.
        	if(nameValuePairs != null){
        		request.setEntity(new UrlEncodedFormEntity(nameValuePairs));
        	}
        	response = client.execute(request);
        	if(response != null){
            	authHeader = response.getFirstHeader("X-proxy-login");
            	if(authHeader != null){
            		this.isLoginOk =  authHeader.getValue().equals("ok");
            	}else {
            		this.isLoginOk = true;
	                in = new BufferedReader(new InputStreamReader(response.getEntity().getContent()));
	                StringBuffer sb = new StringBuffer("");
	                String line = "";
	                String NL = System.getProperty("line.separator");
	                while ((line = in.readLine()) != null) {
	                    sb.append(line + NL);
	                }
	                in.close();
	                Object o = new JSONTokener(sb.toString()).nextValue();
	                if(o instanceof JSONObject){
	                	result = (JSONObject)o; 
	                }else {
	                	Log.d(TAG, o.toString());
	                }
            	}
        	}
		} catch (ConnectException e) {
			//this.isConnected = false;
			Log.e(TAG, "error:" + e.toString() + ", url:" + query.toString());
		} catch(ConnectTimeoutException e){
			Log.e(TAG, "error:" + e.toString() + ", url:" + query.toString());
		}catch (Exception e) {
			Log.e(TAG, "error:" + e.toString(), e);
		} finally{
			isConnected = authHeader != null || result != null;
			if(in != null){
				try {
					in.close();
				} catch (IOException e) {
					Log.e(TAG, "error:" + e.toString());
				}
			}
		}
		
		return result;
	} 
}
