package org.demo.five;

import org.demo.five.gui.ChessMan;

public interface Player {

	/**
	 * 轮换走棋
	 * @param isHold Ture表示轮到该玩家走棋， False该玩家等待对方走棋。
	 */
	public void handover(boolean isHold);

	/**
	 * 玩家走一步棋子。
	 */
	public void play(int x, int y);

	/**
	 * 是否轮到改玩家走棋。
	 */
	public boolean isActive();
	
	/**
	 * 设置玩家加入的棋局。
	 */
	public void setFiveChess(FiveChess c);
	
	/**
	 * 设置玩家的输赢结果。
	 * @param r
	 */
	public void setResult(int r); 
	
	/**
	 * 设置玩家使用的棋子。
	 * @param c
	 */
	public void setChessMan(ChessMan c);
	
	public ChessMan getChessMan();
}
