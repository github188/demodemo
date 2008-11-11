package org.demo.five.gui;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;

/**
 * ÆåÅÌ
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
			//»­ºáÏß
			g.drawString(i + "", startX - 4, topPadding - 8);
			g.drawLine(leftPadding, startY, endX, startY);
			
			g.drawString(i + "", leftPadding - 16, startY + 4);
			g.drawLine(startX, topPadding, startX, endY);
			
		}
	}
}
