package com.sigma.demo.web.command;

import nl.justobjects.pushlet.core.Dispatcher;
import nl.justobjects.pushlet.core.Event;

import com.sigma.demo.DemoEvent;
import com.sigma.demo.DemoListener;
import com.sigma.demo.event.*;

public class PushletDemoListener implements DemoListener {
	private String sessionId = null;
	private Dispatcher dispatcher = null;
	
	/**
	 * @param sessionId It's pushlet client session, not a web session.
	 */
	public PushletDemoListener(String sessionId){
		this.sessionId = sessionId;
		
		dispatcher = Dispatcher.getInstance();
	}

	public void dmCall(DMCallEvent event) {
		Event e = new Event("exec");
		
		e.setField("cmd", event.getCommand());
		e.setField("args", event.getArgs());
		dispatcher.unicast(e, sessionId);		
	}

	public void info(DemoMsgEvent event) {
		Event e = new Event("info");
		
		e.setField("msg", event.getMessage());
		e.setField("type", event.getType());
		dispatcher.unicast(e, sessionId);		
	}
	
	public void updateGraph(GraphEvent event) {
		Event e = new Event("graph");
		
		e.setField("series", event.getSerisName());
		e.setField("time", event.getXData());
		e.setField("data", event.getYData() + "");
		
		dispatcher.unicast(e, sessionId);
	}	

	public void updateConsole(ConsoleEvent event) {
		Event e = new Event("console");
		
		dispatcher.unicast(e, sessionId);		
	}	
}
