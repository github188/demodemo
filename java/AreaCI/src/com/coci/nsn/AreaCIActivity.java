package com.coci.nsn;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

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
 * @author deon
 */

public class AreaCIActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        SharedPreferences settings = getSharedPreferences(AreaCI.PREFS_NAME, 0);
        String acsid = settings.getString("ACSID", "");
        if(acsid == null || acsid.trim().length() == 0){
            Log.i("areaci", "start to show login dialog");        
            setContentView(R.layout.login_dialog);   	
        }else {
        	Log.i("areaci", "Get login session, and show main tab activity");
        	Intent intent = new Intent().setClass(getApplicationContext(), MainTabActivity.class);
    	    this.startService(intent);
    	    finish();
        } 
    } 
    
    public void clickLogin(View v){
    	Log.i("areaci", "click login button.");  
    }
}