/* TerminalApplet.java
 *
 * Sigma Resources & Technologies, Inc.
 * Copyright (C) 2005-2008, sigma-rt.com
 *
 * PROPRIETARY RIGHTS of Sigma Resources & Technologies are involved in
 * the subject matter of this material. All manufacturing, reproduction,
 * use, and sales rights pertaining to this subject matter are governed
 * by the license agreement. The recipient of this software implicitly
 * accepts the terms of the license.
 *
 * Created on 2008-3-18, by Alexandre Tsu.
 * 
 * Project:	DemoServer
 * Author :	James Che
 * Auditor: 
 * 
 * # any description here # 
 *
 * For more information, visit:
 * http://www.sigma-rt.com
 * 
 * Change History
 * ---------------------------------------------------
 * REVISION          DATE/TIME        AUTHOR
 * ---------------------------------------------------
 * $Log$
 * ---------------------------------------------------
 *
 */
package com.sigma.applet;

import java.awt.FlowLayout;

import javax.swing.JApplet;

/**
 * @author James Che
 * 
 */
public class TerminalApplet extends JApplet
{
    private static final long serialVersionUID = -7832309771013966499L;
    private Terminal terminal = null;

    public void init()
    {
	this.terminal = new Terminal();
	FlowLayout layout = new FlowLayout();
	layout.setAlignment(FlowLayout.LEFT);
	this.getContentPane().setLayout(layout);
	this.getContentPane().add(terminal.getPanel());
	this.getContentPane().setBackground(terminal.getPanel().getBackground());
    }

    public void start()
    {
    }

    public void stop()
    {
    }

    public void destroy()
    {
	this.terminal = null;
    }
}
