package org.notebook.services;

import java.io.File;
import java.lang.reflect.Method;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import javax.swing.Action;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.DataStorage;
import org.notebook.cache.DocumentDefine;
import org.notebook.cache.LocalFileStorage;
import org.notebook.gui.AboutDialog;
import org.notebook.gui.MainFrame;
import org.notebook.gui.MenuToolbar.BookAction;

public class DefaultBookController implements BookController{
	private static Log log = LogFactory.getLog("DefaultBookController");
	private DataStorage storage = null;
	//private SyncService sync = null;

	//private NoteBook book = null;
	private MainFrame mainFrame = null;
	private ThreadPoolExecutor syncThread = null;
	private ThreadPoolExecutor actionThread = null;
	private MenuAction menuActions = new MenuAction();
	
	private boolean runningJNLP = false;
	private boolean runningSandBox = false;
	private boolean visibleTrayIcon = false;
	private int autoLoginTimes = 0;
	//初始化了本地NoteBook路径.
	private boolean initedBook = false;
	private boolean isWin = false;
	
	public DefaultBookController(MainFrame mainJFrame, boolean isJNLP, boolean isSandBox){
		isWin = System.getProperty("sun.desktop").equals("windows");
		this.mainFrame = mainJFrame;
		this.runningJNLP = isJNLP;
		this.runningSandBox = isSandBox;
		
		syncThread = new ThreadPoolExecutor(2, 5, 60, 
				TimeUnit.SECONDS, 
				new LinkedBlockingDeque<Runnable>(100));
		actionThread = new ThreadPoolExecutor(1, 1, 60, 
				TimeUnit.SECONDS, 
				new LinkedBlockingDeque<Runnable>(100));
		
		//this.sync.start();
		
		this.storage = createPersistenceService();
	}
	
	public boolean runingJNLP(){return this.runningJNLP;}
	public boolean runingSandBox(){return this.runningSandBox;};	
	
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
	
	public void processEvent(BookAction e){
		final String HANDLER = "__actionHander";
		String command = (String)e.getValue(Action.ACTION_COMMAND_KEY);
		log.debug("processEvent:" + command);
		Method m = (Method)e.getValue(HANDLER);
		if(m == null){
			try {
				m = MenuAction.class.getMethod(command, new Class[]{BookAction.class});
				if(m != null){
					e.putValue(HANDLER, m);
					log.debug("get action process for menu, " + command);
				}
			} catch (Exception e1) {
			}
		}
		if(m == null){
			log.warn("Not found action handler, for " + command);
		}else {
			try {
				m.invoke(this.menuActions, new Object[]{e});
			} catch (Exception e1) {
				log.error(e1.toString(), e1.getCause());
			}
		}
	}
	
	public void dispatchEvent(String command, Object param) {
		BookAction action = this.mainFrame.menu.$(command);
		if(action != null){
			action.actionPerformed(null, param);
		}else {
			log.warn("Not found action:" + command);
		}
	}
	
	public void dispatchEvent(String command) {
		dispatchEvent(command, null);
	}	
	
	public boolean visibleTrayIcon(){
		return visibleTrayIcon;		
	}
	public boolean setVisibleTrayIcon(boolean visible){
		boolean old = visibleTrayIcon;
		visibleTrayIcon = visible;
		return old;
	}
	
	protected boolean authencation(){

		return false;
	}
	
	
	private void restoreLastStatus(){

	}
	
	class MenuAction {

		public void Loaded(BookAction event){
			log.info("applcation loaded...");
			DocumentDefine doc = storage.loadDocument("simple.cfg");
			mainFrame.updateDocumentDefine(doc);		
		}
		
		public void Exit(BookAction event) {
			log.info("shutdown applcation...");
			mainFrame.dispose();
			System.exit(0);
		}
		
		public void About(BookAction event){
			//mainFrame.showSettings();
			AboutDialog about = new AboutDialog(mainFrame);
			about.setLocationRelativeTo(mainFrame);
			about.setVisible(true);
		}

		public void Print(BookAction event){
			//mainFrame.showSettings();
			log.info("print view...");			
		}		
		
	}
}
