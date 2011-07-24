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
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.ResourceBundle;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JPopupMenu;
import javax.swing.JToolBar;
import javax.swing.JTree;
import javax.swing.KeyStroke;

import org.notebook.events.EventQueue;
import org.notebook.i18n.SimpleResourceBound;

public class MenuToolbar {
	public static final String LOADED = "Loaded";
	public static final String UPDATEDSETTINGS = "UpdatedSettings";
	public static final String OPEN = "Open";
	
	public static final String PRINT = "Print";
	
	public static final String SAVE = "Save";
	public static final String EXIT = "Exit";	
	public static final String ABOUT = "About";
	
	public static final String NEW_CATEGORY = "NewCategory";
	public static final String NEW_NOTE = "NewNote";
	public static final String DELETE = "Remove";
	public static final String RENAME = "ReName";
	
	public static final String SYNC = "Sync";
	public static final String NEXT_REC = "DataNext";
	public static final String PRE_REC = "DataPre";
	public static final String SYNCSTATUS = "SyncStatus";
	
	public static final String SETTINGS = "Settings";
	public static final String SHOWWINDOW = "ShowWindow";
	public static final String HIDEWINDOW = "HideWindow";
	
	public static final String ORDERBY_POSITION = "OrderByPosition";
	public static final String ORDERBY_CREATEDATE = "OrderByCreateDate";
	public static final String ORDERBY_UPDATEDATE = "OrderByUpdate";
	public static final String ORDERBY_NAME = "OrderByName";

	public static final String ORDER_MOVE_UP = "OrderMoveUp";
	public static final String ORDER_MOVE_DOWN = "OrderMoveDown";
	
	public static final String GUI_INITED = "GuiInited";
	
	public static final String LOAD_DOC_PANEL = "LoadDocPanel";
	public static final String LOAD_DOC_DATA = "LoadDocData";

	public static final String SAVE_DOC_PANEL = "SaveDocPanel";
	public static final String SAVE_DOC_DATA = "SaveDocData";
	
	public static final String EVENT_DATA_DOC_PANEL = "eData_doc_panel";
	public static final String EVENT_DATA_DOC_DATA = "eData_doc_data";
	public static final String EVENT_DATA_PRINT = "eData_doc_print";
	
	private static  ResourceBundle rb = new SimpleResourceBound();	
	//private MainFrame owner = null;
	private EventQueue eventQueue = null;
	private Map<String, BookAction> actions = new HashMap<String, BookAction>();
	private Collection<JComponent> extraToolBar = new ArrayList<JComponent>(); 
	
	public class BookAction extends AbstractAction {
		private static final long serialVersionUID = -6101997393914923387L;
		
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
			eventQueue.dispatchEvent(event);
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
		if(!actions.containsKey(name))
			throw new Error("Not found action by name '" + name + "'");
		return actions.get(name);
	}

	public MenuToolbar(EventQueue eventQueue){
		this.eventQueue = eventQueue;
		
		//$(OPEN, "star_off.gif", 0);
		$(LOADED, "", 0);
		$(UPDATEDSETTINGS, "", 0);
		
		$(OPEN, "folder_open.gif", KeyEvent.VK_O);				
		$(PRINT, "printview.gif", KeyEvent.VK_P);
		
		$(SAVE, "save_edit.gif", KeyEvent.VK_S);
		$(EXIT, "", 0);
		$(SETTINGS, "debugt_obj.gif", 0);
		
		$(NEW_CATEGORY, "folder_add.gif", 0);
		$(NEW_NOTE, "new_file.gif", 0);	
		$(DELETE, "delete_obj.gif", 0);
		$(RENAME, "", 0);		
		
		$(SYNC, "cvs_synch.gif", 0);			
		$(NEXT_REC, "down_load.png", 0);	
		$(PRE_REC, "up_load.png", 0);
		$(SYNCSTATUS, "", 0);
		
		$(SHOWWINDOW, "editor.gif", 0);	
		$(HIDEWINDOW, "", 0);	
		
		//$(ORDER_MOVE_UP, "up.png", KeyEvent.VK_UP);	
		//$(ORDER_MOVE_DOWN, "down.png", KeyEvent.VK_DOWN);
		$(ORDER_MOVE_UP, "", KeyEvent.VK_UP);	
		$(ORDER_MOVE_DOWN, "", KeyEvent.VK_DOWN);

		
		$(ORDERBY_POSITION, "", 0);	
		$(ORDERBY_NAME, "", 0);
		$(ORDERBY_CREATEDATE, "", 0);	
		$(ORDERBY_UPDATEDATE, "", 0);
		$(ABOUT, "", 0);
	}
		
	public JMenuBar getMenuBar(){
		JMenuBar menubar = new JMenuBar();
		JMenu fileMenu = new JMenu(i18n("File"));
		//fileMenu.set

		//fileMenu.add($(OPEN));
		//fileMenu.add($(OPEN));
		//fileMenu.add($(NEW_CATEGORY));
		//fileMenu.add($(NEW_NOTE));
		
		fileMenu.add($(PRINT));
		fileMenu.addSeparator();
		
		fileMenu.add($(SAVE));
		
		fileMenu.addSeparator();
		fileMenu.add($(EXIT));

		menubar.add(fileMenu);
		
		JMenu toolMenu = new JMenu(i18n("Tools"));
		//toolMenu.setName("工具");
		//toolMenu.add($(SYNC));
		//toolMenu.add($(SYNCDOWNLOAD));
		//toolMenu.add($(SYNCUPLOAD));
		//toolMenu.add($(SYNCSTATUS));

		toolMenu.addSeparator();
		toolMenu.add($(SETTINGS));

		JMenu aboutMenu = new JMenu(i18n("Help"));
		aboutMenu.add($(ABOUT));		

		menubar.add(fileMenu);
		menubar.add(toolMenu);
		menubar.add(aboutMenu);
		
		//menubar.add		
		//toolMenu.setName("工具");
		
		//menubar.getMenu(0).getName()		
		return menubar;
	}
	
	public JPopupMenu getNavigationContextMenu(JTree tree){
	   
	    return null;
	}

	public JToolBar getToolBar(){
		JToolBar toolbar = new JToolBar();
		
		//toolbar.add($(OPEN));
		toolbar.add($(SAVE));
		toolbar.addSeparator();
		
		toolbar.add($(PRINT));
		
		//toolbar.add($(NEW_CATEGORY));
		//toolbar.add($(NEW_NOTE));
		//toolbar.add($(DELETE));
		
		toolbar.addSeparator();
		toolbar.add($(NEXT_REC));
		toolbar.add($(PRE_REC));
		//toolbar.add($(SYNC));		
		//toolbar.addSeparator();
		//toolbar.add($(SETTINGS));
		toolbar.addSeparator();
		for(JComponent c: this.extraToolBar){
			toolbar.add(c);
		}		

		toolbar.setFloatable(true);
		return toolbar;
	}
	
	public void addExtraToolBar(JComponent[] extras){
		for(JComponent t: extras){
			this.extraToolBar.add(t);
		}
		//this.extraToolBar.a
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
	
	public static String i18n(String key){
		if(rb == null){
			rb = ResourceBundle.getBundle("NoteBook", new Locale("zh", "CN"));
		}
		if(rb.containsKey(key)){
			String xx = rb.getString(key);
			return xx;
		}else {
			return key;
		}
	}
}

