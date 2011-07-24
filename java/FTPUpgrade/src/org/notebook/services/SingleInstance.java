package org.notebook.services;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.gui.MainFrame;

public interface SingleInstance {
	public static Log log = LogFactory.getLog("SingleInstance");
	public boolean checkRunning(MainFrame app);
}
