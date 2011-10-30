package com.coci.provider;

import android.net.Uri;
import android.provider.BaseColumns;

public class AreaCI {
	public static final String AUTHORITY = "com.coci.provider.AreaCI";
	
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
    }	

}
