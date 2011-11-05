package com.coci.nsn;

import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;

import com.coci.provider.AreaCI.Project;
import com.coci.provider.AreaCI.TaskInfo;

public class TaskQueueActivity extends Activity {
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //TextView textview = new TextView(this);
        //textview.setText("This is the task queue tab");
        //setContentView(textview);
        
        setContentView(R.layout.task_queue);
        
        ListView lv = (ListView) findViewById(R.id.task_list);
        
        /*
        Intent intent = getIntent();
        if (intent.getData() == null) {
            intent.setData(Project.CONTENT_URI);
        }*/   
        
        Cursor cursor = managedQuery(TaskInfo.TASK_QUEUE_URI, 
        		new String[] {TaskInfo._ID, TaskInfo.NAME, TaskInfo.STATUS }, 
        		null, null,
                Project.DEFAULT_SORT_ORDER);

        // Used to map notes entries from the database to views
        SimpleCursorAdapter adapter = new SimpleCursorAdapter(this, R.layout.projectlist_item, 
        		cursor,
                new String[] {TaskInfo.NAME, TaskInfo.STATUS}, 
                new int[] {R.id.name, R.id.status});
        lv.setAdapter(adapter);       
    }
    
    public void doRefresh(View view){
    	Log.i("areaci.task_queue", "click to refresh...");
    	Intent intent = new Intent().setClass(this, DataSyncService.class);
	    this.startService(intent);    	
    	/*
    	try {
			URL url = new URL("http://10.0.2.2:8000/areaci/worker_config");
			InputStream ins = url.openStream();
			byte[] buffer = new byte[1024];
			int len = ins.read(buffer);
			Log.i("areaci.task_queue", new String(buffer, 0, len));			
		} catch (Exception e) {
			Log.e("areaci.task_queue", e.toString());
		}*/
    } 
}
