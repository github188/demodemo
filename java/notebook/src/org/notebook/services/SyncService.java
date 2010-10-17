package org.notebook.services;

import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;
import java.util.LinkedList;
import java.util.concurrent.ThreadPoolExecutor;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.Category;
import org.notebook.cache.NoteBook;
import org.notebook.cache.NoteMessage;
import org.notebook.io.AuthcationException;
import org.notebook.io.ClientException;
import org.notebook.io.NoteBookClient;

public class SyncService {
	private static Log log = LogFactory.getLog(SyncService.class);
	public NoteBook book = null;
	public NoteBookClient client = null;
	public Category root = null;
	public Collection<SyncListener> ls = new ArrayList<SyncListener>();
	
	public LinkedList<SyncTask> taskQueue = new LinkedList<SyncTask>();
	public LinkedList<SyncTask> doneTask = new LinkedList<SyncTask>();	
	private boolean running = false;
	private SyncRunner runner = null;
	
	public void start(NoteBook book, ThreadPoolExecutor pool){
		if(this.runner != null){
			this.runner.close();
		}
		running = true;	
		this.book = book;
		this.root = book.root;
		//this.threadPool = pool;
		if(this.book != null && book.proxy != null && !book.proxy.trim().equals("")){
			log.info("http proxy:" + book.proxy);
			System.setProperty("HTTP_PROXY", book.proxy);
			try {
				URL proxy = new URL(book.proxy);
				System.setProperty("http.proxyHost", proxy.getHost());
				System.setProperty("http.proxyPort", proxy.getPort() + "");
			} catch (MalformedURLException e) {
				log.error(e.toString(), e);
			}
		}
		runner = new SyncRunner();
		pool.execute(runner);
	}
	
	public void stop(){
		running = false;
		if(this.runner != null){
			this.runner.close();
			this.runner = null;
		}
	}
	
	public boolean isRunning(){
		return running;
	}
	
	public void download(final Category cate, boolean force){
		SyncTask task = new SyncTask(SyncTask.TASK_DOWN, cate.copy());
		task.force = force;
		scheduleTask(task);
	}
	
	public void upload(final Category cate, boolean force){
		SyncTask task = new SyncTask(SyncTask.TASK_UP, cate.copy());
		task.force = force;
		scheduleTask(task);
	}
	
	public void sync(final Category cate, boolean force){
		SyncTask task = new SyncTask(SyncTask.TASK_DOWN, cate.copy());
		task.force = false;
		scheduleTask(task);
		task = new SyncTask(SyncTask.TASK_UP, cate);
		task.force = false;
		scheduleTask(task);
	}
	
	public void downloadRemote(final Category cate, boolean force){
		SyncTask task = new SyncTask(SyncTask.TASK_DOWN, null);
		task.force = force;
		task.remote = cate.copy();
		scheduleTask(task);
	}	
	
	public void scheduleTask(SyncTask task){
		synchronized(this.taskQueue){
			this.taskQueue.add(task);
			this.taskQueue.notify();
		}
	}
	
	protected void processDownLoad(SyncTask task){
		Category parent = null, local=null, remote=null;
		if(task.local != null && !task.local.flushed){
			task.status = SyncTask.IGNORED;
			return;
		}
		try {
			EventProxy.start(task);
			if(task.local != null){
				remote = client.getCategory(task.local.id);
				if(remote != null){
					task.remote = remote.copy();
				}
				local = this.root.search(task.local.id);
				if(local == null){
					task.exception = new Exception("Not found local data with id " +
							task.local.id ); 
					task.status = SyncTask.IGNORED;
					return;
				}
			}else {
				parent = this.root.search(task.remote.parentId);
				if(parent == null || parent.isLeaf()){
					parent = this.root.getConflict();
				}
				local = this.root.search(task.remote.id);
				if(local == null){
					local = task.remote;
					task.newCreated = true;
					task.status = SyncTask.NEW_CREATED;
				}else {
					task.local = local.copy();
				}
				
				parent.addCategory(local, false);
			}
			if(remote == null || local == null){
				task.exception = new Exception("Local or remote not found for download"); 
				task.status = SyncTask.IGNORED;
				return;
			}
			if(!task.newCreated){
				if(task.force){
					_upldateLocalNode(local, remote);
					task.status = SyncTask.DOWN_LOAD;
				}else {
					int result = cmpDate(task.remote.lastUpdated, task.local.lastUpdated);
					if(result > 0){
						_upldateLocalNode(local, remote);
						task.status = SyncTask.DOWN_LOAD;
					}else {
						task.status = SyncTask.NO_UPDATE;
						
					}
				}
			}
			if(!task.status.equals(SyncTask.NO_UPDATE) && !remote.isLeaf()){
				for(Category c: remote.children){
					SyncTask newTask = new SyncTask(SyncTask.TASK_DOWN, null);
					newTask.remote = c;
					newTask.force = task.force;
					taskQueue.add(newTask);
				}
			}
		}catch(Exception e){
			EventProxy.syncError(task, e);
			log.error(e, e);
		}finally{
			task.doneDate = new Date(System.currentTimeMillis());
			EventProxy.done(task);
		}
	}
	
