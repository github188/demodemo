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
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.security.AccessControlException;
import java.security.AccessController;
import java.security.PrivilegedAction;
import java.security.PrivilegedActionException;
import java.security.PrivilegedExceptionAction;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.Version;
import org.notebook.gui.MenuToolbar.BookAction;
import org.notebook.gui.editor.DocumentEditor;
import org.notebook.gui.editor.TextEditorPlane;
import org.notebook.services.BookController;
import org.notebook.services.DefaultBookController;
import org.notebook.services.SingleInstance;
import org.notebook.services.SocketSingleInstance;

public class MainFrame extends JFrame {
	private static final long serialVersionUID = -4362026054606144515L;
	private Log log = LogFactory.getLog("MainFrame");

	public MenuToolbar menu = null;	
	public NavigationTree tree = null;
	public DocumentEditor editor = null;
	public StatusBar statusBar = null;

	private BookController controller = null;
		
	//只用来判断是否已经有一个进程在运行.
	private static SingleInstance singlton = new SocketSingleInstance();
	//初始化托盘时, Controller还没有创建。把安装的结果临时保存。等Contoller创建后，再传给Controller.
	private boolean visibleTrayIcon = false;
	
	public MainFrame(){
		super();
		this.setTitle("NoteBook");
		setLayout(new BorderLayout());
		this.initGui();
		pack(); 
		setSize(670,548);
	}
	
	public void initNoteBookController(){
		controller =  createPrivilegedProxy(new DefaultBookController(this,
						runingJNLP(),
					 	runningSandbox()
					  ));
		controller.setVisibleTrayIcon(visibleTrayIcon);
		controller.dispatchEvent(MenuToolbar.OPENNOTEBOOK);
	}
	
	private boolean runningSandbox(){
		try {
			System.getenv("USERNAME");
			return false;
		}catch(AccessControlException e){
			return true;
		}
	}
	
	private boolean runingJNLP(){
		try {
			Class.forName("javax.jnlp.ServiceManager");
			return true;
		} catch (ClassNotFoundException e) {
			return false;
		}
	}
	
	@SuppressWarnings("unchecked")
	public static void main(final String[] args){
		System.out.println("NoteBook " + Version.getVersion());
		//System.setProperty("file.encoding", "UTF-8");
		//System.setProperty("sun.jnu.encoding", "UTF-8");
		//System.out.println("中文");
    	
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
		            UIManager.getSystemLookAndFeelClassName());
		}catch(Exception e){
			System.out.println(e.toString());
		}
		
    	//showMessageBox("ss");
		if(singlton.checkRunning(null)){
			JOptionPane.showMessageDialog(null,
				    "NoteBook已经在运行中.",
				    "Error",
				    JOptionPane.ERROR_MESSAGE);
			System.exit(1);
		}else {
			//使用Event thread来初始化界面。Swing的部分控件方法只能在Event thread调用。
			SwingUtilities.invokeLater(new Runnable() {
				public void run(){
	            	final MainFrame main = new MainFrame();
	            	main.installTrayIcon();
	            	if(main.runingJNLP()){
	            		main.setupJNLPSingltenService();
	            	}
	            	//窗口居中.
	            	main.setLocationRelativeTo(null);
	            	main.setVisible(true);
				}
	        });	
		}
	}  

	protected void initGui() {
		menu = new MenuToolbar(this);
		this.getRootPane().setJMenuBar(menu.getMenuBar());
		
		tree = new NavigationTree(null, menu);
		editor = new DocumentEditor();
		menu.addExtraToolBar(editor.getToolBar());

		JScrollPane leftTree = new JScrollPane(tree);
		Dimension minSize = new Dimension(150, 400);		
		leftTree.setMinimumSize(minSize);		
		JSplitPane splitPane = new JSplitPane(
				JSplitPane.HORIZONTAL_SPLIT, 
				leftTree,
				new TextEditorPlane(editor)
		);
		
		statusBar = new StatusBar();

		Container contentPane = getContentPane();
		contentPane.add(menu.getToolBar(), BorderLayout.NORTH);
		contentPane.add(splitPane, BorderLayout.CENTER);
		contentPane.add(statusBar, BorderLayout.SOUTH);
		
		//controller = createPrivilegedProxy(new DefaultBookController(this));
		
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	    addComponentListener(new ComponentAdapter() {
	        public void componentShown(ComponentEvent ce) {
	        	if(controller == null){
	        		try{
	        			initNoteBookController();
	        			setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
	        		    addWindowListener(new WindowAdapter(){
	        		    	public void windowClosing(WindowEvent e){
	        		    		processEvent(menu.$(MenuToolbar.HIDEWINDOW));
	        		    	}
	        		    });
	        		}catch(Exception e){
	        			JOptionPane.showMessageDialog(null,
	        				    "程序初始化出错!",
	        				    "Error",
	        				    JOptionPane.ERROR_MESSAGE);
	        			e.printStackTrace();
	        		}
	        	}
	        }
	    });
	    	    
	    this.setIconImage(appIcon16());
	}	
	
	public void processEvent(BookAction e) {
		if(controller != null){
			this.controller.processEvent(e);
		}else {
			JOptionPane.showMessageDialog(null,
				    "程序初始化失败!",
				    "Error",
				    JOptionPane.ERROR_MESSAGE);			
		}
	}
	
	public void showSettings(){
		NoteBookSettings settings = new NoteBookSettings(this, controller);		
		settings.setLocationRelativeTo(this);
		settings.setVisible(true);
	}
		
	public void status(String msg){
		this.statusBar.setText(msg);
	}
	
	private Image appIcon(){
		return MenuToolbar.icon("org/notebook/gui/images/application.png").getImage();			
	}
	
	private Image appIcon16(){
		return appIcon().getScaledInstance(16, 16, Image.SCALE_SMOOTH);			
	}
	
	
	private void installTrayIcon(){
		visibleTrayIcon = false;
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
				visibleTrayIcon = true;
		    }
		}catch (Exception e){
			log.error(e, e);
		}
	}
	
	private void setupJNLPSingltenService(){
		try {
			Class cl = Class.forName("org.notebook.services.JNLPSingleInstance");
			SingleInstance ins = (SingleInstance)cl.newInstance();
			ins.checkRunning(this);
		}catch(ClassNotFoundException e) {
			log.error("failed to load JNLP JNLPSingleInstance", e);
		} catch (Exception e) {
			log.error("failed to setup JNLP SingleInstanceService", e);
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

