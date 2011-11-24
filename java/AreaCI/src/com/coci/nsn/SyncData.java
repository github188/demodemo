package com.coci.nsn;

import java.util.List;

import android.content.ContentValues;

public class SyncData {
	public List<ContentValues> taskList = null;
	public List<ContentValues> projectList = null;
	public List<ContentValues> deviceList = null;
	
	public boolean hasMore = false;
	public boolean isOK = false;
}
