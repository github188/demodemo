package org.socialnet.core;

import java.lang.ref.WeakReference;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.json.simple.JSONAware;
import org.json.simple.JSONObject;

public class Relation implements Cloneable, JSONAware{
	private static Log log = LogFactory.getLog(Relation.class);
	public DataNode start;
	public int end_id;
	public float weight;	
	public String description;

	private WeakReference<DataNode> _related;
	public Relation(int end_id, float weight, String descritpion){
		this.end_id = end_id;
		this.weight = weight;
		this.description = descritpion;
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
	
	public String toJSONString(){
		StringBuffer sb = new StringBuffer();
		sb.append("[");
		sb.append(start.id());
		sb.append(",");
		sb.append(end_id);
		sb.append(",");
		sb.append("\"" + JSONObject.escape(description) + "\"");
		sb.append("]");
		return sb.toString();		
	}
}
