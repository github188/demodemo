package org.notebook.gui;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.lang.reflect.Method;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import javax.swing.Action;
import javax.swing.JOptionPane;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.Category;
import org.notebook.cache.NoteBook;
import org.notebook.cache.SimpleObjectCache;
import org.notebook.gui.MenuToolbar.BookAction;

public class BookController {
	private static Log log = LogFactory.getLog(BookController.class);
	private NavigationTree tree = null;
	private DocumentEditor editor = null;
	private SimpleObjectCache cache = null;
	private NoteBook book = null;
	private StatusBar statusBar = null;
	private MainFrame mainFrame = null;
	
	private SyncService sync = null;
	private ThreadPoolExecutor syncThread = null;	
	private Category curMessage = null;
	private MenuAction menuActions = new MenuAction();
	
	public BookController(NoteBook book, NavigationTree tree, 
			DocumentEditor editor,
			SimpleObjectCache cache,
			StatusBar status,
			MainFrame mainJFrame
			){
		this.book = book;
		this.tree = tree;
		this.editor = editor;
		this.cache = cache;
		this.statusBar = status;
		this.mainFrame = mainJFrame;
		sync = new SyncService(book);
		syncThread = new ThreadPoolExecutor(1, 1, 60, 
							TimeUnit.SECONDS, 
							new LinkedBlockingDeque<Runnable>(100)); 		
		this.tree.addMouseListener(new BookMouseLisenter());
		this.book.root.addTreeModelListener(new BookTreeModelListener());
		this.sync.addListener(new BookSyncListener());
		this.sync.start();
		this.downloadNoteBook(book.root);
	}
	
	public void processEvent(BookAction e){
		final String HANDLER = "__actionHander";
		String command = (String)e.getValue(Action.ACTION_COMMAND_KEY);
		log.info("processEvent:" + command);
		Method m = (Method)e.getValue(HANDLER);
		if(m == null){
			try {
				m = MenuAction.class.getMethod(command, new Class[]{});
				e.putValue(HANDLER, m);
				log.info("get action process for menu, " + command);
			} catch (Exception e1) {
			}
		}
		if(m == null){
			log.warn("Not found action handler, for " + command);
		}else {
			try {
				m.invoke(this.menuActions, new Object[]{});
			} catch (Exception e1) {
				log.error(e1.toString(), e1.getCause());
			}
		}
	}
	
	class BookMouseLisenter implements MouseListener{
		@Override
		public void mouseClicked(MouseEvent e) {
			if(e.getSource().equals(tree) && e.getClickCount() > 1){
				if(tree.getSelectionPath() != null){
					Category node = (Category)tree.getSelectionPath().getLastPathComponent();
					if(node.isLeaf()){
						statusBar.setText(String.format("Remove %s", node.name));
						saveCurMessage();
						editor.openDocument(node);
						curMessage = node;
					}
				}
			}
		}
	
		@Override
		public void mousePressed(MouseEvent e) {
			// TODO Auto-generated method stub
			
		}
	
		@Override
		public void mouseReleased(MouseEvent e) {
			// TODO Auto-generated method stub			
		}
	
		@Override
		public void mouseEntered(MouseEvent e) {
			// TODO Auto-generated method stub
		}
	
		@Override
		public void mouseExited(MouseEvent e) {
			// TODO Auto-generated method stub			
		}
	
	}
	
	public void save(){
		this.cache.saveNoteBook(this.book);
		saveCurMessage();
	}
	
	protected void saveCurMessage(){
		if(curMessage != null){
			curMessage.getMessage().text = this.editor.getText();
			this.cache.save(curMessage.getMessage());
			
			//更新最后更新时间.
			curMessage.setName(curMessage.name);
		}
	}
	
	class BookTreeModelListener implements TreeModelListener{
		@Override
		public void treeNodesChanged(TreeModelEvent e) {
			Category c = (Category)e.getTreePath().getLastPathComponent();
			uploadNoteBook(c);
		}
	
		@Override
		public void treeNodesInserted(TreeModelEvent e) {
			Category c = (Category)e.getTreePath().getLastPathComponent();
			uploadNoteBook(c);
			tree.expandPath(e.getTreePath().getParentPath());
		}
	
