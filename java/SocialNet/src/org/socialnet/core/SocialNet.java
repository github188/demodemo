package org.socialnet.core;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class SocialNet {
	public static final int[] EMPTY = new int[0];
	protected SessionManager sm = null;
	protected NodeHeap pool = null; 
	
	/**
	 * 搜索节点路径，通过开始节点，和结束节点，指定搜索的深度。
	 * @param start 开始节点ID,
	 * @param end 结束节点ID,
	 * @param deep 搜索关系深度，-1，表示没有深度限制。
	 * @return 返回节点的最短路径。如果节点不存在，或再指定深度内没有找到路径，
	 * 返回空数组。
	 */
	public int[] searchPath(int start, int end, int deep) throws SearchException{
		DataNode srcNode = null, desNode = null;
		int[] result = EMPTY;
		int session = -1;
		    
		srcNode = pool.fetch(start, true);
		desNode = pool.fetch(end, true);
		if(srcNode == null || desNode == null){
			return EMPTY;
		}
		
		session = sm.newSession(false);
		if (session < 0) throw new SearchException(SearchException.NO_SESSION);
		
		try{
			Collection<DataNode> tmp = new ArrayList<DataNode>(1);
			tmp.add(srcNode);
			
			//如果deep为-1,没有搜索深度限制。
			deep = deep == -1 ? Integer.MAX_VALUE : deep;
			desNode = this.searchNode(session, desNode, tmp.iterator(), null, deep);
			if (desNode != null){
				result = this.retrievePath(session, desNode);
			}
			this.cleanSearch(session, srcNode);		
		}finally{
			sm.releaseSession(session);
		}
		
		return result;
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
	
	/**
	 * 清除搜索时的标识位。
	 * @param start
	 */
	
	private void cleanSearch(int session, DataNode start){
		LinkedList<DataNode> cleanList = new LinkedList<DataNode>();
		while(start != null){
			cleanList.addAll(start.resetVisitChildren(session));
			start = cleanList.poll();
		}		
	}
	
	/**
	 * 按层次搜索目标节点，返回查找到得目标节点。
	 * @param session 当前查询的Session Id.
	 * @param desNode 需要查询的目标节点。
	 * @param curList 当前层次的所有节点。
	 * @param nextList 保存下一个搜索层次的节点。
	 * @param deep 有效的搜索深度。
	 * @return 返回查询到得节点。
	 */
	private DataNode searchNode(int session, DataNode desNode, Iterator<DataNode> curList, 
								Collection<DataNode> nextList, int deep){
		if(deep < 0) return null;
		if(nextList == null) nextList = new ArrayList<DataNode>(100);
		
		DataNode node = null;
		
		for(;curList.hasNext(); ){
			node = curList.next();
			nextList.addAll(node.visitChildren(session));
			
			//visiChildren把所有相关的节点标志为已访问。
			if(desNode.sessionArrived(session)){
				return desNode;
			}
		}
		
		return searchNode(session, desNode, nextList.iterator(), null, deep-1);
	}
	
	private int[] retrievePath(int session, DataNode node){
		return EMPTY;
	} 
}
