package org.goku.client;

import java.util.Comparator;
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
	private boolean isActive = false;
	private boolean isStopped = false;
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
        adapter = new AlarmListAdapter();
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
                		if(r.newAlarm > 0){
                			onNewAlarm(r);
                		}
                	}else if(r.status != 0){
                		showMessage(r.getMessage());
                	}
                }
            }
        }; 
        
        timer.scheduleAtFixedRate(new TimerTask (){
			@Override
			public void run() {
				Message msg = handler.obtainMessage(realAlarm);
				handler.sendMessage(msg);
			}
        }, 100, 60 * 1000);        
    } 
    
    @Override
    protected void onStart() {
        super.onStart();
        Log.d(TAG, "start....");
        isActive = true;
    }
    
    protected void onNewAlarm(GokuResult result){
    	//如果界面已经关闭，用通知提示，如果应用还在使用中，用toast提示。
    	if(isStopped){
    		this.alarmNotification(result);
    	}else if(!isActive){
    		this.showMessage(String.format("有%s条基站告警", result.newAlarm));
    	}
    }
    
    protected void alarmNotification(GokuResult result){
    	
    }
    
    protected void onResume() {
    	super.onResume();
    	isActive = true;
    	isStopped = false;
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
    	isStopped = true;
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
					adapter.clear();
					for(AlarmRecord al: goku.realTimeAlram){
						adapter.add(al);
					}
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
		if(!isActive) return;
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
    	AlarmListAdapter(){
            super(RealAlarmActivity.this, R.layout.real_alarm_item);
            this.sort(new Comparator<AlarmRecord>(){
    			@Override
    			public int compare(AlarmRecord al, AlarmRecord al_other) {
    				String d1 = al.startTime;
    				String d2 = al_other.startTime;
    				if(d1 != null && d2 != null){
    					return d1.compareTo(d2) * -1;
    				}
    				return -1;
    			}});            
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
    		 AlarmRecord al = this.getItem(position);
    		 if(al != null){    		 
    			 wrapper.getName().setText(al.getAlaramType() + " " + al.alarmName + " " + al.getBTSName() + " "
    					 + al.startTime);
    			 wrapper.getStatus().setText(al.status + "");
    		 }
             return row;    		 
    	}
    }  
}
