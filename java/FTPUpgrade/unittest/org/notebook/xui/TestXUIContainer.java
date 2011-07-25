package org.notebook.xui;

import java.net.URL;

import javax.swing.JFrame;

import org.junit.Test;

public class TestXUIContainer {
	
	@Test
	public void testSimpleJFrameLoad(){
		URL u = this.getClass().getClassLoader().getResource("org/notebook/xui/simple_jframe.xml");
		System.out.println(u.toString());
		XUIContainer xui = new XUIContainer();
		xui.load(u);
		
		JFrame f = (JFrame)xui.getByName("main");
		//f.setSize(width, height)
		if(f != null) f.setVisible(true);
	}
	
	public static void main(String[] args){
		new  TestXUIContainer().testSimpleJFrameLoad();
	}
}
