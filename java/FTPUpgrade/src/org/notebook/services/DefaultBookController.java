package org.notebook.services;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JSplitPane;
import javax.swing.SwingUtilities;
import javax.swing.border.EmptyBorder;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.Configuration;
import org.notebook.cache.DataStorage;
import org.notebook.cache.LocalFileStorage;
import org.notebook.cache.TaskStatus;
import org.notebook.events.BroadCastEvent;
import org.notebook.events.EventAction;
import org.notebook.gui.Events;
import org.notebook.gui.MainFrame;
import org.notebook.gui.MainTable;
import org.notebook.gui.MainTable.StatusModel;
import org.notebook.xui.XUIContainer;

public class DefaultBookController implements BookController{
	private static Log log = LogFactory.getLog("services");
	private DataStorage storage = null;
	private MainFrame mainFrame = null;
	private ThreadPoolExecutor syncThread = null;
	private MenuAction menuActions = new MenuAction();
	
	private boolean runningJNLP = false;
	private boolean runningSandBox = false;
	private boolean visibleTrayIcon = false;
	private boolean isWin = false;
	private FTPSyncService ftpSync = null;
	
	private JLabel fileInfo = null;
	private JLabel taskInfo = null;
	private JLabel toolInfo = null;
	private JProgressBar fileProgress = null;
	private JProgressBar toolProgress = null;	
	
		
	public DefaultBookController(boolean isJNLP, boolean isSandBox){
		isWin = System.getProperty("sun.desktop").equals("windows");
		this.runningJNLP = isJNLP;
		this.runningSandBox = isSandBox;
		
		syncThread = new ThreadPoolExecutor(2, 5, 60, 
				TimeUnit.SECONDS, 
				new LinkedBlockingDeque<Runnable>(100));
		//this.sync.start();
		
		this.storage = createPersistenceService();
	}
	
	public void setTopWindow(MainFrame mainJFrame){
		this.mainFrame = mainJFrame;		
	}
	
	public boolean runingJNLP(){return this.runningJNLP;}
	public boolean runingSandBox(){return this.runningSandBox;};	
	
	public Object getEventActions(){
		return this.menuActions;
	}
	
	private DataStorage createPersistenceService(){
		DataStorage  storge = null;		
		if(runningJNLP && runningSandBox){
			try {
				Class cl = Class.forName("org.notebook.cache.JNLPServiceStorage");
				storge = (DataStorage)cl.newInstance();
			}catch(ClassNotFoundException e) {
				log.error("failed to load JNLP Stroage", e);
			} catch (Exception e) {
				log.error("failed to create JNLP Stroage", e);
			}			
		}else {
			File root = null;
			if(isWin){
				root = new File(System.getenv("APPDATA"), ".notebook");
			}else {
				root = new File(System.getenv("HOME"), ".notebook");
			}
			storge = new LocalFileStorage(root);
		}
		return storge;
	}	
	
	public boolean visibleTrayIcon(){
		return visibleTrayIcon;		
	}
	public boolean setVisibleTrayIcon(boolean visible){
		boolean old = visibleTrayIcon;
		visibleTrayIcon = visible;
		return old;
	}
	
	public class MenuAction {
				
		@EventAction(order=1)
		public void Exit(BroadCastEvent event) {
			log.info("shutdown applcation...");
			/*
			DocumentDefine doc = mainFrame.saveDocumentDefine();
			try {
				OutputStreamWriter out = new OutputStreamWriter(System.out);
				doc.save(out);
				out.flush();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}*/
			mainFrame.dispose();
			System.exit(0);
		}
		
