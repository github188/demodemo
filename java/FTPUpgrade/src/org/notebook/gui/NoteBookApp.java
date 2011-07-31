package org.notebook.gui;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.net.URL;
import java.security.AccessControlException;
import java.security.AccessController;
import java.security.PrivilegedAction;
import java.security.PrivilegedActionException;
import java.security.PrivilegedExceptionAction;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.Version;
import org.notebook.events.EventQueue;
import org.notebook.services.BookController;
import org.notebook.services.DefaultBookController;
import org.notebook.services.SingleInstance;
import org.notebook.services.SocketSingleInstance;
import org.notebook.xui.XUIContainer;

public class NoteBookApp {
	private Log log = LogFactory.getLog("app");
	//只用来判断是否已经有一个进程在运行.
	private static SingleInstance singlton = new SocketSingleInstance();
	private JFrame main = null;
	private EventQueue eventQueue = null;
	private BookController services = null;
	private XUIContainer xui = new XUIContainer();
	
	@SuppressWarnings("unchecked")
	public static void main(final String[] args){
		//Inno Setup
		System.out.println(Version.getVerName());
		System.out.println("java.home:" + System.getProperty("java.home"));
		System.out.println("java.runtime.version:" + System.getProperty("java.runtime.version"));
		System.out.println("java.runtime.name:" + System.getProperty("java.runtime.name"));
		System.out.println("jar:" + System.getProperty("java.runtime.name"));
		//URL curJar = NoteBookApp.class.getProtectionDomain().getCodeSource().getLocation();
    	
    	AccessController.doPrivileged(
				new PrivilegedAction() {
					public Object run(){
						mainPrivileged(args);
						return null;
					}
				});				
	}
	
	public static void mainPrivileged(String[] args){
    	//showMessageBox("ss");
		if(singlton.checkRunning(null)){
			JOptionPane.showMessageDialog(null,
				    "NoteBook已经在运行中.",
				    "Error",
				    JOptionPane.ERROR_MESSAGE);
			System.exit(1);
		}else {
			 new NoteBookApp().startup();
		}
	}
	
	public void startup(){
		eventQueue = new EventQueue(null);		
		//lafManager = new LookAndFeelSelector();		
		//lafManager.setLookAndFeel(LookAndFeelSelector.DEFAULT_SKIN); 
		
		//把界面风格相关的事件注册。
		//eventQueue.registerAction(lafManager);
		
		xui.setEventQueue(eventQueue);
		//xui.
		
		try{
			UIManager.setLookAndFeel(
		            UIManager.getSystemLookAndFeelClassName());
		}catch(Exception e){
			System.out.println(e.toString());
		}	
		
		services = createPrivilegedProxy(new DefaultBookController(runingJNLP(),
	 			runningSandbox()));	
		
		eventQueue.registerAction(services.getEventActions());		
		/*
		 * 使用Event thread来初始化界面。Swing的部分控件方法只能在Event thread调用。
		 * 使用自定义LAF后，只能在Event thread创建JFrame
		 */
		SwingUtilities.invokeLater(new Runnable() {
			public void run(){
				URL layout = this.getClass().getClassLoader().getResource("org/notebook/layout/MainFrame.xml");
				if(layout != null) {
					xui.load(layout);				
					main = (JFrame)xui.getByName("main");
					
					services.setTopWindow(main);
					//main.initGui();
	            	//窗口居中.
	            	main.setLocationRelativeTo(null);
	            	main.setVisible(true);
				}else {
					log.error("Not found applicaton layout configuration.");
				}
			}
        });	
		
	}
	
	private boolean runingJNLP(){
		try {
			Class.forName("javax.jnlp.ServiceManager");
			return true;
		} catch (ClassNotFoundException e) {
			return false;
		}
	}
	
	private boolean runningSandbox(){
		try {
			System.getenv("USERNAME");
			return false;
		}catch(AccessControlException e){
			return true;
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
