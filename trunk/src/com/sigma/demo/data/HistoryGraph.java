/* HistoryGraph.java
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
 * Created on Nov 22, 2007, by Alexandre Tsu.
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
public class HistoryGraph extends Graph
{
    private String picture = null;

    /**
     * @return the picture
     */
    public String getPicture()
    {
        return picture;
    }

    /**
     * @param picture the picture to set
     */
    public void setPicture(String picture)
    {
        this.picture = picture;
    }

    public HistoryGraph()
    {
        super(Graph.TYPE_History);
    }

    public void derDecode(DerInputStream in) throws IOException
    {
        super.derDecode(in);
        picture = DerUtil.getString(in);

    }

    public void derEncode(DerOutputStream out) throws IOException
    {
        super.derEncode(out);
        DerUtil.putUTF8String(out, picture);
    }

    public String toString()
    {
        return super.toString() + ";picture:" + picture;
    }
}
