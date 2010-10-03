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

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.net.URL;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.ResourceBundle;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JPopupMenu;
import javax.swing.JToolBar;
import javax.swing.JTree;
import javax.swing.KeyStroke;

import org.notebook.i18n.SimpleResourceBound;

public class MenuToolbar {
	public static final String OPEN = "Open";
	public static final String SAVE = "Save";
	public static final String EXIT = "Exit";
	
	public static final String NEW_CATEGORY = "NewCategory";
	public static final String NEW_NOTE = "NewNote";
	public static final String DELETE = "Remove";
	
	public static final String SYNC = "Sync";
	
	public static final String SETTINGS = "Settings";
	
	private MainFrame owner = null;
	private ResourceBundle rb = new SimpleResourceBound();	
	private Map<String, BookAction> actions = new HashMap<String, BookAction>();
	
	public class BookAction extends AbstractAction {
		private static final long serialVersionUID = -6101997393914923387L;
		public BookAction(String name, String icon, int accelerator){
			super(name, new ImageIcon(_("org/notebook/gui/" + icon + ".gif")));
			if(accelerator > 0){
				this.putValue(this.ACCELERATOR_KEY, 
						KeyStroke.getKeyStroke(accelerator, ActionEvent.CTRL_MASK));
			}
			this.putValue(Action.ACTION_COMMAND_KEY, name);
			this.putValue(Action.NAME, i18n(name));
		}
		public void actionPerformed(ActionEvent event) {
			owner.processEvent(this);
		}
	}
		
	public void $(String name, String icon, int accelerator){
		actions.put(name, new BookAction(name, icon, accelerator));
	}
	public BookAction $(String name){
		return actions.get(name);
	}	
	/*
		new BookAction[]{
			new BookAction(OPEN, "star_off", 0),
			new BookAction(SAVE, "star_off", KeyEvent.VK_S),
			new BookAction(EXIT, "star_off", 0),
			new BookAction(SETTINGS, "star_off", 0),
	};*/

	public MenuToolbar(MainFrame owner){
		this.owner = owner;
		$(OPEN, "star_off", 0);
		$(SAVE, "star_off", KeyEvent.VK_S);
		$(EXIT, "star_off", 0);
		$(SETTINGS, "star_off", 0);	
		
		$(NEW_CATEGORY, "star_off", 0);			
		$(NEW_NOTE, "star_off", 0);	
		$(DELETE, "star_off", 0);
		$(SYNC, "star_off", 0);		
	}
		
	public JMenuBar getMenuBar(){
		JMenuBar menubar = new JMenuBar();
		JMenu fileMenu = new JMenu("File");

		fileMenu.add($(OPEN));
		fileMenu.add($(SAVE));
		fileMenu.add($(EXIT));

		menubar.add(fileMenu);
		
		JMenu toolMenu = new JMenu("Tools");

		toolMenu.add($(SETTINGS));

		menubar.add(fileMenu);
		menubar.add(toolMenu);
		
		return menubar;
	}
	
	public JPopupMenu getNavigationContextMenu(JTree tree){
	    final JPopupMenu menu = new JPopupMenu();
	    //menu.
	    //JMenuItem newCategory = new JMenuItem("新建目录");	
	    
	    menu.add($(NEW_CATEGORY));
	    menu.add($(NEW_NOTE));
	    menu.addSeparator();
	    menu.add($(SYNC));
	    menu.add($(DELETE));
	    
		return menu;
	}

	public JToolBar getToolBar(){
		JToolBar toolbar = new JToolBar();
		
		toolbar.add($(OPEN));
		toolbar.add($(SETTINGS));
		return toolbar;
	}

	protected static URL _(String name){
		return MenuToolbar.class.getClassLoader().getResource(name);
	}
	
	public String i18n(String key){
		if(rb == null){
			//ResourceBundle.ge
			//ResourceBundle.Control
			rb = ResourceBundle.getBundle("NoteBook", new Locale("zh", "CN"));
		}
		if(rb.containsKey(key)){
			String xx = rb.getString(key);
			System.out.println(key + "->" + xx);
			return xx;
		}else {
			return key;
		}
	}
}

