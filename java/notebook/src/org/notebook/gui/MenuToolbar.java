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

import javax.swing.AbstractAction;
import javax.swing.ImageIcon;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JToolBar;
import javax.swing.KeyStroke;

public class MenuToolbar {
	public static final String OPEN = "Open";
	public static final String SAVE = "Save";
	public static final String EXIT = "Exit";
	private MainFrame owner = null;
	public class BookAction extends AbstractAction {
		private static final long serialVersionUID = -6101997393914923387L;
		public BookAction(String name, String icon, int accelerator){
			super(name, new ImageIcon(_("org/notebook/gui/" + icon + ".gif")));
			if(accelerator > 0){
				this.putValue(this.ACCELERATOR_KEY, 
						KeyStroke.getKeyStroke(accelerator, ActionEvent.CTRL_MASK));
			}
		}
		public void actionPerformed(ActionEvent event) {
			//System.out.println("event:" + this.getValue(NAME) + ",id:" + event.getID());			
			owner.processEvent(this);
		}
	}
	private BookAction[] actions = new BookAction[]{
			new BookAction(OPEN, "star_off", 0),
			new BookAction(SAVE, "star_off", KeyEvent.VK_S),
			new BookAction(EXIT, "star_off", 0),
	};

	public MenuToolbar(MainFrame owner){
		this.owner = owner;
	}
		
	public JMenuBar getMenuBar(){
		JMenuBar menubar = new JMenuBar();
		JMenu fileMenu = new JMenu("File");

		fileMenu.add(actions[0]);
		fileMenu.add(actions[1]);		
		fileMenu.add(actions[2]);

		menubar.add(fileMenu);
		return menubar;
	}

	public JToolBar getToolBar(){
		JToolBar toolbar = new JToolBar();

		toolbar.add(actions[0]);
		return toolbar;
	}

	protected static URL _(String name){
		return MenuToolbar.class.getClassLoader().getResource(name);
	}
}

