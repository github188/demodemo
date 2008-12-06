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
import java.awt.Image;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.util.logging.Logger;

import javax.swing.JOptionPane;

import org.demo.five.Player;
import org.demo.five.model.ModelEvent;
import org.demo.five.model.ModelListener;

/**
 * @author deon
 *
 */
public class ChessBoard extends Canvas implements ModelListener{
	private static Logger log = Logger.getLogger("ChessBoard");

	private int leftPadding = 20;
	private int topPadding = 20;	
	
	private int minPadding = 15;
	
	private int gridCount = 18;
	private Player actor = null;
	
	private int curCellWidth = 0;
	private Image frameBuffer;
 
	
	public ChessBoard(){
		this(18);
	}
	
	public ChessBoard(int size){
		this.gridCount = size;
		this.setBackground(Color.white);
		this.initMouseAdapter();
	}	
	
	
	public void paint(Graphics g) {
		if (frameBuffer == null){
			this.initBuffer();
		}
		
		g.drawImage(frameBuffer, 0, 0, null);
	}
	
	private void initBuffer(){
        frameBuffer = new BufferedImage(this.getWidth(),
                		  this.getHeight(),
                          BufferedImage.TYPE_3BYTE_BGR);
        
        frameBuffer.getGraphics().setColor(Color.gray);
        frameBuffer.getGraphics().fillRect(0, 0, this.getX(), this.getY());
        this.paintGridding(frameBuffer.getGraphics());
        //frameBuffer.getGraphics()
	}
	
	private void paintGridding(Graphics g){
		int width = this.getWidth();
		int height = this.getHeight();
		
		curCellWidth = (width > height) ? height : width;
		curCellWidth = (curCellWidth - minPadding * 2) / gridCount;
		//new Font()
		//g.setFont()
		g.setColor(Color.RED);
		int endY = topPadding + gridCount * curCellWidth;
		int endX = leftPadding + gridCount * curCellWidth;
		for(int i = 0; i <= gridCount; i++){
			int startX = leftPadding + i * curCellWidth;
			int startY = topPadding + i * curCellWidth;

			g.drawString(i + "", startX - 4, topPadding - 8);
			g.drawLine(leftPadding, startY, endX, startY);
			
			g.drawString(i + "", leftPadding - 16, startY + 4);
			g.drawLine(startX, topPadding, startX, endY);
			
		}
	}
	
	public void setPlayer(Player p){
		this.actor = p;
	}
	
	protected void mouseClickedHandler(MouseEvent e) {
		int gridX = Math.round((e.getX() - leftPadding) / (float) this.curCellWidth);
		int gridY = Math.round((e.getY() - topPadding) / (float) this.curCellWidth);
		log.info("Clicked: X:" + gridX + ", Y:" + gridY);
		if (gridX >= 0 && gridX <= gridCount &&
			gridY >= 0 && gridY <= gridCount){
			if(this.actor != null && this.actor.isActive()){
				this.actor.play(gridX, gridY);
			}else {
				this.showMessage("还没有开始!");
			}
		}
	}
	
	private void initMouseAdapter() {
		final ChessBoard board = this;
		this.addMouseListener(new MouseAdapter() {
			public void mouseClicked(MouseEvent e) {
				board.mouseClickedHandler(e);
			}
		});
	}


	public void update(ModelEvent e) {
		log.info("ModelEvent: A:" + e.getAction() + ",X:" + e.getX() + ",Y:" + e.getY() + ",D:" + e.getData());
		ChessMan c = null;
		if(this.frameBuffer == null){
			log.info("not inited board!");
		}
		if(e.getAction() == ModelEvent.ADD){
			 c = ChessMan.load(e.getData());
			 int x = e.getX() * this.curCellWidth + leftPadding;
			 int y = e.getY() * this.curCellWidth + topPadding;
			 c.paint(x, y, this.curCellWidth - (int)(this.curCellWidth * 0.2), this.frameBuffer.getGraphics());
			 this.repaint();
		}
	}
	
	protected void showMessage(String message){
		JOptionPane.showMessageDialog(this,
				message, 
			    "Message", JOptionPane.WARNING_MESSAGE);	
	}
	
	public void reset() {
		this.initBuffer();
		this.repaint();
	}
}
