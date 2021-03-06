package org.notebook.services;

import java.awt.Image;
import java.awt.print.PrinterException;
import java.awt.print.PrinterJob;
import java.io.File;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import javax.swing.JOptionPane;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.DataStorage;
import org.notebook.cache.Document;
import org.notebook.cache.DocumentDefine;
import org.notebook.cache.LocalFileStorage;
import org.notebook.events.BroadCastEvent;
import org.notebook.events.EventAction;
import org.notebook.gui.AboutDialog;
import org.notebook.gui.MainFrame;
import org.notebook.gui.MenuToolbar;

public class DefaultBookController implements BookController{
	private static Log log = LogFactory.getLog("services");
	private DataStorage storage = null;
	//private SyncService sync = null;

	//private NoteBook book = null;
	private MainFrame mainFrame = null;
	private ThreadPoolExecutor syncThread = null;
	private ThreadPoolExecutor actionThread = null;
	private MenuAction menuActions = new MenuAction();
	private Map<String, Method> actionMapping = new HashMap<String, Method>();
	
	private boolean runningJNLP = false;
	private boolean runningSandBox = false;
	private boolean visibleTrayIcon = false;
	private int autoLoginTimes = 0;
	//初始化了本地NoteBook路径.
	private boolean initedBook = false;
	private boolean isWin = false;
	
	//演示数据
	private int dataCursor = 0;
	private Map<String, List<Document>> _temp = new HashMap<String, List<Document>>();
	private List<Document> _cur = null;
	
	public DefaultBookController(boolean isJNLP, boolean isSandBox){
		isWin = System.getProperty("sun.desktop").equals("windows");
		this.runningJNLP = isJNLP;
		this.runningSandBox = isSandBox;
		
		syncThread = new ThreadPoolExecutor(2, 5, 60, 
				TimeUnit.SECONDS, 
				new LinkedBlockingDeque<Runnable>(100));
		actionThread = new ThreadPoolExecutor(1, 1, 60, 
				TimeUnit.SECONDS, 
				new LinkedBlockingDeque<Runnable>(100));
		
		//this.sync.start();
		
		this.storage = createPersistenceService();
	}
	
	public void setTopWindow(MainFrame mainJFrame){
		this.mainFrame = mainJFrame;		
	}
	
	public boolean runingJNLP(){return this.runningJNLP;}
	public boolean runingSandBox(){return this.runningSandBox;};	
	
	public Object getEventActions(){
		return this.menuActions;
	}
	
	private DataStorage createPersistenceService(){
		DataStorage  storge = null;		
		if(runningJNLP && runningSandBox){
			try {
				Class cl = Class.forName("org.notebook.cache.JNLPServiceStorage");
				storge = (DataStorage)cl.newInstance();
			}catch(ClassNotFoundException e) {
				log.error("failed to load JNLP Stroage", e);
			} catch (Exception e) {
				log.error("failed to create JNLP Stroage", e);
			}			
		}else {
			File root = null;
			if(isWin){
				root = new File(System.getenv("APPDATA"), ".notebook");
			}else {
				root = new File(System.getenv("HOME"), ".notebook");
			}
			storge = new LocalFileStorage(root);
		}
		return storge;
	}	
	
	public boolean visibleTrayIcon(){
		return visibleTrayIcon;		
	}
	public boolean setVisibleTrayIcon(boolean visible){
		boolean old = visibleTrayIcon;
		visibleTrayIcon = visible;
		return old;
	}
	
	protected boolean authencation(){

		return false;
	}
	
	
	private void restoreLastStatus(){

	}
	
	public class MenuAction {
		
		@EventAction(order=1)
		public void Loaded(BroadCastEvent event){
			log.info("applcation loaded...");
			DocumentDefine doc = storage.loadDocument("simple.cfg");
			Map<String, Object> param = new HashMap<String, Object>();
			param.put(MenuToolbar.EVENT_DATA_DOC_PANEL, doc);			
			event.fireNewEvent(MenuToolbar.LOAD_DOC_PANEL, mainFrame,  param);
			_cur = new ArrayList<Document>();
		}	
		
		@EventAction(order=1)
		public void Exit(BroadCastEvent event) {
			log.info("shutdown applcation...");
			/*
			DocumentDefine doc = mainFrame.saveDocumentDefine();
			try {
				OutputStreamWriter out = new OutputStreamWriter(System.out);
				doc.save(out);
				out.flush();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}*/
			mainFrame.dispose();
			System.exit(0);
		}
		
		@EventAction(order=1)
		public void DataPre(BroadCastEvent event){
			if(dataCursor - 1 < 0){
				JOptionPane.showMessageDialog(mainFrame,
					    "没有找到上一条记录.",
					    "消息",
					    JOptionPane.INFORMATION_MESSAGE);			
			}else {
				dataCursor--;
				if(dataCursor >= _cur.size())
					dataCursor = _cur.size() - 1; 
				
				Map<String, Object> param = new HashMap<String, Object>();
				param.put(MenuToolbar.EVENT_DATA_DOC_DATA, _cur.get(dataCursor));			
				event.fireNewEvent(MenuToolbar.LOAD_DOC_DATA, mainFrame,  param);
			}
		}		
		
		@EventAction(order=1)		
		public void DataNext(BroadCastEvent event){
			if(dataCursor + 1 >= _cur.size()){
				JOptionPane.showMessageDialog(mainFrame,
					    "没有找到下一条记录.",
					    "消息",
					    JOptionPane.INFORMATION_MESSAGE);			
			}else {
				dataCursor++;				
				Map<String, Object> param = new HashMap<String, Object>();
				param.put(MenuToolbar.EVENT_DATA_DOC_DATA, _cur.get(dataCursor));			
				event.fireNewEvent(MenuToolbar.LOAD_DOC_DATA, mainFrame,  param);
			}
		}

		@EventAction(order=100)
		public void Save(BroadCastEvent event){
			Object data = event.get(MenuToolbar.EVENT_DATA_DOC_DATA); 
			if(data != null && data instanceof Document){
				_cur.add((Document) data);
				JOptionPane.showMessageDialog(mainFrame,
					    "打印文档数据保存成功!",
					    "消息",
					    JOptionPane.INFORMATION_MESSAGE);
				dataCursor = _cur.size();
			}else {
				JOptionPane.showMessageDialog(mainFrame,
					    "没有找到需要保存的数据",
					    "错误",
					    JOptionPane.ERROR_MESSAGE);				
			}		
		}
		
		@EventAction(order=100)
		public void Print(BroadCastEvent event){
			//mainFrame.showSettings();		
			log.info("print view...");	
			Object data = event.get(MenuToolbar.EVENT_DATA_PRINT); 
			if(data != null && data instanceof List){
				PrinterJob printJob = PrinterJob.getPrinterJob();				
			    printJob.setPrintable(new ImagePrintable((List<Image>)data));
			    if (printJob.printDialog()){
			    	try {
				        printJob.print();
				    } catch(PrinterException pe) {
				    	log.error(pe.toString());
				    }
			    }
			}else {
				JOptionPane.showMessageDialog(mainFrame,
					    "没有找到需要可以打印的数据",
					    "错误",
					    JOptionPane.ERROR_MESSAGE);				
			}
		}
		
		public String toString(){
			return "BookController Actions";
		}
	}
}
