package org.notebook.gui;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;
import java.util.LinkedList;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.Category;
import org.notebook.cache.NoteBook;
import org.notebook.cache.NoteMessage;
import org.notebook.io.NoteBookClient;

public class SyncService {
	private static Log log = LogFactory.getLog(SyncService.class);
	public NoteBook book = null;
	public NoteBookClient client = null;
	public Category root = null;
	public Collection<SyncListener> ls = new ArrayList<SyncListener>();
	
	private boolean running = false;
	//public 
	
	public void setNoteBook(NoteBook book){
		if(book != null){
			client = new NoteBookClient(book);
			this.root = book.root;
			this.book = book;
		}
	}
	
	public void start(){
		running = true;	
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
	}
	
	public void stop(){
		running = false;
	}
	
	public boolean isRunning(){
		return running;
	}
	
	public void download(final Category cate){
		download(cate, false);
	}
	
	public void download(final Category cate, boolean force){
		//if(!(running && cate.flushed)) return;
		if(!running){
			log.warn("Sync service is not running");
			return;
		}	
		if(!cate.flushed){
			log.debug(String.format("Ignored upload not saved node, id:%s, name%s",
					cate.id, cate.name));
			return;
		}		
		
		LinkedList<Category> queue = new LinkedList<Category>();
		
		Category remote = null, local = null, parent = null;
		try {
			remote = client.getCategory(cate.id);
			if(remote != null){
				if(force){
					cate.lastUpdated = new Date(cate.lastUpdated.getTime() - 10 * 365 * 24 * 3600);
				}
				queue.add(remote);
			}
						
			while(queue.size() > 0 && running){
				remote = queue.poll();
				EventProxy.checkDownload(remote);
				local = cate.search(remote.id);
				if(local == null){
					parent = cate.search(remote.parentId);
					if(parent == null){
						EventProxy.syncError(remote, new Exception("Not found parent."));
						continue;
					}else if(parent.isLeaf()){
						parent = parent.getConflict();
					}
					//不能因为同步而修改,更新时间.
					Date lastUpdate = parent.lastUpdated;
					boolean isDirty = parent.isDirty;
					parent.addCategory(remote);
					parent.lastUpdated = lastUpdate;
					parent.isDirty = isDirty;
					
					local = remote;
					if(local.isLeaf()){ //添加本地新节点.
						NoteMessage note = client.downLoadMessage(local.id);
						if(note != null){
							local.getMessage(true).setText(note.text);
						}
						local.isDirty = false;
					}
					EventProxy.updatedLocal(local);
				}
				if(!local.flushed){
					log.debug(String.format("Ignored upload not saved node, id:%s, name%s",
							local.id, local.name));
					continue;
				}
				
				int result = cmpDate(remote.lastUpdated, local.lastUpdated);
				if(result > 0){
					local.setName(remote.name);
					local.lastUpdated = remote.lastUpdated;
					local.isDirty = false;
					if(remote.isLeaf()){
						NoteMessage note = client.downLoadMessage(local.id);
						local.getMessage(true).setText(note.text);
					}else {
						Collection<Category> children = client.listCategory(remote);
						this.removeLocalChildren(children, local);
						queue.addAll(children);
					}
					EventProxy.updatedLocal(local);
				}else if(result == -2){
					log.error("remote:" + remote.lastUpdated + ", local:" + local.lastUpdated);
				}
			}
		} catch (Exception e) {
			EventProxy.syncError(remote, e);
			log.error(e, e);			
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
	
	public void upload(final Category cate ){
		upload(cate, false);
	}
	
	public void upload(final Category cate, boolean force){
		if(!running){
			log.warn("Sync service is not running");
			return;
		}
		
		LinkedList<Category> queue = new LinkedList<Category>();
		
		Category remote=null, local = null, parent=null;
		try {
			if(force && cate.flushed){
				EventProxy.updateRemote(cate);
				client.updateCategory(cate);
				if(cate.isLeaf()){
					client.uploadMessage(cate.getMessage());
				}else {
					queue.addAll(cate.children);
				}
			}else {
				queue.add(cate);
			}
			while(queue.size() > 0 && running){
				local = queue.poll();
				if(!local.flushed){
					log.debug(String.format("Ignored upload not saved node, id:%s, name%s",
							local.id, local.name));
					continue;
				}
				EventProxy.checkUpload(local);
				remote = client.getCategory(local.id);
				if(remote == null){
					if(local.parent != null){
						parent = client.getCategory(local.parent.id);
					}else { //上传根节点。
						EventProxy.updateRemote(local);
						client.updateCategory(local);
						continue;
					}
					if(parent != null && cmpDate(local.lastUpdated, parent.lastUpdated) > 0){
						EventProxy.updateRemote(local);
						client.updateCategory(local);
						remote = local;
						local.isDirty = false;
						if(local.isLeaf()){
							client.uploadMessage(local.getMessage(true));
						}
					}else {
						log.error(String.format("Can't find parent to new category, id:%s, name:%s, pid:%s",
								local.id, local.name, 
								local.parent != null ? local.parent.id: ""));
					}
					continue;
				}
				
				int result = cmpDate(local.lastUpdated, remote.lastUpdated);				
				if(result > 0 || local.isDirty){
					EventProxy.updateRemote(local);
					client.updateCategory(local);
					if(local.isLeaf()){
						client.uploadMessage(local.getMessage(true));
					}else {
						for(Category c: client.listCategory(local)){
							if(root.search(c.id) == null){
								EventProxy.removeRemote(c);
								client.removeCategory(c);
							}
						}
						queue.addAll(local.children);
					}
					local.isDirty = false;
				}
				
				//if(!local.isLeaf()){
				//	queue.addAll(local.children);
				//}
			}
		} catch (Exception e) {
			EventProxy.syncError(local, e);
			log.error(e, e);
		}
	}
	
	public void sync(Category cate){
		if(!running) return;
		download(cate);
		upload(cate);
		syncCategoryId();
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
			log.error(e, e);
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
			EventProxy.removeLocal(c);
			c.remove();
		}
	}
	
	private SyncListener EventProxy = new SyncListener(){

		@Override
		public void removeLocal(Category c) {
			for(SyncListener e: ls){
				e.removeLocal(c);
			}			
		}

		@Override
		public void updatedLocal(Category c) {
			for(SyncListener e: ls){
				e.updatedLocal(c);
			}
		}

		@Override
		public void removeRemote(Category c) {
			for(SyncListener e: ls){
				e.removeRemote(c);
			}		
		}

		@Override
		public void updateRemote(Category c) {
			for(SyncListener e: ls){
				e.updateRemote(c);
			}
		}
		
		@Override
		public void syncError(Category c, Exception e) {
			for(SyncListener l: ls){
				l.syncError(c, e);
			}
		}

		@Override
		public void checkUpload(Category c) {
			for(SyncListener e: ls){
				e.checkUpload(c);
			}			
		}

		@Override
		public void checkDownload(Category c) {
			for(SyncListener e: ls){
				e.checkDownload(c);
			}
		}
	};
}
