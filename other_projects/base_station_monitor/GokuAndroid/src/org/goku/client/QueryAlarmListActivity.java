package org.goku.client;

import java.util.Comparator;
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
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class QueryAlarmListActivity extends Activity {
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
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);        
        setContentView(R.layout.real_alarm_list);
        
        TextView title = (TextView) findViewById(R.id.last_synced);
        title.setText("历史告警列表");
        
        ListView lv = (ListView) findViewById(R.id.task_list);
        //BTSListAdapter.
        goku = GokuClient.getInstance();
        adapter = new AlarmListAdapter();
        lv.setAdapter(adapter);                 
    } 
    
    @Override
    protected void onStart() {
        super.onStart();
        Log.d(TAG, "start....");
        isActive = true;
        updateAlarmList();
    }
    
    protected void onResume() {
    	super.onResume();
    	isActive = true;
    	isStopped = false;
    	Log.d(TAG, "resume....");
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
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.real_alarm_menu, menu);
                
        return true;
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
    	
    	Log.d(TAG, "clicked:" + item.toString());
        switch (item.getItemId()) {
        	default:
        		return super.onOptionsItemSelected(item);
        }
    }
    
  
    
    private void updateAlarmList(){
    	if(!isActive) return;
		runOnUiThread(
			new Runnable(){
				public void run(){
					adapter.clear();
					for(AlarmRecord al: goku.queryAlram){
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
            super(QueryAlarmListActivity.this, R.layout.real_alarm_item);
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
