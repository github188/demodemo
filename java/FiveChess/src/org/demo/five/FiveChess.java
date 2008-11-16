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

import org.demo.five.model.DataModel;
import org.demo.five.model.ModelListener;

public class FiveChess {
	public static final int WIN = 1;
	public static final int FAIL = -1;
	public static final int DRAW = 0;
	
	public static final int WAITING = 0;
	public static final int READY = 1;
	public static final int PLAYING = 2;
	public static final int END = 3;
	
	public DataModel data = null;
	public Setting setting = null;
	private Player playerOne = null;
	private Player playerTwo = null;
	public int status = WAITING;
	
	public FiveChess(){
		data = new DataModel();
	}
	
	public void play(Player p, int x, int y) {
		if (this.status != PLAYING) return;
		
		if((p == this.playerOne || p == this.playerTwo) && p.isActive()){
			if(this.data.isEmpty(x, y)){
				this.data.set(x, y, p.getChessMan().getValue());
				if(this.checkResult(x, y)){
					this.end(p, WIN);
				}else {
					this.handover(p, false);
				}
			}
		}
	}

	public void undo(int step){
		
	}

	public void redo(int step){
		
	}
	
	public void addPlayer(Player p){
		if(playerOne == null){
			playerOne = p;
		}else {
			playerTwo = p;
		}
		p.setFiveChess(this);
		if(this.playerOne != null && this.playerTwo != null){
			this.status = READY;
		}
	}
	
	public void start(Player p){
		this.handover(p, true);
		this.status = PLAYING;
	}
	
	protected void handover(Player p, boolean hold){
		p.handover(hold);
		if(this.playerOne == p){
			this.playerTwo.handover(!hold);
		}else if(this.playerTwo == p) {
			this.playerOne.handover(!hold);
		}
	}
	
	public void addModelListener(ModelListener l) {
		this.data.addListener(l);
	}
	
	public void end(Player p, int result){
		this.status = END;
		
		p.setResult(result);
		if(result == WIN){
			result = FAIL;
		}else if(result == FAIL){
			result = WIN;
		}
		
		if(p == this.playerOne){
			this.playerTwo.setResult(result);
		}else {
			this.playerOne.setResult(result);
		}
	}
	
	protected boolean checkResult(int x, int y){
		if(this.data.isEmpty(x, y)) return false;
		byte[][] data = this.data.data;
		byte[][] dir = {{1, 0}, {1, -1}, {0, -1}, {-1, -1},
						{-1, 0}, {-1, 1}, {0, 1}, {1, 1}};
		int[] result = {0, 0, 0, 0};
		
		byte idata = data[x][y];
		int ix, iy;
		for(int i = 0; i < dir.length; i++){
			for(int j = 1; j < 5; j++){
				ix = dir[i][0] * j + x;
				iy = dir[i][1] * j + y;
				if(ix >= data.length || ix < 0 || 
				   iy >= data[0].length || iy < 0 ||
				   data[ix][iy] != idata
				   ) break;
				result[i % result.length]++;
				if(result[i % result.length] >= 4)return true;
			}
		}
		
		return false;
	}
	
}
