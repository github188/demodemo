/*
 * This file is part of "JTA - Telnet/SSH for the JAVA(tm) platform".
 *
 * (c) Matthias L. Jugel, Marcus Mei脽ner 1996-2005. All Rights Reserved.
 *
 * Please visit http://javatelnet.org/ for updates and contact.
 *
 * --LICENSE NOTICE--
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * --LICENSE NOTICE--
 *
 */
package com.sigma.applet;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.ClipboardOwner;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.Transferable;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.IOException;
import java.util.Hashtable;

import javax.swing.JPanel;

import de.mud.jta.event.FocusStatus;
import de.mud.jta.event.SoundRequest;
import de.mud.terminal.SwingTerminal;
import de.mud.terminal.vt320;

/**
 * The terminal plugin represents the actual terminal where the data will be
 * displayed and the gets the keyboard input to sent back to the remote host.
 * <P>
 * <B>Maintainer:</B> Matthias L. Jugel
 * 
 * @version $Id: Terminal.java 510 2005-10-28 06:46:44Z marcus $
 * @author Matthias L. Jugel, Marcus Mei锟絥er
 */
public class Terminal implements ClipboardOwner
{

    private final static boolean personalJava = false;

    private final static int debug = 0;

    /** holds the actual terminal emulation */
    protected SwingTerminal terminal;
    protected vt320 emulation;
    /** the terminal panel that is displayed on-screen */
    protected JPanel tPanel;

    /**
     * The default encoding is ISO 8859-1 (western). However, as you see the
     * value is set to latin1 which is a value that is not even documented and
     * thus incorrect, but it forces the default behaviour for western
     * encodings. The correct value does not work in most available browsers.
     */
    protected String encoding = "latin1"; // "ISO8859_1";
    /** if we have a url to an audioclip use it as ping */
    protected SoundRequest audioBeep = null;

    private Thread reader = null;

    private Hashtable colors = new Hashtable();

    private boolean localecho_overridden = false;

    /** Access to the system clipboard */
    private Clipboard clipboard = null;

    private Color codeToColor(String code)
    {
	if (colors.get(code) != null)
	    return (Color) colors.get(code);
	else
	    try
	    {
		if (Color.getColor(code) != null)
		    return Color.getColor(code);
		else
		    return Color.decode(code);
	    } catch (Exception e)
	    {
		try
		{
		    // try one last time
		    return Color.decode(code);
		} catch (Exception ex)
		{
		    // ignore
		}
		error("ignoring unknown color code: " + code);
	    }
	return null;
    }

