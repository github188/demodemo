package org.demo.five.player;

import org.demo.five.FiveChess;
import org.demo.five.Player;
import org.demo.five.exception.NotJoinChess;
import org.demo.five.gui.ChessMan;

public class AbstractPlayer implements Player {
	protected FiveChess chess = null;
	private boolean isActive = false;
	private int goal = 0;
	private ChessMan cm = null;
	
	public AbstractPlayer(String name){
		
	}
	public AbstractPlayer(String name, ChessMan c){
		this(name);
		this.setChessMan(c);
	}

	
	@Override
	public void handover(boolean isHold) {
		this.isActive = isHold;
	}

	@Override
	public boolean isActive() {
		return this.isActive;
	}

	public void play(int x, int y) {
		if (chess != null){
			chess.play(this, x, y);
		}else {
			throw new NotJoinChess("not join chess");
		}
	}

	@Override
	public void setFiveChess(FiveChess c) {
		chess = c;
	}
	
	public void setResult(int r){
		this.goal = r;
	}

	@Override
	public void setChessMan(ChessMan c) {
		this.cm = c;
	}

	@Override
	public ChessMan getChessMan() {
		return this.cm;
	}

}
