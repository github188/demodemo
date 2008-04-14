/* DemoData.java
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
 * Created on Oct 25, 2007, by Alexandre Tsu.
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
 * ---------------------------------------------------
 * $Id: DemoData.java 788 2008-02-21 05:00:39Z dwu $
 * ---------------------------------------------------
 *
 */
package com.sigma.demo.data;

import java.io.IOException;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.json.simple.JSONObject;

import com.sigma_rt.asn1.DerInputStream;
import com.sigma_rt.asn1.DerOutputStream;
import com.sigma_rt.asn1.DerUtil;
import com.sigma_rt.asn1.DerValue;
import com.sigma_rt.encoding.DerEncoder;

/**
 * @author James Che
 * 
 */
public class DemoData implements DerEncoder
{
    public static final String FULL_NAME_SPLIT = "\\|";
    public static final String FULL_NAME_SPLIT1 = "|";

    private String name = null;

    private String fullName = null;

    private String brief = null;

    private long duration = 0;

    private String description = null;

    private byte[] topologyImage = null;

    private String topologyURL = null;

    private List<DemoVNC> vncs = new ArrayList<DemoVNC>();

    private List<DemoStep> steps = new ArrayList<DemoStep>();

    private Map<String, String> vars = new HashMap<String, String>();

    private String provision = null;

    private String preparation = null;

    private String healthy = null;

    private List<DemoConsole> consoles = new ArrayList<DemoConsole>();

    private List<Graph> graphs = new ArrayList<Graph>();

    private List<Series> serieses = new ArrayList<Series>();

    public void addSeries(Series series)
    {
        serieses.add(series);
    }

    public void removeSeries(Series series)
    {
        serieses.remove(series);
    }

    public Series getSeries(String sname)
    {
        for(Series series : serieses)
        {
            if(series.getName().equals(sname))
                return series;
        }
        return null;
    }

    public Series[] getSerieses()
    {
        return serieses.toArray(new Series[serieses.size()]);
    }

    public void clearSeries()
    {
        serieses.clear();
    }

    public void addGraph(Graph graph)
    {
        graphs.add(graph);
    }

    public void removeGraph(Graph graph)
    {
        graphs.remove(graph);
    }

    public Graph getGraph(String gname)
    {
        for(Graph graph : graphs)
        {
            if(graph.getName().equals(gname))
                return graph;
        }
        return null;
    }

    public Graph[] getGraphs()
    {
        return graphs.toArray(new Graph[graphs.size()]);
    }

    public void clearGraph()
    {
        graphs.clear();
    }

    public void addVariable(String name, String value)
    {
        vars.put(name, value);
    }

    public String getVariable(String name)
    {
        return vars.get(name);
    }

    public String[] getVariableNames()
    {
        return vars.keySet().toArray(new String[vars.keySet().size()]);
    }

    public String[] getVariableValues()
    {
        return vars.values().toArray(new String[vars.values().size()]);
    }

    public void addConsole(DemoConsole console)
    {
        consoles.add(console);
    }

    public void removeConsole(DemoConsole console)
    {
        consoles.remove(console);
    }

    public DemoConsole getConsole(String cname)
    {
        for(DemoConsole console : consoles)
        {
            if(console.getName().equals(cname))
                return console;
        }
        return null;
    }

    public DemoConsole[] getConsoles()
    {
        return consoles.toArray(new DemoConsole[consoles.size()]);
    }

    public void addStep(DemoStep step)
    {
        steps.add(step);
    }

    public DemoStep getStep(String stepName)
    {
        if(stepName == null)
            return null;

        for(DemoStep step : steps)
        {
            if(step == null)
                continue;

            if(stepName.equals(step.getName()))
                return step;
        }

        return null;
    }

    public void removeStep(DemoStep step)
    {
        steps.remove(step);
    }

