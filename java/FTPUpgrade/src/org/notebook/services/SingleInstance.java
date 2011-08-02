package org.notebook.services;

import javax.swing.JFrame;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public interface SingleInstance {
	public static Log log = LogFactory.getLog("SingleInstance");
	public boolean checkRunning(JFrame app);
}
