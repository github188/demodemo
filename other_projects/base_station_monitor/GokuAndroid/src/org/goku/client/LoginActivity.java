package org.goku.client;

import org.goku.io.GoKu;
import org.goku.io.GokuClient;
import org.goku.io.GokuResult;

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
import android.widget.TextView.BufferType;
import android.widget.Toast;

public class LoginActivity extends Activity {
	private EditText userName = null;
	private EditText password = null;
	private EditText server = null;
	
	private SharedPreferences settings = null;
	private final int WAITING_DIALOG = 1;
	private final String TAG = "goku";
	private boolean inWaiting = false;
	
	private GokuClient client = null;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        settings = getSharedPreferences(GoKu.PREFS_NAME, 0);
        //String acsid = settings.getString("ACSID", "");
        Intent intent = getIntent(); 

        setContentView(R.layout.login_dialog); 
        userName = (EditText)this.findViewById(R.id.username);
        password = (EditText)this.findViewById(R.id.password);    
        server = (EditText)this.findViewById(R.id.server_ip);    
        
        userName.setText(settings.getString(GoKu.PREFS_USERNAME, ""), BufferType.NORMAL);
        password.setText(settings.getString(GoKu.PREFS_PASSWORD, ""), BufferType.NORMAL);
        server.setText(settings.getString(GoKu.PREFS_SERVER, ""), BufferType.NORMAL);  
        client = GokuClient.getInstance();
    } 
    
    public void clickLogin(View v){
    	Log.i("areaci", "click login button.");  
    	final String username = userName.getText().toString();
    	final String passwd = password.getText().toString();
    	final String serverIp = server.getText().toString();
    	
    	Log.i("areaci", String.format("login user:%s, password:%s.", username, passwd));
    	
    	if(username.length() >= 2 && passwd.length() > 2){
    		login(username, passwd, serverIp);
    	}else {
    		showMessage("请输入用户名和密码.");
    	}
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
    
    private void login(final String user, final String passwd, final String server){
		showDialog(WAITING_DIALOG);		
		inWaiting = true;
		new Thread(){
			public void run(){
				GokuResult r = client.connect(server);
				if(r.message != null && r.message.equals("OK")){
					r = client.login(user, passwd);
					if(r.status == 0){
						runOnUiThread(
							new Runnable(){
								public void run(){
									loginOK();
								}
							}
						);
					}else {
						showMessage(r.getMessage());
					}
				}else {
					showMessage("服务器连接失败");
				}
			};
		}.start();
		
    }
    
	private void showMessage(final String message){
		runOnUiThread(
				new Runnable(){
					public void run(){
						if(inWaiting){
							dismissDialog(WAITING_DIALOG);
							inWaiting = false;
						}
						Context context = getApplicationContext();
						CharSequence text = message;
						int duration = Toast.LENGTH_LONG;
						Toast toast = Toast.makeText(context, text, duration);
						toast.show(); 
					}
				}
		);
	}
	
	private void loginOK(){
		if(inWaiting){
			dismissDialog(WAITING_DIALOG);
			inWaiting = false;
		}
		
    	SharedPreferences.Editor editor = settings.edit();
    	editor.putString(GoKu.PREFS_USERNAME, userName.getText().toString());
    	editor.putString(GoKu.PREFS_PASSWORD, password.getText().toString());
    	editor.putString(GoKu.PREFS_SERVER, server.getText().toString());    	
    	editor.commit();
		
    	Intent intent = new Intent().setClass(getApplicationContext(), MainTabActivity.class);
    	this.startActivity(intent);
	    finish();		
	}

}
