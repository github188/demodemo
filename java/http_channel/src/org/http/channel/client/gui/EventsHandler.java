package org.http.channel.client.gui;

import java.awt.CardLayout;

import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JTextField;

import org.http.channel.client.ProxyClient;
import org.http.channel.client.gui.events.BroadCastEvent;
import org.http.channel.client.gui.events.EventAction;
import org.http.channel.client.gui.xui.XUIContainer;

public class EventsHandler {
	public static final String REMOTE_DOMAIN = "remote_domain";
	public static final String INTERNAL_DOMAIN = "internal_domain";
	public static final String PROXY_PASSWORD = "proxy_password";
	public static final String HTTP_PROXY = "http_proxy";
	
	public static final String STATUS_REMOTE = "status_remote";
	public static final String STATUS_LOCAL = "status_local";
	public static final String STATUS_REQUEST = "status_request";
	public static final String STATUS_ACTIVE_USER = "status_active_user";
	public static final String STATUS_UPDATED = "status_updated";	
	
	public static final String SAVE_SETTINGS = "saveSettings";
	public static final String CLOSE_SETTINGS = "closeSettings";
	public static final String SHOW_SETTINGS = "ShowSettings";
	public static final String SHOW_STATUS = "ShowStatus";
	
	private XUIContainer xui = null;
	private ProxyClient proxy = null;
	public EventsHandler(XUIContainer xui, ProxyClient proxy){
		this.xui = xui;
		this.proxy = proxy;
	}
	
	/**
	 * 菜单-选择代理状态显示，
	 * @param event
	 */
	@EventAction(order=1)
	public void ProxyStatus(final BroadCastEvent event){
		JPanel actionPanel = (JPanel)xui.getByName("mainLayout");
		CardLayout layout = (CardLayout)actionPanel.getLayout();
		layout.show(actionPanel, "status");
	}

	/**
	 * 菜单-选择代理设置
	 * @param event
	 */
	@EventAction(order=1)
	public void Connection(final BroadCastEvent event){
		JPanel actionPanel = (JPanel)xui.getByName("mainLayout");
		CardLayout layout = (CardLayout)actionPanel.getLayout();
		layout.show(actionPanel, "login");		
	}
	
	/**
	 * 切换到状态显示面板
	 * @param event
	 */
	@EventAction(order=1)
	public void ShowStatus(final BroadCastEvent event){
		JTextField name = (JTextField)xui.getByName(STATUS_REMOTE);
		name.setText("http://proyx-nsn.deonwu84.com:8080");
		
		/*
		NoteBookSettings settings = new NoteBookSettings(this, controller);		
		settings.setLocationRelativeTo(this);
		settings.setVisible(true);
		*/
	}
	
	@EventAction(order=1)
	public void About(final BroadCastEvent event){
		JDialog about = (JDialog)xui.getByName("about");
		about.setLocationRelativeTo(about.getParent());
		about.setVisible(true);		
	}		
}
