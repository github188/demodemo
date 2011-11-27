package org.goku.client;

import java.util.HashMap;
import java.util.Map;

import org.goku.io.GokuClient;
import org.goku.io.GokuResult;

import android.app.Activity;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

public class QueryAlarmActivity extends Activity {
	private final int WAITING_DIALOG = 1;
	private final String TAG = "goku";
	private boolean inWaiting = false;	
	private GokuClient client = null;	
	
	private EditText editBTS = null;
	private Spinner category = null;
	private Spinner status = null;
	
	class Item{
		public String code = "";
		public String label = "";
		public Item(String c, String l){code = c; label = l;};
		public String toString(){return label;};
	}
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.query_alarm_dialog);
        
        client = GokuClient.getInstance();
        editBTS = (EditText)findViewById(R.id.alarm_bts);
        
        category = (Spinner)this.findViewById(R.id.alarm_type);        
        ArrayAdapter <Item> adapter =
        	  new ArrayAdapter <Item> (this, android.R.layout.simple_spinner_item );
        adapter.add(new Item("5001", "图片告警"));
        adapter.add(new Item("1001", "外部报警"));
        adapter.add(new Item("1003", "动态检测"));
        
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        category.setAdapter(adapter);

        status = (Spinner)this.findViewById(R.id.alarm_status);        
        adapter =
      	  new ArrayAdapter <Item> (this, android.R.layout.simple_spinner_item );
        adapter.add(new Item("3", "已确认"));
        adapter.add(new Item("2", "自动确认"));
        adapter.add(new Item("1", "未确认"));       
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        status.setAdapter(adapter);         
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

    public void clickQuery(View v){
    	Log.i(TAG, "query");
    	String uuid = editBTS.getEditableText().toString();
    	String cate = ((Item)category.getSelectedItem()).code;
    	String st = ((Item)status.getSelectedItem()).code;
    	Log.i(TAG, "query:" + uuid + ", cate:" + cate + ", st:" + st);    	
		if(uuid.length() > 0){
			showDialog(WAITING_DIALOG);
			inWaiting = true;
	    	final Map<String, String> param = new HashMap<String, String>();
	    	param.put("type", cate);
	    	param.put("status", st);
	    	param.put("uuid", uuid);			
			new Thread(){
				public void run(){
					GokuResult r = client.queryAlaram(param);
					if(r.count > 0){
						showQueryAlarm(r);
					}else if(r.status != 0){
						showMessage(r.getMessage());
					}else {
						showMessage("没有找到符合条件的告警");
					}
				};
			}.start(); 
		}else {
			showMessage("请输入需要查询的基站编号.");
		}
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
    
	private void showQueryAlarm(final GokuResult r){
		Log.d(TAG, "query alarm result count:" + r.count);
		runOnUiThread(new Runnable(){
			public void run(){
				dismissDialog(WAITING_DIALOG);
				inWaiting = false;				
		    	Intent intent = new Intent().setClass(getApplicationContext(), QueryAlarmListActivity.class);
		    	startActivity(intent);
			}
			});
	}
}
