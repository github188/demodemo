package com.coci.nsn;

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
import com.coci.provider.AreaCI.Project;

public class AreaCiProvider extends ContentProvider {
	
    private static final String TAG = "areaci_provider";

    private static final String DATABASE_NAME = "area_ci.db";
    private static final int DATABASE_VERSION = 2;
    //private static final String NOTES_TABLE_NAME = "notes";

   // private static HashMap<String, String> sNotesProjectionMap;
   // private static HashMap<String, String> sLiveFolderProjectionMap;

    private static final int PROJECTS = 1;
    private static final int PROJECT_ID = 2;

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
                    + Project.NAME + " TEXT,"
                    + Project.STATUS + " TEXT,"
                    + Project.CREATED_DATE + " INTEGER,"
                    + Project.MODIFIED_DATE + " INTEGER"
                    + ");");
            
            this.initTestData(db);
        }

        @Override
        public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
            Log.w(TAG, "Upgrading database from version " + oldVersion + " to "
                    + newVersion + ", which will destroy all old data");
            db.execSQL("DROP TABLE IF EXISTS notes");
            onCreate(db);
        }
        
        private void initTestData(SQLiteDatabase db){
            ContentValues values = new ContentValues();
            values.put(Project.NAME, "first project");
            values.put(Project.STATUS, "idle");
            db.insert(Project.DB_TABLE_NAME, Project.NAME, values);
            values.put(Project.NAME, "first project2");
            values.put(Project.STATUS, "running");
            db.insert(Project.DB_TABLE_NAME, Project.NAME, values);
        	
        }
    }

    private DatabaseHelper mOpenHelper;	

	@Override
	public boolean onCreate() {
		mOpenHelper = new DatabaseHelper(getContext());		
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
		    default:
		    	throw new IllegalArgumentException("Unknown URI " + uri);
		}
		getContext().getContentResolver().notifyChange(uri, null);
		return count;
	}

	@Override
    public int update(Uri uri, ContentValues values, String where, String[] whereArgs) {
        SQLiteDatabase db = mOpenHelper.getWritableDatabase();
        int count;
        switch (sUriMatcher.match(uri)) {
	        case PROJECTS:
	            count = db.update(Project.DB_TABLE_NAME, values, where, whereArgs);
	            break;	
	        case PROJECT_ID:
	            String noteId = uri.getPathSegments().get(1);
	            count = db.update(Project.DB_TABLE_NAME, values, Project._ID + "=" + noteId
	                    + (!TextUtils.isEmpty(where) ? " AND (" + where + ')' : ""), whereArgs);
	            break;	
	        default:
	            throw new IllegalArgumentException("Unknown URI " + uri);
        }
        getContext().getContentResolver().notifyChange(uri, null);
        return count;
    }
	
    static {
        sUriMatcher = new UriMatcher(UriMatcher.NO_MATCH);
        sUriMatcher.addURI(AreaCI.AUTHORITY, "projects", PROJECTS);
        sUriMatcher.addURI(AreaCI.AUTHORITY, "projects/#", PROJECT_ID);     
     }	

}
