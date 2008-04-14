/* Graph.java
 *
 * Sigma Resources & Technologies, Inc.
 * Copyright (C) 2005-2007, sigma-rt.com
 *
 * PROPRIETARY RIGHTS of Sigma Resources & Technologies are involved in
 * the subject matter of this material. All manufacturing, reproduction,
 * use, and sales rights pertaining to this subject matter are governed
 * by the license agreement. The recipient of this software implicitly
 * accepts the terms of the license.
 *
 * Created on Oct 30, 2007, by Alexandre Tsu.
 * 
 * Project:	DemoLauncher
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
package com.sigma.demo.data;

import java.io.IOException;

import com.sigma_rt.asn1.DerInputStream;
import com.sigma_rt.asn1.DerOutputStream;
import com.sigma_rt.asn1.DerUtil;
import com.sigma_rt.encoding.DerEncoder;

/**
 * @author James Che
 * 
 */
public abstract class Graph implements DerEncoder
{
    public static final String TYPE_SMP = "realtime.graph.type";

    public static final String TYPE_History = "history.graph.type";

    private String name = null;

    private String title = null;

    private String type = null;

    protected Graph(String type)
    {
        this.type = type;
    }

    /**
     * @return the type
     */
    public String getType()
    {
        return type;
    }

    /**
     * @return the name
     */
    public String getName()
    {
        return name;
    }

    /**
     * @param name the name to set
     */
    public void setName(String name)
    {
        this.name = name;
    }

    public void derDecode(DerInputStream in) throws IOException
    {
        type = DerUtil.getString(in);
        name = DerUtil.getString(in);
        title = DerUtil.getString(in);
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        DerUtil.putUTF8String(out, type);
        DerUtil.putUTF8String(out, name);
        DerUtil.putUTF8String(out, title);
    }

    /**
     * @return the title
     */
    public String getTitle()
    {
        return title;
    }

    /**
     * @param title the title to set
     */
    public void setTitle(String title)
    {
        this.title = title;
    }

    public static Graph decode(DerInputStream in) throws IOException
    {
        String type = null;
        Graph graph = null;
        DerInputStream din = null;
        if(in.markSupported())
        {
            in.mark(Integer.MAX_VALUE);
            type = DerUtil.getString(in);
            in.reset();
            din = in;
        }
        else
        {
            din = new DerInputStream(in.toByteArray());
            type = DerUtil.getString(in);
        }
        if(TYPE_SMP.equals(type))
        {
            graph = new RealtimeGraph();
        }
        else if(TYPE_History.equals(type))
        {
            graph = new HistoryGraph();
        }
        else
        {
            return null;
        }
        graph.derDecode(din);
        return graph;
    }

    public boolean equals(Object o)
    {
        if(!(o instanceof Graph))
            return false;

        Graph g = (Graph)o;
        if(g.getName().equals(name) && g.getType().equals(type))
            return true;
        else
            return false;
    }

    public String toString()
    {
        return "Graph name:" + name + ";title:" + title + ";type:" + type;
    }
}
