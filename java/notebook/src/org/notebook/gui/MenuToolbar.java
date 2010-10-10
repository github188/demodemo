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

import java.awt.MenuItem;
import java.awt.PopupMenu;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.io.UnsupportedEncodingException;
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
	public static final String OPENNOTEBOOK = "OpenNoteBook";
	public static final String UPDATEDSETTINGS = "UpdatedSettings";
	public static final String OPEN = "Open";
	public static final String SAVE = "Save";
	public static final String EXIT = "Exit";
	
	public static final String NEW_CATEGORY = "NewCategory";
	public static final String NEW_NOTE = "NewNote";
	public static final String DELETE = "Remove";
	
	public static final String SYNC = "Sync";
	public static final String SYNCDOWNLOAD = "SyncDownLoad";
	public static final String SYNCUPLOAD = "SyncUpLoad";
	
	public static final String SETTINGS = "Settings";
	public static final String SHOWWINDOW = "ShowWindow";
	public static final String HIDEWINDOW = "HideWindow";
	
	
	private MainFrame owner = null;
	private ResourceBundle rb = new SimpleResourceBound();	
	private Map<String, BookAction> actions = new HashMap<String, BookAction>();
	
	public class BookAction extends AbstractAction {
		private static final long serialVersionUID = -6101997393914923387L;
		//private boolean processing = false;
		public Object attachedObject = null;
		public ActionEvent attachedEvent = null;
		
		public BookAction(String name, String icon, int accelerator){
			super(name, icon("org/notebook/gui/images/" + icon));
			if(accelerator > 0){
				this.putValue(ACCELERATOR_KEY, 
						KeyStroke.getKeyStroke(accelerator, ActionEvent.CTRL_MASK));
			}
			this.putValue(Action.ACTION_COMMAND_KEY, name);
			this.putValue(Action.NAME, i18n(name));
		}
		public void actionPerformed(ActionEvent event) {
			//this.SHORT_DESCRIPTION
			this.actionPerformed(event, null);
		}
		public synchronized void actionPerformed(ActionEvent event, Object param) {
			if(!this.isEnabled())return;
			//this.processing = true;
			try{
				this.attachedObject = param;
				this.attachedEvent = event;
				owner.processEvent(this);
			}finally{
				//this.processing = false;
				this.attachedObject = null;
				this.attachedEvent = null;
			}
		}
		
		/**
		 * 用于系统托盘菜单.
		 * @return
		 */
		public MenuItem toAWTMenu(){
			MenuItem item = new MenuItem();
			item.setName((String)this.getValue(Action.ACTION_COMMAND_KEY));
			item.setLabel((String)this.getValue(Action.ACTION_COMMAND_KEY));
			final AbstractAction action = this; 
			item.addActionListener(new ActionListener(){
				@Override
				public void actionPerformed(ActionEvent e) {
					action.actionPerformed(e);
				}});			
			return item;
		}
	}
		
	public void $(String name, String icon, int accelerator){
		actions.put(name, new BookAction(name, icon, accelerator));
	}
	public BookAction $(String name){
		return actions.get(name);
	}

	public MenuToolbar(MainFrame owner){
		this.owner = owner;
		//$(OPEN, "star_off.gif", 0);
		$(OPENNOTEBOOK, "", 0);
		$(UPDATEDSETTINGS, "", 0);
		
		$(OPEN, "", KeyEvent.VK_O);
		$(SAVE, "save_edit.gif", KeyEvent.VK_S);
		$(EXIT, "", 0);
		$(SETTINGS, "debugt_obj.gif", 0);
		
		$(NEW_CATEGORY, "folder_add.gif", 0);
		$(NEW_NOTE, "new_file.gif", 0);	
		$(DELETE, "delete_obj.gif", 0);
		$(SYNC, "cvs_synch.gif", 0);	
		
		$(SHOWWINDOW, "editor.gif", 0);	
		$(HIDEWINDOW, "", 0);	
	}
		
	public JMenuBar getMenuBar(){
		JMenuBar menubar = new JMenuBar();
		JMenu fileMenu = new JMenu("File");

		//fileMenu.add($(OPEN));
		fileMenu.add($(NEW_CATEGORY));
		fileMenu.add($(NEW_NOTE));
		fileMenu.addSeparator();
		
		fileMenu.add($(SAVE));
		
		fileMenu.addSeparator();
		fileMenu.add($(EXIT));

		menubar.add(fileMenu);
		
		JMenu toolMenu = new JMenu("Tools");
		toolMenu.add($(SETTINGS));
		toolMenu.add($(SYNC));

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
		
		//toolbar.add($(OPEN));
		toolbar.add($(SAVE));
		toolbar.addSeparator();
		toolbar.add($(NEW_CATEGORY));
		toolbar.add($(NEW_NOTE));
		toolbar.add($(DELETE));
		toolbar.addSeparator();
		toolbar.add($(SYNC));
		toolbar.add($(SETTINGS));
		return toolbar;
	}
	
	public PopupMenu getTrayMenu(){
	    final PopupMenu menu = new PopupMenu();
	    //menu.
	    //JMenuItem newCategory = new JMenuItem("新建目录");	
	    //MenuItem xx = new MenuItem();
	    //xx.s
	    //menu.a
	    menu.add($(SHOWWINDOW).toAWTMenu());
	    menu.addSeparator();
	    menu.add($(EXIT).toAWTMenu());
	    
		return menu;
	}	

	public static ImageIcon icon(String name){
		ImageIcon icon = null;
		try{
			icon = new ImageIcon(MenuToolbar.class.getClassLoader().getResource(name));
		}catch(Exception e){
			System.out.println("failed to load:" + name);
			//e.printStackTrace();
		}
		return icon;
	}
	
	public String i18n(String key){
		if(rb == null){
			//ResourceBundle.ge
			//ResourceBundle.Control
			rb = ResourceBundle.getBundle("NoteBook", new Locale("zh", "CN"));
		}
		if(rb.containsKey(key)){
			String xx = rb.getString(key);
			//System.out.println(key + "->" + xx);
			return xx;
		}else {
			return key;
		}
	}
}

