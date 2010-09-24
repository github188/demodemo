package org.notebook.cache;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;
import java.util.Iterator;
import java.util.List;

import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;

public class Category implements TreeModel, Serializable{
	private static final long serialVersionUID = 8989783162442987982L;
	public final static int DIRECTORY = 1;
	public final static int FILE = 2;
	public String id;
	public int nodeType = DIRECTORY;
	public String name;
	public List<Category> children = null;
	public Date createDate;
	public Date lastUpdated;
	
	public transient NoteMessage file = null;
	public transient Collection<TreeModelListener> ls = new ArrayList<TreeModelListener>();	
	public transient Category parent = null;
	private transient Category root = null;
	private transient EventProxy evtProxy = null;
	private int nextID = 0;
	public Category(){
		root = this;
		evtProxy = new EventProxy();
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
		//this.id = this.getNextId();
	}
	
	@Override
	public void addTreeModelListener(TreeModelListener listener) {
		System.out.println("add Listener:" + listener.toString());
		ls.add(listener);		
	}
	@Override
	public Category getChild(Object arg0, int index) {
		Category c = (Category)arg0;
		return c.children.get(index);
	}
	@Override
	public int getChildCount(Object arg0) {
		Category c = (Category)arg0;
		return c.children.size();
	}
	@Override
	public int getIndexOfChild(Object arg0, Object arg1) {
		Category c = (Category)arg0;
		Category c1 = (Category)arg1;
		for(int i = 0; i < c.children.size(); i++ ){
			if(c1.equals(c.children.get(i))){
				return i;
			}
		}
		return 0;
	}
	@Override
	public Object getRoot() {
		return this.root;
	}
	@Override
	public boolean isLeaf(Object arg0) {
		Category c = (Category)arg0;
		return c.nodeType == FILE;
	}
	@Override
	public void removeTreeModelListener(TreeModelListener arg0) {
		//arg0.treeNodesChanged(e)
		this.ls.remove(arg0);
	}
	
	@Override
	public void valueForPathChanged(TreePath path, Object node) {
		Object x = path.getLastPathComponent();
		Category cate = (Category)x;
		cate.setName(node.toString());
	}	
	
	public Category addCategory(String name){
		if(this.nodeType == FILE) return null;
		return createNew(name, DIRECTORY);
	}
	
	public Category addMessage(String name){
		if(this.nodeType == FILE) return null;
		Category c = createNew(name, FILE); //new Category(name, FILE);
		c.file = new NoteMessage(c.id);
		return c;
	}
	
	public Category addCategory(Category c){
		if(this.nodeType == FILE) return null;
		this.children.add(c);
		c.parent = this;
		c.root = this.root;
		c.restore(); //恢复子节点Root.
		TreeModelEvent evt = new TreeModelEvent(this, this.getPath(this),
				new int[]{this.children.size() -1},
				new Object[]{c, });
		getEventProxy().treeNodesInserted(evt);
		return c;
	}	
	
	private Category createNew(String name, int type){
		Category c = new Category(name, type);
		this.children.add(c);
		c.root = this.root;
		c.parent = this;
		c.id = this.getNextId();
				
		TreeModelEvent evt = new TreeModelEvent(this, this.getPath(this),
							new int[]{this.children.size() -1},
							new Object[]{c, });
		getEventProxy().treeNodesInserted(evt);
		//this.root.evtProxy.treeStructureChanged(evt);
		return c;		
	}
	
	public void remove(){
		System.out.println("remove:" + this.name + "\tparent:" + this.parent);		
		if(this.parent != null){
			int index = this.parent.children.indexOf(this);
			this.parent.children.remove(index);
			TreeModelEvent evt = new TreeModelEvent(this.parent, this.getPath(this.parent),
					new int[]{index, },
					new Object[]{this, });
			getEventProxy().treeNodesRemoved(evt);
		}
	}
	
	public NoteMessage getMessage(){
		if(this.nodeType != FILE) return null;
		if(this.file == null){
			this.file = SimpleObjectCache.getInstance().load(this.id);
		}
		return this.file;
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
	private void restore(){
		if(this.children !=null){
			for(Category c : this.children){
				c.parent = this;
				c.root = this.root;
				c.restore();
			}
		}
	}
	
	private String getNextId(){
		this.root.nextID++;
		return String.format("%07d", this.root.nextID);
	}
	
	public void setName(String name){
		this.name = name;
	}
	
	public void initDefaultNode(){
		this.addCategory("服务器管理");
		this.addCategory("数据库");
		this.addMessage("今天的工作.");		
	}
	
	private EventProxy getEventProxy(){
		if(this.root.evtProxy == null){
			this.root.evtProxy = new EventProxy();
		}
		return this.root.evtProxy;
	}
	
	class EventProxy implements TreeModelListener{

		@Override
		public void treeNodesChanged(TreeModelEvent arg0) {
			for(Iterator<TreeModelListener> iter = ls.iterator(); iter.hasNext();){
				iter.next().treeNodesChanged(arg0);
			}			
		}

		@Override
		public void treeNodesInserted(TreeModelEvent arg0) {
			for(Iterator<TreeModelListener> iter = ls.iterator(); iter.hasNext();){
				iter.next().treeNodesInserted(arg0);
			}
		}

		@Override
		public void treeNodesRemoved(TreeModelEvent arg0) {
			for(Iterator<TreeModelListener> iter = ls.iterator(); iter.hasNext();){
				iter.next().treeNodesRemoved(arg0);
			}
		}

		@Override
		public void treeStructureChanged(TreeModelEvent arg0) {
			for(Iterator<TreeModelListener> iter = ls.iterator(); iter.hasNext();){
				iter.next().treeStructureChanged(arg0);
			}			
		}
		
	}
	
}
