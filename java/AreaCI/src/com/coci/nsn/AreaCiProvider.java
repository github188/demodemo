package com.coci.nsn;

import java.util.TreeSet;

import android.content.ContentProvider;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.content.UriMatcher;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.database.sqlite.SQLiteQueryBuilder;
import android.net.Uri;
import android.text.TextUtils;
import android.util.Log;

import com.coci.provider.AreaCI;
import com.coci.provider.AreaCI.Device;
import com.coci.provider.AreaCI.Project;
import com.coci.provider.AreaCI.TaskInfo;

public class AreaCiProvider extends ContentProvider {
	
    private static final String TAG = "areaci_provider";

    private static final String DATABASE_NAME = "area_ci.db";
    private static final int DATABASE_VERSION = 1;
    //private static final String NOTES_TABLE_NAME = "notes";

   // private static HashMap<String, String> sNotesProjectionMap;
   // private static HashMap<String, String> sLiveFolderProjectionMap;

    private static final int PROJECTS = 1;
    private static final int PROJECT_ID = 2;
    
    private static final int TASK_INFO = 3;
    private static final int TASK_INFO_ID = 4;
    private static final int EXPIRED_DATA = 5;
    
    private static final int TASK_QUEUE = 6;
    private static final int TASK_RESULT = 7;

    private static final int DEVICES = 8;
    private static final int DEVICE_ID = 9;
    
    //private static final int TASK_RESULT = 7;

    
    private static final UriMatcher sUriMatcher;

    /**
     * This class helps open, create, and upgrade the database file.
     */
    private static class DatabaseHelper extends SQLiteOpenHelper {

        DatabaseHelper(Context context) {
            super(context, DATABASE_NAME, null, DATABASE_VERSION);
        }

        @Override
        public void onCreate(SQLiteDatabase db) {
        	Log.i(TAG, "Create new table...");
            db.execSQL("CREATE TABLE " + Project.DB_TABLE_NAME + " ("
                    + Project._ID + " INTEGER PRIMARY KEY,"
                    + Project.PROJECT_ID + " TEXT,"
                    + Project.CATEGORY + " TEXT,"
                    + Project.NAME + " TEXT,"
                    + Project.STATUS + " TEXT,"
                    + Project.HOST_IP + " TEXT,"
                    + Project.TEST_COUNT + " TEXT,"
                    + Project.TASK_STATUS + " TEXT,"
                    + Project.CREATED_DATE + " INTEGER,"
                    + Project.MODIFIED_DATE + " INTEGER,"
                    + Project.SYNC_TIME + " INTEGER"
                    + ");");
            
            db.execSQL("CREATE TABLE " + TaskInfo.DB_TABLE_NAME + " ("
                    + TaskInfo._ID + " INTEGER PRIMARY KEY,"                    
                    + TaskInfo.PROJECT_ID + " TEXT,"
                    + TaskInfo.CATEGORY + " TEXT,"
                    + TaskInfo.PRIORITY + " INTEGER,"
                    + TaskInfo.NAME + " TEXT,"
                    + TaskInfo.RESULT + " TEXT,"
                    + TaskInfo.STATUS + " TEXT,"

                    + TaskInfo.USER + " TEXT,"
                    + TaskInfo.HOST + " TEXT,"
                    
                    + TaskInfo.SW_BUILD + " TEXT,"
                    + TaskInfo.TEST_COUNT + " INTEGER,"
                    + TaskInfo.RESULT_COUNT + " INTEGER,"
                    + TaskInfo.RESULT_PASS + " INTEGER,"
                    + TaskInfo.RESULT_FAIL + " INTEGER,"
                    + TaskInfo.DETAIL + " TEXT,"

                    + TaskInfo.CREATED_DATE + " TEXT,"
                    + TaskInfo.MODIFIED_DATE + " TEXT,"
                    + TaskInfo.START_DATE + " TEXT,"
                    + TaskInfo.DONE_DATE + " TEXT,"
                    + TaskInfo.SYNC_TIME + " INTEGER"
                    + ");");

            db.execSQL("CREATE TABLE " + Device.DB_TABLE_NAME + " ("
                    + Device._ID + " INTEGER PRIMARY KEY,"
                    + Device.PROJECT_ID + " TEXT,"
                    + Device.NAME + " TEXT,"
                    + Device.STATUS + " TEXT,"
                    + Device.HOST_IP + " TEXT,"
                    + Device.TEST_ENV + " TEXT,"
                    + Device.PKG_ENV + " TEXT,"
                    + Device.CUR_TASK + " TEXT,"
                    + Device.AVAILABLE + " TEXT,"
                    + Device.RESERVED + " TEXT,"
                    
                    + Device.CREATED_DATE + " INTEGER,"
                    + Device.MODIFIED_DATE + " INTEGER,"
                    + Device.SYNC_TIME + " INTEGER"
                    + ");");
        }