		@EventAction(order=1)
		public void XuiLoaded(final BroadCastEvent event){
			//event.
			XUIContainer xui = (XUIContainer)event.getSource();			
			JPanel padding = (JPanel)xui.getByName("padding");
			padding.setBorder(new EmptyBorder(10,10,10,10));
			
			JLabel info = (JLabel)xui.getByName("warn_info");
			info.setForeground(Color.red);
			
			JSplitPane l1 = (JSplitPane)xui.getByName("l1");
			JSplitPane l2 = (JSplitPane)xui.getByName("l2");
			l1.setBorder(new EmptyBorder(0, 0, 0, 0));
			l2.setBorder(new EmptyBorder(0, 0, 0, 0));
			
			//PreferredSize
			MainTable mainTable = (MainTable)xui.getByName("mainTable");
						
			JPanel status = (JPanel)xui.getByName("status");
			status.setPreferredSize(new Dimension(100, 40));
			
			JFrame main = (JFrame)xui.getByName("main");
			
			ftpSync = new FTPSyncService((StatusModel)mainTable.getModel(),
					 event.queue, main,
					 new Configuration(),
					 syncThread
					);
			
			main.addWindowListener(new WindowAdapter() {
		        public void windowClosing(WindowEvent ce) {
		        	System.exit(0);		        	
		        	//this.windowOpened(e)
		        }
		        
		        public void windowOpened(WindowEvent ce){
					syncThread.execute(new Runnable(){
						@Override
						public void run() {
							ftpSync.scanLocalPath();
					}});
		        }
		    });
			
			//注册FTP响应是事件。
			event.queue.registerAction(ftpSync);
			//event.queue.
			
			JButton connect = (JButton)xui.getByName("connect");
			connect.addActionListener(new ActionListener(){
				@Override
				public void actionPerformed(final ActionEvent arg0) {
					syncThread.execute(new Runnable(){
						@Override
						public void run() {
							ftpSync.connect();
					}});
				}		
			});			

			JButton upgrade = (JButton)xui.getByName("upgrade");		
			upgrade.addActionListener(new ActionListener(){
				@Override
				public void actionPerformed(final ActionEvent arg0) {
					syncThread.execute(new Runnable(){
						@Override
						public void run() {
							ftpSync.upgrade();
					}});
				}
			});			
			
			fileInfo = (JLabel)xui.getByName("fileInfo");
			fileInfo.setAlignmentX(JLabel.CENTER_ALIGNMENT);

			taskInfo = (JLabel)xui.getByName("taskInfo");
			taskInfo.setAlignmentX(JLabel.CENTER_ALIGNMENT);

			toolInfo = (JLabel)xui.getByName("toolInfo");
			toolInfo.setAlignmentX(JLabel.CENTER_ALIGNMENT);
			
			fileProgress = (JProgressBar)xui.getByName("fileProgress");
			toolProgress = (JProgressBar)xui.getByName("taskProgress");			
		}
		
		@EventAction(order=1)
		public void UpdateProgressBar(final BroadCastEvent event){
			final TaskStatus status = (TaskStatus)event.get(Events.PROGRESS_PARAM);
			if(status == null)return;
			if(SwingUtilities.isEventDispatchThread()){
				updateProcessBar(status);
			}else {
				SwingUtilities.invokeLater(new Runnable() {
		            public void run() {
		            	updateProcessBar(status);
		            }
		        });
			}			
		}
		
		private void updateProcessBar(TaskStatus status){
			String tInfo = String.format("已传文件%s个%1.2fM, 待传文件%s个%1.2fM",
					status.doneFiles, status.doneBytes /1024.0/1024.0,
					status.totalFiles - status.doneFiles, 
					(status.totalBytes - status.doneBytes) /1024.0/1024.0
					);
			String fInfo = null;
			if(status.curFile != null){
				fInfo = String.format("%s 进度.", status.curFile);
			}else {
				fInfo = "单个文件进度.";
			}
			toolInfo.setText(tInfo);
			fileInfo.setText(fInfo);
			
			fileProgress.setMaximum((int)status.fileBytes);
			fileProgress.setValue((int)status.doneFileByptes);
			
			toolProgress.setMaximum((int)status.totalBytes);
			toolProgress.setValue((int)status.doneBytes);			
		}
		
		public String toString(){
			return "BookController Actions";
		}
	}
}
