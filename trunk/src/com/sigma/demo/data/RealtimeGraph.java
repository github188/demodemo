/* RealtimeGraph.java
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
 * Created on Nov 22, 2007, by Alexandre Tsu.
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
import java.util.ArrayList;
import java.util.List;

import com.sigma_rt.asn1.DerInputStream;
import com.sigma_rt.asn1.DerOutputStream;
import com.sigma_rt.asn1.DerUtil;
import com.sigma_rt.asn1.DerValue;

/**
 * @author James Che
 * 
 */
public class RealtimeGraph extends Graph
{

    private GraphStyle graphStyle = null;

    private List<String> lines = new ArrayList<String>();

    public RealtimeGraph()
    {
        super(Graph.TYPE_SMP);
    }

    public void addSeries(String line)
    {
        if(line != null)
            lines.add(line);
    }

    public Series getSeries(String sName, DemoData demo)
    {
        if(sName == null || demo == null)
            return null;

        return demo.getSeries(sName);
    }

    public Series[] getSeries(DemoData demo)
    {
        if(demo == null)
            return new Series[0];

        Series[] ss = new Series[lines.size()];
        for(int i = 0; i < ss.length; i++)
        {
            ss[i] = demo.getSeries(lines.get(i));
        }
        return ss;
    }

    public void removeSeries(String line)
    {
        lines.remove(line);
    }

    public void clearSeries()
    {
        lines.clear();
    }

    public boolean isContains(String series)
    {
        return lines.contains(series);
    }

    public void derDecode(DerInputStream in) throws IOException
    {
        super.derDecode(in);
        DerValue dv = in.getDerValue();
        if(dv.tag != DerValue.tag_Null)
        {
            graphStyle = GraphStyle.decode(dv.data());
        }
        else
        {
            graphStyle = null;
        }
        int size = in.getInteger();
        for(int i = 0; i < size; i++)
        {
            String line = DerUtil.getString(in);
            lines.add(line);
        }
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        super.derEncode(out);
        if(graphStyle != null)
        {
            DerOutputStream dout = new DerOutputStream();
            graphStyle.derEncode(dout);
            out.putOctetString(dout.toByteArray());
        }
        else
        {
            out.putNull();
        }
        int size = lines.size();
        out.putInteger(size);
        for(String line : lines)
        {
            DerUtil.putUTF8String(out, line);
        }
    }

    /**
     * @return the graphStyle
     */
    public GraphStyle getGraphStyle()
    {
        return graphStyle;
    }

    /**
     * @param graphStyle the graphStyle to set
     */
    public void setGraphStyle(GraphStyle style)
    {
        this.graphStyle = style;
    }

    public String toString()
    {
        String str = super.toString();
        str += ";" + graphStyle.toString();
        str += ";lines:";
        for(int i = 0; i < lines.size(); i++)
        {
            if(i != 0)
                str += ",";
            str += lines.get(i);
        }
        return str;
    }
}