	private void _upldateLocalNode(Category local, Category remote) throws IOException, ClientException{				
		if(local.isLeaf() != remote.isLeaf()){
			if(remote.isLeaf()){
				NoteMessage note = client.downLoadMessage(remote.id);
				Category c = local.getConflict().addMessage(remote.name);
				c.getMessage(true).update(note);
				c.lastUpdated = remote.lastUpdated;
			}else {
				Category c = local.getConflict().addCategory(remote.name);
				c.lastUpdated = remote.lastUpdated;
			}
		}else {
			local.setName(remote.name);
			local.lastUpdated = remote.lastUpdated;		
			if(local.isLeaf()){
				NoteMessage note = client.downLoadMessage(remote.id);
				local.getMessage(true).update(note); // .setText(note.text);
			}else {
				//Collection<Category> children = client.listCategory(remote);
				if(remote.children == null){
					remote.children = client.listCategory(remote);
				}
				this.removeLocalChildren(remote.children, local);
			}
		}
	}
		
	
	protected void processUpLoad(SyncTask task){
		Category local=null;
		if(!task.local.flushed){
			task.status = SyncTask.IGNORED;
			this.doneTask.add(task);
			return;
		}
		local = this.root.search(task.local.id);
		try{
			if(local == null){
				task.exception = new Exception("Not found local data with id " +
						task.local.id ); 
				task.status = SyncTask.IGNORED;
				return;
			}
			EventProxy.start(task);
			task.remote = client.getCategory(local.id);
			if(task.force){
				_uploadLocalNode(local);
				task.status = SyncTask.UP_LOAD;
			}else {
				//使用计划时间的比较,保证更新结果和界面显示一致.
				int result = cmpDate(task.local.lastUpdated, task.remote.lastUpdated);
				if(result > 0 || local.isDirty){
					_uploadLocalNode(local);
					task.status = SyncTask.UP_LOAD;
				}else {
					task.status = SyncTask.NO_UPDATE;
				}
			}
			if(!task.status.equals(SyncTask.NO_UPDATE) && !local.isLeaf()){
				for(Category c: local.children){
					SyncTask newTask = new SyncTask(SyncTask.TASK_UP, c.copy());
					newTask.force = task.force;
					taskQueue.add(newTask);		
				}
			}
			local.isDirty = false;
		}catch(Exception e){
			EventProxy.syncError(task, e);
			log.error(e, e);
		}finally{
			task.doneDate = new Date(System.currentTimeMillis());
			this.doneTask.add(task);
			EventProxy.done(task);
		}
	}
	
	private void _uploadLocalNode(Category local) throws Exception{		
		client.updateCategory(local);
		if(local.isLeaf()){
			client.uploadMessage(local.getMessage(true));
		}else {
			for(Category c: client.listCategory(local)){
				if(root.search(c.id) == null){
					client.removeCategory(c);
				}
			}
		}
	}
	
	protected void processSync(SyncTask task){
		log.error("Not support sync processing");
	}
	
	class SyncRunner implements Runnable{
		private boolean terminal = false;
		@Override
		public void run() {
			SyncTask t = null;
			log.info("Start SyncRunner");
			client = new NoteBookClient(book);			
			syncCategoryId();
			while(running && !this.terminal){
				synchronized(taskQueue){
					t = taskQueue.poll();
					if(t == null){
						try {
							//更新一论结束后需要更新ID.
							syncCategoryId();
							taskQueue.wait();
							continue;
						} catch (InterruptedException e) {
							continue;
						}
					}
				}
				try{
					if(t.task.equals(SyncTask.TASK_SYNC)){
						processSync(t);
					}else if(t.task.equals(SyncTask.TASK_UP)){
						processUpLoad(t);
					}else if(t.task.equals(SyncTask.TASK_DOWN)){
						processDownLoad(t);
					}
				}catch(Exception e){
					t.exception = e;
				}finally{
					doneTask.add(t);
				}
			}
			log.info("Stop SyncRunner");
		}
		public void close(){
			this.terminal = true;
			synchronized(taskQueue){
				taskQueue.notify();
			}
		}
	}
	
	private int cmpDate(Date d1, Date d2){
		if(d1 == null || d2== null){
			return -2;
		}
		long t1 = d1.getTime() / 1000;
		long t2 = d2.getTime() / 1000;
		if(t1 == t2) return 0;
		return t1 > t2 ? 1: -1;
	}
	
	public void syncCategoryId(){
		if(!running) return;
		try {
			Category id = client.getCategory("__nextId");
			if(id == null){
				id = new Category();
				id.id = "__nextId";
				id.name = "0";
			}
			int remoteId = Integer.parseInt(id.name);
			int localId = this.root.curId(-1);
			if(remoteId > localId){
				this.root.curId(remoteId);
				log.info("update local next id:" + remoteId);
			}else {
				id.setName("" + localId);
				this.client.updateCategory(id);
				log.info("update remote next id:" + localId);
			}
		} catch (Exception e) {
			EventProxy.syncError(null, e);
			/*
			if(!(e instanceof AuthcationException)){
				log.error(e, e);
			}*/
		}
	}
	
	public void addListener(SyncListener l){
		this.ls.add(l);
	}
	
	public void removeListener(SyncListener l){
		this.ls.remove(l);
	}	
	
	private void removeLocalChildren(Collection<Category> remoteNodes, Category local){
		Collection<Category> localList = new ArrayList<Category>();
		localList.addAll(local.children);
		for(Category c: localList){
			if(!c.flushed) continue;
			if(remoteNodes.contains(c))continue;
			c.remove();
		}
	}
	
	private SyncListener EventProxy = new SyncListener(){
		@Override
		public void start(SyncTask task) {
			for(SyncListener e: ls){
				e.start(task);
			}	
		}

		@Override
		public void done(SyncTask task) {
			for(SyncListener e: ls){
				e.done(task);
			}			
		}

		@Override
		public void syncError(SyncTask task, Exception e) {
			for(SyncListener l: ls){
				l.syncError(task, e);
			}				
		}
	};
}
