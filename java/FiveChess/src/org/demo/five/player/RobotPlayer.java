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

import org.demo.five.Player;
import org.demo.five.ai.GameTree;
import org.demo.five.gui.ChessMan;

public class RobotPlayer extends AbstractPlayer{
	
	protected Player competitor = null;
	protected GameTree brain = null;
	protected boolean isFirst = false; //是否先手走棋。
	protected int level = 0;

	public RobotPlayer(String name, ChessMan c) {
		super(name, c);
	}

	public void handover(boolean isHold) {
		super.handover(isHold);
		if(isHold){
			GameTree.Position p = this.brain.search(this.level, this.isFirst);
			this.play(p.x, p.y);
		}
	}
	
	@Override
	public void start(Player p, boolean isFirst) {
		
		this.competitor = p;
		this.isFirst = isFirst;
		int robot, human;
		if(isFirst) {
			robot = this.getChessMan().getValue();
			human = this.competitor.getChessMan().getValue();
		}else {
			robot = this.competitor.getChessMan().getValue();
			human = this.getChessMan().getValue();
		}
		
		this.brain = new GameTree(this.chess.data, robot, human);
		
		/*
		if(this.isFirst){
			GameTree.Position position = this.brain.search(this.level, this.isFirst);
			//激活用户。
			super.handover(true);
			this.play(position.x, position.y);
		} */
	}
	
	public void setLevel(int l) {
		this.level = l;
	}
	
}
