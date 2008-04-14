/* AppletTest.java
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
 * Created on 2008-3-20, by Alexandre Tsu.
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
package com.sigma.demo.test;

import java.awt.Graphics;

import javax.swing.JApplet;

/**
 * @author James Che
 * 
 */
public class AppletTest extends JApplet
{
    private String msg = "";

    public void showMsg(String msg)
    {
	this.msg = msg;
	this.repaint();
    }

    public void paint(Graphics g)
    {
	g.drawString(msg, 10, 10);
    }
}