    /**
     * Create a new terminal plugin and initialize the terminal emulation.
     */
    public Terminal()
    {
	// create the terminal emulation
	emulation = new vt320()
	{
	    public void write(byte[] b)
	    {
		try
		{
		    Terminal.this.write(b);
		} catch (IOException e)
		{
		    reader = null;
		}
	    }
	};
	emulation.setCursorPosition(0,0);
	// create terminal
	terminal = new SwingTerminal(emulation);
	terminal.setBorder(1, false);
	// initialize colors
	colors.put("black", Color.black);
	colors.put("red", Color.red);
	colors.put("green", Color.green);
	colors.put("yellow", Color.yellow);
	colors.put("blue", Color.blue);
	colors.put("magenta", Color.magenta);
	colors.put("orange", Color.orange);
	colors.put("pink", Color.pink);
	colors.put("cyan", Color.cyan);
	colors.put("white", Color.white);
	colors.put("gray", Color.gray);
	colors.put("darkgray", Color.darkGray);

	
	  // the container for our terminal must use double-buffering
	    // or at least reduce flicker by overloading update()
	    tPanel = new JPanel(new BorderLayout()) {
	      // reduce flickering
	      public void update(java.awt.Graphics g) {
	        paint(g);
	      }

	      // we don't want to print the container, just the terminal contents
	      public void print(java.awt.Graphics g) {
	        terminal.print(g);
	      }
	    };
	    tPanel.add("Center", terminal);
	    tPanel.setBackground(terminal.getBackground());

	    terminal.addFocusListener(new FocusListener() {
	      public void focusGained(FocusEvent evt) {
	        if (debug > 0)
	          System.err.println("Terminal: focus gained");
	        terminal.setCursor(Cursor.getPredefinedCursor(Cursor.TEXT_CURSOR));
	      }

	      public void focusLost(FocusEvent evt) {
	        if (debug > 0)
	          System.err.println("Terminal: focus lost");
	        terminal.setCursor(Cursor.getDefaultCursor());
	      }
	    });

	    // get a reference to the system clipboard.  
	    try {
	      clipboard = tPanel.getToolkit().getSystemClipboard();
	      System.out.println("Got the clipboard reference ok - copy & paste enabled");
	    } catch(Exception ex) {
	      System.out.println("Failed to get clipboard - copy and paste will not work");
	      /* ex.printStackTrace(); */
	    }


		// *******************************
	// code to handle copy and paste
	// from embeded terminal
	// *******************************
	terminal.addMouseListener(new MouseListener()
	{
	    public void mouseClicked(MouseEvent me)
	    {
		// make sure it only does the paste on button2(right mouse)
		if (me.getButton() == me.BUTTON3 && clipboard != null)
		{
		    paste(clipboard);
		}
	    }

	    public void mouseExited(MouseEvent arg0)
	    {
	    }

	    public void mousePressed(MouseEvent arg0)
	    {
		// System.out.println(">>>>MOUSE pressed");
	    }

	    public void mouseReleased(MouseEvent me)
	    {
		// make sure it only does the copy on button 1 (left mouse)
		// System.out.println(">>>>MOUSE RELEASED");
		if (me.getButton() == me.BUTTON1 && clipboard != null)
		{
		    String selection = terminal.getSelection();
		    // System.out.println(">>>>SELECTION = " + selection);
		    if (selection != null && selection.trim().length() > 0)
		    {
			copy(clipboard);
		    }
		} else
		{
		    // not left mouse
		    // System.out.println("NOT BUTTON 1(left mouse): " +
		    // me.getButton());
		}
	    }

	    public void mouseEntered(MouseEvent arg0)
	    {
	    }
	});

    }

    /**
     * Continuously read from our back end and display the data on screen.
     */
    public void run()
    {
	byte[] b = new byte[256];
	int n = 0;
	while (n >= 0)
	    try
	    {
		n = read(b);
		if (debug > 1 && n > 0)
		    System.err.println("Terminal: \""
			    + (new String(b, 0, n, encoding)) + "\"");
		if (n > 0)
		    emulation.putString(new String(b, 0, n, encoding));
		terminal.repaint();
	    } catch (IOException e)
	    {
		reader = null;
		break;
	    }
    }

    public int read(byte[] b) throws IOException
    {
	// return source.read(b);
	return 0;
    }

    public void write(byte[] b) throws IOException
    {
	// source.write(b);
    }

    public void copy(Clipboard clipboard)
    {
	String data = terminal.getSelection();
	// check due to a bug in the hotspot vm
	if (data == null)
	    return;
	StringSelection selection = new StringSelection(data);
	clipboard.setContents(selection, this);
    }

    public void paste(Clipboard clipboard)
    {
	if (clipboard == null)
	    return;
	Transferable t = clipboard.getContents(this);
	try
	{
	    /*
	     * InputStream is =
	     * (InputStream)t.getTransferData(DataFlavor.plainTextFlavor);
	     * if(debug > 0) System.out.println("Clipboard: available:
	     * "+is.available()); byte buffer[] = new byte[is.available()];
	     * is.read(buffer); is.close();
	     */
	    byte buffer[] = ((String) t
		    .getTransferData(DataFlavor.stringFlavor)).getBytes();
	    try
	    {
		write(buffer);
	    } catch (IOException e)
	    {
		reader = null;
	    }
	} catch (Exception e)
	{
	    // ignore any clipboard errors
	    if (debug > 0)
		e.printStackTrace();
	}
    }

    public void lostOwnership(Clipboard clipboard, Transferable contents)
    {
	terminal.clearSelection();
    }

    /**
     * Print an error message to stderr prepending the plugin name. This method
     * is public due to compatibility with Java 1.1
     * 
     * @param msg
     *                the error message
     */
    public void error(String msg)
    {
	String name = getClass().toString();
	name = name.substring(name.lastIndexOf('.') + 1);
	System.err.println(name + ": " + msg);
    }
    
    public JPanel getPanel(){
	return this.tPanel;
    }
}
