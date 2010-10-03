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
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;

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
	private static Log log = LogFactory.getLog(MainFrame.class);
	private SimpleObjectCache cache = null;
	private NoteBook notebook = null;
	private BookController controller = null;
	private MenuToolbar menu = null;
	private boolean firstRun = false;
	
	public MainFrame(){
		super("Deon的记事本"); 
		
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
		setLayout(new BorderLayout());
		this.initGui();
		pack(); 
		setSize(670,548);
	}
 	
    public static void main(String[] args){ 
		try{
		UIManager.setLookAndFeel(
			"com.sun.java.swing.plaf." +
			"windows.WindowsLookAndFeel");
		}catch(Exception e){
			System.out.println(e.toString());
		}

		MainFrame main = new MainFrame(); 
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
	    		processEvent(menu.$(MenuToolbar.EXIT));
	    	}
	    });
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
		
}

