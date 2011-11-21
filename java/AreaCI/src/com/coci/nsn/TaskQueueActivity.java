package com.coci.nsn;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.ContentObserver;
import android.database.Cursor;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;
import android.widget.TextView;

import com.coci.provider.AreaCI;
import com.coci.provider.AreaCI.TaskInfo;

public class TaskQueueActivity extends Activity {
	private final static String TAG = "areaci.queue";
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
        setContentView(R.layout.task_queue);
        settings = getSharedPreferences(AreaCI.PREFS_NAME, 0);
        
        lastSynced = (TextView)this.findViewById(R.id.last_synced);        
        lastSynced.setText("Last sync:" + settings.getString(AreaCI.PREFS_LAST_SYNC_TIME_STR, "n/a"));
        this.order_by = settings.getInt(AreaCI.PREFS_QUEUE_ORDER_BY, R.id.updated_time);        
        updateOrderBy(this.order_by, "", false);
        
        ListView lv = (ListView) findViewById(R.id.task_list);
        
        this.updateManagedCursor();
        // Used to map notes entries from the database to views
        adapter = new SimpleCursorAdapter(this, R.layout.task_queue_item, 
        		cursor,
                new String[] {TaskInfo.STATUS, TaskInfo.NAME, 
        		TaskInfo.HOST,
        		TaskInfo.SW_BUILD, TaskInfo.CREATED_DATE}, 
                new int[] {R.id.status, R.id.name, 
        		R.id.host_ip, R.id.sw_build, R.id.date_time}
        );
        adapter.setViewBinder(new QueueViewBinder());        
        lv.setAdapter(adapter);  
        
