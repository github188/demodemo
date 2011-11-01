package com.coci.nsn;

import com.coci.provider.AreaCI.Project;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;
import android.widget.TextView;

public class TaskQueueActivity extends Activity {
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //TextView textview = new TextView(this);
        //textview.setText("This is the task queue tab");
        //setContentView(textview);
        
        setContentView(R.layout.main);
        
        ListView lv = (ListView) findViewById(R.id.list);
        
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
}
