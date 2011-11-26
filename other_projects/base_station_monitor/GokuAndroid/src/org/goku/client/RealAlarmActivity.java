package org.goku.client;

import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import org.goku.io.AlarmRecord;
import org.goku.io.GokuClient;
import org.goku.io.GokuResult;

import android.app.Activity;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class RealAlarmActivity extends Activity {
	private static final String TAG = "bts";
	private GokuClient goku = null;
	private Handler handler = null;
	private AlarmListAdapter adapter = null;
	private Timer timer = new Timer();
	private boolean inWaiting = false;
	private boolean isActive = true;
	private final int WAITING_DIALOG = 1;	

	private GokuClient client = null;
	private final int realAlarm = 1;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);        
        setContentView(R.layout.real_alarm_list);
        
        TextView title = (TextView) findViewById(R.id.last_synced);
        title.setText("实时告警列表");
        
        ListView lv = (ListView) findViewById(R.id.task_list);
        //BTSListAdapter.
        goku = GokuClient.getInstance();
        adapter = new AlarmListAdapter(goku.realTimeAlram);
        lv.setAdapter(adapter);         
        
        /**
         * 定时更新告警数据。
         */
        HandlerThread ht = new HandlerThread("real_alarm");
        ht.start();
        handler = new Handler(ht.getLooper()){
            public void handleMessage(Message msg){
            	if(msg.what == realAlarm){
                	final GokuResult r = goku.realTimeAlarm();
                	if(r.count > 0){
                		updateAlarmList();
                	}else {
                		showMessage(r.getMessage());
                	}
                }
            }
        };        
    } 
    
    @Override
    protected void onStart() {
        super.onStart();
        Log.d(TAG, "start....");
        isActive = true;
        timer.scheduleAtFixedRate(new TimerTask (){
			@Override
			public void run() {
				Message msg = handler.obtainMessage(realAlarm);
				handler.sendMessage(msg);
			}        	
        }, 100, 60 * 1000);
    }
    
    protected void onResume() {
    	super.onResume();
    	isActive = true;
    	Log.d(TAG, "resume....");
    	updateAlarmList();
    }    

    protected void onPause() {
    	super.onPause();
    	Log.d(TAG, "pause....");
    	isActive = false;
    }
    
    protected void onStop() {
    	super.onStop();
    	Log.d(TAG, "stop....");
    	isActive = false;
    }
    
    protected void onDestroy() {
    	super.onDestroy();
    	timer.cancel();
    }
    
    private void updateAlarmList(){
    	if(!isActive) return;
		runOnUiThread(
			new Runnable(){
				public void run(){
					/*
					adapter.clear();
					for(AlarmRecord alarm: goku.realTimeAlram){
						adapter.add(alarm);
					}*/
					adapter.notifyDataSetChanged();
				}
			}
    	);
    }
    
    protected Dialog onCreateDialog(int id) {
    	ProgressDialog dialog = null;
        switch(id) {
        case WAITING_DIALOG:
        	dialog = new ProgressDialog(this);
        	dialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        	dialog.setMessage("加载中. 请稍等...");
        	dialog.setIndeterminate(false);
            return dialog;
        default:
            return null;
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
    
    class ViewWrapper{
        View base;
        TextView name = null;
        TextView status = null;
        
        ViewWrapper(View base){
            this.base = base;
        }
        
        TextView getName(){
            if(name == null)
                name = (TextView)base.findViewById(R.id.name);
            return name;
        }

        TextView getStatus(){
            if(status == null)
            	status = (TextView)base.findViewById(R.id.status);
            return status;
        }        
    }
    
    class AlarmListAdapter extends ArrayAdapter<AlarmRecord>{
    	AlarmListAdapter(List<AlarmRecord> list){
            super(RealAlarmActivity.this, R.layout.real_alarm_item, list);
        }
    	
    	public View getView(int position, View convertView, ViewGroup parent) {
    		 View row = convertView;
    		 
    		 ViewWrapper wrapper = null;
    		 if(row == null){
                 LayoutInflater inflater = getLayoutInflater();
                 row = inflater.inflate(R.layout.real_alarm_item, parent, false);
                 wrapper = new ViewWrapper(row);
                 row.setTag(wrapper);
             } else {
            	 wrapper = (ViewWrapper)row.getTag();
             }
    		 AlarmRecord bs = this.getItem(position);
    		 
    		 wrapper.getName().setText(bs.uuid + "_" + bs.uuid);
    		 wrapper.getStatus().setText(bs.status + "");
         
             return row;    		 
    	}    	
    }  
}
