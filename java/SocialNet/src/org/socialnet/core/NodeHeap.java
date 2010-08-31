package org.socialnet.core;

import java.lang.ref.WeakReference;
import java.util.HashMap;
import java.util.Map;

/**
 * 数据节点管理，支持动态加载节点关系，使用内存管理，过期节点内存释放, 最近访问缓存
 * 。
 * @author Lenovo
 *
 */

public class NodeHeap {
	private static NodeHeap ins = null;
	//使用Weak引用，支持GC回收不再使用的节点。
	protected Map<Integer, WeakReference<DataNode>> cache = new HashMap<Integer, WeakReference<DataNode>>();
	protected DataSource ds = null;
	
	public static NodeHeap curInstance(){
		return ins;		
	}
	
	public NodeHeap(DataSource ds){
		this.ds = ds;
		ins = this;
	}
	
	/**
	 * 查找节点，并且判断在数据库中是否存在。
	 * @param nodeId
	 * @param exists
	 * @return
	 */
	public synchronized DataNode fetch(int nodeId, boolean exists){
		DataNode node = null;
		WeakReference<DataNode> ref = this.cache.get(nodeId);
		if(ref != null && ref.get() != null){
			node = ref.get();	//缓存命中。
			if(!node.expired()){ //未过期。
				return node;
			}
		}
		
		if(exists && !this.ds.isExist(nodeId)){
			return null;
		}
		
		node = new DataNode(nodeId, this.ds.loadEdges(nodeId));
		this.cache.put(nodeId, new WeakReference<DataNode>(node));
		
		return node;
	}
	
	public synchronized DataNode expireNode(int nodeId){
		DataNode node = null;
		WeakReference<DataNode> ref = this.cache.get(nodeId);
		if(ref != null && ref.get() != null){
			node = ref.get();	//缓存命中。
			node.setExpired();
		}
		return node;
	}
	
}


	
	