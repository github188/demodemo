package com.coci.nsn;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;

import com.coci.provider.AreaCI.Project;

public class AreaCIActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        Log.i("areaci", "start to run AreaCIActivity");
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