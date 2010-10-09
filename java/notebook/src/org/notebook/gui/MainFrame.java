/*  
  * Copyright [2008] DeonWu@gmail.com 
  *  
  * Licensed under the Apache License, Version 2.0 (the "License");  
  * you may not use this file except in compliance with the License.  
  * You may obtain a copy of the License at  
  *      http://www.apache.org/licenses/LICENSE-2.0  
  * Unless required by applicable law or agreed to in writing, software  
  * distributed under the License is distributed on an "AS IS" BASIS,  
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
  * See the License for the specific language governing permissions and  
  * limitations under the License. 
  *  
  * $ Name LastChangeRevision LastChangeDate LastChangeBy $ 
  * $Id$ 
  */ 
 
package org.notebook.gui;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.net.BindException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.security.AccessController;
import java.security.PrivilegedAction;
import java.security.PrivilegedActionException;
import java.security.PrivilegedExceptionAction;

import javax.swing.Action;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.Category;
import org.notebook.cache.LocalFilePersistence;
import org.notebook.cache.NoteBook;
import org.notebook.cache.DataStorage;
import org.notebook.gui.MenuToolbar.BookAction;

public class MainFrame extends JFrame {
	public static final int SINGLE_INSTANCE_NETWORK_SOCKET = 44331;
	private static Log log = LogFactory.getLog(MainFrame.class);

	public MenuToolbar menu = null;
	private DataStorage dao = null;
	private NoteBook notebook = null;
	private BookController controller = null;
	private boolean firstRun = false;
	
	//只用来判断是否已经有一个进程在运行.
	private static ServerSocket socket = null;
	
	public MainFrame(){
		super();		
		//cache = new PersistenceService(this.getRootPath());
		dao = this.createPersistenceService();
		notebook = dao.loadNoteBook();
		if(notebook == null){
			notebook = new NoteBook();
			notebook.root = new Category();
			notebook.root.initDefaultNode();
			notebook.root.setName("Deon的记事本");
			dao.saveNoteBook(notebook);
			firstRun = true;
		}
		this.setTitle(notebook.name);
		setLayout(new BorderLayout());
		this.initGui();
		pack(); 
		setSize(670,548);
	}
	
	private DataStorage createPersistenceService(){
		DataStorage  storge = null;
		if(!isRuningJNLP()){
			File root = new File(System.getenv("APPDATA"), ".notebook");
			storge = new LocalFilePersistence(root);
		}else {
			try {
				Class cl = Class.forName("org.notebook.cache.JNLPPersistence");
				storge = (DataStorage)cl.newInstance();
			}catch(ClassNotFoundException e) {
				log.error("failed to load JNLP Stroage", e);
			} catch (Exception e) {
				log.error("failed to create JNLP Stroage", e);
			}
		}
		return storge;
	}
	
	private boolean isRuningJNLP(){
		try {
			Class.forName("javax.jnlp.ServiceManager");
			return true;
		} catch (ClassNotFoundException e) {
			return false;
		}
	}
	
	@SuppressWarnings("unchecked")
	public static void main(final String[] args){
    	if(System.getSecurityManager() != null) {
    		System.out.println("SecurityManager:" + System.getSecurityManager().toString());
    		//System.setSecurityManager(new SecurityManager());
    	}else {
    		System.out.println("SecurityManager is null.");
    	}
    	
    	AccessController.doPrivileged(
				new PrivilegedAction() {
					public Object run(){
						mainPrivileged(args);
						return null;
					}
				});				
	}
 	
    public static void mainPrivileged(String[] args){  	
		try{
			UIManager.setLookAndFeel(
					"com.sun.java.swing.plaf." +
					"windows.WindowsLookAndFeel");
		}catch(Exception e){
			System.out.println(e.toString());
		}
		
    	//showMessageBox("ss");
		if(checkRunning()){
			JOptionPane.showMessageDialog(null,
				    "NoteBook已经在运行中.",
				    "Error",
				    JOptionPane.ERROR_MESSAGE);
			System.exit(1);
		}else {
			//使用Event thread来初始化界面。Swing的部分控件方法只能在Event thread调用。
			SwingUtilities.invokeLater(new Runnable() {
	            @SuppressWarnings("unchecked")
				public void run() {
	            	AccessController.doPrivileged(
	        				new PrivilegedAction() {
	        					public Object run(){
	        		            	final MainFrame main = new MainFrame();
	        		            	main.installTrayIcon();
	        		            	//窗口居中.
	        		            	main.setLocationRelativeTo(null);
	        		            	main.setVisible(true); 
	        						return null;
	        					}
	        		});		            	
	            }
	        });	
		}
	}  

