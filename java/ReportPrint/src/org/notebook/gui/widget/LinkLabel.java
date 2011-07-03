/*
 * ikakeibo 0.6.0
 * Copyright (C)2009 wateray and contributors
 * wateray@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

package org.notebook.gui.widget;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Vector;

import javax.swing.JLabel;
import javax.swing.UIManager;

/**
 * @author wateray
 * @create 2009-5-1
 */
public class LinkLabel extends JLabel {

	/** serialVersionUID */
	private static final long serialVersionUID = 5536368501963764796L;

	private Color defaultColor;
	private Color selectedColor;
	private Color indicatedColor;

	private boolean isDrawLine = true;

	/** Whether show the hand cursor or not. */
	private boolean handCursor = true;

	private Cursor defaultCursor = this.getCursor();

	private boolean hovered;
	private boolean pressed;

	public LinkLabel() {
		super();
		defaultColor = this.getForeground();
		this.addMouseListener(new MouseAdapter() {

			public void mouseEntered(MouseEvent event) {
				HandleMouseEntered(event);
			}

			public void mouseExited(MouseEvent event) {
				HandleMouseExited(event);
			}

			public void mouseReleased(MouseEvent event) {
				HandleMouseReleased(event);
			}

			public void mouseClicked(MouseEvent event) {
				HandleMouseClicked(event);
			}
		});
	}

	public Color getDefaultColor() {

		return defaultColor;
	}

	public Color getSelectedColor() {
		return selectedColor;
	}

	public Color getIndicatedColor() {
		return indicatedColor;
	}

	public void setDefaultColor(Color c) {
		defaultColor = c;
	}

	public void setDrawLine(boolean b) {
		isDrawLine = b;
	}

	public void setIndicatedColor(Color c) {
		indicatedColor = c;
	}

	public void setSelectedColor(Color c) {
		selectedColor = c;
	}

	private Vector<ActionListener> listeners = new Vector<ActionListener>();

	public void addActionListener(ActionListener listener) {
		if (!listeners.contains(listener))
			listeners.add(listener);
	}

	public void removeActionListener(ActionListener listener) {
		if (listeners.contains(listener))
			listeners.remove(listener);
	}

	protected void fireActionPerformed(ActionEvent e) {
		for (ActionListener listener : listeners)
			listener.actionPerformed(e);
	}

	public void HandleMouseClicked(MouseEvent event) {
		/** The actionPerformed. */
		this.fireActionPerformed(new ActionEvent(this, this.hashCode(), this
				.getText()));
		
		//DrawLine color
		pressed = true;
	}

	public void HandleMouseEntered(MouseEvent event) {
		setHovered(true);
		if (handCursor) {
			this.setCursor(new Cursor(Cursor.HAND_CURSOR));
		}
		Color c = GuiUtils.getLookAndFeelColor("lightColor")==null?UIManager.getColor("Menu.foreground"): GuiUtils.getLookAndFeelColor("lightColor");
		c = c==null?Color.DARK_GRAY:c;
		this.setForeground(c);
		if (isDrawLine) {
		 this.repaint();
		}
	
	}

	public void HandleMouseExited(MouseEvent event) {
		setHovered(false);
		this.setForeground(defaultColor);
		if (isDrawLine) {
			this.repaint();
		}
	}

	public void HandleMouseReleased(MouseEvent event) {

	}

	public void paint(Graphics g) {
		super.paint(g);
		int w = this.getWidth();
		int h = this.getHeight();
		int wd = 0;
		
		FontMetrics fm = g.getFontMetrics();
		if (this.getText() != null)
			wd += fm.stringWidth(this.getText());
		int x = (int)(w  - wd);
		
		
		if (isHovered()) {
			int y = (h - fm.getHeight()) / 2 + fm.getAscent()+2;
			g.setColor(defaultColor);
			g.drawLine(x, y, x + w, y);
		}
	}

	public boolean isHovered() {
		return hovered;
	}

	public void setHovered(boolean hovered) {
		this.hovered = hovered;
	}

}
