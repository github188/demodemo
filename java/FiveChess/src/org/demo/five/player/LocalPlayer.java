package org.demo.five.player;

import org.demo.five.FiveChess;
import org.demo.five.gui.ChessMan;

public class LocalPlayer extends AbstractPlayer{
	//private 
	
	public LocalPlayer(String name, ChessMan c) {
		super(name, c);
	}
	
	
	public void setResult(int r){
		super.setResult(r);
		if (r == FiveChess.WIN){
			System.out.println("I WIN!!!");
		}
	}	

}
