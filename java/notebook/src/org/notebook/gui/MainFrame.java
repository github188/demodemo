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
import java.net.BindException;
import java.net.InetAddress;
import java.net.ServerSocket;

import javax.swing.Action;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.UIManager;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.Category;
import org.notebook.cache.NoteBook;
import org.notebook.cache.SimpleObjectCache;
import org.notebook.gui.MenuToolbar.BookAction;

public class MainFrame extends JFrame {
	public static final int SINGLE_INSTANCE_NETWORK_SOCKET = 44331;
	private static Log log = LogFactory.getLog(MainFrame.class);
	private SimpleObjectCache cache = null;
	private NoteBook notebook = null;
	private BookController controller = null;
	private MenuToolbar menu = null;
	private boolean firstRun = false;
	
	//只用来判断是否已经有一个进程在运行.
	private static ServerSocket socket = null;
	
	public MainFrame(){
		super(); 
		
		File root = new File(System.getenv("APPDATA"));
		cache = new SimpleObjectCache(root);
		notebook = cache.loadNoteBook();
		if(notebook == null){
			notebook = new NoteBook();
			notebook.root = new Category();
			notebook.root.initDefaultNode();
			notebook.root.setName("Deon的记事本");
			cache.saveNoteBook(notebook);
			firstRun = true;
		}
		this.setTitle(notebook.name);
		setLayout(new BorderLayout());
		this.initGui();
		pack(); 
		setSize(670,548);
	}
 	
    public static void main(String[] args){
    	checkingIfRunning();
		try{
		UIManager.setLookAndFeel(
			"com.sun.java.swing.plaf." +
			"windows.WindowsLookAndFeel");
		}catch(Exception e){
			System.out.println(e.toString());
		}

		MainFrame main = new MainFrame();
		main.installTrayIcon();
		main.setLocationRelativeTo(null);
		main.setVisible(true); 
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
		
		controller = new BookController(notebook, tree, editor, 
						this.cache,
						status,
						this);
		
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
	
	private static void checkingIfRunning(){
		try {
			socket = new ServerSocket(SINGLE_INSTANCE_NETWORK_SOCKET,0,
		    		InetAddress.getByAddress(new byte[] {127,0,0,1}));
		}catch (BindException e) {
		    System.err.println("Already running.");
		    System.exit(1);
		}catch (IOException e) {
		    System.err.println("Unexpected error.");
		    System.exit(2);
		}
	}	
}

