package org.socialnet.servlet;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.socialnet.core.Relation;
import org.socialnet.core.SearchException;
import org.socialnet.core.SocialNet;

public class SearchServlet extends BaseServlet{
	protected void process(HttpServletRequest request, 
			Map<String, Object>status, Map<String, Object>data) throws SearchException{	
		int intStart=0, intEnd=0, intDeep = 3;
		
		intStart = intParam(request, "s", -1);
		intEnd = intParam(request, "e", -1);
		intDeep = intParam(request, "d", 3);
		if(intStart == -1 || intEnd == -1){
			status.put("code", PARAM_ERR);
			status.put("mesg", "参数错误!, int s -- 开始节点, int e -- 结束节点, int d -- 搜索深度(default 0)");
			return;
		}
		
		SocialNet net = SocialNet.curInstance();
		
		List<Relation> path = SocialNet.EMPTY;
		path = net.searchPath(intStart, intEnd, intDeep);
		data.put("path", convertPath(path));
		data.put("path_help", "<start>,<end>,<type>");
		if(path.size() == 0){
			status.put("code", NOT_FOUND);
		}else {
			data.put("deep", path.size() - 1);
		}
	}
}
