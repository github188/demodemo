package com.sigma.demo.web;

import javax.servlet.*;
import org.apache.log4j.*;

import com.sigma.demo.web.command.*;
import com.sigma.web.*;

public class SiteLoader implements ServletContextListener {
	/*
	public final static String SERVICE_CONTEXT = "_service_context_";
	public final static String DEPLOYER = "deployer_service";
	*/
	
	public final static String SITE_CONTEXT = "site_context";
	public final static String DEMO_FACADE = "demo_facade";
	
	protected static final Logger log = LogManager.getLogger(SiteLoader.class);
	private ServletContext ctx = null;
	
	//private Config config = null;

	static{
		DispatcherServlet.addUrlHanlder(UnknowCommand.class,null);
		DispatcherServlet.addUrlHanlder(DemoListCommand.class,"/demo-list\\.");
		DispatcherServlet.addUrlHanlder(DemoCommand.class,"/demo\\.");
		DispatcherServlet.addUrlHanlder(AuthenticateCommand.class,"/auth\\.");
		DispatcherServlet.addUrlHanlder(AppletClassLoader.class,"/cl/");
		///applet/*
		//AuthenticateCommand
		// /login.cgi
	}
	
	public void contextInitialized(ServletContextEvent evt) {
		log.debug("Site loading........");
		ctx = evt.getServletContext();
		//Config
		ctx.setAttribute(SITE_CONTEXT, this);
	}
	
	public void contextDestroyed(ServletContextEvent evt) {
		log.debug("Site destroyed........");
	}

	



}
