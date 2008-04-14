/* GraphsApplet.java
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
package com.sigma.applet;

import java.util.Date;

import javax.swing.JApplet;
import javax.swing.JTabbedPane;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.data.time.Minute;
import org.jfree.data.time.TimeSeries;
import org.jfree.data.time.TimeSeriesCollection;
import org.jfree.data.xy.XYDataset;

/**
 * @author James Che
 * 
 */
public class GraphsApplet extends JApplet
{
    /**
     * 
     */
    private static final long serialVersionUID = 1L;

    public void init()
    {
	final JTabbedPane tabs = new JTabbedPane();

	XYDataset data1 = createTimeSeriesCollection1(1.1);
	JFreeChart chart1 = ChartFactory.createTimeSeriesChart("Time Series",
		"Date", "Rate", data1, true, true, false);
	ChartPanel panel1 = new ChartPanel(chart1, 400, 300, 200, 100, 400,
		200, true, false, false, false, true, true);
	tabs.add("Chart 1", panel1);

	XYDataset data2 = createTimeSeriesCollection1(0.8);
	JFreeChart chart2 = ChartFactory.createTimeSeriesChart("Time Series",
		"Date", "Rate", data2, true, true, false);
	ChartPanel panel2 = new ChartPanel(chart2, 400, 300, 200, 100, 400,
		200, true, false, false, false, true, true);
	tabs.add("Chart 2", panel2);

	this.getContentPane().add(tabs);
    }

    private TimeSeriesCollection createTimeSeriesCollection1(double d)
    {

	TimeSeries t1 = new TimeSeries("Realtime", "time", "Value",
		Minute.class);
	try
	{
	    long t = System.currentTimeMillis();
	    if (d > 1)
	    {
		t1.add(new Minute(new Date(t)), new Double(50.1 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(12.3 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(23.9 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(83.4 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(-34.7 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(76.5 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(10.0 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(-14.7 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(43.9 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(49.6 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(37.2 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(17.1 * d));
	    } else
	    {
		t1.add(new Minute(new Date(t)), new Double(43.9 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(49.6 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(37.2 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(17.1 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(-34.7 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(76.5 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(10.0 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(-14.7 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(50.1 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(12.3 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(23.9 * d));
		t += 300000;
		t1.add(new Minute(new Date(t)), new Double(83.4 * d));

	    }

	} catch (Exception e)
	{
	    System.err.println(e.getMessage());
	}

	return new TimeSeriesCollection(t1);

    }

    public void start()
    {
    }

    public void stop()
    {
    }

    public void destroy()
    {
    }
}
