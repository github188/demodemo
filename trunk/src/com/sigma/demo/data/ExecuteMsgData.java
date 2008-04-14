/* ExecuteMsgData.java
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
 * Created on Nov 1, 2007, by Alexandre Tsu.
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

/**
 * @author James Che
 * 
 */
public class ExecuteMsgData extends ExecuteData
{
    public static final int MSG_TYPE_INFO = 0;

    public static final int MSG_TYPE_WARNNING = 1;

    public static final int MSG_TYPE_ERROR = 2;

    private String message = null;

    private int msgType = MSG_TYPE_INFO;

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

    /**
     * @param type
     */
    public ExecuteMsgData()
    {
        super(ExecuteData.TYPE_MSG);
    }

    public void derDecode(DerInputStream in) throws IOException
    {
        super.derDecode(in);
        msgType = in.getInteger();
        message = DerUtil.getString(in);
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        super.derEncode(out);
        out.putInteger(msgType);
        DerUtil.putUTF8String(out, message);
    }

    /**
     * @return the msgType
     */
    public int getMsgType()
    {
        return msgType;
    }

    /**
     * @param msgType the msgType to set
     */
    public void setMsgType(int msgType)
    {
        this.msgType = msgType;
    }

    public String toString()
    {
        StringBuffer buf = new StringBuffer(super.toString());
        buf.append("; message: " + message);
        return buf.toString();
    }
}
