/*
 * CurtainButtonUI.java
 *
 * Created on June 11, 2007, 2:29 PM
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

package org.notebook.gui.widget;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.GradientPaint;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import javax.swing.JComponent;
import javax.swing.UIManager;
import javax.swing.plaf.ComponentUI;

/**
 * 
 * @author William Chen
 */
public class CurtainButtonUI extends ComponentUI implements MouseListener,
		MouseMotionListener {
	static final int PREFERED_HEIGHT = 22;//22
	private static final Dimension PREFERED_SIZE = new Dimension(200,
			PREFERED_HEIGHT);
	private static final int LEADING_TEXT_PADDING = 12;
	private static final int TEXT_ICON_PADDING = 6;

	// Define colors
	private static Color pressed_dark_color = new Color(254, 145, 78);
	private static Color pressed_light_color = new Color(254, 204, 135);
	private static Color hovered_dark_color = new Color(255, 211, 150);
	private static Color hovered_light_color = new Color(255, 244, 204);

	// lightColor
	private  Color light_color;
	// darkColor
	private  Color dark_color;
	// borderColor
	private  Color border_color;

	private boolean pressed;
	private boolean hovered;
	private CurtainButton button;

	/** Creates a new instance of CurtainButtonUI */
	public CurtainButtonUI() {
		light_color = GuiUtils.getLookAndFeelColor("lightColor") == null ? UIManager.getColor("Panel.background")
				: GuiUtils.getLookAndFeelColor("lightColor");
		light_color=light_color==null?Color.LIGHT_GRAY:light_color;

		dark_color = GuiUtils.getLookAndFeelColor("darkColor") == null ? UIManager.getColor("Menu.selectionBackground")
				: GuiUtils.getLookAndFeelColor("darkColor");
	    
		dark_color=dark_color==null?Color.LIGHT_GRAY:dark_color;

		border_color = GuiUtils.getLookAndFeelColor("borderColor") == null ? UIManager.getColor("Panel.background")
				: GuiUtils.getLookAndFeelColor("borderColor");
		border_color=border_color==null?Color.DARK_GRAY:border_color;
	}

	public static ComponentUI createUI(JComponent c) {
		return new CurtainButtonUI();
	}

	public void installUI(JComponent c) {
		button = (CurtainButton) c;
		button.addMouseListener(this);
		button.addMouseMotionListener(this);
	}

	public void uninstallUI(JComponent c) {
		button.removeMouseListener(this);
		button.removeMouseMotionListener(this);
	}

	public void mouseClicked(MouseEvent e) {
	}

	public void mousePressed(MouseEvent e) {
		setPressed(true);
	}

	public void mouseReleased(MouseEvent e) {
		setPressed(false);
		button
				.fireActionPerformed(new ActionEvent(button, 0, button
						.getText()));
	}

	public void mouseEntered(MouseEvent e) {
		setHovered(true);
	}

	public void mouseExited(MouseEvent e) {
		setHovered(false);
	}

	public void mouseDragged(MouseEvent e) {
	}

	public void mouseMoved(MouseEvent e) {
	}

	public boolean isPressed() {
		return pressed;
	}

	public void setPressed(boolean pressed) {
		this.pressed = pressed;
		button.repaint();
	}

	public boolean isHovered() {
		return hovered;
	}

	public void setHovered(boolean hovered) {
		this.hovered = hovered;
		button.repaint();
	}

	public Dimension getPreferredSize(JComponent c) {
		return PREFERED_SIZE;
	}

	public void paint(Graphics g, JComponent c) {
		paintComponentBackground(g);
		int w = button.getWidth();
		int h = button.getHeight();
		int wd = 0;
		if (button.getIcon() != null)
			wd += button.getIcon().getIconWidth();
		FontMetrics fm = g.getFontMetrics();
		if (button.getText() != null)
			wd += fm.stringWidth(button.getText()) + TEXT_ICON_PADDING;
		int x = (int) (LEADING_TEXT_PADDING + (w - 2 * LEADING_TEXT_PADDING - wd)
				* button.getAlignment());
		if (button.getIcon() != null) {
			int y = (h - button.getIcon().getIconHeight()) / 2;
			button.getIcon().paintIcon(button, g, x, y);
			x += button.getIcon().getIconWidth() + TEXT_ICON_PADDING;
		}
		if (button.getText() != null) {
			int y = (h - fm.getHeight()) / 2 + fm.getAscent();
			g.setColor(button.getForeground());
			g.drawString(button.getText(), x, y);
		}
	}

	protected void paintComponentBackground(Graphics g) {
		int w = button.getWidth();
		int h = button.getHeight();
		Color top_color = hovered ? (pressed ? pressed_dark_color
				: hovered_light_color) : light_color;
		Color bottom_color = hovered ? (pressed ? pressed_light_color
				: hovered_dark_color) : dark_color;
		GradientPaint gp = new GradientPaint(1, 1, top_color, 1, h - 2,
				bottom_color);
		Graphics2D g2d = (Graphics2D) g;
		g2d.setPaint(gp);
		g2d.fillRect(1, 1, w - 2, h - 2);
//		g.setColor(Color.white);
		g.setColor(border_color);
		g.drawLine(0, 0, w - 1, 0);
		g.drawLine(0, 0, 0, h - 1);
		g.setColor(border_color);
		g.drawLine(0, h - 1, w - 1, h - 1);
		g.drawLine(w - 1, 0, w - 1, h - 1);
	}
}
