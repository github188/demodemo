package com.coci.nsn;

import android.app.Activity;
import android.content.SharedPreferences;
import android.database.ContentObserver;
import android.database.Cursor;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;
import android.widget.TextView;

import com.coci.nsn.TaskQueueActivity.QueueViewBinder;
import com.coci.provider.AreaCI;
import com.coci.provider.AreaCI.Device;
import com.coci.provider.AreaCI.TaskInfo;

public class DevicesActivity extends Activity {
	private final static String TAG = "areaci.device";
	static final int FILTER_DIALOG_ID = 1;
	private Cursor cursor = null;
	private SimpleCursorAdapter adapter = null;
	private int order_by = 0;
	private int filter = 0;
	private String order_by_sql = "priority desc";	
	private String filter_sql = "";	
	private TextView lastSynced = null;
	private SharedPreferences settings = null;	
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.device_list);
        settings = getSharedPreferences(AreaCI.PREFS_NAME, 0);
        
        lastSynced = (TextView)this.findViewById(R.id.last_synced);        
        lastSynced.setText("Last sync:" + settings.getString(AreaCI.PREFS_LAST_SYNC_TIME_STR, "n/a"));
        this.order_by = settings.getInt(AreaCI.PREFS_DEV_ORDER_BY_ID, 0);        
        updateOrderBy(this.order_by, "", false);
        
        ListView lv = (ListView) findViewById(R.id.task_list);
        
        this.updateManagedCursor();
        // Used to map notes entries from the database to views
        adapter = new SimpleCursorAdapter(this, R.layout.device_list_item, 
        		cursor,
                new String[] {Device.NAME, Device.STATUS, 
        		Device.TEST_ENV, Device.AVAILABLE,
        		Device.RESERVED}, 
                new int[] {R.id.name, R.id.status,  
        		R.id.pate_info, R.id.ava_time, R.id.ohter_info}
        );
        adapter.setViewBinder(new DeviceViewBinder());        
        lv.setAdapter(adapter);  
        
        getContentResolver().registerContentObserver(Device.CONTENT_URI, true,
        		new ContentObserver(new Handler()) {
        			@Override
    				public void onChange(boolean selfChange) {
        				Log.i(TAG, "The device list is updated.");
        				updateManagedCursor();
    				}
        	}
        );        
        
        getContentResolver().registerContentObserver(AreaCI.UPDATED_DATA, true,
        	new ContentObserver(new Handler()) {
				@Override
				public void onChange(boolean selfChange) {
					Log.i(TAG, "The device list is updated.");
					if(lastSynced != null){
						lastSynced.post(new Runnable(){
							public void run(){
								lastSynced.setText("Last sync:" + settings.getString(AreaCI.PREFS_LAST_SYNC_TIME_STR, "n/a"));
							}
						});
					}
				}
        	}        		
        );
    }
    
    private void updateOrderBy(int id, String label, boolean save){
        switch (id) {
			case R.id.prioirty:
				order_by_sql = "priority desc, modified desc";
				break;
			case R.id.task_name:
				order_by_sql = "name asc, modified desc";
				break;
			case R.id.pate_name:
				order_by_sql = "host_ip asc, priority desc";
				break;								
			case R.id.create_time:
				order_by_sql = "created desc, modified desc";
				break;				
			case R.id.updated_time:
				order_by_sql = "modified desc, priority desc";
				break;
			default:
				order_by_sql = "name";
        }
        if(save){
        	SharedPreferences settings = getSharedPreferences(AreaCI.PREFS_NAME, 0);
        	SharedPreferences.Editor editor = settings.edit();
        	editor.putInt(AreaCI.PREFS_QUEUE_ORDER_BY_ID, id);
        	editor.commit();
        } 
        this.order_by = id;
    }   
    
    private void updateManagedCursor(){
    	Log.i(TAG, "task order by:" + this.order_by_sql);
    	if(cursor != null){
    		stopManagingCursor(cursor);
    		cursor.close();
    	}
        cursor = managedQuery(Device.CONTENT_URI, 
        		new String[] {Device.NAME, Device.HOST_IP, Device.STATUS,
        		Device.TEST_ENV, Device.PKG_ENV, Device.AVAILABLE,
        		Device.CUR_TASK, Device.RESERVED,
        		Device._ID
        		}, 
        		this.filter_sql, null,
        		this.order_by_sql); 
        if(adapter != null){
        	adapter.changeCursor(cursor);
        	//adapter.notifyDataSetChanged();
        }
    }    
    
    class DeviceViewBinder implements SimpleCursorAdapter.ViewBinder{
		@Override
		public boolean setViewValue(View view, Cursor cursor, int columnIndex) {
			if(view.getId() == R.id.status){
				TextView o = (TextView)view;
				String st = cursor.getString(cursor.getColumnIndex(Device.STATUS));
				if (st.equals("idle") || st.equals("running")){
					o.setTextColor(Color.GREEN);
				}else if (st.equals("reserved") || st.equals("prereserve")){
					o.setTextColor(Color.YELLOW);
				}else {
					o.setTextColor(Color.RED);
				}
				o.setText(st);
				return true;
			}
			
			if(view.getId() == R.id.name){
				/* "%s:%s" */
				TextView o = (TextView)view;
				String name = cursor.getString(cursor.getColumnIndex(Device.NAME));
				name += " " + cursor.getString(cursor.getColumnIndex(Device.HOST_IP));				
				o.setText(name);
				return true;
			}
			
			if(view.getId() == R.id.pate_info){
				TextView o = (TextView)view;
				String name = cursor.getString(cursor.getColumnIndex(Device.TEST_ENV));		
				String pkg_env = cursor.getString(cursor.getColumnIndex(Device.PKG_ENV));
				if(pkg_env.indexOf(',') > 0){
					String[] _t = pkg_env.split(",");
					pkg_env = _t[_t.length - 1];
				}
				name += " " + pkg_env;
				if(name.trim().length() == 0){
					name = "Not found PKG/TEST ENV";
				}
				o.setText(name);
				return true;
			}	
			
			if(view.getId() == R.id.ohter_info){
				TextView o = (TextView)view;
				String st = cursor.getString(cursor.getColumnIndex(Device.STATUS));
				String name = "";
				if(st.equals("reserved")){
					name = "reserved by " + cursor.getString(cursor.getColumnIndex(Device.RESERVED));
				}
				o.setText(name);
				return true;
			}
			return false;
		}   
    }
}
