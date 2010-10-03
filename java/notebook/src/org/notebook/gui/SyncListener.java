package org.notebook.gui;

import org.notebook.cache.Category;

public interface SyncListener {
	public void removeLocal(Category c);
	public void updateLocal(Category c);
	
	public void removeRemote(Category c);
	public void updateRemote(Category c);
	
	public void syncError(Exception e);
}
