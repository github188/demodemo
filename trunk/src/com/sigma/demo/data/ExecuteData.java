/* ExecuteData.java
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
import com.sigma_rt.encoding.DerEncoder;

/**
 * @author James Che
 * 
 */
public abstract class ExecuteData implements DerEncoder
{
    protected static final int TYPE_MSG = 1;

    protected static final int TYPE_POP = 2;

    protected static final int TYPE_ACTIVE = 3;

    protected static final int TYPE_SWITCH = 4;

    private static final String pop_prefix = "popup";

    private static final String active_prefix = "active";

    private static final String switch_prefix = "switch";

    private int type = 0;

    private String demoFullName = null;

    private String stepName = null;

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
     * @return the stepName
     */
    public String getStepName()
    {
        return stepName;
    }

    /**
     * @param stepName the stepName to set
     */
    public void setStepName(String stepName)
    {
        this.stepName = stepName;
    }

    public ExecuteData(int type)
    {
        this.type = type;
    }

    public void derDecode(DerInputStream in) throws IOException
    {
        type = in.getInteger();
        demoFullName = DerUtil.getString(in);
        stepName = DerUtil.getString(in);
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        out.putInteger(type);
        DerUtil.putUTF8String(out, demoFullName);
        DerUtil.putUTF8String(out, stepName);
    }

    /**
     * @return the type
     */
    public int getType()
    {
        return type;
    }

//    /**
//     * @param type the type to set
//     */
//    public void setType(int type)
//    {
//        this.type = type;
//    }

    public String toString()
    {
        StringBuffer buf = new StringBuffer();
        buf.append("demo full name: " + demoFullName);
        buf.append("; step name: " + stepName);
        return buf.toString();
    }

    public static ExecuteData decode(DerInputStream in) throws IOException
    {
        int type = 0;
        ExecuteData data = null;
        DerInputStream din = null;
        if(in.markSupported())
        {
            in.mark(Integer.MAX_VALUE);
            type = in.getInteger();
            in.reset();
            din = in;
        }
        else
        {
            din = new DerInputStream(in.toByteArray());
            type = in.getInteger();

        }
        switch(type)
        {
            case TYPE_MSG:
                data = new ExecuteMsgData();
                data.derDecode(din);
            break;
            case TYPE_POP:
                data = new ExecutePopData();
                data.derDecode(din);
            break;
            case TYPE_ACTIVE:
                data = new ExecuteActiveData();
                data.derDecode(din);
            break;
            case TYPE_SWITCH:
                data = new ExecuteSwitchData();
                data.derDecode(din);
            break;
            default:
                return null;
        }
        return data;
    }

    public static ExecuteData createExecuteData(String message)
    {
        if(message == null)
            return null;

        message = message.trim();
        if(message.toLowerCase().startsWith(pop_prefix))
        {
            String msg = message.substring(pop_prefix.length());
            ExecutePopData data = new ExecutePopData();
            data.setMessage(msg.trim());
            return data;
        }
        else if(message.toLowerCase().startsWith(active_prefix))
        {
            String msg = message.substring(active_prefix.length());
            ExecuteActiveData data = new ExecuteActiveData();
            data.setMessage(msg.trim());
            return data;
        }
        else if(message.toLowerCase().startsWith(switch_prefix))
        {
            String msg = message.substring(switch_prefix.length());
            ExecuteSwitchData data = new ExecuteSwitchData();
            data.setMessage(msg.trim());
            return data;
        }
        else
        {
            ExecuteMsgData data = new ExecuteMsgData();
            data.setMessage(message);
            return data;
        }
    }

}
