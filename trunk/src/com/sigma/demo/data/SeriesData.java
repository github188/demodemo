/* SeriesData.java
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
 * Created on Nov 8, 2007, by Alexandre Tsu.
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
public class SeriesData implements DerEncoder
{
    private String demoFullName = null;

    private String name = null;

    private long dataX = 0;

    private double dataY = 0;

    public void derDecode(DerInputStream in) throws IOException
    {
        demoFullName = DerUtil.getString(in);
        name = DerUtil.getString(in);
        dataX = in.getBigInteger().longValue();
        String _dataY = in.getUTF8String();
        dataY = Double.valueOf(_dataY);
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        DerUtil.putUTF8String(out, demoFullName);
        DerUtil.putUTF8String(out, name);
        out.putInteger(BigInteger.valueOf(dataX));
        out.putUTF8String(String.valueOf(dataY));
    }

    /**
     * @return the demoFullName
     */
    public String getDemoFullName()
    {
        return demoFullName;
    }

    /**
     * @param demoFullName the demoFullName to set
     */
    public void setDemoFullName(String demoFullName)
    {
        this.demoFullName = demoFullName;
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
     * @return the dataX
     */
    public long getDataX()
    {
        return dataX;
    }

    /**
     * @param dataX the dataX to set
     */
    public void setDataX(long dataX)
    {
        this.dataX = dataX;
    }

    /**
     * @return the dataY
     */
    public double getDataY()
    {
        return dataY;
    }

    /**
     * @param dataY the dataY to set
     */
    public void setDataY(double dataY)
    {
        this.dataY = dataY;
    }

    public String toString()
    {
        StringBuffer buf = new StringBuffer();
        buf.append("name: " + name);
        buf.append("; demo full name: " + demoFullName);
        buf.append("; X: " + dataX);
        buf.append("; Y: " + dataY);
        return buf.toString();
    }
}
