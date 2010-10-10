package org.notebook.gui;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public interface SingleInstance {
	public static Log log = LogFactory.getLog(SingleInstance.class);
	public boolean checkRunning(MainFrame app);
}
