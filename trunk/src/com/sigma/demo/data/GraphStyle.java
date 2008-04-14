/* GraphStyle.java
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
 * Created on Dec 3, 2007, by Alexandre Tsu.
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
public abstract class GraphStyle implements DerEncoder
{
    private static final String styleA = "gsa";

    private static final String styleB = "gsb";

    private String name = null;

    protected GraphStyle(String name)
    {
        this.name = name;
    }

    /**
     * @return the name
     */
    public String getName()
    {
        return name;
    }

    public void derDecode(DerInputStream in) throws IOException
    {
        name = DerUtil.getString(in);
    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        DerUtil.putUTF8String(out, name);
    }

    public static GraphStyle decode(DerInputStream in) throws IOException
    {
        String _name = null;
        GraphStyle _style = null;
        DerInputStream din = null;
        if(in.markSupported())
        {
            in.mark(Integer.MAX_VALUE);
            _name = DerUtil.getString(in);
            in.reset();
            din = in;
        }
        else
        {
            din = new DerInputStream(in.toByteArray());
            _name = DerUtil.getString(in);
        }
        if(styleA.equals(_name))
        {
            _style = getStyleA();
        }
        else if(styleB.equals(_name))
            ;
        {
            _style = getStyleB();
        }
        if(_style != null)
        {
            _style.derDecode(din);
        }
        return _style;
    }

    public static GraphStyle getStyleA()
    {
        return new GraphStyle(styleA)
        {
        };
    }

    public static GraphStyle getStyleB()
    {
        return new GraphStyle(styleB)
        {
        };
    }

    public String toString()
    {
        return "style:" + name;
    }
}
