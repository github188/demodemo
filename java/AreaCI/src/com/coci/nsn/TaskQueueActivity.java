package com.coci.nsn;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;
import android.widget.TextView;

import com.coci.provider.AreaCI.Project;
import com.coci.provider.AreaCI.TaskInfo;

public class TaskQueueActivity extends Activity {
	private final static String TAG = "areaci.queue";
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //TextView textview = new TextView(this);
        //textview.setText("This is the task queue tab");
        //setContentView(textview);
        
        /**
         * Favirote, cate/name/status/
         * 
         * Toast:
         *  PATE, 
         *  SW,
         *  Create time 
         */
        
        setContentView(R.layout.task_queue);
        
        ListView lv = (ListView) findViewById(R.id.task_list);
        
        Cursor cursor = managedQuery(TaskInfo.TASK_QUEUE_URI, 
        		new String[] {TaskInfo._ID, TaskInfo.CATEGORY, TaskInfo.NAME, TaskInfo.STATUS,
        		TaskInfo.HOST, TaskInfo.SW_BUILD 
        		}, 
        		null, null,
                Project.DEFAULT_SORT_ORDER);

        // Used to map notes entries from the database to views
        SimpleCursorAdapter adapter = new SimpleCursorAdapter(this, R.layout.task_queue_item, 
        		cursor,
                new String[] {TaskInfo.CATEGORY, TaskInfo.NAME, TaskInfo.STATUS,
        		TaskInfo.HOST,
        		TaskInfo.SW_BUILD,}, 
                new int[] {R.id.category, R.id.name, R.id.status,
        		R.id.host_ip, R.id.sw_build}
        );
        
        adapter.setViewBinder(new QueueViewBinder());
        
        lv.setAdapter(adapter);       
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
