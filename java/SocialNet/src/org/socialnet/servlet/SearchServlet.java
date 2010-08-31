package org.socialnet.servlet;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.simple.JSONValue;
import org.socialnet.core.Relation;
import org.socialnet.core.SearchException;
import org.socialnet.core.SocialNet;

public class SearchServlet extends HttpServlet{
	public static final int PARAM_ERR = 1;
	public static final int NOT_FOUND = 2;
	public static final int SEARCH_EXCEPTION = 3;
	
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
	throws ServletException, IOException {
		this.doPost(request, response);
	}
	
	/**
		{
		  'code': 0,          // 错误代码。错误时为大于0的整数，否则为0
		  'mesg': '错误描述',  // 错误描述。code 大于0时，为非空字符串
		  'time': 123456789,  // 消息发生的时间。UNIX时间戳格式
		  'token': 'dsa4gdfgfh', // 单次通信信任码。字符串，必要时使用
		  'data': {}          // 其他附加数据定义块
		} 
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) 
	throws ServletException, IOException {
		//response.getWriter().println("hello!");
		response.setContentType("text/plain");
		response.setCharacterEncoding("utf8");
		
		Map<String, Object> data = new HashMap<String, Object>();
		String start = request.getParameter("s");
		String end = request.getParameter("e");
		String deep = request.getParameter("d");
		
		data.put("code", 0);
		data.put("mesg", "");
		data.put("time", System.currentTimeMillis());
		data.put("token", "");
		Map<String, Object> result = new HashMap<String, Object>();
		data.put("data", result);
		
		if(start == null || end == null){
			data.put("code", PARAM_ERR);
			data.put("mesg", "参数错误!, int s -- 开始节点, int e -- 结束节点, int d -- 搜索深度(default 0)");
		}else {		
			processSearch(start, end, deep, data, result);
		}
		JSONValue.writeJSONString(data, response.getWriter());
	}
	
	private void processSearch(String start, String end, String deep, 
			Map<String, Object> data, Map<String, Object>result){
		int intStart=0, intEnd=0, intDeep = 3;
		try{
			intStart = Integer.parseInt(start);
		}catch(Throwable e){};
		try{
			intEnd = Integer.parseInt(end);
		}catch(Throwable e){};
		try{
			intDeep = Integer.parseInt(deep);
		}catch(Throwable e){};
		
		SocialNet net = SocialNet.curInstance();
		
		List<Relation> path = SocialNet.EMPTY;
		try{
			System.out.println("s:" + intStart +" s:" + intEnd + " d:" + intDeep);
			path = net.searchPath(intStart, intEnd, intDeep);
			List<List<Integer>> xxx = new ArrayList<List<Integer>>();
			for(Relation r: path){
				List<Integer> rel = new ArrayList<Integer>();
				rel.add(r.start.id());
				rel.add(r.end_id);
				rel.add(r.relationType);
				xxx.add(rel);
			}
			result.put("path", xxx);
			result.put("path_help", "<start>,<end>,<type>");
			if(path.size() == 0){
				data.put("code", NOT_FOUND);
			}else {
				result.put("deep", path.size() - 1);
			}
		}catch(SearchException e){
			data.put("code", SEARCH_EXCEPTION);
			data.put("mesg", e.toString());
		}
	}
	
	//private int[] 
	
	//searchPath

}
