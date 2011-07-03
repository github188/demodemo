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
import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.security.AccessControlException;
import java.security.AccessController;
import java.security.PrivilegedAction;
import java.security.PrivilegedActionException;
import java.security.PrivilegedExceptionAction;
import java.util.List;

import javax.swing.BorderFactory;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.plaf.ColorUIResource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.jvnet.lafwidget.LafWidget;
import org.jvnet.lafwidget.utils.LafConstants;
import org.jvnet.substance.SubstanceLookAndFeel;
import org.jvnet.substance.api.SubstanceColorScheme;
import org.jvnet.substance.api.SubstanceConstants;
import org.jvnet.substance.api.SubstanceSkin;
import org.notebook.Version;
import org.notebook.cache.Document;
import org.notebook.cache.DocumentDefine;
import org.notebook.gui.MenuToolbar.BookAction;
import org.notebook.gui.editor.SimplePrintPanel;
import org.notebook.gui.widget.GuiUtils;
import org.notebook.gui.widget.ListPane;
import org.notebook.gui.widget.QQStylePane;
import org.notebook.services.BookController;
import org.notebook.services.DefaultBookController;
import org.notebook.services.SingleInstance;
import org.notebook.services.SocketSingleInstance;

public class MainFrame extends JFrame {
	private static final long serialVersionUID = -4362026054606144515L;
	private Log log = LogFactory.getLog("main");

	public MenuToolbar menu = null;	
	//public DocumentEditor editor = null;
	public StatusBar statusBar = null;
	public SimplePrintPanel mainPanel = null;

	private BookController controller = null;
	private JScrollPane _panel = null;
		
	//只用来判断是否已经有一个进程在运行.
	private static SingleInstance singlton = new SocketSingleInstance();
	//初始化托盘时, Controller还没有创建。把安装的结果临时保存。等Contoller创建后，再传给Controller.
	private boolean visibleTrayIcon = false;
	
	public MainFrame(){
		super();
		this.setTitle("动物免疫公文打印&查询系统");
		setLayout(new BorderLayout());
		this.initGui();
		pack(); 
		setSize(670,548);
	}
	
	public void initNoteBookController(){
		controller =  createPrivilegedProxy(new DefaultBookController(this,
						runingJNLP(),
					 	runningSandbox()
					  ));
		controller.setVisibleTrayIcon(visibleTrayIcon);
		controller.dispatchEvent(MenuToolbar.LOADED);
	}
	
	private boolean runningSandbox(){
		try {
			System.getenv("USERNAME");
			return false;
		}catch(AccessControlException e){
			return true;
		}
	}
	
	private boolean runingJNLP(){
		try {
			Class.forName("javax.jnlp.ServiceManager");
			return true;
		} catch (ClassNotFoundException e) {
			return false;
		}
	}
	
	@SuppressWarnings("unchecked")
	public static void main(final String[] args){
		//Inno Setup
		System.out.println(Version.getVerName());
		System.out.println("java.home:" + System.getProperty("java.home"));
		System.out.println("java.runtime.version:" + System.getProperty("java.runtime.version"));
		System.out.println("java.runtime.name:" + System.getProperty("java.runtime.name"));
    	
    	AccessController.doPrivileged(
				new PrivilegedAction() {
					public Object run(){
						mainPrivileged(args);
						return null;
					}
				});				
	}
 	
