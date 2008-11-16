package org.demo.five.player;

import org.demo.five.gui.ChessMan;

public class RobotPlayer extends AbstractPlayer{

	public RobotPlayer(String name, ChessMan c) {
		super(name, c);
	}

	public void handover(boolean isHold) {
		super.handover(isHold);
		if(isHold){
			int x = random();
			int y = random();
			while(! this.chess.data.isEmpty(x, y)){
				x = random();
				y = random();
			}
			
			this.play(x, y);
		}
	}
	
	private int random(){
		int d = (int) Math.round(Math.random() * 18);
		while(d < 0 || d >= 18){
			d = (int) Math.round(Math.random() * 18);
		}
		
		return d;
	}
}
