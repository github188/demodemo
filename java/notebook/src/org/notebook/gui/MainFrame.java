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
import java.awt.event.ActionEvent;
import java.io.File;

import javax.swing.Action;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.UIManager;

import org.notebook.cache.Category;
import org.notebook.cache.NoteBook;
import org.notebook.cache.SimpleObjectCache;

public class MainFrame extends JFrame {
	private SimpleObjectCache cache = null;
	private NoteBook notebook = null;
	private BookController controller = null;
	private MenuToolbar menu = null;
	
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
		main.setVisible(true); 
	}  

	protected void initGui() {
		menu = new MenuToolbar(this);
		this.getRootPane().setJMenuBar(menu.getMenuBar());

		Container contentPane = getContentPane();
		
		NavigationTree tree = new NavigationTree(notebook.root);
		DocumentEditor editor = new DocumentEditor();

		JScrollPane leftTree = new JScrollPane(tree);
		Dimension minSize = new Dimension(150, 400);
		
		leftTree.setMinimumSize(minSize);
		
		JSplitPane splitPane = new JSplitPane(
				JSplitPane.HORIZONTAL_SPLIT, 
				leftTree,
				new JScrollPane(editor)
		);
		

		contentPane.add(menu.getToolBar(), BorderLayout.NORTH);
		contentPane.add(splitPane, BorderLayout.CENTER);
		contentPane.add(new StatusBar(), BorderLayout.SOUTH);
		
		controller = new BookController(tree, editor, this.cache);
		//this.pr
		super.processEvent(null);
	}
	
	public void processEvent(Action e) {
		String command = (String)e.getValue(e.NAME);
		if(command.equals(menu.SAVE)){
			this.cache.saveNoteBook(this.notebook);
			this.controller.save();
		}
		System.out.println("event:" + command);
	}
	
	
	
	

}