    public DemoStep[] getSteps()
    {
        return steps.toArray(new DemoStep[steps.size()]);
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

    /**
     * @return the topologyImage
     */
    public byte[] getTopologyImage()
    {
        return topologyImage;
    }

    /**
     * @param topologyImage the topologyImage to set
     */
    public void setTopologyImage(byte[] topologyImage)
    {
        this.topologyImage = topologyImage;
    }

    /**
     * @return the description
     */
    public String getDescription()
    {
        return description;
    }

    /**
     * @param description the description to set
     */
    public void setDescription(String description)
    {
        this.description = description;
    }

    /**
     * @return the fullName
     */
    public String getFullName()
    {
        return fullName;
    }

    /**
     * @param fullName the fullName to set
     */
    public void setFullName(String fullName)
    {
        this.fullName = fullName;
    }

    /**
     * @return the topologyURL
     */
    public String getTopologyURL()
    {
        return topologyURL;
    }

    /**
     * @param topologyURL the topologyURL to set
     */
    public void setTopologyURL(String topologyURL)
    {
        this.topologyURL = topologyURL;
    }

    /**
     * @return the brief
     */
    public String getBrief()
    {
        return brief;
    }

    /**
     * @param brief the brief to set
     */
    public void setBrief(String brief)
    {
        this.brief = brief;
    }

    /**
     * @return the vname
     */
    public DemoVNC getVnc(String vname)
    {
        for(DemoVNC vnc : vncs)
        {
            if(vnc.getName().equals(vname))
                return vnc;
        }
        return null;
    }

    public DemoVNC[] getVncs()
    {
        return vncs.toArray(new DemoVNC[vncs.size()]);
    }

    /**
     * @param vnc the vnc to set
     */
    public void addVnc(DemoVNC vnc)
    {
        vncs.add(vnc);
    }

    public void removeVnc(DemoVNC vnc)
    {
        vncs.remove(vnc);
    }

    public void clearVncs()
    {
        vncs.clear();
    }

    public void derDecode(DerInputStream in) throws IOException
    {
        name = DerUtil.getString(in);
        fullName = DerUtil.getString(in);
        brief = DerUtil.getString(in);
        description = DerUtil.getString(in);
        preparation = DerUtil.getString(in);
        provision = DerUtil.getString(in);
        healthy = DerUtil.getString(in);
        duration = in.getBigInteger().longValue();
        topologyURL = DerUtil.getString(in);
        DerValue dv = in.getDerValue();
        if(dv.tag == DerValue.tag_OctetString)
        {
            topologyImage = dv.getOctetString();
        }
        else
        {
            topologyImage = null;
        }

        vncs.clear();
        int size = in.getInteger();
        for(int i = 0; i < size; i++)
        {
            DemoVNC vnc = new DemoVNC();
            vnc.derDecode(in);
            vncs.add(vnc);
        }

        steps.clear();
        dv = in.getDerValue();
        if(dv.tag == DerValue.tag_Integer)
        {
            size = dv.getInteger();
            for(int i = 0; i < size; i++)
            {
                DemoStep step = new DemoStep();
                step.derDecode(in);
                steps.add(step);
            }
        }
        else
        {
            steps.clear();
        }

        vars.clear();
        size = in.getInteger();
        for(int i = 0; i < size; i++)
        {
            String key = DerUtil.getString(in);
            String value = DerUtil.getString(in);
            vars.put(key, value);
        }

        consoles.clear();
        size = in.getInteger();
        for(int i = 0; i < size; i++)
        {
            DemoConsole c = new DemoConsole();
            c.derDecode(in);
            consoles.add(c);
        }

        graphs.clear();
        size = in.getInteger();
        for(int i = 0; i < size; i++)
        {
            Graph g = Graph.decode(in);
            if(g != null)
                graphs.add(g);
        }

        serieses.clear();
        size = in.getInteger();
        for(int i = 0; i < size; i++)
        {
            Series series = new Series();
            series.derDecode(in);
            serieses.add(series);
        }
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        DerUtil.putUTF8String(out, name);
        DerUtil.putUTF8String(out, fullName);
        DerUtil.putUTF8String(out, brief);
        DerUtil.putUTF8String(out, description);
        DerUtil.putUTF8String(out, preparation);
        DerUtil.putUTF8String(out, provision);
        DerUtil.putUTF8String(out, healthy);
        out.putInteger(BigInteger.valueOf(duration));
        DerUtil.putUTF8String(out, topologyURL);
        if(topologyImage != null)
            out.putOctetString(topologyImage);
        else
            out.putNull();

        int vsize = vncs.size();
        out.putInteger(vsize);
        for(DemoVNC vnc : vncs)
        {
            vnc.derEncode(out);
        }

        int stepSize = steps.size();
        out.putInteger(stepSize);
        for(DemoStep step : steps)
        {
            step.derEncode(out);
        }

        int varSize = vars.size();
        out.putInteger(varSize);
        for(Map.Entry<String, String> e : vars.entrySet())
        {
            DerUtil.putUTF8String(out, e.getKey());
            DerUtil.putUTF8String(out, e.getValue());
        }

        int size = consoles.size();
        out.putInteger(size);
        for(DemoConsole c : consoles)
        {
            c.derEncode(out);
        }

        size = graphs.size();
        out.putInteger(size);
        for(Graph g : graphs)
        {
            g.derEncode(out);
        }

        size = serieses.size();
        out.putInteger(size);
        for(Series s : serieses)
        {
            s.derEncode(out);
        }
    }

    /**
     * @return the duration
     */
    public long getDuration()
    {
        return duration;
    }

    /**
     * @param duration the duration to set
     */
    public void setDuration(long duration)
    {
        this.duration = duration;
    }

    /**
     * @return the provision
     */
    public String getProvision()
    {
        return provision;
    }

    /**
     * @param provision the provision to set
     */
    public void setProvision(String provision)
    {
        this.provision = provision;
    }

    /**
     * @return the preparation
     */
    public String getPreparation()
    {
        return preparation;
    }

    /**
     * @param preparation the preparation to set
     */
    public void setPreparation(String preparation)
    {
        this.preparation = preparation;
    }

    /**
     * @return the healthy
     */
    public String getHealthy()
    {
        return healthy;
    }

    /**
     * @param healthy the healthy to set
     */
    public void setHealthy(String healthy)
    {
        this.healthy = healthy;
    }

    public List<RealtimeGraph> getGraphs(String seriesName)
    {
        List<RealtimeGraph> result = new ArrayList<RealtimeGraph>();
        for(Graph graph : graphs)
        {
            if(graph.getType().equals(Graph.TYPE_SMP))
            {
                RealtimeGraph rg = (RealtimeGraph)graph;
                if(rg.isContains(seriesName))
                    result.add(rg);
            }
            else
            {
                continue;
            }
        }
        return result;
    }

    public String toString()
    {
        StringBuffer buf = new StringBuffer("the Demo data:\r\n");
        buf.append("  Demo name:" + name);
        buf.append(";fullName:" + fullName);
        buf.append(";brief:" + brief);
        buf.append(";duration:" + duration);
        buf.append(";description:" + description);
        buf.append(";topologyURL:" + topologyURL);
        buf.append(";provision:" + provision);
        buf.append(";preparation:" + preparation);
        buf.append(";healthy:" + healthy);
        for(DemoConsole c : consoles)
        {
            buf.append("\r\n    " + c.toString());
        }
        for(Graph g : graphs)
        {
            buf.append("\r\n    " + g.toString());
        }
        for(Series s : serieses)
        {
            buf.append("\r\n    " + s.toString());
        }
        for(DemoStep ds : steps)
        {
            buf.append("\r\n    " + ds.toString());
        }
        return buf.toString();
    }

}
