package org.notebook.services;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.Writer;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.commons.net.PrintCommandListener;
import org.apache.commons.net.ftp.FTP;
import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPFile;
import org.apache.commons.net.io.CopyStreamEvent;
import org.apache.commons.net.io.CopyStreamListener;
import org.notebook.cache.Configuration;
import org.notebook.cache.TaskStatus;
import org.notebook.cache.UpgradeModel;
import org.notebook.events.EventQueue;
import org.notebook.gui.Events;
import org.notebook.gui.MainTable.StatusModel;

public class FTPSyncService {
	public static final int UPLOAD_MODE = 1;
	public static final int DOWNLOAD_MODE = 2;
	
	private static Log log = LogFactory.getLog("ftp.sync");
	
	
	private ThreadPoolExecutor syncThread = null;
	private StatusModel model = null;
	private EventQueue eventQueue = null;
	private JFrame win = null;
	private FTPClient ftp = null;
	private Configuration cfg = null;
	private boolean logined = false;
	private FTPStreamListener listener = new FTPStreamListener();
	private TaskStatus status = null;
	private File localRoot = new File(".");
	private String curVersion = null;
	private DatabaseService database = null;
	
	private boolean isDownloading = false;
	
	//Ftp服务器端，上传中断的文件
	private Map<String, FTPFile> ftpCache = new HashMap<String, FTPFile>();
	
	//当前传输的文件的输出流。
	private OutputStream curOs = null;
	private InputStream curIn = null;
	
	/*
	 * 当前工作模式， 上传数据模式／下载数据模式。
	 */
	private int mode = DOWNLOAD_MODE;
	
	public FTPSyncService(StatusModel model, EventQueue eventQueue, JFrame win, 
			Configuration cfg,
			ThreadPoolExecutor threadPool,
			DatabaseService dbs
			){
		this.model = model;
		this.eventQueue = eventQueue;
		this.win = win;
		this.cfg = cfg;
		this.syncThread = threadPool;
		this.database = dbs;
	}
	
	/**
	 * 由界面触发，开始连接服务器。
	 */
	public void connect(){
		if(ftp != null && ftp.isConnected() && ftp.isAvailable()){
			JOptionPane.showMessageDialog(win,
				    "当前FTP处于连接状态！",
				    "Error",
				    JOptionPane.INFORMATION_MESSAGE);
			return;
		}else if(ftp != null){
			try {
				ftp.disconnect();
			} catch (IOException e) {
				log.error(e.toString(), e);
			}
		}
		ftp = new FTPClient();
		ftp.addProtocolCommandListener(new PrintCommandListener(new PrintWriter(System.out), true));
		try {
			ftp.connect(cfg.getHost());
			updateStatusBar("FTP连接成功,正在登录...");
		} catch (Exception e) {
			updateStatusBar("FTP连接失败, 服务器" + cfg.getHost());
			log.error(e.toString(), e);
		}
		if(ftp.isConnected()){
			try {
				if(ftp.login(cfg.getUsername(), cfg.getPassword())){
					ftp.setCopyStreamListener(this.listener);
					updateStatusBar("FTP登录成功!");
					ftp.setFileType(FTP.BINARY_FILE_TYPE);
					logined = true;
					syncThread.execute(new Runnable(){
						@Override
						public void run() {
							updateFTPFilelist();							
						}});
				}else {
					updateStatusBar("FTP登录失败, 用户名:" + cfg.getUsername());					
				}
			} catch (IOException e) {
				updateStatusBar("FTP网络连接出错");
				log.error(e.toString(), e);
			}finally{
				if(!logined){
					try {
						ftp.disconnect();
					} catch (IOException e) {
					}
				}
			}
		}
	}
	
	public void setMode(int mode){
		this.mode = mode;
	}
	
	private void initFTPConnection(){
		
	}
	
