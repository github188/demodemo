package org.socialnet.servlet;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.socialnet.core.NodePath;
import org.socialnet.core.SearchException;
import org.socialnet.core.SocialNet;

public class ListRelatedServlet extends BaseServlet{
	protected void process(HttpServletRequest request, 
			final Map<String, Object>status, final Map<String, Object>data) throws SearchException{	
		int start, min, max, count;
		
		start = intParam(request, "s", -1);
		min = intParam(request, "min", 1);
		max = intParam(request, "max", 6);
		count = intParam(request, "count", 100);
		if(start == -1){
			status.put("code", PARAM_ERR);
			status.put("mesg", "参数错误!, int s -- 开始节点, int min -- 最小层数(1), int max -- 最大层数(6), int count -- 最大返回(100)");
			return;
		}
		
		SocialNet net = SocialNet.curInstance();
		
		Collection<NodePath> datList = new ArrayList<NodePath>(0);
		datList = net.listRelatedNode(start, min, max, count);
		
		Collection<Map<String, Object>> xxx = new ArrayList<Map<String, Object>>(datList.size());
		for(NodePath node: datList){
			Map<String, Object> tmp = new HashMap<String, Object>();
			tmp.put("user", node.node.id());
			tmp.put("path", convertPath(node.path));
			xxx.add(tmp);
		}
		
		data.put("list", xxx);
		data.put("count", datList.size());
	}
}
