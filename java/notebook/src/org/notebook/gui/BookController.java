package org.notebook.gui;

import org.notebook.cache.Category;
import org.notebook.gui.MenuToolbar.BookAction;

public interface BookController {
	public void processEvent(BookAction e);
	public void syncNoteBook(Category cate);
	public void dispatchEvent(String command);
}
