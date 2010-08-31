package org.socialnet.core;

import java.lang.ref.WeakReference;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class Relation implements Cloneable{
	private static Log log = LogFactory.getLog(Relation.class);
	public DataNode start;
	public int end_id;
	public int relationType;
	public float weight;	

	private WeakReference<DataNode> _related;
	public Relation(int end_id, int relationType, float weight){
		this.end_id = end_id;
		this.relationType = relationType;
		this.weight = weight;
	}
	
	public DataNode endNode(){
		NodeHeap pool = NodeHeap.curInstance();
		if (_related == null || _related.get() == null ||
			_related.get().expired()){
			_related = new WeakReference<DataNode>(pool.fetch(end_id, false));
		}
		return _related.get();
	}
	
	public Relation clone() {
		try{
			return (Relation)super.clone();
		}catch(CloneNotSupportedException e){
			log.error(e.toString(), e);
			return null;
		}
	}
}
