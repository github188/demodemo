package org.notebook.services;

import java.awt.AWTEvent;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.event.AWTEventListener;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.util.Date;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
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
import org.notebook.cache.UpgradeModel;
import org.notebook.events.BroadCastEvent;
import org.notebook.events.EventAction;
import org.notebook.gui.AboutDialog;
import org.notebook.gui.Events;
import org.notebook.gui.MainTable;
import org.notebook.gui.MainTable.TableModel;
import org.notebook.gui.SettingsDialog;
import org.notebook.xui.XUIContainer;

public class DefaultBookController implements BookController{
	private static Log log = LogFactory.getLog("services");
	private DataStorage storage = null;
	private JFrame mainFrame = null;
	private ThreadPoolExecutor syncThread = null;
	private EventHandler menuActions = new EventHandler();
	
	private boolean runningJNLP = false;
	private boolean runningSandBox = false;
	private boolean visibleTrayIcon = false;
	private boolean isWin = false;
	private FTPSyncService ftpSync = null;
	
	private JLabel fileInfo = null;
	private JLabel taskInfo = null;
	private JLabel toolInfo = null;
	
	private JLabel server_info = null;
	private JLabel client_info = null;
	private TableModel tableModel = null;
	
	private JProgressBar fileProgress = null;
	private JProgressBar toolProgress = null;	
	private XUIContainer xui = null;
	private Configuration config = new Configuration();
	private String curMode = null;
	private DatabaseService database = null;
	private ReentrantLock ftpLock = new ReentrantLock();
	
		
	public DefaultBookController(boolean isJNLP, boolean isSandBox){
		isWin = System.getProperty("sun.desktop").equals("windows");
		this.runningJNLP = isJNLP;
		this.runningSandBox = isSandBox;
		
		syncThread = new ThreadPoolExecutor(4, 10, 60, 
				TimeUnit.SECONDS, 
				new LinkedBlockingDeque<Runnable>(100));
		//this.sync.start();
		
		this.storage = createPersistenceService();
		
		config.load(new File(storage.getRootPath(), "ftp.properties"));
		config.loadRegistry();	
		database = new DatabaseService(config);
	}
	
