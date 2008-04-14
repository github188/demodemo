/* ExecuteActiveData.java
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
 * Created on Nov 15, 2007, by Alexandre Tsu.
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
public class ExecuteActiveData extends ExecuteData
{
    public static final String ACTIVE_GRAPH = "graph";

    public static final String ACTIVE_CONSOLE = "console";

    private String message = null;

    private String type = null;

    private String[] params = null;

    public ExecuteActiveData()
    {
        super(TYPE_ACTIVE);
    }

    public void derDecode(DerInputStream in) throws IOException
    {
        super.derDecode(in);
        message = DerUtil.getString(in);
        if(message != null)
        {
            message = message.trim();
            String[] strs = message.split(" ");
            if(strs != null && strs.length > 0)
            {
                String _type = strs[0];
                if(_type.toLowerCase().equals(ACTIVE_GRAPH))
                    type = ACTIVE_GRAPH;
                else if(_type.toLowerCase().equals(ACTIVE_CONSOLE))
                    type = ACTIVE_CONSOLE;
                params = new String[strs.length - 1];
                for(int i = 0; i < params.length; i++)
                {
                    params[i] = strs[i + 1];
                }
            }
        }
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        super.derEncode(out);
        DerUtil.putUTF8String(out, message);
    }

    /**
     * @return the type
     */
    public String getActiveType()
    {
        return type;
    }

    /**
     * @return the params
     */
    public String[] getParams()
    {
        return params;
    }

    /**
     * @param message the message to set
     */
    public void setMessage(String message)
    {
        this.message = message;
    }

    public String toString()
    {
        StringBuffer buf = new StringBuffer(super.toString());
        buf.append("; message: " + message);
        return buf.toString();
    }

}
