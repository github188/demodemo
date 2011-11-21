package com.coci.nsn;

import android.app.Activity;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import com.coci.client.CoCiClient;
import com.coci.provider.AreaCI;

/**
 * 
 * Android进阶：ListView性能优化异步加载图片 使滑动效果流畅
 * http://blog.csdn.net/wanglong0537/article/details/6334005#comments
 * 
 * 
 * 
 * 定义Sina围脖菜单样式
 * http://blog.csdn.net/wanglong0537/article/details/6434728
 * 
 * Network
 * http://www.vogella.de/articles/AndroidNetworking/article.html
 * 
 * Login demo
 * http://androidsamples.blogspot.com/2009/06/how-to-use-http-connection-saxparser.html
 * 
 * @author deon
 */

public class AreaCIActivity extends Activity {
    /** Called when the activity is first created. */
	private CoCiClient client = null;
	private EditText userName = null;
	private EditText password = null;
	private final int WAITING_DIALOG = 1;
	private SharedPreferences settings = null;
	private final String TAG = "areaci";
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        settings = getSharedPreferences(AreaCI.PREFS_NAME, 0);
        String acsid = settings.getString("ACSID", "");
        Intent intent = getIntent(); 
        
        //是否是在启动中.
        boolean isLaunching = intent.getData() == null;
        if(acsid == null || acsid.trim().length() == 0 || 
           !isLaunching //通过菜单进入登录界面。不自动跳过
          ){
            Log.i("areaci", "start to show login dialog");        
            setContentView(R.layout.login_dialog); 
            userName = (EditText)this.findViewById(R.id.username);
            password = (EditText)this.findViewById(R.id.password);
            client = new CoCiClient();
        }else {
        	Log.i("areaci", "Get login session, and show main tab activity");
        	intent = new Intent().setClass(getApplicationContext(), MainTabActivity.class);
    	    this.startActivity(intent);
    	    finish();
        } 
    } 
    
    public void clickLogin(View v){
    	Log.i("areaci", "click login button.");  
    	final String username = userName.getText().toString();
    	final String passwd = password.getText().toString();
    	Log.i("areaci", String.format("login user:%s, password:%s.", username, passwd));
    	
    	Log.i("areaci", "xxxxxxxxxx");
    	if(username.length() >= 4 && passwd.length() > 6){
    		showDialog(WAITING_DIALOG); 
    		login(username, passwd);
    	}else {
			Context context = getApplicationContext();
			CharSequence text = "Please fill the NSN-intra and password.";
			int duration = Toast.LENGTH_LONG;
			Toast toast = Toast.makeText(context, text, duration);
			toast.show();    		
    	}
    	
    	/*
        new Thread(new Runnable() {
            public void run() {
            	login(username, passwd);
            }
        }).start();
        */    	
    	//this.    	
    	//dismissDialog(WAITING_DIALOG);
    }
    
    protected Dialog onCreateDialog(int id) {
    	ProgressDialog dialog = null;
        switch(id) {
        case WAITING_DIALOG:
        	dialog = new ProgressDialog(this);
        	dialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        	dialog.setMessage("Loading. Please wait...");
        	dialog.setIndeterminate(false);
            return dialog;
        default:
            return null;
        }
    }
    
    private void login(String user, String passwd){
    	if(!client.isConnected){
    		if(!connectoToAreaCI()){
    			dismissDialog(WAITING_DIALOG);
    			Context context = getApplicationContext();
    			CharSequence text = "Can't connect to AreaCI server!";
    			int duration = Toast.LENGTH_LONG;

    			Toast toast = Toast.makeText(context, text, duration);
    			toast.show();
    			return;
    		}
    	}
    	
    	if(!client.login(user, passwd)){
    		dismissDialog(WAITING_DIALOG);
			Context context = getApplicationContext();
			CharSequence text = "User name or password error!";
			int duration = Toast.LENGTH_LONG;

			Toast toast = Toast.makeText(context, text, duration);
			toast.show();
			return;
    	}else {
    		SharedPreferences.Editor editor = settings.edit();    		
        	editor.putString("ACSID", client.acsid.getValue());
        	Log.i(TAG, "auth key:" + client.acsid.getValue());
        	editor.commit();
        	dismissDialog(WAITING_DIALOG);
        	Intent intent = new Intent().setClass(getApplicationContext(), MainTabActivity.class);
        	this.startActivity(intent);
    	    finish();
    	}
    }
    
	private synchronized boolean connectoToAreaCI(){
		String[][] connection = DataSyncService.connection;
		for(int i = 0; i < connection.length; i++){
			if(client.connect(connection[i][0], connection[i][1])){
				return true;
			}
		}
		return false;
	}    
}