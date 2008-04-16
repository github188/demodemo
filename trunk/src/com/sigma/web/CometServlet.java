package com.sigma.web;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSessionBindingEvent;
import javax.servlet.http.HttpSessionBindingListener;

import org.apache.catalina.CometEvent;
import org.apache.catalina.CometProcessor;
import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

import com.sigma.demo.DemoException;
import com.sigma.demo.DemoListener;
import com.sigma.demo.DemoService;
import com.sigma.demo.acl.DemoFacade;
import com.sigma.demo.event.ConsoleEvent;
import com.sigma.demo.event.DMCallEvent;
import com.sigma.demo.event.DemoMsgEvent;
import com.sigma.demo.event.GraphEvent;
import com.sigma.demo.web.SiteLoader;

public class CometServlet extends HttpServlet implements CometProcessor {
		protected static final Logger log = LogManager.getLogger(DemoFacade.class);	    
	    /**
	     * Process the given Comet event.
	     * 
	     * @param event The Comet event that will be processed
	     * @throws IOException
	     * @throws ServletException
	     */
	    public void event(CometEvent event)
	        throws IOException, ServletException {
	    	HttpServletRequest request = event.getHttpServletRequest();
	        if (event.getEventType() == CometEvent.EventType.BEGIN) {		        
		        HttpServletResponse response = event.getHttpServletResponse();	    	
		        DemoService service = requestDemoService(request);		    	
		    	if(service != null) {
		    		log("Create new comet listener: " + event.toString());		    		
		    		ComentListener l = new ComentListener(response.getWriter(), service);
		    		try {
						service.addListener(l);
					} catch (DemoException e) {
						log.error(e,e);
					}
		    		request.getSession(true).setAttribute("_comet_", l);
		    	}else {		    		
		    		log("not found service! ");
		    		event.close();
		    	}
	        } else if (event.getEventType() == CometEvent.EventType.ERROR) {
	        	request.getSession(true).removeAttribute("_comet_");
	        	event.close();
	        } else if (event.getEventType() == CometEvent.EventType.END) {
	        	request.getSession(true).removeAttribute("_comet_");
	        	event.close();
	        }
	    }
	    
	    private DemoService requestDemoService(HttpServletRequest request){
	    	DemoFacade facade = (DemoFacade)request.getSession(true).getAttribute(SiteLoader.DEMO_FACADE);
	    	DemoService service = null;
	    	if(facade == null) {
	    		String dn = request.getParameter("dn");
	    		service = facade.lookupDemo(dn);	    		
	    	}
	    	return service;
	    }
	    
	    class ComentListener implements DemoListener, HttpSessionBindingListener {
	    	DemoService service = null;
	    	PrintWriter writer = null;
	    	public ComentListener(PrintWriter writer, DemoService service) {
	    		this.writer = writer;
	    		this.service = service;
	    	}

			public void dmCall(DMCallEvent event) {

			}

			public void info(DemoMsgEvent event) {
				
			}

			public void updateConsole(ConsoleEvent event) {
				
			}

			public void updateGraph(GraphEvent event) {
				/*
				e.setField("series", event.getSerisName());
				e.setField("time", event.getXData());
				e.setField("data", event.getYData() + "");
				*/
				String msg = event.getSerisName() + " " + event.getXData() + " " + event.getYData();
				log.debug("Graph Data:" + msg);
				writer.print(msg);
				writer.flush();
			}

			public void valueBound(HttpSessionBindingEvent arg0) {
			}

			public void valueUnbound(HttpSessionBindingEvent arg0) {
				log.info("Remove graph listener form service");
				try {
					this.service.removeListener(this);
				} catch (DemoException e) {
					log.error(e,e);
				}				
			}
	    	
	    }
	}