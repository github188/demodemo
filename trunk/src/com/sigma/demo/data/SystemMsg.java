/* SystemMsg.java
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
 * Created on Nov 12, 2007, by Alexandre Tsu.
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
public class SystemMsg implements DerEncoder
{
    public static final int TYPE_INFO = 0;

    public static final int TYPE_WARN = 1;

    public static final int TYPE_ERROR = 2;

    private int type = TYPE_INFO;

    private String message = null;

    public SystemMsg()
    {
    }

    public SystemMsg(String message)
    {
        this(TYPE_INFO, message);
    }

    public SystemMsg(int type, String message)
    {
        this.type = type;
        this.message = message;
    }

    /**
     * @return the type
     */
    public int getType()
    {
        return type;
    }

    /**
     * @param type the type to set
     */
    public void setType(int type)
    {
        this.type = type;
    }

    /**
     * @return the message
     */
    public String getMessage()
    {
        return message;
    }

    /**
     * @param message the message to set
     */
    public void setMessage(String message)
    {
        this.message = message;
    }

    public void derDecode(DerInputStream in) throws IOException
    {
        type = in.getInteger();
        message = DerUtil.getString(in);

    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        out.putInteger(type);
        DerUtil.putUTF8String(out, message);
    }

}