        getContentResolver().registerContentObserver(TaskInfo.TASK_LIST_URI, true,
        		new ContentObserver(new Handler()) {
        			@Override
    				public void onChange(boolean selfChange) {
        				Log.i(TAG, "The task list is updated.");
        				updateManagedCursor();
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
        
        getContentResolver().registerContentObserver(AreaCI.UPDATED_DATA, true,
        	new ContentObserver(new Handler()) {
				@Override
				public void onChange(boolean selfChange) {
					Log.i(TAG, "The task list is updated.");
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
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.task_menu, menu);
        
        MenuItem item = menu.findItem(order_by);
        if(item != null){
        	item.setChecked(true);
        }
        
        item = menu.findItem(R.id.cate_all);
        if(item != null){
        	item.setChecked(true);
        }        
        return true;
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
    	Log.d(TAG, "clicked:" + item.toString());
        switch (item.getItemId()) {
        	case R.id.prioirty:
        	case R.id.task_name:
        	case R.id.pate_name:
        	case R.id.create_time:
        	case R.id.updated_time:
        		if (item.isChecked()) item.setChecked(false);
        		else item.setChecked(true);
        			updateOrderBy(item.getItemId(), item.getTitle().toString(), true);
        			updateManagedCursor(); 
            	return true;     
        	
        	case R.id.cate_all:
        	case R.id.cate_smt:
        	case R.id.cate_lwt:
        	case R.id.cate_crt:
        	case R.id.cate_sys:
        		if (item.isChecked()) item.setChecked(false);
        		else item.setChecked(true);
        			updateFilter(item.getItemId(), item.getTitle().toString(), true);
        			updateManagedCursor(); 
        		//showDialog(FILTER_DIALOG_ID);
        		return true;

        	case R.id.stop_sync:
        		//showDialog(FILTER_DIALOG_ID);
        		Intent intent = new Intent().setClass(this, DataSyncService.class);
        	    this.stopService(intent);      		
        		return true;
        	
        	case R.id.login:
        		Intent login = new Intent().setClass(this, AreaCIActivity.class);
        		login.setData(AreaCI.EXPRIED_DATA_URI);
        	    this.startActivity(login);      		
        		return true;  
        		
        	default:
        		return super.onOptionsItemSelected(item);
        }
        
    }
    
    private void updateFilter(int id, String label, boolean save){
	    switch (id) {
			case R.id.cate_all:
				this.filter_sql = "";
				break;
			case R.id.cate_smt:
				this.filter_sql = "category = 'smt'";
				break;
			case R.id.cate_lwt:
				this.filter_sql = "category = 'lwt'";
				break;								
			case R.id.cate_crt:
				this.filter_sql = "category = 'crt'";
				break;				
			case R.id.cate_sys:
				this.filter_sql = "category = 'sys'";
				break;
	    }
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
        }
        if(save){
        	SharedPreferences settings = getSharedPreferences(AreaCI.PREFS_NAME, 0);
        	SharedPreferences.Editor editor = settings.edit();
        	editor.putInt(AreaCI.PREFS_QUEUE_ORDER_BY_ID, id);
        	editor.commit();
        } 
        this.order_by = id;
    }
    
    @Override
    protected Dialog onCreateDialog(int id) {
        switch (id) {
        	case FILTER_DIALOG_ID:
        		return createFilterDialog();
        }
        return null;
    }    
    
    private Dialog createFilterDialog(){
    	Context mContext = getApplicationContext();
    	Dialog dialog = new Dialog(mContext);
    	dialog.setContentView(R.layout.task_filter_dialog);
    	dialog.setTitle("Task filter Dialog");
    	Log.d(TAG, "xxx:" + dialog.toString());
    	return dialog;    	
    }
    
    private void updateManagedCursor(){
    	Log.i(TAG, "task order by:" + this.order_by_sql);
    	if(cursor != null){
    		stopManagingCursor(cursor);
    		cursor.close();
    	}
        cursor = managedQuery(TaskInfo.TASK_QUEUE_URI, 
        		new String[] {TaskInfo._ID, TaskInfo.CATEGORY, TaskInfo.NAME, TaskInfo.STATUS,
        		TaskInfo.HOST, TaskInfo.SW_BUILD, 
        		TaskInfo.CREATED_DATE, TaskInfo.START_DATE,
        		TaskInfo.PRIORITY
        		}, 
        		this.filter_sql, null,
        		this.order_by_sql); 
        if(adapter != null){
        	adapter.changeCursor(cursor);
        	//adapter.notifyDataSetChanged();
        }
    }
    
    public void doRefresh(View view){
    	Log.i("areaci.task_queue", "click to refresh...");
    	Intent intent = new Intent().setClass(this, DataSyncService.class);
    	intent.setData(TaskInfo.TASK_QUEUE_URI);
	    this.startService(intent);
	    
    }
    
    class QueueViewBinder implements SimpleCursorAdapter.ViewBinder{
		@Override
		public boolean setViewValue(View view, Cursor cursor, int columnIndex) {
			if(view.getId() == R.id.status){
				TextView o = (TextView)view;
				String st = cursor.getString(cursor.getColumnIndex(TaskInfo.STATUS));
				if (st.equals("running") || st.equals("preparing")){
					o.setTextColor(Color.GREEN);
				}else if (st.equals("waiting")){
					o.setTextColor(Color.YELLOW);
				}else {
					o.setTextColor(Color.RED);
				}
				o.setText(st);
				return true;
			}
			
			if(view.getId() == R.id.sw_build){
				TextView o = (TextView)view;
				o.setText("  SW:" + cursor.getString(cursor.getColumnIndex("sw_build")));
				return true;
			}
			if(view.getId() == R.id.name){
				/* "%s:%s@%s" */
				TextView o = (TextView)view;
				String name = cursor.getString(cursor.getColumnIndex(TaskInfo._ID));
				name += " " + cursor.getString(cursor.getColumnIndex(TaskInfo.NAME));
				name += " @" + cursor.getString(cursor.getColumnIndex(TaskInfo.PRIORITY));				
				o.setText(name);
				return true;
			}
			
			if(view.getId() == R.id.date_time){
				TextView o = (TextView)view;
				o.setText("Create:" + cursor.getString(cursor.getColumnIndex(TaskInfo.CREATED_DATE)) + 
						" Start:" + cursor.getString(cursor.getColumnIndex(TaskInfo.START_DATE))
						);
				return true;
			}			
			return false;
		}
    }
}
