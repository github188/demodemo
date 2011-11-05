package com.coci.nsn;

import com.coci.provider.AreaCI.Project;
import com.coci.provider.AreaCI.TaskInfo;

import android.app.Activity;
import android.database.Cursor;
import android.os.Bundle;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;
import android.widget.TextView;

public class TaskResultActivity extends Activity {
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.task_queue);
        
        ListView lv = (ListView) findViewById(R.id.task_list);
        
        /*
        Intent intent = getIntent();
        if (intent.getData() == null) {
            intent.setData(Project.CONTENT_URI);
        }*/   
        
        Cursor cursor = managedQuery(TaskInfo.TASK_RESULT_URI, 
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
}
