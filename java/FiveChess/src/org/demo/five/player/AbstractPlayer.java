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

package org.demo.five.player;

import org.demo.five.FiveChess;
import org.demo.five.Player;
import org.demo.five.exception.NotJoinChess;
import org.demo.five.gui.ChessMan;

public class AbstractPlayer implements Player {
	protected FiveChess chess = null;
	private boolean isStart = false;
	private boolean isActive = false;
	private int goal = 0;
	private ChessMan cm = null;
	private String name = "";
	
	public AbstractPlayer(String name){
		this.name = name;
		
	}
	public AbstractPlayer(String name, ChessMan c){
		this(name);
		this.setChessMan(c);
	}

	
	
	public void handover(boolean isHold) {
		System.out.println(String.format("%s,%s", this.name(), isHold));
		this.isActive = isHold;
	}

	
	public boolean isActive() {
		return this.isStart && this.isActive;
	}

	public void play(int x, int y) {
		if (chess != null){
			chess.play(this, x, y);
		}else {
			throw new NotJoinChess("not join chess");
		}
	}

	
	public void setFiveChess(FiveChess c) {
		chess = c;
	}
	
	public void setResult(int r){
		this.goal = r;
		this.isStart = false;
	}

	
	public void setChessMan(ChessMan c) {
		this.cm = c;
	}

	
	public ChessMan getChessMan() {
		return this.cm;
	}
	
	public void start(Player p, boolean isFirst) {
		this.isStart = true;
	}
	
	public String name(){
		return this.name;
	}

}
