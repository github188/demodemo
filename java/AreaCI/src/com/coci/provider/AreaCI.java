package com.coci.provider;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

import android.net.Uri;
import android.provider.BaseColumns;

public class AreaCI {
	public static final String PREFS_NAME = "areaci_config";
	
	public static final String PREFS_QUEUE_ORDER_BY = "prefs_query_order_by";
	//
	public static final String PREFS_QUEUE_ORDER_BY_ID = "prefs_query_order_by_id";
	public static final String PREFS_RESULT_ORDER_BY_ID = "prefs_result_order_by_id";
	public static final String PREFS_LAST_SYNC_TIME = "prefs_last_sync_time";
	public static final String PREFS_LAST_SYNC_TIME_STR = "prefs_last_sync_time_str";
	
	
	public static final String AUTHORITY = "com.coci.provider.AreaCI";
	
	//所有过期数据的URI.
    public static final Uri EXPRIED_DATA_URI
    = Uri.parse("content://" + AUTHORITY + "/expired");

	public static Map<String, Set<String>> DB_COLUMNS = new HashMap<String, Set<String>>();
	static{
		Set<String> f = new TreeSet<String>();
		f.add("id");
		f.add(TaskInfo.PROJECT_ID);
		f.add(TaskInfo.CATEGORY);
		f.add(TaskInfo.PRIORITY);
		f.add(TaskInfo.NAME);
		f.add(TaskInfo.RESULT);
		f.add(TaskInfo.STATUS);
		f.add(TaskInfo.USER);
		f.add(TaskInfo.HOST);
		f.add(TaskInfo.SW_BUILD);		
		f.add(TaskInfo.TEST_COUNT);
		f.add(TaskInfo.RESULT_COUNT);
		f.add(TaskInfo.RESULT_PASS);
		f.add(TaskInfo.RESULT_FAIL);
		f.add(TaskInfo.CREATED_DATE);
		f.add(TaskInfo.MODIFIED_DATE);		
		DB_COLUMNS.put(TaskInfo.DB_TABLE_NAME, f);
		
	}
	
    /**
     * Notes table
     */
    public static final class Project implements BaseColumns {
        public static final String DB_TABLE_NAME = "project";
    	
        /**
         * The content:// style URL for this table
         */
        public static final Uri CONTENT_URI
                = Uri.parse("content://" + AUTHORITY + "/projects");
        
        public static final String CONTENT_TYPE = "vnd.android.cursor.dir/vnd.coci.projects";
        public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/vnd.coci.project";

        /**
         * The default sort order for this table
         */
        public static final String DEFAULT_SORT_ORDER = "modified DESC";

        /**
         * The title of the note
         * <P>Type: TEXT</P>
         */
        public static final String NAME = "name";

        public static final String STATUS = "status"; 
        /**
         * The timestamp for when the note was created
         * <P>Type: INTEGER (long)</P>
         */
        public static final String CREATED_DATE = "created";

        /**
         * The timestamp for when the note was last modified
         * <P>Type: INTEGER (long)</P>
         */
        public static final String MODIFIED_DATE = "modified";
        
        public static final String SYNC_TIME = "sync_time";
    }
    
    /**
     * Notes table
     */
    public static final class TaskInfo implements BaseColumns {
        public static final String DB_TABLE_NAME = "task_info";

        public static final Uri TASK_LIST_URI
        = Uri.parse("content://" + AUTHORITY + "/tasks");
        
        public static final Uri TASK_QUEUE_URI
        = Uri.parse("content://" + AUTHORITY + "/task_queue");
        public static final Uri TASK_RESULT_URI
        = Uri.parse("content://" + AUTHORITY + "/task_result");

        
        /**
         * The content:// style URL for this table
         */
        public static final Uri CONTENT_URI
                = Uri.parse("content://" + AUTHORITY + "/tasks");
        
        public static final String CONTENT_TYPE = "vnd.android.cursor.dir/vnd.coci.tasks";
        public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/vnd.coci.task";

        /**
         * The default sort order for this table
         */
        public static final String DEFAULT_SORT_ORDER = "modified DESC";
    
        public static final String PROJECT_ID = "project_id";
        public static final String CATEGORY = "category";
        public static final String PRIORITY = "priority";
        public static final String NAME = "name";
        public static final String RESULT = "result";
        public static final String STATUS = "status";
        public static final String USER = "user_name";
        
        public static final String HOST = "host_ip";
        public static final String SW_BUILD = "sw_build";
        
        public static final String TEST_COUNT = "test_count";
        
        public static final String RESULT_COUNT = "result_count";
        public static final String RESULT_PASS = "result_pass";
        public static final String RESULT_FAIL = "result_fail";
        
        public static final String DETAIL = "detail";
        //public static final String DETAIL = "detail";        
        
        /**
         * The timestamp for when the note was created
         * <P>Type: INTEGER (long)</P>
         */
        public static final String CREATED_DATE = "created";

        /**
         * The timestamp for when the note was last modified
         * <P>Type: INTEGER (long)</P>
         */
        public static final String MODIFIED_DATE = "modified";
        public static final String SYNC_TIME = "sync_time";
    }    

}
