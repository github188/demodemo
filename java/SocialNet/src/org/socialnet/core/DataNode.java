package org.socialnet.core;

import java.util.Collection;

public class DataNode {
	public int nodeId;
	public int[] edges; //related DataNode from current Node.
	public DataNode[] visitFrom = null; //标识访问该节点的来源节点, 数组的最大值是Max Session数。
	
	
	/**
	 * 标志遍历的节点。
	 * @param session
	 * @return
	 */
	public Collection<DataNode> visitChildren(int session){
		return null;
	}

	/**
	 * 清除遍历标志。
	 * @param session
	 * @return
	 */	
	public Collection<DataNode> resetVisitChildren(int session){
		return null;
	}	
}
