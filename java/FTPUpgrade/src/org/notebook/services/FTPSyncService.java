package org.notebook.services;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.UpgradeModel;
import org.notebook.events.EventQueue;
import org.notebook.gui.Events;
import org.notebook.gui.MainTable.StatusModel;

public class FTPSyncService {
	private static Log log = LogFactory.getLog("ftp.sync");
	
	private StatusModel model = null;
	private EventQueue eventQueue = null;
	private JFrame win = null;
	
	public FTPSyncService(StatusModel model, EventQueue eventQueue, JFrame win){
		this.model = model;
		this.eventQueue = eventQueue;
		this.win = win;
	}
	
	/**
	 * 由界面触发，开始连接服务器。
	 */
	public void connect(){
		JOptionPane.showMessageDialog(win,
			    "功能还没有实现！",
			    "Error",
			    JOptionPane.ERROR_MESSAGE);
	}
	
	/**
	 * 由界面触发，开始执行升级操作。
	 */	
	public void upgrade(){
		JOptionPane.showMessageDialog(win,
			    "功能还没有实现！",
			    "Error",
			    JOptionPane.ERROR_MESSAGE);		
	}
	
	public void scanLocalPath(){
		//for()
		File root = new File(".");
		log.info("Scan local path:" + root.getAbsolutePath());
		
		Map<String, Object> param = new HashMap<String, Object>();
		param.put(Events.STATUS_PARAM, "当前目录：" + root.getAbsolutePath());
		eventQueue.fireEvent(Events.STATUS_EVENT, this, param);
		
		UpgradeModel m = null;
		for(File f: root.listFiles()){
			if(!f.isFile())continue;
			m = new UpgradeModel();
			m.source = f.getName();
			m.sourceSize = f.length();
			model.addUpgradeModel(m);
		}
		model.fireTableDataChanged();
	}

	public void listFiles(){
		
	}	
}
