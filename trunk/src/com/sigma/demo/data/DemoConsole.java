/* DemoConsole.java
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
public class DemoConsole implements DerEncoder
{
    private String name = null;

    private String title = null;

    private String prompt = null;

    private String target = null;

    private String args = null;

    private String type = null;
    
    private String device = null;

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
     * @return the args
     */
    public String getArgs()
    {
        return args;
    }

    /**
     * @param args the args to set
     */
    public void setArgs(String args)
    {
        this.args = args;
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
     * @return the prompt
     */
    public String getPrompt()
    {
        return prompt;
    }

    /**
     * @param prompt the prompt to set
     */
    public void setPrompt(String prompt)
    {
        this.prompt = prompt;
    }

    public void derDecode(DerInputStream in) throws IOException
    {
        name = DerUtil.getString(in);
        title = DerUtil.getString(in);
        prompt = DerUtil.getString(in);
        target = DerUtil.getString(in);
        args = DerUtil.getString(in);
        type = DerUtil.getString(in);
        device = DerUtil.getString(in);
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        DerUtil.putUTF8String(out, name);
        DerUtil.putUTF8String(out, title);
        DerUtil.putUTF8String(out, prompt);
        DerUtil.putUTF8String(out, target);
        DerUtil.putUTF8String(out, args);
        DerUtil.putUTF8String(out, type);
        DerUtil.putUTF8String(out, device);
    }

    /**
     * @return the title
     */
    public String getTitle()
    {
        return title;
    }

    /**
     * @param title the title to set
     */
    public void setTitle(String title)
    {
        this.title = title;
    }

    public boolean equals(Object o)
    {
        if(!(o instanceof DemoConsole))
            return false;

        DemoConsole con = (DemoConsole)o;
        if(con.getName().equals(name))
            return true;
        else
            return false;
    }

    public String toString()
    {
        return "Console name:" + name + ";title:" + title + ";type:" + type
                + ";target:" + target + ";args:" + args;
    }

	public String getDevice() {
		return device;
	}

	public void setDevice(String device) {
		this.device = device;
	}

}
