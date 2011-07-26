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

import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.events.BroadCastEvent;
import org.notebook.events.EventAction;
import org.notebook.events.EventQueue;
import org.notebook.gui.editor.SimplePrintPanel;

public class MainFrame extends JFrame {
	private static final long serialVersionUID = -4362026054606144515L;
	private Log log = LogFactory.getLog("main");

	public MenuToolbar menu = null;	
	public EventQueue events = null;
	public StatusBar statusBar = null;
	public SimplePrintPanel mainPanel = null;

	private JScrollPane _panel = null;
		
	//初始化托盘时, Controller还没有创建。把安装的结果临时保存。等Contoller创建后，再传给Controller.
	private boolean visibleTrayIcon = false;

	//内部类需要引用。
	private MainFrame mainFrame = null;
	//private LookAndFeelSelector laf
	
	public MainFrame(){
		super();
	}
	
	public MainFrame(EventQueue eventQueue){
		super();
		//this.setTitle("动物免疫公文打印&查询系统");
		this.events = eventQueue;
		this.mainFrame = this;
		
		//setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	    
    public void addToolBar(MenuToolbar bar){
    	menu = new MenuToolbar(events);
    	this.getRootPane().setJMenuBar(menu.getMenuBar());
    }

	
	public void showSettings(){
		JPanel p = new JPanel();
		//p.setSize(width, height)
		//NoteBookSettings settings = new NoteBookSettings(this, controller);		
		//settings.setLocationRelativeTo(this);
		//settings.setVisible(true);
	}
	
	@EventAction(order=1)
	public void About(BroadCastEvent event){
		AboutDialog about = new AboutDialog(mainFrame);
		about.setLocationRelativeTo(mainFrame);
		about.setVisible(true);
		event.done();
	}
	
		
	public void status(String msg){
		this.statusBar.setText(msg);
	}
	
	private Image appIcon(){
		return MenuToolbar.icon("org/notebook/gui/images/application.png").getImage();			
	}
	
	private Image appIcon16(){
		return appIcon().getScaledInstance(16, 16, Image.SCALE_SMOOTH);			
	}
	
	
	private void installTrayIcon(){
		visibleTrayIcon = false;
		try {
			if(java.awt.SystemTray.isSupported()){// 判断当前平台是否支持系统托盘
				java.awt.SystemTray  st = java.awt.SystemTray.getSystemTray();
				java.awt.TrayIcon ti = new java.awt.TrayIcon(appIcon16());
				ti.setToolTip (this.getTitle());
				ti.setPopupMenu(menu.getTrayMenu());
				ti.addActionListener(new ActionListener(){
					@Override
					public void actionPerformed(ActionEvent e) {
						events.fireEvent(MenuToolbar.SHOWWINDOW, mainFrame);
					}});
				st.add(ti);
				visibleTrayIcon = true;
		    }
		}catch (Exception e){
			log.error(e, e);
		}
	}
}

