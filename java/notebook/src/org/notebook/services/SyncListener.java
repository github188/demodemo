package org.notebook.services;

import org.notebook.cache.Category;

public interface SyncListener {
	public static final int CONFLICT_DIRTY = 1;
	public static final int CONFLICT_EXPIRED = 2;	
	public static final int UPDATE_FORCE = 1;
	public static final int UPDATE_IGNORE = -1;
	public static final int UPDATE_PADDING = 0;
	
	public void start(SyncTask task);
	public void done(SyncTask task);
	public void waiting();
	public void syncError(SyncTask task, Exception e);
	public int conflict(Category local, Category remote, int cause);

}
