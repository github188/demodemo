package org.notebook.gui;

import org.notebook.cache.Category;

public interface SyncListener {
	public void checkUpload(Category c);
	public void checkDownload(Category c);
	
	public void removeLocal(Category c);
	public void updatedLocal(Category c);
	
	public void removeRemote(Category c);
	public void updateRemote(Category c);
	
	public void syncError(Category c, Exception e);
}
