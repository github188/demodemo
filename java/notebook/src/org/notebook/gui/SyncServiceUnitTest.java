package org.notebook.gui;

import java.io.File;

import org.notebook.cache.Category;
import org.notebook.cache.NoteBook;
import org.notebook.cache.SimpleObjectCache;

public class SyncServiceUnitTest {
	private SimpleObjectCache cache = null;
	private NoteBook notebook = null;
	//private Category notebook = null;	
	private SyncService service = null;
	
	public SyncServiceUnitTest(){
		File root = new File(".test");
		cache = new SimpleObjectCache(root);
		notebook = new NoteBook();
		notebook.root = new Category();
		notebook.root.loader = cache;
		
		notebook.user = "test1";
		notebook.endpoint = "http://127.0.0.1:8080/note";
		
		service = new SyncService(notebook);
		service.start();
		service.addListener(new SyncListener(){
			public void checkUpload(Category c){output("checkUpload", c);}
			public void checkDownload(Category c){output("checkDownload", c);}
			public void removeLocal(Category c){output("removeLocal", c);}
			public void updateLocal(Category c){output("updateLocal", c);}
			public void removeRemote(Category c) {output("removeRemote", c);}
			public void updateRemote(Category c) {output("updateRemote", c);}
			public void syncError(Exception e) {e.printStackTrace();}
			private void output(String action, Category c){
				System.out.println(action + " name:" + c.name + ", id:" + c.id);
			}
		});		
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		SyncServiceUnitTest tester = new SyncServiceUnitTest();
		
		tester.testUploadData();
		
		//tester.testDownloadCategory();
		
		System.out.println("done");
	}
	
	public void testDownloadCategory(){
		
	}
	
	public void testRemoveLocalData(){
		
	}

	public void testUploadData(){
		System.out.println("testUploadData...");
		Category c = notebook.root.addCategory("cate1");
		c = notebook.root.addCategory("cate2");
		
		c.addCategory("cate2->sub1");
		c = c.addMessage("cate2->msg1");
		c.getMessage().setText("test msg1");
		cache.saveNoteBook(notebook);
		
		service.upload(notebook.root);
	}
	
	public void testRemoveRemoteData(){
		
	}
}
