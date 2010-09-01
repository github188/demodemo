package org.socialnet.core;

import java.util.Collection;

public class NodePath {
	public DataNode node = null;
	public Collection<Relation> path = null;
	public NodePath(DataNode node, Collection<Relation> path){
		this.node = node;
		this.path = path;
	}
}
