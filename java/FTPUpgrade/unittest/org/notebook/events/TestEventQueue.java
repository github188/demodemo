package org.notebook.events;

import static org.junit.Assert.assertEquals;

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;

import org.junit.Test;

public class TestEventQueue {
	
	@Test
	public void testRegisterAction(){
		Action baseAction = new AbstractAction(){
			@Override
			public void actionPerformed(ActionEvent arg0) {
			}
		};
		
		EventQueue queue = new EventQueue(baseAction);
		
		int order = queue.registerAction("test", -1, baseAction);		
		assertEquals("Firstly action order should be 0", 0, order);

		order = queue.registerAction("test", -1, baseAction);		
		assertEquals("2ed action order should be 1", 1, order);

		order = queue.registerAction("te*", 10, baseAction);		
		assertEquals("The order is defined by parameter.", 10, order);
		
		int count = queue.fireEvent("test", new Object());
		assertEquals("Should have 4 action processed the event.", 4, count);

		count = queue.fireEvent("atest", new Object());
		assertEquals("only the default action processed the event.", 1, count);

		count = queue.fireEvent("te1", new Object());
		assertEquals("only the default action processed the event.", 2, count);		
	}

}
