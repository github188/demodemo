/* 
 * Copyright [2008] DeonWu@gmail.com
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at 
 *	http://www.apache.org/licenses/LICENSE-2.0 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 * 
 * $ Name LastChangeRevision LastChangeDate LastChangeBy $
 * $Id$
 */

package org.demo.five;

import org.demo.five.gui.ChessMan;

public interface Player {

	/**
	 * 开局
	 * @param p 对手。
	 * @param isFirst Ture表示先手开局， False表示后手开局。
	 */
	public void start(Player p, boolean isFirst);
	
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
	
	public String name();
}
