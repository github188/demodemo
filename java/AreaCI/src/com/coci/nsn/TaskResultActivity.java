package com.coci.nsn;

import android.app.Activity;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView.AdapterContextMenuInfo;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;
import android.widget.TextView;

import com.coci.provider.AreaCI.Project;
import com.coci.provider.AreaCI.TaskInfo;

public class TaskResultActivity extends Activity {
	private final static String TAG = "areaci.result";
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.task_result);
        
        ListView lv = (ListView) findViewById(R.id.task_list);        
        /*
        Intent intent = getIntent();
        if (intent.getData() == null) {
            intent.setData(Project.CONTENT_URI);
        }*/   
        
        Cursor cursor = managedQuery(TaskInfo.TASK_RESULT_URI, 
        		new String[] {TaskInfo._ID, TaskInfo.CATEGORY, TaskInfo.NAME, TaskInfo.STATUS,
        		TaskInfo.HOST, TaskInfo.SW_BUILD, 
        		TaskInfo.TEST_COUNT,
        		TaskInfo.RESULT_COUNT, TaskInfo.RESULT_PASS, TaskInfo.RESULT_FAIL,
        		TaskInfo.RESULT
        		},  
        		null, null,
                Project.DEFAULT_SORT_ORDER);

        // Used to map notes entries from the database to views
        SimpleCursorAdapter adapter = new SimpleCursorAdapter(this, R.layout.task_result_item, 
        		cursor,
                new String[] {TaskInfo.CATEGORY, TaskInfo.NAME, TaskInfo.STATUS,
                	TaskInfo.SW_BUILD,}, 
                new int[] {R.id.category, R.id.name, R.id.status,
                	R.id.result_summary}
        		);
        
        adapter.setViewBinder(new QueueViewBinder());
        lv.setAdapter(adapter);
        //lv.registerForContextMenu()
        registerForContextMenu(lv);
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
