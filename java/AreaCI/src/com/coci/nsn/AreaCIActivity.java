package com.coci.nsn;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;

import com.coci.provider.AreaCI.Project;

/**
 * 
 * Android进阶：ListView性能优化异步加载图片 使滑动效果流畅
 * http://blog.csdn.net/wanglong0537/article/details/6334005#comments
 * 
 * 
 * 
 * 定义Sina围脖菜单样式
 * http://blog.csdn.net/wanglong0537/article/details/6434728
 * 
 * Network
 * http://www.vogella.de/articles/AndroidNetworking/article.html
 * 
 * @author deon
 */

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