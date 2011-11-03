package com.coci.client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URI;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

import android.content.ContentValues;
import android.util.Log;

public class CoCiClient {
	private final static String TAG = "areaci.http";
	private HttpClient client = null;
	private URI endpoint = null;
	
	
	public CoCiClient(URI server){
		 client = new DefaultHttpClient();
		 endpoint = server;
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
							data.add(jsonToContentValues(_tmp.getJSONObject(i)));
						}
					}
				}
			}
		}catch(JSONException e){
			Log.e(TAG, e.toString());
		}
		return data;
	}
	
	private ContentValues jsonToContentValues(JSONObject json){
		ContentValues v = new ContentValues();
		for(Iterator<String> iter = json.keys(); iter.hasNext(); ){
			String key = iter.next();
			try {
				v.put(key, json.getString(key));
			} catch (JSONException e) {
				Log.e(TAG, e.toString());
			}
		}
		return v;
	}
	
	private Object getRPCData(String api, Map<String, String> param){
		HttpPost request = new HttpPost(endpoint.resolve(api));
		HttpResponse response = null;
		BufferedReader in = null;
		JSONObject result = null;
		
        List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>(param.size());
        for(Entry<String, String> item : param.entrySet()){          	
        	nameValuePairs.add(new BasicNameValuePair(item.getKey(), item.getValue()));
        }
        try {
        	request.setEntity(new UrlEncodedFormEntity(nameValuePairs));
        	response = client.execute(request);
        	if(response != null){
                in = new BufferedReader(new InputStreamReader(response.getEntity().getContent()));
                StringBuffer sb = new StringBuffer("");
                String line = "";
                String NL = System.getProperty("line.separator");
                while ((line = in.readLine()) != null) {
                    sb.append(line + NL);
                }
                in.close();
                result = (JSONObject) new JSONTokener(sb.toString()).nextValue();
        	}
		} catch (Exception e) {
			Log.e(TAG, "error:" + e.toString());
		} finally{
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
