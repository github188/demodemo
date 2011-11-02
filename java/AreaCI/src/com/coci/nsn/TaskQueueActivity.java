package com.coci.nsn;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;

import com.coci.provider.AreaCI.Project;

public class TaskQueueActivity extends Activity {
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //TextView textview = new TextView(this);
        //textview.setText("This is the task queue tab");
        //setContentView(textview);
        
        setContentView(R.layout.task_queue);
        
        ListView lv = (ListView) findViewById(R.id.task_list);
        
        Intent intent = getIntent();
        if (intent.getData() == null) {
            intent.setData(Project.CONTENT_URI);
        }        
        
        Cursor cursor = managedQuery(getIntent().getData(), 
        		new String[] {Project._ID, Project.NAME, Project.STATUS }, 
        		null, null,
                Project.DEFAULT_SORT_ORDER);

        // Used to map notes entries from the database to views
        SimpleCursorAdapter adapter = new SimpleCursorAdapter(this, R.layout.projectlist_item, 
        		cursor,
                new String[] {Project.NAME, Project.STATUS}, 
                new int[] {R.id.name, R.id.status});
        lv.setAdapter(adapter);        
    }
    
    public void doRefresh(View view){
    	Log.i("areaci.task_queue", "click to refresh...");
    } 
}