	/**
	 * 简单下载处理流程。
	 */
	public void simpleDownload(){
		if(isDownloading){
			JOptionPane.showMessageDialog(win,
				    "已经开始传输文件！",
				    "Error",
				    JOptionPane.INFORMATION_MESSAGE);
			return;		
		}
			
		isDownloading = true;
		
		status = new TaskStatus();
		
		Collection<UpgradeModel> fileList = this.getUpdatingList();
		status.totalFiles = fileList.size();
		for(UpgradeModel f: fileList){
			long s = (mode == DOWNLOAD_MODE) ? f.dstSize : f.sourceSize;
			status.totalBytes += s;
		}
		
		updateStatusBar("开始下载文件...");		
		for(UpgradeModel f: fileList){
			try {
				downLoadFile(f);
			} catch (IOException e) {
				if(curOs != null){
					try {
						curOs.close();
					} catch (IOException e1) {
					}
				}
				updateStatusBar("FTP连接错误, 中止下载任务.");
				try {
					ftp.disconnect();
				} catch (IOException e1) {
				}
				
				//退出下载任务.
				break;
			}
		}
		
		isDownloading = false;
		//updateStatusBar("文件下载完毕.");
		
	}
	
	private boolean uploadFile(UpgradeModel file) {
		status.fileBytes = file.sourceSize;
		status.curFile = file.source;
		status.isZip = false;
		String cacheName = file.source + ".cache";
		
		long offset = 0;
		FTPFile f = null;
		if(ftpCache.containsKey(cacheName)){
			f = ftpCache.get(cacheName);
			offset = f.getSize();
		}
		status.doneFileByptes = offset;
		updateProgressBar();
		
		InputStream in = null;
		try{
			status.isUploading = true;
			in = new FileInputStream(new File(this.localRoot, file.source));
			in.skip(offset);			
			ftp.appendFile(cacheName, in);			
			in.close();
			
			//如果服务器上存在同名文件，先删除。
			if(file.dst != null){
				ftp.deleteFile(file.dst);
			}
			ftp.rename(cacheName, file.source);
			
			status.doneFiles++;
			status.doneBytes += file.sourceSize;
			status.isUploading = false;
			//status.isZip = true;
		}catch(IOException e){
			log.error(e.toString(), e);
			return false;
		}
		updateProgressBar();
		return true;
	}
	
	private void downLoadFile(UpgradeModel file) throws IOException{
		//status.fileBytes = 0;
		status.fileBytes = file.dstSize;
		status.curFile = file.dst;
		status.isZip = false;
		File dstFile = new File(this.localRoot, file.dst + ".cache");
		long offset = 0;
		if(dstFile.isFile()){
			offset = dstFile.length();
		}
		
		status.doneFileByptes = offset;
		updateProgressBar();
		ftp.setRestartOffset(offset);
		curOs = new FileOutputStream(dstFile, true);
		status.isUploading = true;
		ftp.retrieveFile(file.dst, curOs);
		
		if(curOs != null)curOs.close();
		curOs = null;
		status.doneFiles++;
		status.doneBytes += file.dstSize;
		status.isUploading = false;
		
		File newFile = new File(this.localRoot, file.dst);
		if(newFile.isFile()){
			newFile.delete();
		}
		dstFile.renameTo(newFile);
		
		updateProgressBar();
	}
	
	/**
	 * 取得需要更新的文件列表。
	 * @return
	 */
	private Collection<UpgradeModel> getUpdatingList(){
		Collection<UpgradeModel> fileList = new ArrayList<UpgradeModel>();
		for(UpgradeModel m: model.getData()){
			if(m.isUpdate){
				fileList.add(m);
			}
		}
		return fileList;
	}
	
	public void updateFTPFilelist(){
		if(!logined)return;
		try {
			updateStatusBar("读取FTP服务文件列表");
			FTPFile[] files = ftp.listFiles();
			this.ftpCache.clear();
			for(FTPFile f: files){
				if(!f.isFile()) continue;
				if(f.getName().endsWith(".cache")){
					this.ftpCache.put(f.getName(), f);
					continue;
				}
				updateFTPFileInfo(f);
			}
			
			for(UpgradeModel m:model.getData()){
				m.isUpdate = false;
				if(m.dstSize != m.sourceSize){
					//如果是下载模式，FTP上没有文件。不需要更新。
					if(mode == DOWNLOAD_MODE && m.dst == null){
						continue;
					}
					//上传的时候，如果本地没有文件不上传。
					else if(mode == UPLOAD_MODE && m.source == null){
						continue;
					}
					m.isUpdate = true;
				}				
			}
			model.fireTableDataChanged();
			updateStatusBar("文件列表读取完成，准备更新文件...");
		} catch (IOException e) {
			updateStatusBar("FTP网络连接出错");
			log.error(e.toString(), e);
		}
	}
	