	protected void initGui() {
		menu = new MenuToolbar(this);
		this.getRootPane().setJMenuBar(menu.getMenuBar());

		Container contentPane = getContentPane();
		
		NavigationTree tree = new NavigationTree(notebook.root, menu);
		DocumentEditor editor = new DocumentEditor();

		JScrollPane leftTree = new JScrollPane(tree);
		Dimension minSize = new Dimension(150, 400);
		
		leftTree.setMinimumSize(minSize);
		
		JSplitPane splitPane = new JSplitPane(
				JSplitPane.HORIZONTAL_SPLIT, 
				leftTree,
				new JScrollPane(editor)
		);
		
		StatusBar status = new StatusBar();

		contentPane.add(menu.getToolBar(), BorderLayout.NORTH);
		contentPane.add(splitPane, BorderLayout.CENTER);
		contentPane.add(status, BorderLayout.SOUTH);
		
		controller = createPrivilegedProxy(new DefaultBookController(notebook, tree, editor, 
						this.dao,
						status,
						this));
		
		setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
	    addComponentListener(new ComponentAdapter() {
	        public void componentShown(ComponentEvent ce) {
	        	if(firstRun){
	        		showSettings();
	        	}
	        }
	    });
	    
	    addWindowListener(new WindowAdapter(){
	    	public void windowClosing(WindowEvent e){
	    		processEvent(menu.$(MenuToolbar.HIDEWINDOW));
	    	}
	    });
	    
	    this.setIconImage(appIcon16());
	}	
	
	public void processEvent(BookAction e) {
		String command = (String)e.getValue(Action.ACTION_COMMAND_KEY);
		log.info("processEvent:" + command);
		if(command.equals(MenuToolbar.SETTINGS)){
			this.showSettings();
		}else {
			this.controller.processEvent(e);
		}
	}
	
	private void showSettings(){
		NoteBookSettings settings = new NoteBookSettings(this, notebook, controller);
		
		settings.setLocationRelativeTo(this);
		settings.setVisible(true);
	}
	
	private Image appIcon(){
		return MenuToolbar.icon("org/notebook/gui/images/application.png").getImage();			
	}
	
	private Image appIcon16(){
		return appIcon().getScaledInstance(16, 16, Image.SCALE_SMOOTH);			
	}
	
	
	private void installTrayIcon(){
		try {
			if(java.awt.SystemTray.isSupported()){// 判断当前平台是否支持系统托盘
				java.awt.SystemTray  st = java.awt.SystemTray.getSystemTray();
				java.awt.TrayIcon ti = new java.awt.TrayIcon(appIcon16());
				ti.setToolTip (this.getTitle());
				ti.setPopupMenu(menu.getTrayMenu());
				ti.addActionListener(new ActionListener(){
					@Override
					public void actionPerformed(ActionEvent e) {
						processEvent(menu.$(MenuToolbar.SHOWWINDOW));
					}});
				st.add(ti);
		    }
		}catch (Exception e){
			log.error(e, e);
		}
	}
	
	private static boolean checkRunning(){
		try {
			socket = new ServerSocket(SINGLE_INSTANCE_NETWORK_SOCKET,0,
		    		InetAddress.getByAddress(new byte[] {127,0,0,1}));
			return false;
		}catch (BindException e) {
			return true;
		}catch (IOException e) {
			return true;
		}
	}	
	
	private BookController createPrivilegedProxy(final BookController stub){
    	return (BookController)Proxy.newProxyInstance(MainFrame.class.getClassLoader(), 
				   new Class[]{BookController.class}, 
				   new InvocationHandler(){
						@SuppressWarnings("unchecked")
						public Object invoke(Object prxoy, final Method method,
								final Object[] args) throws Throwable {
							try{
								return AccessController.doPrivileged(
										new PrivilegedExceptionAction() {
											public Object run() throws Exception{
												return method.invoke(stub, args);
											}
										});	
							}catch (PrivilegedActionException e){
								String name = method.getName();
								log.error("Exception:" + e.getCause().toString() +
										  "\n Method:" + name,
										  e);
								throw e.getException();
							}
						}
					}
					);		
	}
}

