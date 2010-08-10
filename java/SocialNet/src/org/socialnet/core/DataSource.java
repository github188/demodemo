package org.socialnet.core;

/**
 * 数据库访问接口。
 * @author Lenovo
 *
 */
public interface DataSource {
	public int[] loadEdges(int nodeId);
	public boolean saveEdges(int start, int end);
	public boolean removeEdges(int start, int end);
	public boolean isExist(int nodeId);
}