        @Override
        public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
            Log.w(TAG, "Upgrading database from version " + oldVersion + " to "
                    + newVersion + ", which will destroy all old data");
            db.execSQL("DROP TABLE IF EXISTS " + Project.DB_TABLE_NAME);
            db.execSQL("DROP TABLE IF EXISTS " + TaskInfo.DB_TABLE_NAME);
            db.execSQL("DROP TABLE IF EXISTS " + Device.DB_TABLE_NAME);
            onCreate(db);
        }
    }

    private DatabaseHelper mOpenHelper;	

	@Override
	public boolean onCreate() {
		mOpenHelper = new DatabaseHelper(getContext());
		
		/**
		 * 检查数据库版本更新。
		 */
		mOpenHelper.getWritableDatabase();
		//mOpenHelper.initTestData(mOpenHelper.getWritableDatabase());
		return true;
	}

    @Override
    public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs,
            String sortOrder) {
        SQLiteQueryBuilder qb = new SQLiteQueryBuilder();
        //qb.setTables(NOTES_TABLE_NAME);

        switch (sUriMatcher.match(uri)) {
        case PROJECTS:
            //qb.setProjectionMap(sNotesProjectionMap);
        	qb.setTables(Project.DB_TABLE_NAME);
            break;
        case PROJECT_ID:
        	qb.setTables(Project.DB_TABLE_NAME);
            qb.appendWhere(Project._ID + "=" + uri.getPathSegments().get(1));
            break;
        case TASK_INFO:
        	qb.setTables(TaskInfo.DB_TABLE_NAME);
        	break;
        case TASK_INFO_ID:
        	qb.setTables(TaskInfo.DB_TABLE_NAME);
            qb.appendWhere(TaskInfo._ID + "=" + uri.getPathSegments().get(1));
            break;
        case DEVICES:
        	qb.setTables(Device.DB_TABLE_NAME);
        	break;
        case DEVICE_ID:
        	qb.setTables(Device.DB_TABLE_NAME);
            qb.appendWhere(Device._ID + "=" + uri.getPathSegments().get(1));
            break;
        case TASK_QUEUE:
        	//在Queue里面超过1天没有更新的记录为过期记录不显示。
        	long startTime = System.currentTimeMillis() - 1000 * 60 * 60 * 24;
        	qb.setTables(TaskInfo.DB_TABLE_NAME);
            qb.appendWhere("status in ('pending', 'waiting', 'running', 'preparing') and sync_time > " + startTime);
            break;            
        case TASK_RESULT:
        	long startTime2 = System.currentTimeMillis() - 1000 * 60 * 60 * 24 * 3;
        	qb.setTables(TaskInfo.DB_TABLE_NAME);
            qb.appendWhere("status in ('timout', 'done', 'error') and sync_time > " + startTime2);
            break;           
            
        default:
            throw new IllegalArgumentException("Unknown URI " + uri);
        }

        // If no sort order is specified use the default
        String orderBy;
        if (TextUtils.isEmpty(sortOrder)) {
            orderBy = AreaCI.Project.DEFAULT_SORT_ORDER;
        } else {
            orderBy = sortOrder;
        }
        
        if(selection != null && selection.trim().length() > 0){
        	qb.appendWhere(" and " + selection);
        }

        // Get the database and run the query
        SQLiteDatabase db = mOpenHelper.getReadableDatabase();
        Cursor c = qb.query(db, projection, selection, selectionArgs, null, null, orderBy);

        // Tell the cursor what uri to watch, so it knows when its source data changes
        c.setNotificationUri(getContext().getContentResolver(), uri);
        return c;
	}

	@Override
	public String getType(Uri paramUri) {
        switch (sUriMatcher.match(paramUri)) {
	        case PROJECTS:
	            return Project.CONTENT_TYPE;
	        case PROJECT_ID:
	            return Project.CONTENT_ITEM_TYPE;
	
	        default:
	            throw new IllegalArgumentException("Unknown URI " + paramUri);
        }
	}

	/**
	 * 
	 * @param ContentValues -- 一个Map类型的对象。
	 */
	@Override
	public Uri insert(Uri paramUri, ContentValues initialValues) {
        // Validate the requested uri
        if (sUriMatcher.match(paramUri) != PROJECTS) {
            throw new IllegalArgumentException("Unknown URI " + PROJECTS);
        }

        ContentValues values;
        if (initialValues != null) {
            values = new ContentValues(initialValues);
        } else {
            values = new ContentValues();
        }

        Long now = Long.valueOf(System.currentTimeMillis());
        // Make sure that the fields are all set
        if (values.containsKey(Project.CREATED_DATE) == false) {
            values.put(Project.CREATED_DATE, now);
        }
        if (values.containsKey(Project.MODIFIED_DATE) == false) {
            values.put(Project.MODIFIED_DATE, now);
        }

        SQLiteDatabase db = mOpenHelper.getWritableDatabase();
        long rowId = db.insert(Project.DB_TABLE_NAME, Project.NAME, values);
        if (rowId > 0) {
            Uri noteUri = ContentUris.withAppendedId(Project.CONTENT_URI, rowId);
            getContext().getContentResolver().notifyChange(noteUri, null);
            return noteUri;
        }
        throw new SQLException("Failed to insert row into " + paramUri);
	}

	@Override
	public int delete(Uri uri, String where, String[] whereArgs) {
		SQLiteDatabase db = mOpenHelper.getWritableDatabase();
		int count;
		switch (sUriMatcher.match(uri)) {
		    case PROJECTS:
		        count = db.delete(Project.DB_TABLE_NAME, where, whereArgs);
		        break;		
		    case PROJECT_ID:
		        String noteId = uri.getPathSegments().get(1);
		        count = db.delete(Project.DB_TABLE_NAME, Project._ID + "=" + noteId
		        		+ (!TextUtils.isEmpty(where) ? " AND (" + where + ')' : ""), whereArgs);
		        break;
		    case EXPIRED_DATA:
		    	count = deleteExpiredData();
		    	break;
		    default:
		    	throw new IllegalArgumentException("Unknown URI " + uri);
		}
		getContext().getContentResolver().notifyChange(uri, null);
		return count;
	}

	@Override
    public int update(Uri uri, ContentValues values, String where, String[] whereArgs) {
        SQLiteDatabase db = mOpenHelper.getWritableDatabase();
        int count = 0;
        switch (sUriMatcher.match(uri)) {
	        case PROJECTS:
	            updateProject(values, db);
	            break;	
	        case PROJECT_ID:
	        	count = db.update(Project.DB_TABLE_NAME, values, where, whereArgs);
	        	return 0;
	        case DEVICES:
	        	updateDevice(values, db);
	        	return 0;
	        case TASK_INFO:
	        	updateTasks(values, db);
	        	return 0; //避免每一个task 更新了都做出notify操作， task update应该是一个批量的操作。
	        default:
	            throw new IllegalArgumentException("Unknown URI " + uri);
        }
        getContext().getContentResolver().notifyChange(uri, null);
        return count;
    }

	private void updateDevice(ContentValues values, SQLiteDatabase db){
		//int taskId = values.getAsInteger("id");
		//values.remove("id");
		String hostIp = values.getAsString(Device.HOST_IP);
		values.remove(Device.HOST_IP);
		values.put(Device.SYNC_TIME, Long.valueOf(System.currentTimeMillis()));
        int count = db.update(Device.DB_TABLE_NAME, values, Device.HOST_IP + "=?", new String[]{hostIp});
		if(count == 0){
			Log.d(TAG, String.format("insert new device '%s:%s'", hostIp,
					values.getAsString("name")));
			values.put(Device.HOST_IP, hostIp);
			db.insert(Device.DB_TABLE_NAME, Device.NAME, values);
		}else {
			Log.d(TAG, String.format("update device '%s:%s'", hostIp,
					values.getAsString("name")));
		}
	}	
	
	private void updateProject(ContentValues values, SQLiteDatabase db){
		String taskId = values.getAsString("id");
		values.remove("id");
		values.put(Project.SYNC_TIME, Long.valueOf(System.currentTimeMillis()));
        int count = db.update(Project.DB_TABLE_NAME, values, Project._ID + "=?", new String[]{taskId});
		if(count == 0){
			Log.d(TAG, String.format("insert new project '%s:%s'", taskId,
					values.getAsString("name")));
			values.put(Project._ID, taskId);
			db.insert(Project.DB_TABLE_NAME, Project.NAME, values);
		}else {
			Log.d(TAG, String.format("update prject '%s:%s'", taskId,
					values.getAsString("name")));
		}
	}
	
	private void updateTasks(ContentValues values, SQLiteDatabase db){
		String taskId = values.getAsString("id");
		values.remove("id");
		values.put(TaskInfo.SYNC_TIME, Long.valueOf(System.currentTimeMillis()));
        int count = db.update(TaskInfo.DB_TABLE_NAME, values, TaskInfo._ID + "=?", new String[]{taskId});
		if(count == 0){
			Log.d(TAG, String.format("insert new task '%s:%s'", taskId,
					values.getAsString("name")));
			values.put(TaskInfo._ID, taskId);
			db.insert(TaskInfo.DB_TABLE_NAME, TaskInfo.NAME, values);
		}else {
			Log.d(TAG, String.format("update task '%s:%s'", taskId,
					values.getAsString("name")));
		}
	}
	
	private int deleteExpiredData(){
		SQLiteDatabase db = mOpenHelper.getWritableDatabase();
		
		/*
		 * delete from xxx where _id not in (select _id from xxx order by sync_time desc limit 500); 
		 */			
		int count = 0, tmp =0;
		count += db.delete(TaskInfo.DB_TABLE_NAME, 
				"_id not in (select _id from " + TaskInfo.DB_TABLE_NAME + " order by sync_time desc limit 500)", null);
		Log.d(TAG, "deleted expired task data, count:" + count);
		
		tmp = db.delete(Project.DB_TABLE_NAME, 
				"_id not in (select _id from " + Project.DB_TABLE_NAME + " order by sync_time desc limit 100)", null);
		count += tmp;
		Log.d(TAG, "deleted expired prject data, count:" + tmp);
		
		tmp = db.delete(Device.DB_TABLE_NAME, 
				"host_ip not in (select host_ip from " + Device.DB_TABLE_NAME + " order by sync_time desc limit 100)", null);
		count += tmp;
		Log.d(TAG, "deleted expired device data, count:" + tmp);
		return count;
	}
	
    static {
        sUriMatcher = new UriMatcher(UriMatcher.NO_MATCH);
        sUriMatcher.addURI(AreaCI.AUTHORITY, "projects", PROJECTS);
        sUriMatcher.addURI(AreaCI.AUTHORITY, "projects/#", PROJECT_ID);
        
        sUriMatcher.addURI(AreaCI.AUTHORITY, "tasks", TASK_INFO);
        sUriMatcher.addURI(AreaCI.AUTHORITY, "tasks/#", TASK_INFO_ID);
        
        sUriMatcher.addURI(AreaCI.AUTHORITY, "devices", DEVICES);
        sUriMatcher.addURI(AreaCI.AUTHORITY, "devices/#", DEVICE_ID);        
        
        sUriMatcher.addURI(AreaCI.AUTHORITY, "expired", EXPIRED_DATA);
        sUriMatcher.addURI(AreaCI.AUTHORITY, "task_queue", TASK_QUEUE);
        sUriMatcher.addURI(AreaCI.AUTHORITY, "task_result", TASK_RESULT);        
     }	

}
