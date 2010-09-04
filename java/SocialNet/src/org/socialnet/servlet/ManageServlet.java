package org.socialnet.servlet;

import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.socialnet.core.SearchException;
import org.socialnet.core.SocialNet;

public class ManageServlet extends BaseServlet {

	@Override
	protected void process(HttpServletRequest request,
			Map<String, Object> status, Map<String, Object> data)
			throws SearchException {
		String action = request.getParameter("action");
		int start = this.intParam(request, "s", -1);
		int end = this.intParam(request, "e", -1);
		if(start == -1 || action==null){
			status.put("code", PARAM_ERR);
			status.put("mesg", "参数错误!, int s -- 开始节点, int e -- 结束节点, string action [add|remove]");
			return;
		}
		
		SocialNet net = SocialNet.curInstance();
		if (action.equals("add")){
			net.createRelation(start, end);
		}else if(action.equals("remove")){
			net.removeRelation(start, end);
		}
	}

}
