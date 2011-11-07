package com.coci.nsn;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.ContentObserver;
import android.database.Cursor;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView.AdapterContextMenuInfo;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;
import android.widget.TextView;

import com.coci.provider.AreaCI;
import com.coci.provider.AreaCI.Project;
import com.coci.provider.AreaCI.TaskInfo;

public class TaskResultActivity extends Activity {
	private final static String TAG = "areaci.result";
	static final int FILTER_DIALOG_ID = 1;
	private Cursor cursor = null;
	private SimpleCursorAdapter adapter = null;
	private int order_by = 0;
	private String order_by_sql = null;		
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.task_result);
        
        SharedPreferences settings = getSharedPreferences(AreaCI.PREFS_NAME, 0); 
        
        this.order_by = settings.getInt(AreaCI.PREFS_RESULT_ORDER_BY_ID, R.id.updated_time);        
        updateOrderBy(this.order_by, "", false);        

        this.updateManagedCursor(order_by_sql);
        
        // Used to map notes entries from the database to views
        adapter = new SimpleCursorAdapter(this, R.layout.task_result_item, 
        		cursor,
                new String[] {TaskInfo.CATEGORY, TaskInfo.NAME, TaskInfo.STATUS,
                	TaskInfo.SW_BUILD,}, 
                new int[] {R.id.category, R.id.name, R.id.status,
                	R.id.result_summary}
        		);
        
        ListView lv = (ListView) findViewById(R.id.task_list);
        adapter.setViewBinder(new QueueViewBinder());
        lv.setAdapter(adapter);
        //lv.registerForContextMenu()
        getContentResolver().registerContentObserver(TaskInfo.TASK_LIST_URI, true,
        		new ContentObserver(new Handler()) {
        			@Override
    				public void onChange(boolean selfChange) {
        				Log.i(TAG, "The task list is updated.");
        				updateManagedCursor(order_by_sql);
    				}
        	}
        );    
        
        registerForContextMenu(lv);
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
        	editor.putInt(AreaCI.PREFS_RESULT_ORDER_BY_ID, id);
        	editor.commit();
        } 
        this.order_by = id;
    }
    
    private void updateManagedCursor(String order_by){
    	if(cursor != null){
    		stopManagingCursor(cursor);
    		cursor.close();
    	}
        cursor = managedQuery(TaskInfo.TASK_RESULT_URI, 
        		new String[] {TaskInfo._ID, TaskInfo.CATEGORY, TaskInfo.NAME, TaskInfo.STATUS,
        		TaskInfo.HOST, TaskInfo.SW_BUILD, 
        		TaskInfo.TEST_COUNT,
        		TaskInfo.RESULT_COUNT, TaskInfo.RESULT_PASS, TaskInfo.RESULT_FAIL,
        		TaskInfo.RESULT
        		}, 
        		null, null,
        		order_by); 	
        if(adapter != null){
        	adapter.changeCursor(cursor);
        }
    }    
    
    @Override
    public void onCreateContextMenu(ContextMenu menu, View v,
                                    ContextMenuInfo menuInfo) {
    	super.onCreateContextMenu(menu, v, menuInfo);
      	MenuInflater inflater = getMenuInflater();
      	inflater.inflate(R.menu.task_result_context_menu, menu);
      	Log.d(TAG, "create context menu...");
    }    
    
    @Override
    public boolean onContextItemSelected(MenuItem item) {
    	AdapterContextMenuInfo info = (AdapterContextMenuInfo) item.getMenuInfo(); 
    	
    	//info.targetView
    	Log.d(TAG, "info id:" + info.id);
    	switch (item.getItemId()) {      
    	}
    	
    	return true;
    }
    
    class QueueViewBinder implements SimpleCursorAdapter.ViewBinder{
    	
		@Override
		public boolean setViewValue(View view, Cursor cursor, int columnIndex) {
			/**
			 * 192.168.1.2, SW:%s, Planed:%s
			 * Result:%s/%s/%s, TA Error by xx
			 */
			if(view.getId() == R.id.result_summary){
				//Log.d(TAG, "summary:" + columnIndex);
				int index = 4; //cursor.getColumnIndex(TaskInfo.HOST);				
				TextView o = (TextView)view;
				String f = String.format("%s, SW:%s, Planed:%s\n" +
						"Result:%s/%s/%s, ", cursor.getString(index),
						cursor.getString(index + 1),
						cursor.getString(index + 2),
						
						cursor.getString(index + 3),
						cursor.getString(index + 4),
						cursor.getString(index + 5)
						);
				o.setText(f);
				String result = cursor.getString(index + 6);
				if(result != null){
					//o.setTextColor(Color.RED);
					o.append(result);
				}
				
				return true;
			}
			return false;
		}	
    }
    
}
