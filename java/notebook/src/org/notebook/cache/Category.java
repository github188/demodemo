package org.notebook.cache;

import java.io.Serializable;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Date;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;

public class Category implements TreeNode, Serializable{
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
	
	//public transient NoteMessage file = null;
	public transient DefaultTreeModel treeModel = null;
	private transient WeakReference<NoteMessage> fileRef = null;	
	public transient Category parent = null;
	public transient DataStorage loader = null;
	public transient String parentId = null;
	private transient Category root = null;
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
	

	public void setRoot(Category c) {
		this.root = c;
	}
	
	public boolean isLeaf() {
		return this.nodeType == FILE;
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
	
	public synchronized Category addCategory(Category c){
		if(this.nodeType == FILE) return null;
		this.children.add(c);
		this.setLastUpdate();
		c.parent = this;
		c.root = this.root;
		c.restore(); //恢复子节点Root.
		if(root != null && root.treeModel != null){
			root.treeModel.nodesWereInserted(this, new int[]{this.children.size() -1 });
		}
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
	
	public synchronized void remove(){
		//System.out.println("remove:" + this.name + "\tparent:" + this.parent);		
		if(this.parent != null){
			int index = this.parent.children.indexOf(this);
			this.parent.children.remove(index);
			this.parent.setLastUpdate();
			if(root != null && root.treeModel != null){
				root.treeModel.nodesWereRemoved(this.parent,
						new int[]{index},
						new Object[]{this});
			}
		}
	}
	
	public NoteMessage getMessage(){
		return getMessage(this.root.loader);
	}
	
	public NoteMessage getMessage(DataStorage pm){
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
		return Integer.parseInt(this.id);
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
	}
	
	public void flush(){
		this.flushed = true;
		if(this.children !=null){
			for(Category c : this.children){
				c.flush();
			}
		}
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
	}
	
	@Override
	public TreeNode getChildAt(int childIndex) {
		return children.get(childIndex);
	}

	@Override
	public int getChildCount() {
		return children.size();
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
