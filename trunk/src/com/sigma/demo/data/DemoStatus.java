/* DemoStatus.java
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

import com.sigma_rt.asn1.DerInputStream;
import com.sigma_rt.asn1.DerOutputStream;
import com.sigma_rt.asn1.DerUtil;
import com.sigma_rt.encoding.DerEncoder;

/**
 * @author James Che
 * 
 */
public class DemoStatus implements DerEncoder
{
    public static final int AVAILABLE = 1;

    public static final int UNAVALIABLE = 2;

    public static final int OCCUPIED = 3;

    private String fullName = null;

    private int status = UNAVALIABLE;

    private String cause = null;

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
     * @return the status
     */
    public int getStatus()
    {
        return status;
    }

    /**
     * @param status the status to set
     */
    public void setStatus(int status)
    {
        this.status = status;
    }

    /**
     * @return the cause
     */
    public String getCause()
    {
        return cause;
    }

    /**
     * @param cause the cause to set
     */
    public void setCause(String cause)
    {
        this.cause = cause;
    }

    public void derDecode(DerInputStream in) throws IOException
    {
        fullName = DerUtil.getString(in);
        status = in.getInteger();
        cause = DerUtil.getString(in);
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        DerUtil.putUTF8String(out, fullName);
        out.putInteger(status);
        DerUtil.putUTF8String(out, cause);
    }

    public String toString()
    {
        String sta = null;
        switch(status)
        {
            case AVAILABLE:
                sta = "AVAILABLE";
            break;
            case UNAVALIABLE:
                sta = "UNAVALIABLE";
            break;
            case OCCUPIED:
                sta = "OCCUPIED";
            break;
        }
        return "The fullName: " + fullName + "; status: " + sta + "; cause: "
                + cause;
    }
}
