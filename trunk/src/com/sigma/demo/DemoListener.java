package com.sigma.demo;

import com.sigma.demo.event.*;

public interface DemoListener {
	
	public void info(DemoMsgEvent event);
	
	public void dmCall(DMCallEvent event);
	
	public void updateGraph(GraphEvent event);
	
	public void updateConsole(ConsoleEvent event);
}
