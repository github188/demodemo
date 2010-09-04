package org.socialnet.core;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class SocialNet {
	private static Log log = LogFactory.getLog(SocialNet.class);
	public static final List<Relation> EMPTY = new ArrayList<Relation>(0);
	protected SessionManager sm = null;
	protected NodeHeap pool = null;
	private static SocialNet ins = null;
	
	public SocialNet(SessionManager sm, NodeHeap pool){
		this.sm = sm;
		this.pool = pool;
		ins = this;
	} 
	
	public static SocialNet curInstance(){
		return ins;		
	}	
	
	/**
	 * 搜索节点路径，通过开始节点，和结束节点，指定搜索的深度。
	 * @param start 开始节点ID,
	 * @param end 结束节点ID,
	 * @param deep 搜索关系深度，-1，表示没有深度限制。
	 * @return 返回节点的最短路径。如果节点不存在，或再指定深度内没有找到路径，
	 * 返回空数组。
	 */
	public List<Relation> searchPath(int start, int end, int deep) throws SearchException{
		DataNode srcNode = null, desNode = null;
		List<Relation> result = EMPTY;
		int session = -1;
		log.info(String.format("searchPath, start:%s, end:%s, deep:%s", start, end, deep));
		srcNode = pool.fetch(start, true);
		desNode = pool.fetch(end, true);
		if(srcNode == null || desNode == null){
			return EMPTY;
		}
		long startTime = System.currentTimeMillis(); 
		
		session = sm.newSession(false);
		if (session < 0) throw new SearchException(SearchException.NO_SESSION);
		
		try{
			Collection<DataNode> tmp = new ArrayList<DataNode>(1);
			tmp.add(srcNode);
			
			//如果deep为-1,没有搜索深度限制。
			deep = deep == -1 ? Integer.MAX_VALUE : deep;
			srcNode.startedNode(session); //加一个标志，为开始节点。
			
			desNode = this.searchNode(session, desNode, tmp.iterator(), null, deep);
			if (desNode != null){
				result = this.retrievePath(session, desNode);
			}
			this.cleanSearch(session, srcNode);
			
			srcNode.cleanStartedNode(session); //清除开始节点标志。
		}finally{
			sm.releaseSession(session);
		}
		
		RunStatus.search_count++;
		startTime = System.currentTimeMillis() - startTime;
		if(startTime > RunStatus.max_search_time){
			RunStatus.max_search_time = startTime;
		}
		RunStatus.avg_search_time = (startTime + RunStatus.avg_search_time) / 2;
		
		return result;
	}
	
	/**
	 * 清除数据节点，从新加载数据节点。因为数据关系发生了变化。SocialNet不支持数据库操作。
	 * @param start
	 * @param end
	 * @return
	 */
	public boolean createRelation(int start, int end){
		pool.expireNode(start);
		RunStatus.relation_add_count++;
		return false;
	}
	
	public boolean removeRelation(int start, int end){
		pool.expireNode(start);
		RunStatus.relation_remove_count++;
		return false;
	}
	
	public boolean removeNode(int nodeId){
		pool.expireNode(nodeId);
		return false;
	}
	
	public int maxSession(){
		return sm.maxSession();
	}	
	
	/**
	 * 清楚所有加载资源，锁，节点缓存。
	 */
	public void reset(){
		
	}
	
	public Collection<NodePath> listRelatedNode(int start, int minDeep, 
			int maxDeep, int index, int maxCount) throws SearchException{
		log.info(String.format("listRelatedNode, start:%s, min:%s, max:%s, index:%s, count:%s", 
				start, minDeep, maxDeep, index, maxCount));
		final DataNode srcNode = pool.fetch(start, true);		
		int session = -1;
		if(srcNode == null){
			return new ArrayList<NodePath>(0);
		}
		
		long startTime = System.currentTimeMillis(); 
		Collection<NodePath> result = null;
		
		session = sm.newSession(false);
		if (session < 0) throw new SearchException(SearchException.NO_SESSION);
		
		try{
			srcNode.startedNode(session); //加一个标志，为开始节点。
			Collection<DataNode> nodeList = this.listRelatedNodeInSession(session, srcNode, minDeep, maxDeep, index, maxCount);
			
			result = new ArrayList<NodePath>(nodeList.size());
			for(DataNode node: nodeList){
				result.add(new NodePath(node, this.retrievePath(session, node)));
			}
			this.cleanSearch(session, srcNode);
			
			srcNode.cleanStartedNode(session); //清除开始节点标志。
		}finally{
			sm.releaseSession(session);
		}
		
		RunStatus.list_related_count++;
		startTime = System.currentTimeMillis() - startTime;
		if(startTime > RunStatus.max_list_related_time){
			RunStatus.max_list_related_time = startTime;
		}
		RunStatus.avg_list_related_time = (startTime + RunStatus.avg_list_related_time) / 2;		
		
		return result;
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
			//System.out.println("Search:" + node.id() + " deep:" + deep);
			nextList.addAll(node.visitChildren(session));
			
			//System.out.println("Search2:" + node.id());
			//visiChildren把所有相关的节点标志为已访问。
			if(desNode.sessionArrived(session)){
				return desNode;
			}
		}
		
		return searchNode(session, desNode, nextList.iterator(), null, deep-1);
	}
	
	private Collection<DataNode> listRelatedNodeInSession(int session, DataNode srcNode, 
			int minDeep, int maxDeep, int index, int maxCount){
		int deep = 0;
		
		Collection<DataNode> tmp = new ArrayList<DataNode>(1);
		tmp.add(srcNode);		
		Iterator<DataNode> curList = tmp.iterator();

		Collection<DataNode> result = new ArrayList<DataNode>(maxCount);
		Collection<DataNode> nextList = null;
		//index
				
		DataNode node = null;		
		while(deep < maxDeep && curList.hasNext()){
			nextList = new ArrayList<DataNode>(maxCount);
			for(;curList.hasNext(); ){
				node = curList.next();
				nextList.addAll(node.visitChildren(session));
				if(deep > minDeep){
					index--;
					if(index < 0) result.add(node);
					if(result.size() > maxCount)return result;
				}
			}
			deep++;
			curList = nextList.iterator();
		}
		
		return result;
	}

	
	
	private List<Relation> retrievePath(int session, DataNode node){
		ArrayList<Relation> path = new ArrayList<Relation>(10);		
		Relation rel = node.arriveFrom(session);		
		while(rel != null && rel.start != null){
			path.add(0, rel);
			rel = rel.start.arriveFrom(session);
		}
		path.trimToSize();
		return path;
	}
}