		@Override
		public void treeNodesRemoved(TreeModelEvent e) {
			Category c = (Category)e.getTreePath().getLastPathComponent();
			uploadNoteBook(c.parent);
		}
	
		@Override
		public void treeStructureChanged(TreeModelEvent e) {
			Category c = (Category)e.getTreePath().getLastPathComponent();
			uploadNoteBook(c.parent);
		}
	
	}
	
	public void syncNoteBook(final Category cate){
		this.syncThread.execute(new Runnable(){
			@Override
			public void run() {
				sync.sync(cate);
				sync.syncCategoryId();
			}});
	}
	public void uploadNoteBook(final Category cate){
		this.syncThread.execute(new Runnable(){
			@Override
			public void run() {
				sync.upload(cate);
				sync.syncCategoryId();
			}});
	}
	public void downloadNoteBook(final Category cate){
		this.syncThread.execute(new Runnable(){
			@Override
			public void run() {
				sync.download(cate);
				sync.syncCategoryId();
			}});		
	}
	
	class BookSyncListener implements SyncListener{
		@Override
		public void removeLocal(Category c) {
			statusBar.setText(String.format("Remove %s", c.name));
		}
	
		@Override
		public void updateLocal(Category c) {
			statusBar.setText(String.format("Update %s", c.name));
			if(c.isLeaf()){
				cache.save(c.getMessage());
			}
		}
	
		@Override
		public void removeRemote(Category c) {
			statusBar.setText(String.format("Remove remote %s", c.name));
		}
	
		@Override
		public void updateRemote(Category c) {
			statusBar.setText(String.format("Update remote %s", c.name));
		}
		
		@Override
		public void syncError(Exception c) {
			statusBar.setText(String.format("Sync error %s", c.toString()));
			sync.stop();
			log.info("Stop sync running....");
		}
	
	}
	
	class MenuAction {
		public void Save(){
			save();
		}
		
		public void NewCategory(){
			if(tree.getSelectionModel().getSelectionPath() == null){
				JOptionPane.showMessageDialog(mainFrame,
					    "没有选择添加到哪一个目录.",
					    "Error",
					    JOptionPane.ERROR_MESSAGE);
			}else {
				Category obj = (Category)tree.getSelectionModel().getSelectionPath().getLastPathComponent();
				if(obj.isLeaf()){
					JOptionPane.showMessageDialog(mainFrame,
						    "不能在文件上面创建子目录.",
						    "Error",
						    JOptionPane.ERROR_MESSAGE);
				}else {
					obj.addCategory("新建目录");
					//tree.setSelectionPath()
				}
			}
		}
		public void NewNote(){
			if(tree.getSelectionModel().getSelectionPath() == null){
				JOptionPane.showMessageDialog(mainFrame,
					    "没有选择添加到哪一个目录.",
					    "Error",
					    JOptionPane.ERROR_MESSAGE);
			}else {
				Category obj = (Category)tree.getSelectionModel().getSelectionPath().getLastPathComponent();
				if(obj.isLeaf()){
					JOptionPane.showMessageDialog(mainFrame,
						    "不能在文件上面创建文件.",
						    "Error",
						    JOptionPane.ERROR_MESSAGE);
				}else {
					obj.addMessage("新建文件");
				}
			}			
		}
		
		public void Remove(){
			if(tree.getSelectionModel().getSelectionPath() == null){
				JOptionPane.showMessageDialog(mainFrame,
					    "请选择需要删除的对象.",
					    "Error",
					    JOptionPane.ERROR_MESSAGE);
			}else {
				Category obj = (Category)tree.getSelectionModel().getSelectionPath().getLastPathComponent();				
				int i = JOptionPane.showConfirmDialog(mainFrame,
					    "确定需要删除 '" + obj.name + "'.",
					    "确认",
					    JOptionPane.OK_CANCEL_OPTION);
				if(i==0){
					obj.remove();
				}
			}			
		}
		
		public void HideWindow(){
			mainFrame.setVisible(false);
		}
		
		public void ShowWindow(){
			mainFrame.setVisible(true);
		}
		
		public void Exit() {
			log.info("shutdown applcation...");
			mainFrame.dispose();
			System.exit(0);
		}
	}
}
