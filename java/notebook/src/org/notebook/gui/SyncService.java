package org.notebook.gui;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.LinkedList;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.Category;
import org.notebook.cache.NoteBook;
import org.notebook.cache.NoteMessage;
import org.notebook.io.ClientException;
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
		if(!(running && cate.flushed)) return;
		
		LinkedList<Category> queue = new LinkedList<Category>();
		
		Category remote, local, parent;
		try {
			remote = client.getCategory(cate.id);
			queue.add(remote);
						
			while(queue.size() > 0 && running){
				remote = queue.poll();
				EventProxy.checkDownload(remote);
				local = root.search(cate.id);
				if(local == null){
					parent = root.search(remote.parentId);
					if(remote.lastUpdated.after(parent.lastUpdated)){
						EventProxy.updateLocal(remote);
						parent.addCategory(remote);
						local = remote;
						if(local.isLeaf()){ //添加本地新节点.
							NoteMessage note = client.downLoadMessage(cate.id);
							local.getMessage().setText(note.text);
						}
					}else {	//远程节点是等待删除的过期节点.
						continue;
					}
				}
				
				if(remote.lastUpdated.after(local.lastUpdated)){
					EventProxy.updateLocal(remote);
					local.setName(remote.name);
					if(remote.isLeaf()){
						NoteMessage note = client.downLoadMessage(cate.id);
						local.getMessage().setText(note.text);
					}else {
						Collection<Category> children = client.listCategory(remote);
						this.removeLocalChildren(children, local);
						queue.addAll(children);
					}
				}else if(!remote.isLeaf()){
					queue.addAll(client.listCategory(remote));
				}
			}
		} catch (Exception e) {
			EventProxy.syncError(e);
			log.error(e, e);			
		}
	}
	
	public void upload(final Category cate){
		if(!(running && cate.flushed)) return;
		LinkedList<Category> queue = new LinkedList<Category>();
		
		Category remote, local, parent;
		try {
			queue.add(cate);						
			while(queue.size() > 0 && running){
				local = queue.poll();
				EventProxy.checkUpload(local);
				remote = client.getCategory(local.id);
				if(remote == null){
					parent = client.getCategory(local.parent.id);
					if(local.lastUpdated.after(parent.lastUpdated)){
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
			EventProxy.syncError(e);
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
			EventProxy.syncError(e);
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
		public void updateLocal(Category c) {
			for(SyncListener e: ls){
				e.updateLocal(c);
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
		public void syncError(Exception c) {
			for(SyncListener e: ls){
				e.syncError(c);
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
