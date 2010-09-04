package org.socialnet.servlet;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.json.simple.JSONValue;
import org.socialnet.core.Relation;
import org.socialnet.core.SearchException;
import org.socialnet.shell.Startup;

public abstract class BaseServlet extends HttpServlet{
	protected static Log log = LogFactory.getLog(BaseServlet.class);
	public static final int PARAM_ERR = 1;
	public static final int NOT_FOUND = 2;
	public static final int SEARCH_EXCEPTION = 3;
	
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
	throws ServletException, IOException {
		//log.info("process thread:" + Thread.currentThread().getId() + "\tname:" + Thread.currentThread().getName());
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
		response.setContentType("text/plain");
		response.setCharacterEncoding("utf8");
		
		Map<String, Object> data = new HashMap<String, Object>();		
		data.put("code", 0);
		data.put("mesg", "");
		data.put("time", System.currentTimeMillis());
		data.put("token", "");
		Map<String, Object> result = new HashMap<String, Object>();
		data.put("data", result);
		try{
			process(request, data, result);
		}catch(SearchException e){
			data.put("code", SEARCH_EXCEPTION);
			data.put("mesg", e.toString());
		}		
		JSONValue.writeJSONString(data, response.getWriter());
	}
	
	protected abstract void process(HttpServletRequest request, 
			Map<String, Object>status, Map<String, Object>data) throws SearchException;
	
	protected List<List<Integer>> convertPath(Collection<Relation> path){
		List<List<Integer>> xxx = new ArrayList<List<Integer>>();
		for(Relation r: path){
			List<Integer> rel = new ArrayList<Integer>();
			rel.add(r.start.id());
			rel.add(r.end_id);
			rel.add(r.relationType);
			xxx.add(rel);
		}
		return xxx;
	}
	
	protected int intParam(HttpServletRequest request, String name, int def){
		int value = def;
		try{
			String start = request.getParameter(name);
			if(start != null){
				value = Integer.parseInt(start);
			}
		}catch(Throwable e){
		};
		return value;
	}
}
