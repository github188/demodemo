/* Environment.java
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
public class Environment implements DerEncoder
{
    private String resourceURL = null;

    /**
     * the default value is 3 second
     */
    private long heartbeatInterval = 3000;

    private int heartbeatRepeat = 3;

    /**
     * The default value is 15 minutes
     */
    private int operateTimeout = 900000;

    /**
     * The default value is 1 minute
     */
    private int checkPeriod = 60000;

    /**
     * The default value is 1 minute
     */
    private int noticeDuring = 60000;

    private int seriesBufferSize = 120;

    /**
     * @return the seriesBufferSize
     */
    public int getSeriesBufferSize()
    {
        return seriesBufferSize;
    }

    /**
     * @param seriesBufferSize the seriesBufferSize to set
     */
    public void setSeriesBufferSize(int seriesBufferSize)
    {
        this.seriesBufferSize = seriesBufferSize;
    }

    /**
     * @return the heartbeatRepeat
     */
    public int getHeartbeatRepeat()
    {
        return heartbeatRepeat;
    }

    /**
     * @param heartbeatRepeat the heartbeatRepeat to set
     */
    public void setHeartbeatRepeat(int heartbeatRepeat)
    {
        this.heartbeatRepeat = heartbeatRepeat;
    }

    public void derDecode(DerInputStream in) throws IOException
    {
        heartbeatInterval = in.getBigInteger().longValue();
        heartbeatRepeat = in.getInteger();
        resourceURL = DerUtil.getString(in);
        operateTimeout = in.getInteger();
        checkPeriod = in.getInteger();
        noticeDuring = in.getInteger();
        seriesBufferSize = in.getInteger();
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        out.putInteger(BigInteger.valueOf(heartbeatInterval));
        out.putInteger(heartbeatRepeat);
        DerUtil.putUTF8String(out, resourceURL);
        out.putInteger(operateTimeout);
        out.putInteger(checkPeriod);
        out.putInteger(noticeDuring);
        out.putInteger(seriesBufferSize);
    }

    /**
     * @return the heartbeatInterval
     */
    public long getHeartbeatInterval()
    {
        return heartbeatInterval;
    }

    /**
     * @param heartbeatInterval the heartbeatInterval to set
     */
    public void setHeartbeatInterval(long heartbeatInterval)
    {
        this.heartbeatInterval = heartbeatInterval;
    }

    /**
     * @return the resourceURL
     */
    public String getResourceURL()
    {
        return resourceURL;
    }

    /**
     * @param resourceURL the resourceURL to set
     */
    public void setResourceURL(String resourceURL)
    {
        this.resourceURL = resourceURL;
    }

    /**
     * @return the operateTimeout
     */
    public int getOperateTimeout()
    {
        return operateTimeout;
    }

    /**
     * @param operateTimeout the operateTimeout to set
     */
    public void setOperateTimeout(int operateTimeout)
    {
        this.operateTimeout = operateTimeout;
    }

    /**
     * @return the checkPeriod
     */
    public int getCheckPeriod()
    {
        return checkPeriod;
    }

    /**
     * @param checkPeriod the checkPeriod to set
     */
    public void setCheckPeriod(int checkPeriod)
    {
        this.checkPeriod = checkPeriod;
    }

    /**
     * @return the noticeDuring
     */
    public int getNoticeDuring()
    {
        return noticeDuring;
    }

    /**
     * @param noticeDuring the noticeDuring to set
     */
    public void setNoticeDuring(int noticeDuring)
    {
        this.noticeDuring = noticeDuring;
    }

}
