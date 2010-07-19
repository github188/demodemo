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
		contentPane.add(toolbar, BorderLayout.NORTH);

		menubar.add(fileMenu);
		getRootPane().setJMenuBar(menubar);

		JTextArea textArea = createTextArea();

		JSplitPane splitPane = new JSplitPane(
				JSplitPane.HORIZONTAL_SPLIT, 
				new JScrollPane(createJTree()),
				new JScrollPane(textArea)
		);

		contentPane.add(splitPane, BorderLayout.CENTER);

		contentPane.add(new StatusBar(), BorderLayout.SOUTH);
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

	private JTree createJTree() {
		DefaultMutableTreeNode root = new DefaultMutableTreeNode("JTree");
		DefaultMutableTreeNode parent = new DefaultMutableTreeNode("colors");
		root.add(parent);
		parent.add(new DefaultMutableTreeNode("blue"));
		parent.add(new DefaultMutableTreeNode("violet"));
		parent.add(new DefaultMutableTreeNode("red"));
		parent.add(new DefaultMutableTreeNode("yellow"));

		DefaultTreeModel model = new DefaultTreeModel(root);

		return new JTree(model);
	}

	class StatusBar extends JPanel {
		public StatusBar() {
			setLayout(new BorderLayout());
			setPreferredSize(new Dimension(10, 23));

			JPanel rightPanel = new JPanel(new BorderLayout());
			rightPanel.add(new JLabel("xxx"), BorderLayout.SOUTH);
			rightPanel.setOpaque(false);

			add(rightPanel, BorderLayout.EAST);
			setBackground(SystemColor.control);
		}

		protected void paintComponent(Graphics g) {
			super.paintComponent(g);

			int y = 0;
			g.setColor(new Color(156, 154, 140));
			g.drawLine(0, y, getWidth(), y);
			y++;
			g.setColor(new Color(196, 194, 183));
			g.drawLine(0, y, getWidth(), y);
			y++;
			g.setColor(new Color(218, 215, 201));
			g.drawLine(0, y, getWidth(), y);
			y++;
			g.setColor(new Color(233, 231, 217));
			g.drawLine(0, y, getWidth(), y);

			y = getHeight() - 3;
			g.setColor(new Color(233, 232, 218));
			g.drawLine(0, y, getWidth(), y);
			y++;
			g.setColor(new Color(233, 231, 216));
			g.drawLine(0, y, getWidth(), y);
			y = getHeight() - 1;
			g.setColor(new Color(221, 221, 220));
			g.drawLine(0, y, getWidth(), y);
		}
	}

}

