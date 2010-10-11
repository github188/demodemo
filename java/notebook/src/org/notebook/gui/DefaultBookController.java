package org.notebook.gui;

import java.io.File;
import java.lang.reflect.Method;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import javax.swing.Action;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.tree.TreePath;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.Category;
import org.notebook.cache.DataStorage;
import org.notebook.cache.LocalFileStorage;
import org.notebook.cache.NoteBook;
import org.notebook.cache.NoteMessage;
import org.notebook.gui.MenuToolbar.BookAction;

public class DefaultBookController implements BookController{
	private static Log log = LogFactory.getLog(DefaultBookController.class);
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
	//初始化了本地NoteBook路径.
	private boolean initedBook = false;
	
	public DefaultBookController(MainFrame mainJFrame, boolean isJNLP, boolean isSandBox){
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
			File root = new File(System.getenv("APPDATA"), ".notebook");
			storge = new LocalFileStorage(root);
		}
		return storge;
	}	
	
	public void processEvent(BookAction e){
		final String HANDLER = "__actionHander";
		String command = (String)e.getValue(Action.ACTION_COMMAND_KEY);
		log.info("processEvent:" + command);
		Method m = (Method)e.getValue(HANDLER);
		if(m == null){
			try {
				m = MenuAction.class.getMethod(command, new Class[]{BookAction.class});
				e.putValue(HANDLER, m);
				log.info("get action process for menu, " + command);
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
		@Override
		public void removeLocal(Category c) {
			updateStatus(String.format("Remove %s", c.name));
		}
	
		@Override
		public void updatedLocal(Category c) {
			updateStatus(String.format("Update %s", c.name));
			if(c.isLeaf()){
				storage.save(c.getMessage());
			}
		}
	
		@Override
		public void removeRemote(Category c) {
			updateStatus(String.format("Remove remote %s", c.name));
		}
	
		@Override
		public void updateRemote(Category c) {
			updateStatus(String.format("Update remote %s", c.name));
		}
		
		@Override
		public void syncError(Category c, Exception e) {
			updateStatus(String.format("Sync error %s", e.toString()));
			sync.stop();
			log.info("Stop sync running....");
		}

		@Override
		public void checkDownload(Category c) {
			updateStatus(String.format("Check download %s", c.toString()));
			
		}

		@Override
		public void checkUpload(Category c) {
			updateStatus(String.format("Check upload %s", c.toString()));
			
		}
		
		private void updateStatus(String s){
			mainFrame.status(s);
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
						NoteMessage msg = node.getMessage(storage);
						mainFrame.editor.openDocument(msg);
					}});
				mainFrame.status("Opening " + node.name);
			}
		}
		
		public void UpdatedSettings(BookAction event){
			Save(event);
		}
		
		public void Settings(BookAction event){
			mainFrame.showSettings();
		}
		
		public void SyncDownLoad(BookAction event){
			final Category cate = selectedTreeNode(event);
			if (cate == null){return;}
			if(!sync.isRunning() && event.attachedEvent != null){
				int i = JOptionPane.showConfirmDialog(mainFrame,
					    "同步连接断开,是否重新连接.",
					    "同步错误",
					    JOptionPane.OK_CANCEL_OPTION);
				if(i==0){
					sync.start();
				}				
			}
			
			syncThread.execute(new Runnable(){
				@Override
				public void run() {
					log.info("Start sync process, id:" + cate.id);
					sync.download(cate);
					sync.syncCategoryId();
				}}
			);		
		}
		public void SyncUpLoad(BookAction event){
			final Category cate = selectedTreeNode(event);
			if (cate == null){return;}
			if(!sync.isRunning() && event.attachedEvent != null){
				int i = JOptionPane.showConfirmDialog(mainFrame,
					    "同步连接断开,是否重新连接.",
					    "同步错误",
					    JOptionPane.OK_CANCEL_OPTION);
				if(i==0){
					sync.start();
				}				
			}	
			syncThread.execute(new Runnable(){
				@Override
				public void run() {
					sync.upload(cate);
					sync.syncCategoryId();
				}}
			);			
		}
		public void Sync(BookAction event){
			final Category cate = selectedTreeNode(event);
			if (cate == null){return;}
			if(!sync.isRunning() && event.attachedEvent != null){
				int i = JOptionPane.showConfirmDialog(mainFrame,
					    "同步连接断开,是否重新连接.",
					    "同步错误",
					    JOptionPane.OK_CANCEL_OPTION);
				if(i==0){
					sync.start();
				}				
			}		
			syncThread.execute(new Runnable(){
				@Override
				public void run() {
					sync.sync(cate);
					sync.syncCategoryId();
				}}
			);
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
			}
			//book.root.addTreeModelListener(new BookTreeModelListener());	
			
			//log.info("loading notebook:" + book.name + ",is:" + SwingUtilities.isEventDispatchThread());
			mainFrame.setTitle(book.name);
			book.root.name = book.name;
			mainFrame.tree.setRoot(book.root);
			sync.setNoteBook(book);
			sync.start();
			dispatchEvent(MenuToolbar.SYNC, book.root);			
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
				note.getCategory().setLastUpdate();
			}
		}
	}
}
