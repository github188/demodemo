package org.socialnet.core;

public class SocialNet {
	public static final int[] EMPTY = new int[0];
	
	/**
	 * 搜索节点路径，通过开始节点，和结束节点，指定搜索的深度。
	 * @param start 开始节点ID,
	 * @param end 结束节点ID,
	 * @param deep 搜索关系深度，-1，表示没有深度限制。
	 * @return 返回节点的最短路径。如果节点不存在，或再指定深度内没有找到路径，
	 * 返回空数组。
	 */
	public int[] searchPath(int start, int end, int deep){
		return EMPTY;
	}
	
	/**
	 * 清除数据节点，从新加载数据节点。因为数据关系发生了变化。SocialNet不支持数据库操作。
	 * @param start
	 * @param end
	 * @return
	 */
	public boolean createRelation(int start, int end){
		return false;
	}
	
	public boolean removeRelation(int start, int end){
		return false;
	}
	
	public boolean removeNode(int nodeId){
		return false;
	}
	
	/**
	 * 清楚所有加载资源，锁，节点缓存。
	 */
	public void reset(){
		
	}
	
	
}
