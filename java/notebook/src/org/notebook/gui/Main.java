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

public class Main extends JFrame {

	public Main(){
		super("Deon的记事本"); 
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

		Main main = new Main(); 
		main.setVisible(true); 
	}  

	protected void initGui() {
		this.getRootPane().setJMenuBar(MenuToolbar.menuBar());

		Container contentPane = getContentPane();

		JSplitPane splitPane = new JSplitPane(
				JSplitPane.HORIZONTAL_SPLIT, 
				new JScrollPane(new NavigationTree()),
				new JScrollPane(new DocumentEditor())
		);

		contentPane.add(MenuToolbar.toolBar(), BorderLayout.NORTH);
		contentPane.add(splitPane, BorderLayout.CENTER);
		contentPane.add(new StatusBar(), BorderLayout.SOUTH);
	}

}

