/* DemoRef.java
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
 * Created on Nov 16, 2007, by Alexandre Tsu.
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
public class DemoRef implements DerEncoder, Comparable<DemoRef>
{
    private String name = null;

    private String fullName = null;

    private String resource = null;

    private String host = null;

    private int port = 0;

    private int status = 0;

    public void derDecode(DerInputStream in) throws IOException
    {
        name = DerUtil.getString(in);
        fullName = DerUtil.getString(in);
        resource = DerUtil.getString(in);
        host = DerUtil.getString(in);
        port = in.getInteger();
        status = in.getInteger();
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        DerUtil.putUTF8String(out, name);
        DerUtil.putUTF8String(out, fullName);
        DerUtil.putUTF8String(out, resource);
        DerUtil.putUTF8String(out, host);
        out.putInteger(port);
        out.putInteger(status);
    }

    public String toString()
    {
        String s = null;
        switch(status)
        {
            case DemoStatus.AVAILABLE:
                s = "available";
            break;
            case DemoStatus.OCCUPIED:
                s = "occupied";
            break;
            case DemoStatus.UNAVALIABLE:
                s = "unavailable";
            break;
        }
        return "DemoRef name:" + name + ";fullName:" + fullName + ";resource:"
                + resource + ";demoServerHost:" + host + ";port:" + port
                + ";status:" + s;
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
     * @return the resource
     */
    public String getResource()
    {
        return resource;
    }

    /**
     * @param resource the resource to set
     */
    public void setResource(String resource)
    {
        this.resource = resource;
    }

    /**
     * @return the host
     */
    public String getHost()
    {
        return host;
    }

    /**
     * @param host the host to set
     */
    public void setHost(String host)
    {
        this.host = host;
    }

    /**
     * @return the port
     */
    public int getPort()
    {
        return port;
    }

    /**
     * @param port the port to set
     */
    public void setPort(int port)
    {
        this.port = port;
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

    public int compareTo(DemoRef o)
    {
        if(o == null)
            return 1;
        return name.compareToIgnoreCase(o.getName());
    }

}