    public static void mainPrivileged(String[] args){
		try{
	        /*
			UIManager.setLookAndFeel(
		            UIManager.getSystemLookAndFeelClassName());
		    */
            UIManager.setLookAndFeel(
            		"org.jvnet.substance.skin.SubstanceOfficeBlue2007LookAndFeel"
                    //ch.randelshofer.quaqua.QuaquaManager.getLookAndFeel()
                );
			UIManager.put(LafWidget.ANIMATION_KIND, LafConstants.AnimationKind.NONE);
			UIManager.put(SubstanceLookAndFeel.TABBED_PANE_CONTENT_BORDER_KIND,
						SubstanceConstants.TabContentPaneBorderKind.SINGLE_FULL);
            
			JFrame.setDefaultLookAndFeelDecorated(true);
			JDialog.setDefaultLookAndFeelDecorated(true);   
			Color c = SubstanceLookAndFeel.getCurrentSkin().getMainActiveColorScheme().getMidColor();			
			UIManager.put("ToolTip.border", BorderFactory.createLineBorder(c));
			UIManager.put("ToolTip.background", new ColorUIResource(Color.WHITE));
			UIManager.put("ToolTip.foreground", new ColorUIResource(Color.BLACK));
			
			SubstanceSkin skin = SubstanceLookAndFeel.getCurrentSkin();
			SubstanceColorScheme scheme = skin.getMainActiveColorScheme();
			
			GuiUtils.putLookAndFeelColor("borderColor", scheme.getMidColor());
			GuiUtils.putLookAndFeelColor("lightColor", scheme.getLightColor());
			GuiUtils.putLookAndFeelColor("lightBackgroundFillColor",
										 scheme.getLightBackgroundFillColor());
			GuiUtils.putLookAndFeelColor("darkColor", scheme.getDarkColor());
			GuiUtils.putLookAndFeelColor("backgroundFillColor",
										 scheme.getBackgroundFillColor());
			GuiUtils.putLookAndFeelColor("lineColor", scheme.getLineColor());
			GuiUtils.putLookAndFeelColor("selectionForegroundColor",
										 scheme.getSelectionForegroundColor());
			GuiUtils.putLookAndFeelColor("selectionBackgroundColor",
										 scheme.getSelectionBackgroundColor());
			GuiUtils.putLookAndFeelColor("foregroundColor",
										 scheme.getForegroundColor());
			GuiUtils.putLookAndFeelColor("focusRingColor",
										 scheme.getFocusRingColor());
			
		}catch(Exception e){
			System.out.println(e.toString());
		}
		
    	//showMessageBox("ss");
		if(singlton.checkRunning(null)){
			JOptionPane.showMessageDialog(null,
				    "NoteBook已经在运行中.",
				    "Error",
				    JOptionPane.ERROR_MESSAGE);
			System.exit(1);
		}else {
			//使用Event thread来初始化界面。Swing的部分控件方法只能在Event thread调用。
			SwingUtilities.invokeLater(new Runnable() {
				public void run(){
	            	final MainFrame main = new MainFrame();
	            	main.log.info(Version.getVerName());
	            	main.log.info("java.home:" + System.getProperty("java.home"));
	            	main.log.info("java.runtime.version:" + System.getProperty("java.runtime.version"));
	            	main.log.info("java.runtime.name:" + System.getProperty("java.runtime.name"));
	            	
	            	//main.installTrayIcon();
	            	if(main.runingJNLP()){
	            		main.setupJNLPSingltenService();
	            	}
	            	//窗口居中.
	            	main.setLocationRelativeTo(null);
	            	main.setVisible(true);
				}
	        });	
		}
	}  
    
    public void updateDocumentDefine(DocumentDefine doc){
    	_panel.remove(this.mainPanel); 
    	this.mainPanel = new SimplePrintPanel();
    	this.mainPanel.bar = this.statusBar;
    	this.mainPanel.loadDocument(doc);
    	_panel.getViewport().add(this.mainPanel);
    }
    
    public void updateDocumentData(Document doc){
    	this.mainPanel.showDocument(doc);
    }
    
    public Document getDocumentData(){
    	return this.mainPanel.getDocumentData();
    }        
    
    public List<Image> printViews(){
    	return this.mainPanel.getPrintScreen();
    }    
    
    
    public DocumentDefine saveDocumentDefine(){
    	return this.mainPanel.saveLayout();
    }    
    
    protected QQStylePane getNavigationBar(){
    	QQStylePane pane = new QQStylePane();    	
    	pane.setAnimated(true);
    	
		ListPane p = new ListPane();
		p.addItem("证明1", MenuToolbar.icon("file_obj.gif"), null);
		p.addItem("证明2", MenuToolbar.icon("file_obj.gif"), null);
		p.setSize(185, 74);
    	
		pane.addPane("免疫证明1", MenuToolbar.icon("editor.gif"), p);
		
		p = new ListPane();
		p.addItem("证明21", MenuToolbar.icon("file_obj.gif"), null);
		p.addItem("证明22", MenuToolbar.icon("file_obj.gif"), null);
		p.setSize(185, 74);		
		pane.addPane("免疫证明2", MenuToolbar.icon("editor.gif"), p);
		
		p = new ListPane();
		p.addItem("申请材料11", MenuToolbar.icon("file_obj.gif"), null);
		p.addItem("申请材料12", MenuToolbar.icon("file_obj.gif"), null);
		p.setSize(185, 74);				
		pane.addPane("免疫申请材料1", MenuToolbar.icon("file_obj.gif"), p);

		p = new ListPane();
		p.addItem("申请材料21", MenuToolbar.icon("file_obj.gif"), null);
		p.addItem("申请材料22", MenuToolbar.icon("file_obj.gif"), null);
		p.setSize(185, 74);						
		pane.addPane("免疫申请材料2", MenuToolbar.icon("file_obj.gif"), p);
		
		pane.setSelectedPane(0);
    	
    	return pane;
    }
    
    

