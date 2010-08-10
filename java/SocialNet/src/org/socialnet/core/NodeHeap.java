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
	private int maxSession = 0;
	//使用Weak引用，支持GC回收不再使用的节点。
	protected Map<Integer, WeakReference<DataNode>> cache = new HashMap<Integer, WeakReference<DataNode>>();
	protected DataSource ds = null;
	
	public static NodeHeap curInstance(){
		return ins;		
	}
	
	public NodeHeap(DataSource ds){
		
	}
	
	/**
	 * 查找节点，并且判断在数据库中是否存在。
	 * @param nodeId
	 * @param exists
	 * @return
	 */
	public synchronized DataNode fetch(int nodeId, boolean exists){
		WeakReference<DataNode> ref = this.cache.get(nodeId);
		if(ref != null && ref.get() != null){
			return ref.get();	//缓存命中。
		}
		
		if(exists && !this.ds.isExist(nodeId)){
			return null;
		}
		
		int[] edges = this.ds.loadEdges(nodeId);
		DataNode node = new DataNode(nodeId, edges);
		this.cache.put(nodeId, new WeakReference<DataNode>(node));
		
		return node;
	}

	/**
	 * 把SocialNet中的maxSession保存到这儿，方便在DataNode中使用。纯属无奈，为了避免DataNode依赖SocialNet.
	 */	
	public void setMaxSession(int i){
		this.maxSession = i;
	}
	
	public int maxSession(){
		return this.maxSession;
	}
}


	
	