package org.goku.db;

import java.util.Collection;

/**
 * 查询记录结果。
 * @author deon
 *
 */
public class QueryResult {
	/**
	 * 查询会话ID.
	 */
	public String sessionId = "";
	/**
	 * 结果总数
	 */
	public int count = 0;	
	public Collection data = null;
}
