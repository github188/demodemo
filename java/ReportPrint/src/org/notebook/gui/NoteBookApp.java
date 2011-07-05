package org.notebook.gui;

import java.awt.event.ActionEvent;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.security.AccessControlException;
import java.security.AccessController;
import java.security.PrivilegedAction;
import java.security.PrivilegedActionException;
import java.security.PrivilegedExceptionAction;

import javax.swing.AbstractAction;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.Version;
import org.notebook.events.BroadCastEvent;
import org.notebook.events.EventQueue;
import org.notebook.gui.widget.LookAndFeelSelector;
import org.notebook.services.BookController;
import org.notebook.services.DefaultBookController;
import org.notebook.services.SingleInstance;
import org.notebook.services.SocketSingleInstance;

public class NoteBookApp {
	private Log log = LogFactory.getLog("app");
	//只用来判断是否已经有一个进程在运行.
	private static SingleInstance singlton = new SocketSingleInstance();
	private MainFrame main = null;
	private EventQueue eventQueue = null;
	private BookController services = null;
	private LookAndFeelSelector lafManager = null;
	
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
		//使用Event thread来初始化界面。Swing的部分控件方法只能在Event thread调用。
		eventQueue = new EventQueue(null);		
		lafManager = new LookAndFeelSelector();
		
		//把界面风格相关的事件注册。
		eventQueue.registerAction(lafManager);
		
		main = new MainFrame(eventQueue);
		services = createPrivilegedProxy(new DefaultBookController(main, runingJNLP(),
	 			runningSandbox()));
		
		eventQueue.registerAction(services.getEventActions());
		
		//主窗口显示的时候，触发加载事件。
		main.addComponentListener(new ComponentAdapter() {
	        public void componentShown(ComponentEvent ce) {
	        	eventQueue.fireEvent(MenuToolbar.LOADED, main);
	        }}
		);
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run(){
				main.initGui();
            	//窗口居中.
            	main.setLocationRelativeTo(null);
            	main.setVisible(true);
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
