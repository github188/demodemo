package com.coci.nsn;

import java.net.URI;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.ContentObserver;
import android.database.Cursor;
import android.graphics.Color;
import android.net.Uri;
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
import com.coci.provider.AreaCI.TaskInfo;

public class TaskResultActivity extends Activity {
	private final static String TAG = "areaci.result";
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

        setContentView(R.layout.task_result);
        
        settings = getSharedPreferences(AreaCI.PREFS_NAME, 0); 

        lastSynced = (TextView)this.findViewById(R.id.last_synced);        
        lastSynced.setText("Last sync:" + settings.getString(AreaCI.PREFS_LAST_SYNC_TIME_STR, "n/a"));
        
        this.order_by = settings.getInt(AreaCI.PREFS_RESULT_ORDER_BY_ID, R.id.updated_time);
        this.filter = settings.getInt(AreaCI.PREFS_RESULT_FILTER_ID, R.id.cate_all);
        updateOrderBy(this.order_by, "", false);
        updateFilter(this.filter, "", false);

        this.updateManagedCursor();
        
        // Used to map notes entries from the database to views
        adapter = new SimpleCursorAdapter(this, R.layout.task_result_item, 
        		cursor,
                new String[] {TaskInfo.CATEGORY, TaskInfo.NAME, TaskInfo.STATUS,
                	TaskInfo.SW_BUILD,}, 
                new int[] {R.id.category, R.id.name, R.id.status,
                	R.id.result_summary}
        		){        	
        	public void bindView(View view, Context context, Cursor cursor){
        		super.bindView(view, context, cursor);
        		String st = cursor.getString(3);
        		int pass = cursor.getInt(8);
        		int count = cursor.getInt(7);
        		if(st.equals("error")){
        			view.setBackgroundColor(Color.RED);
        		}else if(st.equals("done") && pass == count) {
        			view.setBackgroundColor(Color.GREEN);
        		}else if(st.equals("done") && pass != count){
        			view.setBackgroundColor(Color.YELLOW);
        		}else if(st.equals("ignore")) {
        			view.setBackgroundColor(Color.GRAY);
        		}
        		view.setTag(R.id.task_name, cursor.getString(0));
        	}
        };
        
        ListView lv = (ListView) findViewById(R.id.task_list);
        adapter.setViewBinder(new QueueViewBinder());
        lv.setAdapter(adapter);
        //lv.registerForContextMenu()
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
        
        item = menu.findItem(filter);
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
        if(save){
        	SharedPreferences settings = getSharedPreferences(AreaCI.PREFS_NAME, 0);
        	SharedPreferences.Editor editor = settings.edit();
        	editor.putInt(AreaCI.PREFS_RESULT_FILTER_ID, id);
        	editor.commit();
        } 
	    this.filter = id;
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
    
    private void updateManagedCursor(){
    	if(cursor != null){
    		stopManagingCursor(cursor);
    		cursor.close();
    	}
        cursor = managedQuery(TaskInfo.TASK_RESULT_URI, 
        		new String[] {TaskInfo._ID, TaskInfo.CATEGORY, TaskInfo.NAME, TaskInfo.STATUS,
        		TaskInfo.HOST, TaskInfo.SW_BUILD, 
        		TaskInfo.TEST_COUNT,
        		TaskInfo.RESULT_COUNT, TaskInfo.RESULT_PASS, TaskInfo.RESULT_FAIL,
        		TaskInfo.RESULT,
        		TaskInfo.CREATED_DATE, TaskInfo.START_DATE, TaskInfo.DONE_DATE
        		}, 
        		this.filter_sql, null,
        		order_by_sql); 	
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
    	//info.targetView
    	switch (item.getItemId()) {
    		case R.id.open_console:
    			String o = (String)info.targetView.getTag(R.id.task_name);
    			openConsoleText(o);
    		return true;
    	}    	
    	return false;
    }
    
    private void openConsoleText(String id){
    	Log.i(TAG, "open colsole text for task:" + id);
		Intent intent = new Intent();
		intent.setAction(Intent.ACTION_VIEW);
		intent.addCategory(Intent.CATEGORY_BROWSABLE);
		Uri u = Uri.parse("http://proxy-nsn.deonwu84.com:8080/coci/areaci/api/console_text?task_id=" + id);
		intent.setData(u);
		//intent.setDataAndType(u, "plain/text");
		startActivity(intent);
    }
    
    class QueueViewBinder implements SimpleCursorAdapter.ViewBinder{
    	
		@Override
		public boolean setViewValue(View view, Cursor cursor, int columnIndex) {
			//view.setTag(R.id.task_name, cursor.getString(0));
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
				
				o.append("\nCreate:" + cursor.getString(index + 7) + 
						" Start:" + cursor.getString(index + 8) +
						" Done:" + cursor.getString(index + 9)
						);				
				
				return true;
			}
			return false;
		}	
    }
    
}
