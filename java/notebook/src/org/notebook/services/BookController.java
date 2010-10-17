package org.notebook.services;

import org.notebook.cache.NoteBook;
import org.notebook.gui.MenuToolbar;
import org.notebook.gui.MenuToolbar.BookAction;

public interface BookController {
	public void processEvent(BookAction e);
	public void dispatchEvent(String command);
	
	public boolean visibleTrayIcon();
	public boolean setVisibleTrayIcon(boolean visible);
	public boolean runingJNLP();
	public boolean runingSandBox();
	
	public NoteBook getNoteBook();
}
