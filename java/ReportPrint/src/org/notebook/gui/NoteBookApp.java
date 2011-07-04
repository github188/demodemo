package org.notebook.gui;

import java.awt.Color;
import java.security.AccessController;
import java.security.PrivilegedAction;

import javax.swing.BorderFactory;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.plaf.ColorUIResource;

import org.jvnet.lafwidget.LafWidget;
import org.jvnet.lafwidget.utils.LafConstants;
import org.jvnet.substance.SubstanceLookAndFeel;
import org.jvnet.substance.api.SubstanceColorScheme;
import org.jvnet.substance.api.SubstanceConstants;
import org.jvnet.substance.api.SubstanceSkin;
import org.notebook.Version;
import org.notebook.gui.widget.GuiUtils;
import org.notebook.services.SingleInstance;
import org.notebook.services.SocketSingleInstance;

public class NoteBookApp {
	
	//只用来判断是否已经有一个进程在运行.
	private static SingleInstance singlton = new SocketSingleInstance();
	
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
			//使用Event thread来初始化界面。Swing的部分控件方法只能在Event thread调用。
			SwingUtilities.invokeLater(new Runnable() {
				public void run(){
	            	final MainFrame main = new MainFrame();
	            	//main.log.info(Version.getVerName());
	            	//main.log.info("java.home:" + System.getProperty("java.home"));
	            	//main.log.info("java.runtime.version:" + System.getProperty("java.runtime.version"));
	            	//main.log.info("java.runtime.name:" + System.getProperty("java.runtime.name"));
	            	
	            	//main.installTrayIcon();
	            	/*
	            	if(main.runingJNLP()){
	            		main.setupJNLPSingltenService();
	            	}
	            	*/
	            	//窗口居中.
	            	main.setLocationRelativeTo(null);
	            	main.setVisible(true);
				}
	        });	
		}
	}  	

}
