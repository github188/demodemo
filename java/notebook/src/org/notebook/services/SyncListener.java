package org.notebook.services;

public interface SyncListener {
	public void start(SyncTask task);
	public void done(SyncTask task);	
	public void syncError(SyncTask task, Exception e);
}
