/* Series.java
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
 * Created on Nov 30, 2007, by Alexandre Tsu.
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
import java.math.BigInteger;

import com.sigma_rt.asn1.DerInputStream;
import com.sigma_rt.asn1.DerOutputStream;
import com.sigma_rt.asn1.DerUtil;
import com.sigma_rt.encoding.DerEncoder;

/**
 * @author James Che
 * 
 */
public class Series implements DerEncoder
{
    private String name = null;

    private String color = null;

    private String xLabel = null;

    private String yLabel = null;

    private String description = null;

    private String type = null;

    private String ip = null;

    private String community = null;

    private String OID = null;

    private long yMax = 0;

    private int poll = 0;

    public void derDecode(DerInputStream in) throws IOException
    {
        name = DerUtil.getString(in);
        color = DerUtil.getString(in);
        xLabel = DerUtil.getString(in);
        yLabel = DerUtil.getString(in);
        description = DerUtil.getString(in);
        type = DerUtil.getString(in);
        ip = DerUtil.getString(in);
        community = DerUtil.getString(in);
        OID = DerUtil.getString(in);
        yMax = in.getBigInteger().longValue();
        poll = in.getInteger();
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        DerUtil.putUTF8String(out, name);
        DerUtil.putUTF8String(out, color);
        DerUtil.putUTF8String(out, xLabel);
        DerUtil.putUTF8String(out, yLabel);
        DerUtil.putUTF8String(out, description);
        DerUtil.putUTF8String(out, type);
        DerUtil.putUTF8String(out, ip);
        DerUtil.putUTF8String(out, community);
        DerUtil.putUTF8String(out, OID);
        out.putInteger(BigInteger.valueOf(yMax));
        out.putInteger(poll);
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
     * @return the color
     */
    public String getColor()
    {
        return color;
    }

    /**
     * @param color the color to set
     */
    public void setColor(String color)
    {
        this.color = color;
    }

    /**
     * @return the xLabel
     */
    public String getXLabel()
    {
        return xLabel;
    }

    /**
     * @param label the xLabel to set
     */
    public void setXLabel(String label)
    {
        xLabel = label;
    }

    /**
     * @return the yLabel
     */
    public String getYLabel()
    {
        return yLabel;
    }

    /**
     * @param label the yLabel to set
     */
    public void setYLabel(String label)
    {
        yLabel = label;
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
     * @return the yMax
     */
    public long getYMax()
    {
        return yMax;
    }

    /**
     * @param max the yMax to set
     */
    public void setYMax(long max)
    {
        yMax = max;
    }

    /**
     * @return the type
     */
    public String getType()
    {
        return type;
    }

    /**
     * @param type the type to set
     */
    public void setType(String type)
    {
        this.type = type;
    }

    /**
     * @return the ip
     */
    public String getIp()
    {
        return ip;
    }

    /**
     * @param ip the ip to set
     */
    public void setIp(String ip)
    {
        this.ip = ip;
    }

    /**
     * @return the community
     */
    public String getCommunity()
    {
        return community;
    }

    /**
     * @param community the community to set
     */
    public void setCommunity(String community)
    {
        this.community = community;
    }

    /**
     * @return the oID
     */
    public String getOID()
    {
        return OID;
    }

    /**
     * @param oid the oID to set
     */
    public void setOID(String oid)
    {
        OID = oid;
    }

    /**
     * @return the poll
     */
    public int getPoll()
    {
        return poll;
    }

    /**
     * @param poll the poll to set
     */
    public void setPoll(int poll)
    {
        this.poll = poll;
    }

    public String toString()
    {
        return "Series name: " + name + "; color: " + color + ";type: " + type
                + "; xlable:" + xLabel + "; ylable: " + yLabel + ";ymax: "
                + yMax + "; description:" + description + ";poll: " + poll
                + ";ip: " + ip + ";oid: " + OID + ";community:" + community;
    }
}
