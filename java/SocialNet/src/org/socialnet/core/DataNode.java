package org.socialnet.core;

import java.util.ArrayList;
import java.util.Collection;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class DataNode {
	private static Log log = LogFactory.getLog(DataNode.class);
	private int nodeId = 0;
	private Collection<Relation> edges = null; //related DataNode from current Node.
	public Relation[] visitFrom = null; //标识访问该节点的来源节点, 数组的最大值是Max Session数。
	private boolean expired = false;
	
	public DataNode(int nodeId, Collection<Relation> edges){
		this.nodeId = nodeId;
		this.edges = edges;
		for(Relation e : edges){
			e.start = this;
		}
		visitFrom = new Relation[SocialNet.curInstance().maxSession()];
	}
	
	/**
	 * 标志遍历的节点。
	 * @param session
	 * @return
	 */
	public Collection<DataNode> visitChildren(int session){
		Collection<DataNode> children = new ArrayList<DataNode>(this.edges.size());
		DataNode node = null;
		for (Relation rel: this.edges){
			node = rel.endNode();
			if(node.visitFrom[session] == null){
				node.visitFrom[session] = rel.clone();
				log.trace(String.format("[%s]%s->%s, type=%s, weight=%s", session, this.nodeId, node.nodeId, rel.relationType, rel.weight));
			}
			children.add(node);
		}	
		return children;
	}
	
	public boolean expired(){
		return this.expired;
	}
	
	public void setExpired(){
		this.expired = true;
	}
	
	/**
	 * 当前会话已经到达此节点。
	 * @param session
	 * @return
	 */
	public boolean sessionArrived(int session){
		return this.visitFrom[session] != null;
	}
	
	public Relation arriveFrom(int session){
		return this.visitFrom[session];
	}
	
	public void startedNode(int session){
		this.visitFrom[session] = new Relation(this.nodeId, 0, 0);
	}
	
	public void cleanStartedNode(int session){
		this.visitFrom[session] = null;
	}

	/**
	 * 清除遍历标志。
	 * @param session
	 * @return
	 */	
	public Collection<DataNode> resetVisitChildren(int session){
		Collection<DataNode> children = new ArrayList<DataNode>(this.edges.size());
		DataNode node = null;
		for (Relation rel: this.edges){
			node = rel.endNode();
			if(node.arriveFrom(session) != null){
				node.visitFrom[session] = null;
				children.add(node);
			}
		}
		log.trace(String.format("clean up [%s]%s, children:%s", session, this.nodeId, children.size()));
				
		return children;
	}	
	
	public int id(){
		return this.nodeId;
	}
	
	public int hashCode(){
		return this.nodeId;
	}
}
