package org.notebook.services;

import java.io.File;
import java.lang.reflect.Method;
import java.util.Vector;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import javax.swing.Action;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;
import javax.swing.event.TreeExpansionEvent;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.event.TreeWillExpandListener;
import javax.swing.tree.ExpandVetoException;
import javax.swing.tree.TreePath;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.Category;
import org.notebook.cache.DataStorage;
import org.notebook.cache.LocalFileStorage;
import org.notebook.cache.NoteBook;
import org.notebook.cache.NoteMessage;
import org.notebook.gui.AboutDialog;
import org.notebook.gui.LoginDailog;
import org.notebook.gui.MainFrame;
import org.notebook.gui.MenuToolbar;
import org.notebook.gui.MenuToolbar.BookAction;
import org.notebook.gui.SyncStatusDailog;
import org.notebook.io.AuthcationException;

public class DefaultBookController implements BookController{
	private static Log log = LogFactory.getLog("DefaultBookController");
	private DataStorage storage = null;
	private SyncService sync = null;

	private NoteBook book = null;
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
		
		this.sync = new SyncService();
		this.sync.addListener(new BookSyncListener());
		//this.sync.start();
		
		this.storage = createPersistenceService();
		mainJFrame.tree.addTreeWillExpandListener(this.treeWillExpandListener);
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
				e.putValue(HANDLER, m);
				log.debug("get action process for menu, " + command);
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
		book.authToken = null;
		GmailAuthencation auth = new GmailAuthencation();
		log.info("Try login with user, " + book.getUser());
		if(autoLoginTimes < 3){
			if(book.password != null &&
			   auth.login(book.getUser() + "@gmail.com", book.password.toCharArray(),
					null, null)){
				log.info("login OK, with user " + book.getUser());
				book.authToken = auth.authToken;
				book.authSID = auth.authSID;
				autoLoginTimes++;
				return true;
			}else {
				LoginDailog loginBox = new LoginDailog(mainFrame, book);
				loginBox.setLocationRelativeTo(mainFrame);
				loginBox.setVisible(true);
				autoLoginTimes = 0;
				log.debug("authSID:" + book.authSID);
				return book.authToken != null;
			}
		}
		return false;
	}
	
	@Override
	public NoteBook getNoteBook() {
		return this.book;
	}
	
	class BookTreeModelListener implements TreeModelListener{
		@Override
		public void treeNodesChanged(TreeModelEvent e) {
			Category c = (Category)e.getTreePath().getLastPathComponent();
			dispatchEvent(MenuToolbar.SYNCUPLOAD, c);
		}
	
		@Override
		public void treeNodesInserted(TreeModelEvent e) {
			Category c = (Category)e.getTreePath().getLastPathComponent();
			dispatchEvent(MenuToolbar.SYNCUPLOAD, c);
			final TreePath path = e.getTreePath();
			SwingUtilities.invokeLater(new Runnable() {
	            public void run() {
	            	mainFrame.tree.expandPath(path);
	            	//tree.setSelectionPath(path);
	            }
	        });
		}
	
		@Override
		public void treeNodesRemoved(TreeModelEvent e) {
			Category c = (Category)e.getTreePath().getLastPathComponent();
			dispatchEvent(MenuToolbar.SYNCUPLOAD, c.parent);
		}
	
		@Override
		public void treeStructureChanged(TreeModelEvent e) {
			Category c = (Category)e.getTreePath().getLastPathComponent();
			dispatchEvent(MenuToolbar.SYNCUPLOAD, c.parent);
		}
	}
	
	//在后台线程回调.
	class BookSyncListener implements SyncListener{
		private void updateStatus(String s){
			mainFrame.status(s);
		}

		@Override
		public void start(SyncTask task) {
			// TODO Auto-generated method stub	
		}

		@Override
		public void done(SyncTask task) {
			if(task.task.equals(SyncTask.TASK_DOWN_DATA)){
				Category c = null;
				c = book.root.search(task.local.id);
				if(c != null && c.getMessage() != null){
					storage.save(c.getMessage());
					mainFrame.editor.reloadDocument(c.getMessage());
				}
			}
		}

		@Override
		public void syncError(SyncTask task, Exception e) {
			log.info("Stop sync casued by exception, message:" + e.getMessage());
			sync.stop();
			if(e instanceof AuthcationException){
				log.info("Auth error:" + e.getMessage());
				if(authencation()){
					sync.start(book, syncThread);
				}
			}
		}

		@Override
		public int conflict(Category local, Category remote, int cause) {
			if(cause == CONFLICT_EXPIRED){
				return UPDATE_IGNORE;
			}else {
				return UPDATE_FORCE;		
			}
		}

		@Override
		public void waiting() {
			storage.saveNoteBook(book);			
		}
	}
	
	private TreeWillExpandListener treeWillExpandListener = new TreeWillExpandListener() {
		public void treeWillCollapse(TreeExpansionEvent event)
		      throws ExpandVetoException {
			if(book.expaned.contains(event.getPath())){
				book.expaned.remove(event.getPath());
			}
		}
	
		public void treeWillExpand(TreeExpansionEvent event)
		      throws ExpandVetoException {
			if(!book.expaned.contains(event.getPath())){
				book.expaned.add(event.getPath());
			}
		}
	};	
	
	private void restoreLastStatus(){
		SwingUtilities.invokeLater(new Runnable() {
            public void run() {
            	Vector<TreePath> expaned = new Vector<TreePath>();
            	if(book.expaned != null){
            		expaned.addAll(book.expaned);
            		book.expaned.clear();
            	}else {
            		book.expaned = new Vector<TreePath>();
            	}
            	for(TreePath path: expaned){
            		mainFrame.tree.expandPath(path);
            	}
            }
        });
		if(book.curNoteId != null && !book.curNoteId.equals("")){
			Category node = book.root.search(book.curNoteId);
			if(node != null){
				NoteMessage msg = node.getMessage(storage, true);
				mainFrame.editor.openDocument(msg);
			}
		}
	}
	
	class MenuAction {
		public void Save(BookAction event){
			actionThread.execute(new Runnable(){
				@Override
				public void run() {
					saveOpenedMessage();
					storage.saveNoteBook(book);
					mainFrame.status("Saved all");
					sync.upload(book.root, false);
				}});
			mainFrame.status("Saving...");
		}

		public void Open(BookAction event){
			final Category node = selectedTreeNode();
			if(node.isLeaf()){
				actionThread.execute(new Runnable(){
					@Override
					public void run() {
						saveOpenedMessage();
						NoteMessage msg = node.getMessage(storage, true);
						mainFrame.editor.openDocument(msg);
						book.curNoteId = node.id;
					}});
				mainFrame.status("Opening " + node.name + "(" + node.id + ")");
			}
		}
		
		public void UpdatedSettings(BookAction event){
			book.root.name = book.name;
			Save(event);
			sync.start(book, syncThread);
			sync.download(book.root, true);
		}
		
		public void Settings(BookAction event){
			mainFrame.showSettings();
		}
		
		public void About(BookAction event){
			//mainFrame.showSettings();
			AboutDialog about = new AboutDialog(mainFrame);
			about.setLocationRelativeTo(mainFrame);
			about.setVisible(true);
		}		
		
		public void SyncDownLoad(BookAction event){
			final Category cate = selectedTreeNode(event);
			if (cate == null){return;}
			sync.download(cate, true);
			if(!sync.isRunning() && event.attachedEvent != null){
				int i = JOptionPane.showConfirmDialog(mainFrame,
					    "同步连接断开,是否重新连接.",
					    "同步错误",
					    JOptionPane.OK_CANCEL_OPTION);
				if(i==0){
					sync.start(book, syncThread);
				}
			}
		}
		public void SyncUpLoad(BookAction event){
			final Category cate = selectedTreeNode(event);
			if (cate == null){return;}
			sync.upload(cate, true);
			if(!sync.isRunning() && event.attachedEvent != null){
				int i = JOptionPane.showConfirmDialog(mainFrame,
					    "同步连接断开,是否重新连接.",
					    "同步错误",
					    JOptionPane.OK_CANCEL_OPTION);
				if(i==0){
					sync.start(book, syncThread);
				}
			}
		}
		public void Sync(BookAction event){
			final Category cate = selectedTreeNode(event);
			if (cate == null){return;}
			sync.sync(cate, true);
			if(!sync.isRunning() && event.attachedEvent != null){
				int i = JOptionPane.showConfirmDialog(mainFrame,
					    "同步连接断开,是否重新连接.",
					    "同步错误",
					    JOptionPane.OK_CANCEL_OPTION);
				if(i==0){
					sync.start(book, syncThread);
				}
			}		
		}	
		
		public void SyncStatus(BookAction event){
			SyncStatusDailog settings = new SyncStatusDailog(mainFrame, sync);		
			settings.setLocationRelativeTo(mainFrame);
			settings.setVisible(true);
		}
		
		public void NewCategory(BookAction event){
			Category parent = selectedTreeNode();
			if(parent == null){
				JOptionPane.showMessageDialog(mainFrame,
					    "没有选择添加到哪一个目录.",
					    "Error",
					    JOptionPane.ERROR_MESSAGE);
			}else if(parent.isLeaf()){
				JOptionPane.showMessageDialog(mainFrame,
					    "不能在文件上面创建子目录.",
					    "Error",
					    JOptionPane.ERROR_MESSAGE);
			}else {
				parent.addCategory("新建目录");
			}
		}
		public void NewNote(BookAction event){
			Category parent = selectedTreeNode();
			if(parent == null){
				JOptionPane.showMessageDialog(mainFrame,
					    "没有选择添加到哪一个目录.",
					    "Error",
					    JOptionPane.ERROR_MESSAGE);
			}else if(parent.isLeaf()){
				JOptionPane.showMessageDialog(mainFrame,
					    "不能在文件上面创建文件.",
					    "Error",
					    JOptionPane.ERROR_MESSAGE);
			}else {
				parent.addMessage("新建文件");
			}			
		}
		
		public void ReName(BookAction event){
			Category object = selectedTreeNode();
			if(object == null){
				JOptionPane.showMessageDialog(mainFrame,
					    "请选择需要重命名的对象.",
					    "Error",
					    JOptionPane.ERROR_MESSAGE);
			}else {
				String s = (String)JOptionPane.showInputDialog(
						mainFrame,
	                    "输入新名字:\n",
	                    "重命名",
	                    JOptionPane.PLAIN_MESSAGE,
	                    null,
	                    null,
	                    object.getName());
				if(s != null){
					object.setName(s);
				}
			}
			
		}
		
		public void Remove(BookAction event){
			Category object = selectedTreeNode();
			if(object == null){
				JOptionPane.showMessageDialog(mainFrame,
					    "请选择需要删除的对象.",
					    "Error",
					    JOptionPane.ERROR_MESSAGE);
			}else {				
				int i = JOptionPane.showConfirmDialog(mainFrame,
					    "确定需要删除 '" + object.name + "'.",
					    "确认",
					    JOptionPane.OK_CANCEL_OPTION);
				if(i==0){
					object.remove();
				}
			}
		}
		
		public void HideWindow(BookAction event){
			//if(this.s)
			if(visibleTrayIcon()){
				if(mainFrame.isVisible()){
					mainFrame.setVisible(false);
				}
			}else {
				Exit(event);
			}
		}
		
		public void ShowWindow(BookAction event){
			mainFrame.setVisible(true);
		}
		
		public void Exit(BookAction event) {
			log.info("shutdown applcation...");
			mainFrame.dispose();
			System.exit(0);
		}
		
		public void OpenNoteBook(BookAction event){
			book = storage.loadNoteBook();
			if(book == null){
				book = new NoteBook();
				book.root = new Category();
				book.root.initDefaultNode();
				storage.saveNoteBook(book);
				initedBook = true;
			}else {
				initedBook = false;
			}			
			if(initedBook){
				mainFrame.showSettings();
			}else {
				sync.start(book, syncThread);
				dispatchEvent(MenuToolbar.SYNC, book.root);
			}
			mainFrame.setTitle(book.name);
			mainFrame.tree.setRoot(book.root);
			restoreLastStatus();
		}
		
		public void OrderByPosition(BookAction event) {
			Category object = selectedTreeNode();
			object.orderBy("position");
		}
		
		public void OrderByCreateDate(BookAction event) {
			Category object = selectedTreeNode();
			object.orderBy("create_date");
		}
		public void OrderByUpdate(BookAction event) {
			Category object = selectedTreeNode();
			object.orderBy("update_date");
		}
		public void OrderByName(BookAction event) {
			Category object = selectedTreeNode();
			object.orderBy("name");
		}
		public void OrderMoveUp(BookAction event) {
			Category object = selectedTreeNode();
			object.moveOrder(-1);
		}
		public void OrderMoveDown(BookAction event) {
			Category object = selectedTreeNode();
			object.moveOrder(1);
		}
		
		private Category selectedTreeNode(){
			return this.selectedTreeNode(null);
		}
		private Category selectedTreeNode(BookAction event){
			if(event != null && event.attachedObject != null && 
			   event.attachedObject instanceof Category){
				return (Category)event.attachedObject;
			}
			TreePath path = mainFrame.tree.getSelectionPath();
			if(path != null){
				return (Category)path.getLastPathComponent();
			}
			return null;
		}
		
		private void saveOpenedMessage(){
			NoteMessage note = mainFrame.editor.currentDocuemnt();
			if(note != null && note.isDirty){
				storage.save(note);
				note.isDirty = false;
				sync.upload(note.getCategory(), false);
			}
		}
	}
}
