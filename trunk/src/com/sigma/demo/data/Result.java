/* Result.java
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
public class Result implements DerEncoder {
	public static final int RESULT_SUCCSESSFUL = 1;

	public static final int RESULT_FIALED = 2;

	private int result = 0;

	private String cause = null;

	private String msgID = null;

	private String[] msgParams = null;

	/**
	 * @return the result
	 */
	public int getResult() {
		return result;
	}

	/**
	 * @param result
	 *            the result to set
	 */
	public void setResult(int result) {
		this.result = result;
	}

	/**
	 * @return the cause
	 */
	public String getCause() {
		return cause;
	}

	/**
	 * @param cause
	 *            the cause to set
	 */
	public void setCause(String cause) {
		this.cause = cause;
	}

	public void derDecode(DerInputStream in) throws IOException {
		result = in.getInteger();
		cause = DerUtil.getString(in);
		msgID = DerUtil.getString(in);
		int len = in.getInteger();
		msgParams = new String[len];
		for (int i = 0; i < len; i++) {
			msgParams[i] = DerUtil.getString(in);
		}
	}

	public void derEncode(DerOutputStream out) throws IOException {
		out.putInteger(result);
		DerUtil.putUTF8String(out, cause);
		DerUtil.putUTF8String(out, msgID);
		if (msgParams == null) {
			out.putInteger(0);
		} else {
			int len = msgParams.length;
			out.putInteger(len);
			for (int i = 0; i < len; i++) {
				DerUtil.putUTF8String(out, msgParams[i]);
			}
		}
	}

	public String toString() {
		String params = null;
		if (msgParams != null) {
			for (int i = 0; i < msgParams.length; i++) {
				if (i != 0)
					params += ", ";
				params += msgParams[i];
			}
		}
		return "result= "
				+ (result == RESULT_SUCCSESSFUL ? "successful" : "failed")
				+ "; cause: " + cause == null ? "" : cause + "; msgID: "
				+ msgID + "; msgParams: " + params;
	}

	public String getMsgID() {
		return msgID;
	}

	public void setMsgID(String msgID) {
		this.msgID = msgID;
	}

	public String[] getMsgParams() {
		return msgParams;
	}

	public void setMsgParams(String[] msgParams) {
		this.msgParams = msgParams;
	}

}
