/* DemoVNC.java
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
public class DemoVNC implements DerEncoder
{
    private String name = null;

    private String target = null;

    private String port = null;

    private String uname = null;

    private String password = null;

    private String type = null;

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
     * @return the target
     */
    public String getTarget()
    {
        return target;
    }

    /**
     * @param target the target to set
     */
    public void setTarget(String target)
    {
        this.target = target;
    }

    /**
     * @return the port
     */
    public String getPort()
    {
        return port;
    }

    /**
     * @param port the port to set
     */
    public void setPort(String port)
    {
        this.port = port;
    }

    /**
     * @return the uname
     */
    public String getUserName()
    {
        return uname;
    }

    /**
     * @param uname the uname to set
     */
    public void setUserName(String uname)
    {
        this.uname = uname;
    }

    /**
     * @return the password
     */
    public String getPassword()
    {
        return password;
    }

    /**
     * @param password the password to set
     */
    public void setPassword(String password)
    {
        this.password = password;
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

    public void derDecode(DerInputStream in) throws IOException
    {
        name = DerUtil.getString(in);
        target = DerUtil.getString(in);
        port = DerUtil.getString(in);
        uname = DerUtil.getString(in);
        password = DerUtil.getString(in);
        type = DerUtil.getString(in);
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        DerUtil.putUTF8String(out, name);
        DerUtil.putUTF8String(out, target);
        DerUtil.putUTF8String(out, port);
        DerUtil.putUTF8String(out, uname);
        DerUtil.putUTF8String(out, password);
        DerUtil.putUTF8String(out, type);
    }

}
