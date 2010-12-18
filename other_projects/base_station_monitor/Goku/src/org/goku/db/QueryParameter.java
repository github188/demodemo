package org.goku.db;

import java.util.HashMap;
import java.util.Map;

/**
 * 查询参数信息
 * @author deon
 */
public class QueryParameter {
	public String qsid = null;
	public Map<String, String> param = new HashMap<String, String>();
	
	public int offset = 0;
	public int limit = 100;
	public String order = null;
}
