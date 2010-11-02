package org.notebook.cache;

import java.io.Serializable;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Date;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class Category implements TreeNode, Serializable, Cloneable{
	protected static transient Log log = LogFactory.getLog("Category");
	private static final long serialVersionUID = 8989783162442987982L;
	public final static int DIRECTORY = 1;
	public final static int FILE = 2;
	public String id;
	public int nodeType = DIRECTORY;
	public String name;
	public List<Category> children = null;
	public Date createDate = new Date();
	public Date lastUpdated = new Date();
	public boolean flushed = true;
	public boolean isDirty = false;
	public boolean isExpired = false;
	public int position = 0;
	public String orderBy = "position";
	
	//public transient NoteMessage file = null;
	public transient DefaultTreeModel treeModel = null;
	private transient WeakReference<NoteMessage> fileRef = null;
	public transient Category parent = null;
	public transient DataStorage loader = null;
	public transient String parentId = null;
	private transient Category root = null;
	private transient Category conflict = null;
	private transient Category removed = null;
	//private transient Comparator<Category> = null;
	private int nextID = 0;
	public Category(){
		root = this;
		this.name = "root";
		children = new ArrayList<Category>();
		this.id = this.getNextId();
	}
	
	private Category(String name, int type){
		//this()
		this.name = name;
		this.nodeType = type;
		if(type != FILE){
			children = new ArrayList<Category>(5);
		}
		this.setLastUpdate();
		//this.id = this.getNextId();
	}
	
	public String getName(){
		if(this.name == null || this.name.trim().equals("")){
			this.name = "unkown";
		}
		return this.name.trim();
	}

	public void setRoot(Category c) {
		this.root = c;
	}
	
	public boolean isLeaf() {
		return this.nodeType == FILE;
	}
	
	public Category getConflict(){
		Category node = null;
		if(this.root != null && this.root.conflict != null){
			return this.root.conflict;
		}else if(this.root != null){
			node = this.root.search("conflict");
			if(node == null){
				node = this.root.addCategory("同步冲突");
				node.id = "conflict";
				node.position = 1000 - 1;
			}
		}else {
			node = this.search("conflict");
			if(node == null){
				node = this.addCategory("同步冲突");
				node.id = "conflict";
				node.position = 1000 - 1;
			}
		}
		return node;
	}
	
	public Category getRemoved(){
		Category node = null;
		if(this.root != null && this.root.removed != null){
			return this.root.removed;
		}else if(this.root != null){
			node = this.root.search("removed");
			if(node == null){
				node = this.root.addCategory("回收站");
				node.id = "removed";
				node.position = 1000;
			}
		}else {
			node = this.search("removed");
			if(node == null){
				node = this.addCategory("回收站");
				node.id = "removed";
				node.position = 1000;
			}
		}
		return node;
	}	
	
	public Category addCategory(String name){
		if(this.nodeType == FILE) return null;
		//return createNew(name, DIRECTORY);
		return addCategory(createNew(name, DIRECTORY));
	}
	
	public Category addMessage(String name){
		if(this.nodeType == FILE) return null;
		Category c = createNew(name, FILE); //new Category(name, FILE);
		//c.file = new NoteMessage(c.id);
		addCategory(c);
		return c;
	}
	
	public Category addCategory(Category c){
		return addCategory(c, true);
	}
	
	public synchronized Category addCategory(Category c, boolean updateDateTime){
		if(this.nodeType == FILE) return null;
		if(this.children == null) this.children = new ArrayList<Category>(); 
		if(this.children.contains(c)) return null;
		this.children.add(c);
		if(updateDateTime) this.setLastUpdate();
		c.parent = this;
		c.root = this.root;
		c.position = this.children.size();
		c.restore(); //恢复子节点Root. //排序
		this.sort();		
		return c;
	}	
	
	private Category createNew(String name, int type){
		Category c = new Category(name, type);
		c.id = this.getNextId();
		return c;		
	}
	
	public Category search(String id){
		LinkedList<Category> queue = new LinkedList<Category>();
		queue.add(this);
		Category result = null;
		while(queue.size() > 0){
			result = queue.poll();
			if(result.id != null && result.id.equals(id)){
				break;
			}
			if(result.children != null){
				queue.addAll(result.children);
			}
			result = null;
		}
		queue = null;
		
		return result;
	}
	
	public void remove(){
		this.remove(true);
	}
	
	/**
	 * @param recycle -- 是否需要加入回收站。在做节点移动时，会先在原先的节点删除，在加入到新的
	 * 父节点。这种情况的删除操作不能放入回收站。
	 */
	public synchronized void remove(boolean recycle){
		//System.out.println("remove:" + this.name + "\tparent:" + this.parent);
		boolean inRemoved = false;
		if(this.parent != null){
			//如果不在回收站,先加入回收站.
			inRemoved = this.getRemoved().search(this.id) != null;
			int index = this.parent.children.indexOf(this);
			this.parent.children.remove(index);
			this.parent.setLastUpdate();
			this.parent.sort();
			if(!inRemoved && recycle){
				this.getRemoved().addCategory(this);
			}
		}
	}
	
	public NoteMessage getMessage(){
		return getMessage(this.root.loader, false);
	}
	
	public NoteMessage getMessage(boolean create){
		return getMessage(this.root.loader, create);
	}
	
	public NoteMessage getMessage(DataStorage pm, boolean create){
		NoteMessage msg = null;
		if(this.nodeType != FILE) return null;
		if(this.fileRef == null || this.fileRef.get() == null) {
			if(pm != null){
				msg = pm.load(this.id);
				if(msg == null){
					msg = new NoteMessage(this.id);
				}
				this.fileRef = new WeakReference<NoteMessage>(msg);
			}
		}else {
			msg = this.fileRef.get();
		}
		if(msg == null && create){
			msg = new NoteMessage(this.id);
			this.fileRef = new WeakReference<NoteMessage>(msg);
		}
		if(msg != null){
			msg.setCategory(this);
		}
		return msg;
	}
	
	public String toString(){
		return name;
	}
	
	public boolean equals(Object obj){
		if(obj instanceof Category){
			Category other = (Category)obj;
			if(this.id != null && other.id != null &&
			   this.id.equals(other.id)){
				return true;
			}
		}
		return false;
	}
	
	public int hashCode(){
		return (id != null) ? id.hashCode() : 0;
	}
	
	public Category copy(){
		Category c = null;
		try {
			c = (Category)this.clone();
		} catch (CloneNotSupportedException e) {
			c = new Category("clone failed.", FILE);
		}
		return c;
	}
	
	private Object[] getPath(Category c){
		ArrayList<Category> path = new ArrayList<Category>(10);
		path.add(0, c);
		while(c.parent != null){
			path.add(0, c.parent);
			c = c.parent;
		}
		path.trimToSize();
		return path.toArray();
	}
	
	//重置节点关系。当节点被序列化后，Root/Parent被丢失。
	public void restore(){
		this.flushed = true;
		if(this.children !=null){
			for(Category c : this.children){
				c.parent = this;
				c.root = this.root;
				c.restore();
			}
		}
		this.sort();		
	}
	
	private void sort(){
		if(this.isLeaf()) return;
		if(this.children == null) return;
		//log.debug(this.name + "(" + this.id + ") sort by " + this.getOrderBy());
		this.children = Comparators.sort(this.children, this.getOrderBy());
		if(this.getOrderBy().endsWith("position")){
			for(int i = 0; i < this.children.size(); i++){
				this.children.get(i).position = i;
			}
		}
		if(root != null && root.treeModel != null){
			root.treeModel.nodeStructureChanged(this);
			log.debug("root.treeModel.nodeStructureChanged(this);" + this.name + "," + this.id);
		}
	}
	
	public boolean orderBy(String order){
		if(order != null && !this.isLeaf() 
		  && !this.getOrderBy().equals(order)){
			this.orderBy = order;
			this.sort();
			this.setLastUpdate();
			return true;
		}
		return false;
	}
	
	public String getOrderBy(){
		if(this.orderBy == null){
			this.orderBy = "position";
		}
		return this.orderBy;
	}

	public boolean moveOrder(int dir){
		if(this.parent == null)return false;
		if(!parent.getOrderBy().endsWith("position")){
			return false;
		}
		Category other = null;
		int o1 = parent.getIndex(this);
		int p1 = this.position;
		if(dir < 0 && o1 > 0){
			other = (Category)parent.getChildAt(o1 -1);
			if(this.position != other.position){
				this.position = other.position;
				other.position = p1;
			}else {
				this.position += dir;
			}
			this.setLastUpdate();
			other.setLastUpdate();			
		}else if(dir > 0 && o1 < parent.getChildCount() -1){
			other = (Category)parent.getChildAt(o1 +1);
			if(this.position != other.position){
				this.position = other.position;
				other.position = p1;
			}else {
				this.position += dir;
			}
			this.setLastUpdate();
			other.setLastUpdate();
		}
		//log.debug("moveOrder:" + dir + " new position:" + this.position);
		//this.sort();
		this.parent.sort();
		return true;
	}	
	
	public void flush(){
		this.flushed = true;
		if(this.children !=null){
			for(Category c : this.children){
				c.flush();
			}
		}
		if(this.isLeaf()){
			if(this.fileRef != null && this.fileRef.get() != null) {
				NoteMessage msg = this.fileRef.get();
				if(msg != null && msg.isDirty){
					this.root.loader.save(msg);
					msg.isDirty = false;
				}
			}
		}
	}
	
	public List<Category> getChildren(){
		if(this.isLeaf()) return null;
		if(this.children == null){
			this.children = new ArrayList<Category>();
		}
		return this.children;
	}
	
	private String getNextId(){
		this.root.nextID++;
		return String.format("%07d", this.root.nextID);
	}
	
	public int curId(int id){
		if(id > 0){
			this.root.nextID = id;
		}
		return this.root.nextID;
	}
	
	public void setName(String name){
		this.name = name;
		this.setLastUpdate();
	}
	
	public void setLastUpdate(){
		this.flushed = false;
		this.isDirty = true;
		this.lastUpdated = new Date(System.currentTimeMillis());
		if(parent != null){
			parent.setLastUpdate();
		}
		if(root != null && root.treeModel != null){
			root.treeModel.nodeChanged(this);
		}
	}
	
	public void initDefaultNode(){
		this.addCategory("服务器管理");
		this.addCategory("数据库");
		this.addMessage("今天的工作.");
		this.getConflict();
		this.getRemoved();
	}
	
	@Override
	public TreeNode getChildAt(int childIndex) {
		return children.get(childIndex);
	}

	@Override
	public int getChildCount() {
		return children != null ? children.size() : 0;
	}

	@Override
	public TreeNode getParent() {
		if(this.parent == null && this.parentId != null){
			return this.root.search(this.parentId);
		}
		return this.parent;
	}

	@Override
	public int getIndex(TreeNode node) {
		return children.indexOf(node);
	}

	@Override
	public boolean getAllowsChildren() {
		return !this.isLeaf();
	}

	@Override
	public Enumeration<Category> children() {
		final Iterator<Category> iter = children.iterator();
		return new Enumeration<Category>(){
			public boolean hasMoreElements() {
				return iter.hasNext();
			}
			public Category nextElement() {
				return iter.next();
			}
		};
	}
}
