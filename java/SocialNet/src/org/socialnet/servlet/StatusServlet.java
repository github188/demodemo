package org.socialnet.servlet;

import java.util.Date;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.socialnet.core.NodeHeap;
import org.socialnet.core.RunStatus;
import org.socialnet.core.SearchException;

public class StatusServlet extends BaseServlet {

	@Override
	protected void process(HttpServletRequest request,
			Map<String, Object> status, Map<String, Object> data)
			throws SearchException {
		data.put("cache_node_count", NodeHeap.curInstance().nodeCount());
		data.put("search_count", RunStatus.search_count);
		data.put("list_related_count", RunStatus.list_related_count);
		data.put("relation_add_count", RunStatus.relation_add_count);
		data.put("relation_remove_count", RunStatus.relation_remove_count);
		
		data.put("max_search_time", RunStatus.max_search_time);
		data.put("max_list_related_time", RunStatus.max_list_related_time);
		
		data.put("avg_search_time", RunStatus.avg_search_time);
		data.put("avg_list_related_time", RunStatus.avg_list_related_time);
		data.put("db_query_count", RunStatus.db_query_count);
		
		data.put("start_time", RunStatus.start_time.toString());
		Runtime rt = Runtime.getRuntime();
		
		data.put("java_cpu_count", rt.availableProcessors());
		data.put("java_free_memory", rt.freeMemory() / 1024.0 / 1024);
		data.put("java_max_memory", rt.maxMemory() / 1024.0 / 1024);
		data.put("java_total_memory", rt.totalMemory() / 1024.0 / 1024);
		
		data.put("java_thread_count", Thread.activeCount());
		
		data.put("session_max_count", RunStatus.session_max_count);
		data.put("session_max_query", RunStatus.session_max_query);
	}

}
