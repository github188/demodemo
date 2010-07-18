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
import java.awt.*;
import java.awt.event.*; 
import javax.swing.event.*;
import java.net.*;

public class Main extends JFrame {

	private	JList actionList = null;

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
		Action[] actions = {
			new NewAction(),
		};
		JToolBar toolbar = new JToolBar();
		JMenuBar menubar = new JMenuBar();
		JMenu fileMenu = new JMenu("File");

		for(int i=0; i < actions.length; ++i) {
			fileMenu.add(actions[i]);

			//if(i != actions.length-1)
			toolbar.add(actions[i]);

			if(i == 2 || i == actions.length-2){
				toolbar.addSeparator();
				fileMenu.addSeparator();
			}
		}
		Container contentPane = getContentPane();
		menubar.add(fileMenu);
		contentPane.add(toolbar, BorderLayout.NORTH);
		getRootPane().setJMenuBar(menubar);

		JTextArea textArea = createTextArea();

		Action[] actions2 = textArea.getActions();

		actionList = createActionList(actions2);

		JSplitPane splitPane = new JSplitPane(
			JSplitPane.HORIZONTAL_SPLIT, 
			new JScrollPane(textArea),
			new JScrollPane(actionList));

		contentPane.add(splitPane, BorderLayout.CENTER);
	}

	class NewAction extends AbstractAction {
		public NewAction() {
			//getResource(String name) 
			super("New ...", new ImageIcon(_("org/notebook/gui/star_off.gif")));
		}
		public void actionPerformed(ActionEvent event) {
			System.out.println("new...");
		}
	}

	protected URL _(String name){
		return getClass().getClassLoader().getResource(name);
	}

	private JTextArea createTextArea() {
		JTextArea textArea = new JTextArea(
		"line one\nline two\nline three\nline four");

		textArea.setFont(new Font("Dialog", Font.ITALIC, 24));
		textArea.getCaret().setBlinkRate(0);
		return textArea;
	}

	private JList createActionList(Action[] actions) {
		DefaultListModel model = new DefaultListModel();
		final JList list = new JList(model);

		list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

		for(int i=0; i < actions.length; ++i) {
			model.addElement(actions[i]);
		}

		list.setCellRenderer(new DefaultListCellRenderer() {
			public Component getListCellRendererComponent(
				JList list, Object value,
				int index, boolean isSelected,
				boolean cellHasFocus) {
					super.getListCellRendererComponent(list, value,
					index, isSelected, cellHasFocus);

					Action action = (Action)value;
					setText((String)action.getValue(Action.NAME));

					return this;
			}
		});

		list.addListSelectionListener(new ListSelectionListener(){
			public void valueChanged(ListSelectionEvent e) {
				if(!e.getValueIsAdjusting()) {
					Action source = (Action)actionList.getSelectedValue();	
					System.out.println("Action: " +  (String)source.getValue(Action.NAME));
				}
			}
			}
		);

		return list;
	}



}