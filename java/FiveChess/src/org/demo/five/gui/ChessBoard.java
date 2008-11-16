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

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;

/**
 * @author deon
 *
 */
public class ChessBoard extends Canvas{
	private int leftPadding = 20;
	private int topPadding = 20;	
	
	private int minPadding = 15;
	
	private int gridCount = 18;
	
	public ChessBoard(){
		this.setBackground(Color.white);
		
	}
	
	public void paint(Graphics g){
		this.paintGridding(g);		
	}
	
	private void paintGridding(Graphics g){
		int width = this.getWidth();
		int height = this.getHeight();
		
		int cellWidth = (width > height) ? height : width;
		cellWidth = (cellWidth - minPadding) / gridCount;
		//new Font()
		//g.setFont()
		g.setColor(Color.RED);
		int endY = topPadding + 18 * cellWidth;
		int endX = leftPadding + 18 * cellWidth;
		for(int i = 0; i <= 18; i++){
			int startX = leftPadding + i * cellWidth;
			int startY = topPadding + i * cellWidth;

			g.drawString(i + "", startX - 4, topPadding - 8);
			g.drawLine(leftPadding, startY, endX, startY);
			
			g.drawString(i + "", leftPadding - 16, startY + 4);
			g.drawLine(startX, topPadding, startX, endY);
			
		}
	}
}
