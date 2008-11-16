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

package org.demo.five.gui;

import java.awt.Color;
import java.awt.Graphics;


/**
 * @author deon
 */
public class ChessMan {
	public static final ChessMan WHITE = new ChessMan(1);
	public static final ChessMan BLACK = new ChessMan(2);
	
	private int value = 0;
	//private static final long serialVersionUID = 1L;
	private ChessMan(int value){
		this.value = value;
	}
	
	public void paint(int x, int y, int size, Graphics g) {
		if (this == WHITE){
			g.setColor(Color.white);
		}else {
			g.setColor(Color.yellow);
		}
		
		g.fillOval(x - size/2, y - size/2, size, size);
	}
	
	public int getValue(){
		return this.value;
	}
	
	public static ChessMan load(int i){
		if(i == WHITE.getValue()){
			return WHITE;
		}else if(i == BLACK.getValue()){
			return BLACK;
		}else {
			return new ChessMan(i);
		}
			
	}
}
