package org.socialnet.core;

import java.lang.ref.SoftReference;
import java.util.ArrayList;
import java.util.Collection;

public class DataNode {
	private int nodeId = 0;
	private int[] edges = null; //related DataNode from current Node.
	public DataNode[] visitFrom = null; //标识访问该节点的来源节点, 数组的最大值是Max Session数。
	private SoftReference<DataNode>[] refs = null;
	
	public DataNode(int nodeId, int[] edges){
		this.nodeId = nodeId;
		this.edges = edges;
	}
	
	/**
	 * 标志遍历的节点。
	 * @param session
	 * @return
	 */
	public Collection<DataNode> visitChildren(int session){
		Collection<DataNode> children = new ArrayList<DataNode>(this.edges.length);
		SoftReference<DataNode> ref = null;
		DataNode node = null;
		NodeHeap pool = NodeHeap.curInstance();
		
		if(this.refs == null)this.initRefs();
		for(int i=0; i < this.refs.length; i++){
			ref = this.refs[i];
			node = ref.get();
			if(node == null){
				node = pool.fetch(this.edges[i], false);
				this.refs[i] = new SoftReference<DataNode>(node);
			}
			if(node.visitFrom[session] != this){
				node.visitFrom[session] = this;
				children.add(node);
			}
		}
		
		return children;
	}
	
	/**
	 * 当前会话已经到达此节点。
	 * @param session
	 * @return
	 */
	public boolean sessionArrived(int session){
		if(this.refs == null)this.initRefs();
		return this.visitFrom[session] != null;
	}
	
	public DataNode arriveFrom(int session){
		return this.visitFrom[session];
	} 

	/**
	 * 清除遍历标志。
	 * @param session
	 * @return
	 */	
	public Collection<DataNode> resetVisitChildren(int session){
		Collection<DataNode> children = new ArrayList<DataNode>(this.edges.length);
		SoftReference<DataNode> ref = null;
		DataNode node = null;

		if(this.refs == null)this.initRefs();
		for(int i=0; i < this.refs.length; i++){
			ref = this.refs[i];
			node = ref.get();
			if(node == null){ continue;}
			if(node.visitFrom[session] != null){
				node.visitFrom[session] = null;
				children.add(node);
			}
		}
		
		return children;
	}	
	
	public int id(){
		return this.nodeId;
	}
	
	@SuppressWarnings("unchecked")
	private void initRefs(){
		NodeHeap pool = NodeHeap.curInstance();
		this.refs = new SoftReference[this.edges.length];
		for(int i = 0; i < this.refs.length; i++){
			this.refs[i] = new SoftReference<DataNode>(pool.fetch(this.edges[i], false));
		}
		
		visitFrom = new DataNode[pool.maxSession()];
	}
}
