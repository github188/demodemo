package org.socialnet.core;

import java.util.Date;

public class RunStatus {
	public static Date start_time = new Date();
	public static int search_count = 0; // 路经查询次数.
	public static int list_related_count = 0; // 按层查询次数.
	//public static int node_count = 0; // 当前缓存的节点数.
	public static int relation_add_count = 0; // 
	public static int relation_remove_count = 0; // 
	
	public static long max_search_time = 0; // 最长查询耗时.
	public static long max_list_related_time = 0; // 最长查询耗时.
	public static long avg_search_time = 0; // 平均查询耗时.
	public static long avg_list_related_time = 0; // 平均查询耗时.
	
	public static int db_query_count = 0; // 数据库查询次数.
	
	public static int session_max_count = 0; // 数据库查询次数.
	public static int session_max_query = 0; // 数据库查询次数.
}
