package org.goku.io;

import java.net.ConnectException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.http.Header;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.conn.ConnectTimeoutException;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

import android.util.Log;

public class GokuClient {
	private final static String TAG = "areaci.http";
	private static GokuClient ins = null;

	private String server = null;
	private DefaultHttpClient client = null;
	public String sid = null;
	public String encoding = "utf8";
	public boolean isConnected = false;
	public boolean isLoginOk = true;
	
	public List<BaseStation> btsList = new ArrayList<BaseStation>();
	public List<AlarmRecord> realTimeAlram = new ArrayList<AlarmRecord>();
	public List<AlarmRecord> queryAlram = new ArrayList<AlarmRecord>();
	
	public static synchronized GokuClient getInstance(){
		if(ins == null){
			ins = new GokuClient();
		}
		return ins;
	}
	
	public GokuClient(){
		client = new DefaultHttpClient();
	}
	
	public GokuResult connect(String server){
		this.server = "http://" + server;
		return ping();
	}
	
	public GokuResult login(String user, String password){
		GokuResult r = new GokuResult();
		Map<String, String> param = new HashMap<String, String>();
		param.put("user", user);
		param.put("password", password);
		this.getRPCData("login", param, LoginHandler, r);	
		
		btsList.clear();
		realTimeAlram.clear();
		queryAlram.clear();
		return r;
	}

	public GokuResult listBTS(){
		GokuResult r = new GokuResult();
		Map<String, String> param = new HashMap<String, String>();
		this.getRPCData("list_bs_tree", param, BTSHandler, r);
		if(r.btsList != null){
			this.btsList = r.btsList;
		}
		return r;
	}
	
	public GokuResult realTimeAlarm(){
		GokuResult r = new GokuResult();
		Map<String, String> param = new HashMap<String, String>();
		param.put("c", "1");
		this.getRPCData("list_al", param, AlarmHandler, r);
		
		if(r.alarmList != null){
			for(AlarmRecord alarm: r.alarmList){
				if(alarm.status == 3){
					this.realTimeAlram.remove(alarm);
				}else {
					this.realTimeAlram.add(alarm);
				}
			}
		}
		
		return r;
	}
	
	public GokuResult ping(){
		GokuResult r = new GokuResult();
		Map<String, String> param = new HashMap<String, String>();
		this.getRPCData("ping", param, PingHandler, r);	
		return r;
	}
	
	private Object getRPCData(String api, Map<String, String> param, ResultHandler h, GokuResult r){
		HttpResponse response = null;
		Object result = null;
		List<NameValuePair> nameValuePairs = null;
		Header authHeader = null;
		
		HttpPost request = new HttpPost(server);
        
        StringBuffer query = new StringBuffer(); 
        if(param == null) param = new HashMap<String, String>();
    	if(sid != null){
    		param.put("sid", sid);
    	}
    	param.put("en", encoding);
    	param.put("q", api); 
    	nameValuePairs = new ArrayList<NameValuePair>(param.size());
        for(Entry<String, String> item : param.entrySet()){
        	query.append("&" + item.getKey() + "=" + item.getValue());
        	nameValuePairs.add(new BasicNameValuePair(item.getKey(), item.getValue()));
        }
        Log.d(TAG, "Request:" + query.toString());
        try {
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
            		if(h != null){
            			h.processInputStream(response.getEntity().getContent(), r);
            		}
            	}
        	}
		} catch (ConnectException e) {
			Log.e(TAG, "error:" + e.toString() + ", url:" + query.toString());
		} catch(ConnectTimeoutException e){
			Log.e(TAG, "error:" + e.toString() + ", url:" + query.toString());
		}catch (Exception e) {
			Log.e(TAG, "error:" + e.toString(), e);
		} finally{
			isConnected = authHeader != null || result != null;
		}
		
		return result;
	}
	
	protected ResultHandler LoginHandler = new ResultHandler(){
		protected void extraHeader(String l, GokuResult r){
			sid = l.trim();
		}
	};
	
	protected ResultHandler PingHandler = new ResultHandler(){
		protected void processUnkownData(String l, GokuResult r){
			if(l.trim().equals("OK")){
				r.status = 0;
				r.message = "OK";
			}
		}
	};
	
	protected ResultHandler BTSHandler = new ResultHandler(){
		protected void extraHeader(String l, GokuResult r){
			r.count = Integer.parseInt(l.trim().split("\\$")[0]);
		}
		protected void processBody(String l, GokuResult r){
			BaseStation bs = new BaseStation();			
			String[] data = l.split("\\$");
			switch(data.length){
				case 7: bs.setChannels(data[6]);
				case 6: bs.name = data[5];
				case 5: bs.parent = data[4];
				case 4: bs.status = data[3];
				case 3: bs.routeServer = data[2];
				case 2: bs.devType = data[1];
				case 1: bs.uuid = data[0];
			}
			r.addBaseStation(bs);
		}
	};	
	
	protected ResultHandler AlarmHandler = new ResultHandler(){
		protected void extraHeader(String l, GokuResult r){
			String[] data = l.split("\\$");
			switch(data.length){
				case 3: r.session = data[2];
				case 2: r.count = Integer.parseInt(data[1]);
				case 1: r.totalCount = Integer.parseInt(data[0]);
			}
		}
		protected void processBody(String l, GokuResult r){
			AlarmRecord al = new AlarmRecord();			
			String[] data = l.split("\\$");
			switch(data.length){
				case 9: al.devType = data[8];
				case 8: al.category = data[7];
				case 7: al.endTime = data[6];
				case 6: al.startTime = data[5];
				case 5: al.level = data[4];
				case 4: al.status = Integer.parseInt(data[3]);
				case 3: al.btsID = data[2];
				case 2: al.code = data[1];
				case 1: al.uuid = data[0];
			}
			r.addAlarmRecord(al);
		}
	};		
}
