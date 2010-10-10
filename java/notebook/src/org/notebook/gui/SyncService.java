package org.notebook.gui;

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
	//public NoteBook book = null;
	public NoteBookClient client = null;
	public Category root = null;
	public Collection<SyncListener> ls = new ArrayList<SyncListener>();
	
	private boolean running = false;
	//public 
	
	public void setNoteBook(NoteBook book){
		if(book != null){
			client = new NoteBookClient(book);
			this.root = book.root;
		}
	}
	
	public void start(){
		running = true;		
	}
	
	public void stop(){
		running = false;
	}
	
	public void download(final Category cate){
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
			queue.add(remote);
						
			while(queue.size() > 0 && running){
				remote = queue.poll();
				EventProxy.checkDownload(remote);
				local = cate.search(remote.id);
				if(local == null){
					parent = cate.search(remote.parentId);
					if(parent == null){
						EventProxy.syncError(remote, new Exception("Not found parent."));
						continue;
					}
					//不能因为同步而修改,更新时间.
					Date lastUpdate = parent.lastUpdated;
					parent.addCategory(remote);
					parent.lastUpdated = lastUpdate;
					
					local = remote;
					if(local.isLeaf()){ //添加本地新节点.
						NoteMessage note = client.downLoadMessage(cate.id);
						local.getMessage().setText(note.text);
					}
					EventProxy.updatedLocal(local);
				}
				if(!local.flushed){
					log.debug(String.format("Ignored upload not saved node, id:%s, name%s",
							local.id, local.name));
					continue;
				}
				
				if(remote.lastUpdated.after(local.lastUpdated)){
					local.setName(remote.name);
					local.lastUpdated = remote.lastUpdated;
					if(remote.isLeaf()){
						NoteMessage note = client.downLoadMessage(cate.id);
						local.getMessage().setText(note.text);
					}else {
						Collection<Category> children = client.listCategory(remote);
						this.removeLocalChildren(children, local);
						queue.addAll(children);
					}
					EventProxy.updatedLocal(local);
				}else if(!remote.isLeaf()){
					queue.addAll(client.listCategory(remote));
				}
			}
		} catch (Exception e) {
			EventProxy.syncError(remote, e);
			log.error(e, e);			
		}
	}
	
	public void upload(final Category cate){
		if(!running){
			log.warn("Sync service is not running");
			return;
		}
		
		LinkedList<Category> queue = new LinkedList<Category>();
		
		Category remote=null, local = null, parent=null;
		try {
			queue.add(cate);						
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
					}
					if(parent == null || local.lastUpdated.after(parent.lastUpdated)){
						EventProxy.updateRemote(local);
						client.updateCategory(local);
						remote = local;
						if(local.isLeaf()){
							client.uploadMessage(local.getMessage());
							continue;
						}
					}
				}
				
				
				if(local.lastUpdated.after(remote.lastUpdated)){
					EventProxy.updateRemote(local);
					client.updateCategory(local);
					if(remote.isLeaf()){
						client.uploadMessage(local.getMessage());
					}else {
						for(Category c: client.listCategory(local)){
							if(root.search(c.id) == null){
								EventProxy.removeRemote(c);
								client.removeCategory(c);
							}
						}
					}
				}
				
				if(!local.isLeaf()){
					queue.addAll(local.children);
				}
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
			}else {
				id.setName("" + localId);
				this.client.updateCategory(id);
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