	public void setTopWindow(JFrame mainJFrame){
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
				root = new File(System.getenv("APPDATA"), ".ftpupgrade");
			}else {
				root = new File(System.getenv("HOME"), ".ftpupgrade");
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
	
	public void setWorkMode(String name){
		JPanel actionPanel = (JPanel)xui.getByName("actionPanel");
		CardLayout layout = (CardLayout)actionPanel.getLayout();
		if(name != null){
			curMode = name;
			layout.show(actionPanel, name);
		}else {
			layout.first(actionPanel);
		}
	}
	
	private Image appIcon(){
		ImageIcon icon = null;
		String iconUrl = "org/notebook/gui/images/application.png";
		try{
			icon = new ImageIcon(this.getClass()
					.getClassLoader().getResource(iconUrl));
		}catch(Exception e){
			log.error(e.toString(), e);
		}
		return icon.getImage();				
	}
	
	private Image appIcon16(){
		return appIcon().getScaledInstance(16, 16, Image.SCALE_SMOOTH);			
	}	
	
	/**
	 * 注册系统快捷键。
	 */
	public void initSystemShortCut(){
		Toolkit toolkit =  Toolkit.getDefaultToolkit();
		toolkit.addAWTEventListener(new AWTEventListener(){ 
				public void eventDispatched(AWTEvent event){
					if(event.getID() != KeyEvent.KEY_PRESSED)
						return;
					if(event instanceof KeyEvent){
						KeyEvent ke = (KeyEvent)event;
						 if((ke.getModifiers() & KeyEvent.CTRL_MASK) == KeyEvent.CTRL_MASK){
							 if(ke.getKeyCode() == KeyEvent.VK_F9){
								 //config.saveRegistry();
								 SettingsDialog x = new SettingsDialog(mainFrame, config, database);
								 x.setLocationRelativeTo(mainFrame);
								 x.setVisible(true);
							 }else if(ke.getKeyCode() == KeyEvent.VK_F10){
								SwingUtilities.invokeLater(new Runnable() {
						            public void run() {
										 AboutDialog x = new AboutDialog(mainFrame);
										 x.setLocationRelativeTo(mainFrame);
										 x.setVisible(true);
						            }
						        });									 
							 }
						 }
					}
				}},
				AWTEvent.KEY_EVENT_MASK);		
	}
	
	public class EventHandler {
				
		@EventAction(order=1)
		public void Exit(BroadCastEvent event) {
			log.info("shutdown applcation...");
			mainFrame.dispose();
			System.exit(0);
		}
		
		/**
		 * 窗口刚创建，还在没有显示时调用。
		 * @param event
		 */
		@EventAction(order=1)
		public void XuiLoaded(final BroadCastEvent event){
			//event.
			xui = (XUIContainer)event.getSource();			
			JPanel padding = (JPanel)xui.getByName("padding");
			padding.setBorder(new EmptyBorder(10,10,10,10));
			
			JLabel info = (JLabel)xui.getByName("warn_info");
			info.setForeground(Color.red);

			server_info = (JLabel)xui.getByName("server_info");
			server_info.setForeground(Color.red);

			client_info = (JLabel)xui.getByName("client_info");
			client_info.setForeground(Color.red);
			
			JSplitPane l1 = (JSplitPane)xui.getByName("l1");
			JSplitPane l2 = (JSplitPane)xui.getByName("l2");
			l1.setBorder(new EmptyBorder(0, 0, 0, 0));
			l2.setBorder(new EmptyBorder(0, 0, 0, 0));
			
			//PreferredSize
			
			JFrame main = (JFrame)xui.getByName("main");
			//config.saveRegistry();			

			main.addWindowListener(new WindowAdapter() {
		        public void windowClosing(WindowEvent ce) {
		        	event.fireNewEvent("Exit", ce.getSource(), null);        	
		        }		        
		        public void windowOpened(WindowEvent ce){
		        	event.fireNewEvent("windowOpened", ce.getSource(), null);
		        }
		    });
			
			//注册FTP响应是事件。
			//event.queue.registerAction(ftpSync);
			//event.queue.
			
			fileInfo = (JLabel)xui.getByName("fileInfo");
			fileInfo.setAlignmentX(JLabel.CENTER_ALIGNMENT);

			taskInfo = (JLabel)xui.getByName("taskInfo");
			taskInfo.setAlignmentX(JLabel.CENTER_ALIGNMENT);

			toolInfo = (JLabel)xui.getByName("toolInfo");
			toolInfo.setAlignmentX(JLabel.CENTER_ALIGNMENT);
			
			fileProgress = (JProgressBar)xui.getByName("fileProgress");
			fileProgress.setStringPainted(true);
			toolProgress = (JProgressBar)xui.getByName("taskProgress");
			toolProgress.setStringPainted(true);
			
			MainTable mainTable = (MainTable)xui.getByName("mainTable");
			tableModel = (TableModel)mainTable.getModel();
			//System.out.println("run_mode:" + System.getProperty("run_mode"));
			
			main.setIconImage(appIcon16());
			setWorkMode(System.getProperty("run_mode", "simple"));
		}
		
		/**
		 * 窗口显示出来以后调用。
		 * @param event
		 */
		@EventAction(order=1)
		public void windowOpened(final BroadCastEvent event){
			ftpSync = new FTPSyncService(tableModel,
					 event.queue, mainFrame,
					 config,
					 syncThread,
					 database
					);
			if(curMode != null && curMode.equals("server")){
				ftpSync.setMode(FTPSyncService.UPLOAD_MODE);
			}
			
			initSystemShortCut();			
			
			syncThread.execute(new Runnable(){
				@Override
				public void run() {
					boolean isOnlyZip = curMode != null && curMode.equals("client");
					ftpSync.scanLocalPath(isOnlyZip);						

					if(curMode != null && !curMode.equals("simple")){
						if(!database.testConnection()){
        					JOptionPane.showMessageDialog(mainFrame,
        						    "数据库连接失败, 请检查参数配置。",
        						    "错误",
        						    JOptionPane.ERROR_MESSAGE);							
						}
					}
			}});
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
		
		@EventAction(order=1)
		public void connect(final BroadCastEvent event){
			syncThread.execute(new Runnable(){
				@Override
				public void run() {
					if(ftpLock.tryLock()){
						ftpSync.connect(curMode);
						ftpLock.unlock();
					}else {
						JOptionPane.showMessageDialog(mainFrame,
							    "当前正在传输数据！",
							    "消息",
							    JOptionPane.INFORMATION_MESSAGE);						
					}
			}});
		}			
		
		@EventAction(order=1)
		public void simpleDown(final BroadCastEvent event){
			//
			//JButton button = (JButton)event.getSource();
			syncThread.execute(new Runnable(){
				@Override
				public void run() {
					if(ftpLock.tryLock()){
						ftpSync.simpleDownload();
					}else {
						JOptionPane.showMessageDialog(mainFrame,
							    "当前正在传输数据！",
							    "消息",
							    JOptionPane.INFORMATION_MESSAGE);						
					}						
			}});			
		}

		@EventAction(order=1)
		public void serverUpgrade(final BroadCastEvent event){
			//
			//JButton button = (JButton)event.getSource();
			syncThread.execute(new Runnable(){
				@Override
				public void run() {
					if(ftpLock.tryLock()){
						ftpSync.serverUpgrade();
					}else {
						JOptionPane.showMessageDialog(mainFrame,
							    "当前正在传输数据！",
							    "消息",
							    JOptionPane.INFORMATION_MESSAGE);						
					}
			}});
		}

		@EventAction(order=1)
		public void clientUpgrade(final BroadCastEvent event){
			//
			//JButton button = (JButton)event.getSource();
			syncThread.execute(new Runnable(){
				@Override
				public void run() {
					if(ftpLock.tryLock()){
						ftpSync.clientUpgrade();
					}else {
						JOptionPane.showMessageDialog(mainFrame,
							    "当前正在传输数据！",
							    "消息",
							    JOptionPane.INFORMATION_MESSAGE);						
					}
			}});			
		}
		
		@EventAction(order=1)
		public void FtpDownloadDone(final BroadCastEvent event){
			String path = (String)event.get(Events.FTP_PATH_PARAM);
			if(path != null){
				UpgradeModel row = null;
				for(int i = 0; i < tableModel.getRowCount(); i++){
					row = tableModel.getData().get(i);
					if(row.dst.equals(path)){
						row.isUpdate = false;
						row.updateDate = new Date();
						row.source = row.dst;
						row.sourceSize = row.dstSize;
						tableModel.fireTableRowsUpdated(i, i);
						//log.info("update:" + i);
						break;
					}
				}
			}			
		}

		@EventAction(order=1)
		public void FtpUploadDone(final BroadCastEvent event){
			String path = (String)event.get(Events.FTP_PATH_PARAM);
			if(path != null){
				UpgradeModel row = null;
				for(int i = 0; i < tableModel.getRowCount(); i++){
					row = tableModel.getData().get(i);
					if(row.source.equals(path)){
						row.updateDate = new Date();
						row.dst = row.source;
						row.dstSize = row.sourceSize;
						tableModel.fireTableRowsUpdated(i, i);
						break;
					}
				}
			}
		}
		
		@EventAction(order=1)
		public void pause(final BroadCastEvent event){
			//
			JButton button = (JButton)event.getSource();
			//button.getActionCommand()
			//if(button)
			syncThread.execute(new Runnable(){
				@Override
				public void run() {
					ftpSync.pause();
			}});			
		}		
		
		
		private void updateProcessBar(TaskStatus status){
			long doneBytes = status.doneBytes;
			if(!status.isZip && status.isUploading){
				doneBytes += status.doneFileByptes;
			}
			String tInfo = String.format("已传文件%s个%1.2fM, 待传文件%s个%1.2fM",
					status.doneFiles, doneBytes /1024.0/1024.0,
					status.totalFiles - status.doneFiles, 
					(status.totalBytes - doneBytes) /1024.0/1024.0
					);
			String fInfo = null;
			if(status.curFile != null){
				if(status.isZip){
					fInfo = String.format("%s 压缩进度.", status.curFile);
				}else {
					if(curMode.equals("server")){
						fInfo = String.format("%s 上传进度.", status.curFile);
					}else {
						fInfo = String.format("%s 下载进度.", status.curFile);
					}
				}
			}else {
				fInfo = "单个文件进度.";
			}
			toolInfo.setText(tInfo);
			fileInfo.setText(fInfo);
			
			fileProgress.setMaximum((int)status.fileBytes);
			//fileProgress.setm
			fileProgress.setValue((int)status.doneFileByptes);
			
			toolProgress.setMaximum((int)status.totalBytes);
			toolProgress.setValue((int)(doneBytes));
		}
		
		
		@EventAction(order=1)
		public void UpdateStateBar(final BroadCastEvent event){
			if(event.get(Events.STATUS_WARN) != null && 
			  (Boolean)event.get(Events.STATUS_WARN)){
			  final String text = (String)event.get(Events.STATUS_PARAM);
			  if(curMode.equals("server") && server_info != null){
					SwingUtilities.invokeLater(new Runnable() {
			            public void run() {
			            	server_info.setText(text);
			            }
			        });
			  }else if(curMode.equals("client") && client_info != null){
					SwingUtilities.invokeLater(new Runnable() {
			            public void run() {
			            	client_info.setText(text);
			            }
			        });
			  }
			}
			//setText((String)event.get(Events.STATUS_PARAM));
		}		
		
		public String toString(){
			return "BookController Actions";
		}
	}
}
