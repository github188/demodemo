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

import javax.swing.*;
import javax.swing.tree.*;
import java.awt.*;
import java.awt.event.*; 
import javax.swing.event.*;
import java.net.*;

public class MenuToolbar {
	static class BookAction extends AbstractAction {
		public BookAction() {
			//getResource(String name) 
			super("New ...", new ImageIcon(_("org/notebook/gui/star_off.gif")));
		}
		public void actionPerformed(ActionEvent event) {
			System.out.println("new...");
		}
	}	
	
	private static Action[] actions = {
		new BookAction(),
	};

	public static JMenuBar menuBar(){
		JMenuBar menubar = new JMenuBar();
		JMenu fileMenu = new JMenu("File");

		fileMenu.add(actions[0]);

		menubar.add(fileMenu);
		return menubar;
	}

	public static JToolBar toolBar(){
		JToolBar toolbar = new JToolBar();

		toolbar.add(actions[0]);
		return toolbar;
	}

	protected static URL _(String name){
		return MenuToolbar.class.getClassLoader().getResource(name);
	}
}

