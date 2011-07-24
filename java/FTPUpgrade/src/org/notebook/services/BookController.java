package org.notebook.services;

import org.notebook.events.BroadCastEvent;
import org.notebook.gui.MainFrame;

public interface BookController {
	//public void processEvent(BroadCastEvent e);
	//public void dispatchEvent(String command);
	
	public boolean visibleTrayIcon();
	public boolean setVisibleTrayIcon(boolean visible);
	public boolean runingJNLP();
	public boolean runingSandBox();
	public Object getEventActions();	
	public void setTopWindow(MainFrame mainJFrame);
	//public NoteBook getNoteBook();
}
