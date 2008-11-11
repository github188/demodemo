package org.demo.five.gui;

import java.awt.BorderLayout;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JFrame;

public class MainPlane extends JFrame {
	private ChessBoard board = null;
	
	public MainPlane(){
		  super("Java五子棋客户端");
		  setLayout(new BorderLayout());
		  board = new ChessBoard();
		  this.getContentPane().add("Center", board);
		  //add(boardPanel,BorderLayout.CENTER);
		  this.init();
		  pack();
		  setSize(670,548);
		  //setVisible(true);
		  setResizable(false);	  
	}
	
	public void init() {
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				System.exit(0);
			}
		});
	}
	
	public static void main(String[] args){
		MainPlane main = new MainPlane();
		main.setVisible(true);
	}
}
