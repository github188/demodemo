/* Action.java
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
public class Action implements DerEncoder
{
    private String cmd = null;

    private String verify = null;

    private String type = null;

    /**
     * @return the cmd
     */
    public String getCmd()
    {
        return cmd;
    }

    /**
     * @param cmd the cmd to set
     */
    public void setCmd(String cmd)
    {
        this.cmd = cmd;
    }

    /**
     * @return the verify
     */
    public String getVerify()
    {
        return verify;
    }

    /**
     * @param verify the verify to set
     */
    public void setVerify(String verify)
    {
        this.verify = verify;
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
        cmd = DerUtil.getString(in);
        verify = DerUtil.getString(in);
        type = DerUtil.getString(in);
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        DerUtil.putUTF8String(out, cmd);
        DerUtil.putUTF8String(out, verify);
        DerUtil.putUTF8String(out, type);
    }

}
