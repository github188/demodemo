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
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.events.BroadCastEvent;
import org.notebook.events.EventAction;
import org.notebook.events.EventQueue;
import org.notebook.gui.editor.SimplePrintPanel;
import org.notebook.gui.widget.ListPane;
import org.notebook.gui.widget.QQStylePane;

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
	
	public MainFrame(EventQueue eventQueue){
		super();
		this.setTitle("动物免疫公文打印&查询系统");
		this.events = eventQueue;
		this.mainFrame = this;
	}
	
    protected QQStylePane getNavigationBar(){
    	QQStylePane pane = new QQStylePane();    	
    	pane.setAnimated(true);
    	
		ListPane p = new ListPane();
		p.addItem("证明1", MenuToolbar.icon("org/notebook/gui/images/file_obj.gif"), null);
		p.addItem("证明2", MenuToolbar.icon("org/notebook/gui/images/file_obj.gif"), null);
		p.setSize(185, 74);
    	
		pane.addPane("免疫证明1", MenuToolbar.icon("org/notebook/gui/images/editor.gif"), p);
		
		p = new ListPane();
		p.addItem("证明21", MenuToolbar.icon("org/notebook/gui/images/file_obj.gif"), null);
		p.addItem("证明22", MenuToolbar.icon("org/notebook/gui/images/file_obj.gif"), null);
		p.setSize(185, 74);		
		pane.addPane("免疫证明2", MenuToolbar.icon("org/notebook/gui/images/editor.gif"), p);
		
		p = new ListPane();
		p.addItem("申请材料11", MenuToolbar.icon("org/notebook/gui/images/file_obj.gif"), null);
		p.addItem("申请材料12", MenuToolbar.icon("org/notebook/gui/images/file_obj.gif"), null);
		p.setSize(185, 74);				
		pane.addPane("免疫申请材料1", MenuToolbar.icon("org/notebook/gui/images/file_obj.gif"), p);

		p = new ListPane();
		p.addItem("申请材料21", MenuToolbar.icon("org/notebook/gui/images/file_obj.gif"), null);
		p.addItem("申请材料22", MenuToolbar.icon("org/notebook/gui/images/file_obj.gif"), null);
		p.setSize(185, 74);						
		pane.addPane("免疫申请材料2", MenuToolbar.icon("org/notebook/gui/images/file_obj.gif"), p);
		
		pane.setSelectedPane(0);
    	
    	return pane;
    }

	public void initGui() {
		setLayout(new BorderLayout());
		menu = new MenuToolbar(events);
		this.getRootPane().setJMenuBar(menu.getMenuBar());
		
		//触发菜单创建成功事件，其他控件，可以响应事件并更新自己特有的菜单。
		events.fireEvent(MenuToolbar.MENU_LOADED, this);
		
		//editor = new DocumentEditor();
		//menu.addExtraToolBar(editor.getToolBar());

		//JScrollPane leftTree = new JScrollPane(tree);
		Dimension minSize = new Dimension(150, 400);
		
		mainPanel = new SimplePrintPanel();	
		events.registerAction(mainPanel.getEventsHandler());
		_panel = new JScrollPane(mainPanel);
		_panel.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS); 		
		
		JSplitPane splitPanel; 
		splitPanel = new JSplitPane();
		splitPanel.setOrientation(JSplitPane.HORIZONTAL_SPLIT );
		//splitPanel.setAutoscrolls(true);
		splitPanel.setDividerLocation(0.35);
		splitPanel.setOneTouchExpandable(true);
		splitPanel.setLeftComponent(getNavigationBar());
		splitPanel.setRightComponent(_panel);		
		
		statusBar = new StatusBar();
		mainPanel.bar = statusBar;
		
		Container contentPane = getContentPane();
		contentPane.add(menu.getToolBar(), BorderLayout.NORTH);
		
		contentPane.add(splitPanel, BorderLayout.CENTER);
		//contentPane.add(_panel, BorderLayout.CENTER);
		
		contentPane.add(statusBar, BorderLayout.SOUTH);
		
		//controller = createPrivilegedProxy(new DefaultBookController(this));
		
		//setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setIconImage(appIcon16());

		setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		final MainFrame mainFrame = this;

	    addWindowListener(new WindowAdapter(){
	    	//窗口关闭，
	    	public void windowClosing(WindowEvent e){
	    		events.fireEvent(MenuToolbar.EXIT, mainFrame);
	    	}
	    	
	    	//窗口打开，触发初始化加载事件。
	    	public void windowOpened(WindowEvent e){
	    		events.fireEvent(MenuToolbar.LOADED, mainFrame);
	    	}
	    });
	    events.registerAction(this);
	    	    	    	    
		pack(); 
		setSize(670,548);
	    
	}
	
	public void showSettings(){
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

