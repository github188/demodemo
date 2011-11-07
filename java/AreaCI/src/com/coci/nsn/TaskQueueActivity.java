package com.coci.nsn;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.ContentObserver;
import android.database.Cursor;
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
	private String order_by_sql = null;	
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.task_queue);

        SharedPreferences settings = getSharedPreferences(AreaCI.PREFS_NAME, 0); 
        
        this.order_by = settings.getInt(AreaCI.PREFS_QUEUE_ORDER_BY, R.id.updated_time);        
        updateOrderBy(this.order_by, "", false);
        
        ListView lv = (ListView) findViewById(R.id.task_list);
        
        this.updateManagedCursor(order_by_sql);
        // Used to map notes entries from the database to views
        adapter = new SimpleCursorAdapter(this, R.layout.task_queue_item, 
        		cursor,
                new String[] {TaskInfo.CATEGORY, TaskInfo.NAME, TaskInfo.STATUS,
        		TaskInfo.HOST,
        		TaskInfo.SW_BUILD,}, 
                new int[] {R.id.category, R.id.name, R.id.status,
        		R.id.host_ip, R.id.sw_build}
        );                
        adapter.setViewBinder(new QueueViewBinder());        
        lv.setAdapter(adapter);  
        
        getContentResolver().registerContentObserver(TaskInfo.TASK_LIST_URI, true,
        		new ContentObserver(new Handler()) {
        			@Override
    				public void onChange(boolean selfChange) {
        				Log.i(TAG, "The task list is updated.");
        				updateManagedCursor(order_by_sql);
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
        return true;
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
    	Log.d(TAG, "clicked:" + item.toString());
        switch (item.getItemId()) {
        	case R.id.prioirty:
        	case R.id.task_name:
        	case R.id.create_time:
        	case R.id.updated_time:
        		if (item.isChecked()) item.setChecked(false);
        		else item.setChecked(true);
        	updateOrderBy(item.getItemId(), item.getTitle().toString(), true);
            updateManagedCursor(order_by_sql); 
        	return true;     
        	
        	case R.id.filter_by:
        		//showDialog(FILTER_DIALOG_ID);
        		return false;

        	case R.id.stop_sync:
        		//showDialog(FILTER_DIALOG_ID);
        		Intent intent = new Intent().setClass(this, DataSyncService.class);
        	    this.stopService(intent);      		
        		return true;
        		
        	default:
        		return super.onOptionsItemSelected(item);
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
    
    private void updateManagedCursor(String order_by){
    	if(cursor != null){
    		stopManagingCursor(cursor);
    		cursor.close();
    	}
        cursor = managedQuery(TaskInfo.TASK_QUEUE_URI, 
        		new String[] {TaskInfo._ID, TaskInfo.CATEGORY, TaskInfo.NAME, TaskInfo.STATUS,
        		TaskInfo.HOST, TaskInfo.SW_BUILD 
        		}, 
        		null, null,
        		order_by); 	
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
			if(columnIndex == 5 && view instanceof TextView){
				TextView o = (TextView)view;
				o.setText("  SW:" + cursor.getString(cursor.getColumnIndex("sw_build")));
				return true;
			}
			return false;
		}
    }
}
