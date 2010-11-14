package org.notebook.services;

import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
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
	private static Log log = LogFactory.getLog("SyncService");
	public NoteBook book = null;
	public NoteBookClient client = null;
	public Category root = null;
	public Collection<SyncListener> ls = Collections.synchronizedCollection(new ArrayList<SyncListener>());
	protected DateFormat format= new SimpleDateFormat("yyyyMMddhhmmss");
	
	public LinkedList<SyncTask> taskQueue = new LinkedList<SyncTask>();
	public LinkedList<SyncTask> dataQueue = new LinkedList<SyncTask>();
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
		log.info("Starting sync service...");
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
		taskQueue.clear();
		runner = new SyncRunner();
		pool.execute(runner);
	}
	
	public void stop(){
		log.info("Stop sync service...");
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
	
	protected void doDownLoadData(SyncTask task){
		EventProxy.start(task);
		Category local = root.search(task.local.id);
		if(local != null){
			try{
				NoteMessage msg = client.downLoadMessage(local.id);
				local.getMessage(true).update(msg);
				local.isExpired = false;
				local.isDirty = false;
				task.status = SyncTask.DOWN_LOAD;
				EventProxy.done(task);
			}catch(Exception e){
				EventProxy.syncError(task, e);
			}
		}else {
			task.exception = new Exception("Not found local data with id " +
					task.local.id ); 
			task.status = SyncTask.IGNORED;
		}		
	}
	
	protected void doUpLoadData(SyncTask task){
		EventProxy.start(task);
		Category local = root.search(task.local.id);
		if(local != null){
			try{
				client.uploadMessage(local.getMessage(true));
				local.isDirty = false;
				local.isExpired = false;
				task.status = SyncTask.UP_LOAD;
				EventProxy.done(task);
			}catch(Exception e){
				task.status = SyncTask.IGNORED;
				task.exception = e;
				EventProxy.syncError(task, e);
			}
		}else {
			task.exception = new Exception("Not found local data with id " +
					task.local.id ); 
		}		
	}	
	
	protected void doDownLoadStructure(SyncTask task){
		Category parent = null, local=null, remote=null;
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
				remote = task.remote;
				parent = this.root.search(remote.parentId);
				if(parent == null || parent.isLeaf()){
					parent = this.root.getConflict();
				}
				local = this.root.search(remote.id);
				if(local == null){
					local = task.remote.copy();
					task.newCreated = true;
					local.isExpired = true;
					task.status = SyncTask.NEW_CREATED;
					if(local.isLeaf()){
						this.scheduleData(local.copy(), SyncTask.TASK_DOWN_DATA);
					}else if(local.children != null){
						//新增节点的数据需要清空.
						local.children.clear();
					}
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
					task.cause = "force";
					task.status = updateLocalFromRemote(local, remote);
				}else {
					int result = cmpDate(task.remote.lastUpdated, task.local.lastUpdated);
					if(result > 0 || local.isExpired){
						task.cause = result > 0 ? "time" : "expired";
						task.status = updateLocalFromRemote(local, remote);
					}else {
						task.status = SyncTask.NO_UPDATE;
						
					}
				}
			}
			if(task.status.equals(SyncTask.IGNORED)){
				task.exception = new Exception("Ignored by user, local is updating!");
			}
			if(!task.status.equals(SyncTask.NO_UPDATE) && !remote.isLeaf()){
				if(remote.children == null){
					remote.children = client.listCategory(remote);
				}
				for(Category c: remote.children){
					SyncTask newTask = new SyncTask(SyncTask.TASK_DOWN, null);
					newTask.remote = c;
					newTask.force = task.force;
					taskQueue.add(newTask);
				}
			}
		}catch(Exception e){
			if(local != null){
				local.isExpired = true;
			}
			EventProxy.syncError(task, e);
			log.error(e, e);
		}finally{
			task.doneDate = new Date(System.currentTimeMillis());
			EventProxy.done(task);
		}
	}
	
	private String updateLocalFromRemote(Category local, Category remote) throws IOException, ClientException{
		if(local.isLeaf() != remote.isLeaf()){
			if(remote.isLeaf()){
				Category c = local.getConflict().addMessage(remote.name);
				//this.scheduleData(c, SyncTask.TASK_DOWN_DATA);
				c.getMessage().update(client.downLoadMessage(remote.id));
				c.lastUpdated = remote.lastUpdated;
			}else {
				Category c = local.getConflict().addCategory(remote.name);
				c.lastUpdated = remote.lastUpdated;
			}
			return SyncTask.DOWN_CONFLICT;
		}else if(!local.isDirty || 
				SyncListener.UPDATE_FORCE == EventProxy.conflict(local, remote, SyncListener.CONFLICT_DIRTY)){
			local.setName(remote.name);
			local.lastUpdated = remote.lastUpdated;	
			if(local.isLeaf()){
				this.scheduleData(local, SyncTask.TASK_DOWN_DATA);
			}else {
				if(remote.children == null){
					remote.children = client.listCategory(remote);
				}
				this.removeLocalChildren(remote.children, local);
				local.isExpired = false;
			}
			return SyncTask.DOWN_LOAD;
		}else {
			local.isExpired = true;
			return SyncTask.IGNORED;
		}
	}
	
	private void scheduleData(Category node, String task){
		SyncTask newTask = new SyncTask(task, node.copy());
		dataQueue.add(newTask);
		if(task.equals(SyncTask.TASK_DOWN_DATA)){
			node.isExpired = true;
		}else {
			node.isDirty = true;
		}
	}
		
	//doDownLoadStructure
	protected void doUploadStructure(SyncTask task){
		Category local=null;
		if(!task.local.flushed){
			task.status = SyncTask.IGNORED;
			task.exception = new Exception("Local data not flushed.");			
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
				task.cause = "force";
				task.status = uploadToRemote(local, task.remote);
			}else {
				//使用计划时间的比较,保证更新结果和界面显示一致.
				int result = 1;
				if(task.remote != null){
					result = cmpDate(task.local.lastUpdated, task.remote.lastUpdated);
					if(result > 0) {
						task.cause = "time:";
						task.cause += format.format(task.local.lastUpdated) + ",";
						task.cause += format.format(task.remote.lastUpdated);
					}
				}else {
					task.cause = "no remote";
				}
				if(result > 0 || local.isDirty){
					if(local.isDirty) task.cause = "dirty";
					task.status = uploadToRemote(local, task.remote);
				}else {
					task.status = SyncTask.NO_UPDATE;
				}
			}
			if(task.status.equals(SyncTask.IGNORED)){
				task.exception = new Exception("Ignored uploading by user, local is expired!");
			}			
			if(!task.status.equals(SyncTask.NO_UPDATE) && !local.isLeaf()){
				for(Category c: local.children){
					SyncTask newTask = new SyncTask(SyncTask.TASK_UP, c.copy());
					newTask.force = task.force;
					taskQueue.add(newTask);		
				}
			}
			//local.isDirty = false;
		}catch(Exception e){
			EventProxy.syncError(task, e);
			log.error(e, e);
		}finally{
			task.doneDate = new Date(System.currentTimeMillis());
			EventProxy.done(task);
		}
	}
	
	private String uploadToRemote(Category local, Category remote) throws Exception{
		if(local.isLeaf() != remote.isLeaf()){
			if(local.isLeaf()){
				Category c = local.getConflict().addMessage(local.name);
				c.getMessage().update(local.getMessage());
			}else {
				Category c = local.getConflict().addCategory(local.name);
				for(Category xx: local.children){
					c.addCategory(xx);
				}
			}
			return SyncTask.DOWN_CONFLICT;
		}else if(!local.isExpired ||
			SyncListener.UPDATE_FORCE == EventProxy.conflict(local, remote, SyncListener.CONFLICT_EXPIRED)){			
			client.updateCategory(local);
			if(local.isLeaf()){
				this.scheduleData(local, SyncTask.TASK_UP_DATA);
			}else {
				for(Category c: client.listCategory(local)){
					if(root.search(c.id) == null){
						client.removeCategory(c);
					}
				}
			}
			local.isDirty = false;
			return SyncTask.UP_LOAD;
		}else {
			local.isDirty = true;
			return SyncTask.IGNORED;
		}
		
	}
	
	protected void processSync(SyncTask task){
		log.error("Not support sync processing");
	}
	
	class SyncRunner implements Runnable{
		private boolean terminal = false;
		@Override
		public void run() {
			log.info("Start SyncRunner");
			client = new NoteBookClient(book);			
			syncCategoryId();
			for(SyncTask t = next(); t != null; t = next()){
				try{
					if(t.task.equals(SyncTask.TASK_SYNC)){
						processSync(t);
					}else if(t.task.equals(SyncTask.TASK_UP)){
						doUploadStructure(t);
					}else if(t.task.equals(SyncTask.TASK_DOWN)){
						doDownLoadStructure(t);
					}else if(t.task.equals(SyncTask.TASK_DOWN_DATA)){
						doDownLoadData(t);
					}else if(t.task.equals(SyncTask.TASK_UP_DATA)){
						doUpLoadData(t);
					}
				}catch(Exception e){
					if(!(e instanceof AuthcationException)){
						log.error(e, e);
					}
				}finally{
					doneTask.add(t);
					while(doneTask.size() > 50){
						doneTask.pollFirst();
					}
				}
			}
			log.info("Stop SyncRunner");
		}		
		
		private synchronized SyncTask next(){
			SyncTask t = null;
			while(running && !this.terminal){				
				t = taskQueue.poll();
				if(t == null){
					t = dataQueue.poll();
				}
				if(t != null) return t;
				synchronized(taskQueue){
					try {
						//更新一论结束后需要更新ID.
						syncCategoryId();
						EventProxy.waiting();
						taskQueue.wait();
						continue;
					} catch (InterruptedException e) {
						continue;
					}
				}
			}
			return null;
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
		//protected DateFormat format= new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
		String t1 = format.format(d1);
		String t2 = format.format(d2);
		return t1.compareTo(t2);
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
		localList.addAll(local.getChildren());
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

		@Override
		public int conflict(Category local, Category remote, int cause) {
			int result = SyncListener.UPDATE_PADDING;
			for(SyncListener l: ls){
				result = l.conflict(local, remote, cause);
				if(result != SyncListener.UPDATE_PADDING){
					break;
				}
			}			
			return result == SyncListener.UPDATE_PADDING ?
					SyncListener.UPDATE_FORCE:
					result;
		}

		@Override
		public void waiting() {
			for(SyncListener l: ls){
				l.waiting();
			}					
		}
	};
}