	private void updateFTPFileInfo(FTPFile file){
		boolean isFound = false;
		UpgradeModel tmp = null; 
		for(UpgradeModel m: model.getData()){
			if(m.source != null && m.source.equals(file.getName())){
				m.dst = file.getName();
				m.dstSize = file.getSize();
				isFound = true; 
				tmp = m;
			}
		}
		if(!isFound){
			tmp = new UpgradeModel();
			tmp.dst = file.getName();
			tmp.dstSize = file.getSize();
			model.addUpgradeModel(tmp);
		}
	}	
	
	private void updateStatusBar(String msg){
		updateStatusBar(msg, false);
	}
	private void updateStatusBar(String msg, boolean warn){
		Map<String, Object> param = new HashMap<String, Object>();
		param.put(Events.STATUS_PARAM, msg);
		param.put(Events.STATUS_WARN, warn);
		eventQueue.fireEvent(Events.STATUS_EVENT, this, param);
		
	}
	
	private void updateProgressBar(){
		if(status == null) return;
		Map<String, Object> param = new HashMap<String, Object>();
		param.put(Events.PROGRESS_PARAM, this.status);
		eventQueue.fireEvent(Events.PROGRESS_EVENT, this, param);
	}
	
	
	/**
	 * 由界面触发，开始执行升级操作。
	 */	
	public void serverUpgrade(){
		if(isDownloading){
			JOptionPane.showMessageDialog(win,
				    "已经开始传输文件！",
				    "Error",
				    JOptionPane.INFORMATION_MESSAGE);
			return;		
		}
			
		isDownloading = true;
		
		status = new TaskStatus();
		
		Collection<UpgradeModel> fileList = this.getUpdatingList();
		if(fileList.size() > 0){
			//更新当前升级的版本信息。
			this.updateVersionInfo();
			status.totalFiles = fileList.size();
			for(UpgradeModel f: fileList){
				long s = (mode == DOWNLOAD_MODE) ? f.dstSize : f.sourceSize;
				status.totalBytes += s;
			}
			updateStatusBar("开始上传文件...");			
			for(UpgradeModel f: fileList){
				if(!this.uploadFile(f)){
					JOptionPane.showMessageDialog(win,
						    "文件传输出错，中止上传升级文件！",
						    "Error",
						    JOptionPane.ERROR_MESSAGE);
					break;
				}
				this.compressFile(f.source);
			}
			updateStatusBar("开始上传Zip升级包.");
			uploadZipFile();
		}else {
			JOptionPane.showMessageDialog(win,
				    "没有找到需要更新的文件。",
				    "消息",
				    JOptionPane.INFORMATION_MESSAGE);			
		}
		
		isDownloading = false;
	}
	
	/**
	 * 上传升级Zip包。
	 */
	private void uploadZipFile(){
		
	}
	
