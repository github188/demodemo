/* StepResult.java
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

/**
 * @author James Che
 * 
 */
public class StepResult extends Result
{
    private String name = null;

    private String demoFullName = null;

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

    public void derDecode(DerInputStream in) throws IOException
    {
        super.derDecode(in);
        name = DerUtil.getString(in);
        demoFullName = DerUtil.getString(in);
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        super.derEncode(out);
        DerUtil.putUTF8String(out, name);
        DerUtil.putUTF8String(out, demoFullName);
    }

    public String toString()
    {
        String str = "stepName: " + name + "; demoFullName: " + demoFullName
                + "; " + super.toString();
        return str;
    }
}
