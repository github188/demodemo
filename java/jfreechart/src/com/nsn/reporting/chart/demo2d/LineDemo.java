package com.nsn.reporting.chart.demo2d;

import java.awt.BasicStroke;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Font;
import java.awt.GradientPaint;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Shape;
import java.awt.font.FontRenderContext;
import java.awt.font.TextLayout;
import java.awt.geom.AffineTransform;
import java.awt.geom.CubicCurve2D;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;

import javax.swing.JFrame;

public class LineDemo extends JFrame{

	static class DemoCanvas extends Canvas{
		
		public void paint(Graphics g){
			 Ellipse2D.Float shape;
			 GradientPaint gp;
			 // create a Graphics2D
			 
			 Graphics2D g2 = (Graphics2D) g ;
			 
			 // create a Ellipse2D (x,y,w,h)
			 
			 shape = new Ellipse2D.Float (200,200,200,200);
			 
			 // create a Cyclic GradientPaint 
			 
			 // (x1,y1,color1,x2,y2,color2, boolean cyclic)
			 
			 gp = new GradientPaint(170, 170,Color.yellow, 200,200,Color.red,true);
			 
			 g2.setPaint(gp);
			 
			 g2.fill(shape);
			 
			 // create a Line2D
			 Line2D.Float line;
			 
			 CubicCurve2D.Float cubic;			 
			 
			 line = new Line2D.Float(20,390,200,390);
			 
			 // create a CubicCurve2D,比直线多了两个控制点
			 
			 cubic = new CubicCurve2D.Float(70,100,120,50,170,270,220,100);
			 
			 // 设定描绘的粗细
			 
			 g2. setStroke(new BasicStroke(5.0f));
			 
			 //g2.setColor(Color.blue);
			 
			 g2.draw(line);
			 
			 line = new Line2D.Float(30,400,250,400);
			 
			 // 设定描绘的粗细
			 
			 g2. setStroke(new BasicStroke(5.0f));
			 
			 g2.draw(line);			 
			 
			 Shape sha;
			 
			 FontRenderContext frc =g2.getFontRenderContext();
			 
			 TextLayout tl = new TextLayout("Font Test",new Font("Modern",
			 Font.BOLD+Font.ITALIC,20),frc);
			 
			 sha=tl.getOutline(AffineTransform.getTranslateInstance(50,380));
			 
			 g2.setColor(Color.blue);
			 
			 g2.setStroke(new BasicStroke(2.0f));
			 
			 g2.draw(sha);
			 g2.setColor(Color.white);
			 g2.setPaint(gp);
			 
			 g2.fill(sha);			 
			
		}
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		LineDemo app = new LineDemo();
		
		app.getContentPane().add(new DemoCanvas());
		
		app.setSize(300, 400);
		
		app.setVisible(true);

	}

}