	protected void initGui() {
		menu = new MenuToolbar(this);
		this.getRootPane().setJMenuBar(menu.getMenuBar());
		
		//editor = new DocumentEditor();
		//menu.addExtraToolBar(editor.getToolBar());

		//JScrollPane leftTree = new JScrollPane(tree);
		Dimension minSize = new Dimension(150, 400);
		
		mainPanel = new SimplePrintPanel();		
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

		Container contentPane = getContentPane();
		contentPane.add(menu.getToolBar(), BorderLayout.NORTH);
		
		contentPane.add(splitPanel, BorderLayout.CENTER);
		//contentPane.add(_panel, BorderLayout.CENTER);
		
		contentPane.add(statusBar, BorderLayout.SOUTH);
		
		//controller = createPrivilegedProxy(new DefaultBookController(this));
		
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	    addComponentListener(new ComponentAdapter() {
	        public void componentShown(ComponentEvent ce) {
	        	if(controller == null){
	        		try{
	        			initNoteBookController();
	        			setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
	        			//setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
	        		    addWindowListener(new WindowAdapter(){
	        		    	public void windowClosing(WindowEvent e){
	        		    		processEvent(menu.$(MenuToolbar.EXIT));
	        		    	}
	        		    });
	        		}catch(Exception e){
	        			JOptionPane.showMessageDialog(null,
	        				    "程序初始化出错!",
	        				    "Error",
	        				    JOptionPane.ERROR_MESSAGE);
	        			e.printStackTrace();
	        		}
	        	}
	        }
	    });
	    	    
	    this.setIconImage(appIcon16());
	}	
	
	public void processEvent(BookAction e) {
		if(controller != null){
			this.controller.processEvent(e);
		}else {
			JOptionPane.showMessageDialog(null,
				    "程序初始化失败!",
				    "Error",
				    JOptionPane.ERROR_MESSAGE);			
		}
	}
	
	public void showSettings(){
		//NoteBookSettings settings = new NoteBookSettings(this, controller);		
		//settings.setLocationRelativeTo(this);
		//settings.setVisible(true);
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
						processEvent(menu.$(MenuToolbar.SHOWWINDOW));
					}});
				st.add(ti);
				visibleTrayIcon = true;
		    }
		}catch (Exception e){
			log.error(e, e);
		}
	}
	
	private void setupJNLPSingltenService(){
		try {
			Class cl = Class.forName("org.notebook.services.JNLPSingleInstance");
			SingleInstance ins = (SingleInstance)cl.newInstance();
			ins.checkRunning(this);
		}catch(ClassNotFoundException e) {
			log.error("failed to load JNLP JNLPSingleInstance", e);
		} catch (Exception e) {
			log.error("failed to setup JNLP SingleInstanceService", e);
		}
	}
	
	private BookController createPrivilegedProxy(final BookController stub){
    	return (BookController)Proxy.newProxyInstance(MainFrame.class.getClassLoader(), 
				   new Class[]{BookController.class}, 
				   new InvocationHandler(){
						@SuppressWarnings("unchecked")
						public Object invoke(Object prxoy, final Method method,
								final Object[] args) throws Throwable {
							try{
								return AccessController.doPrivileged(
										new PrivilegedExceptionAction() {
											public Object run() throws Exception{
												return method.invoke(stub, args);
											}
										});	
							}catch (PrivilegedActionException e){
								String name = method.getName();
								log.error("Exception:" + e.getCause().toString() +
										  "\n Method:" + name,
										  e);
								throw e.getException();
							}
						}
					}
			);		
	}
}