	public void compressFile(String file){
		File tmpFile = new File(this.localRoot, file + ".zip");
		File srcFile = new File(this.localRoot, file);
		OutputStream os = null;
		ZipOutputStream zOs = null;
		ZipEntry entry = null;
		InputStream ins = null;
		
		try {
			os = new FileOutputStream(tmpFile);
			zOs = new ZipOutputStream(os);
			ins = new FileInputStream(srcFile);
			entry = new ZipEntry(file);
			entry.setSize(srcFile.length());
			entry.setTime(srcFile.lastModified());
			zOs.putNextEntry(entry);
			
			byte[] buffer = new byte[1024 * 64];
			status.curFile = file;
			status.fileBytes = srcFile.length();
			status.doneFileByptes = 0;
			status.isZip = true;
			updateProgressBar();
			long lastUpdate = System.currentTimeMillis();
			for(int len = buffer.length; len == buffer.length;){
				len = ins.read(buffer);
				status.doneFileByptes += len;
				zOs.write(buffer, 0, len);
				if(System.currentTimeMillis() - lastUpdate > 200){
					updateProgressBar();
					lastUpdate = System.currentTimeMillis();
				}
			}
			updateProgressBar();			
		} catch (IOException e) {
			log.error(e.toString(), e);
		}finally{
			if(ins != null){
				try {
					ins.close();
				} catch (IOException e) {
				}
			}
			if(zOs != null){
				try {
					zOs.close();
				} catch (IOException e) {
				}
			}
		}
	}
	
	/**
	 * 暂停传输
	 */	
	public void pause(){
		//simpleDownload();
		log.info("---------pause----------------");
		try {
			ftp.disconnect();
		} catch (IOException e) {
			log.error(e.toString(), e);
		}
	}	
	
	public void scanLocalPath(boolean zip){
		//for()
		File root = new File(".");
		log.info("Scan local path:" + root.getAbsolutePath());
		updateStatusBar("当前目录：" + root.getAbsolutePath());
		
		UpgradeModel m = null;
		for(File f: root.listFiles()){
			if(!f.isFile())continue;
			if(f.getName().endsWith(".cache"))continue;
			if(!zip && f.getName().endsWith(".zip"))continue;
			if(zip && !f.getName().endsWith(".zip"))continue;
			m = new UpgradeModel();
			m.source = f.getName();
			m.sourceSize = f.length();
			model.addUpgradeModel(m);
		}
		model.fireTableDataChanged();
	}

	public void listFiles(){
		
	}
	
	class FTPStreamListener implements CopyStreamListener{
        private long megsTotal = 0;
        private long lastUpdateUI = 0;
        
        public void bytesTransferred(CopyStreamEvent event) {        	
            bytesTransferred(event.getTotalBytesTransferred(), event.getBytesTransferred(), event.getStreamSize());
        }

        public void bytesTransferred(long totalBytesTransferred,
                int bytesTransferred, long streamSize) {
        	if(status != null){
        		//status.doneBytes += event.getBytesTransferred();
        		status.doneFileByptes += bytesTransferred;        		
        		//至少0.2秒才更新一次进度条，避免影响界面操作影响效率。
        		if(System.currentTimeMillis() - lastUpdateUI > 200){
        			lastUpdateUI = System.currentTimeMillis(); 
        			updateProgressBar();
        		}
        	}            
        }
    };
    
    /**
     * 判断当前创建中的版本号。
     */
    private void updateVersionInfo(){
    	File tmpFile = new File(this.localRoot, "_version.cache");
    	//String version = ;
    	this.curVersion = readFile(tmpFile);
    	if(curVersion != null){
    		updateStatusBar("断点续传版本:" + curVersion, true);
    	}else {
    		curVersion = this.database.getNewVersion();
    		updateStatusBar("创建新的升级版本:" + curVersion, true);
    		this.saveFile(tmpFile, curVersion);
    	}    	
    }
    
    public String readFile(File path){
    	BufferedReader r = null;
    	String data = null;
    	try{
    		if(path.isFile() && path.canRead()){
	    		r = new BufferedReader(new InputStreamReader(
	    				new FileInputStream(path)));
	    		data = r.readLine();
    		}
    	}catch(IOException e){
    		log.error(e.toString(), e);
    	} finally{
    		if(r != null){
    			try {
					r.close();
				} catch (IOException e) {
				}
    		}
    	}    	
    	return data;    	
    }
    
    public void saveFile(File path, String data){
    	Writer w = null;
    	try {
			w = new OutputStreamWriter(new FileOutputStream(path));
			w.write(data);
		}catch(Exception e) {
			log.error(e.toString(), e);
		}finally{
			if(w != null){
				try {
					w.close();
				} catch (IOException e) {
				}
			}
		}    	
    }
}
