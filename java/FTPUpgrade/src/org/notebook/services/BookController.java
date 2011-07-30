package org.notebook.services;

import javax.swing.JFrame;

public interface BookController {
	//public void processEvent(BroadCastEvent e);
	//public void dispatchEvent(String command);
	
	public boolean visibleTrayIcon();
	public boolean setVisibleTrayIcon(boolean visible);
	public boolean runingJNLP();
	public boolean runingSandBox();
	public Object getEventActions();	
	public void setTopWindow(JFrame mainJFrame);
	//public NoteBook getNoteBook();
}
