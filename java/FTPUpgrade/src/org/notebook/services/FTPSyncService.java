package org.notebook.services;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ThreadPoolExecutor;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.commons.net.PrintCommandListener;
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
	
	//当前传输的文件的输出流。
	private OutputStream curOs = null;
	
	/*
	 * 当前工作模式， 上传数据模式／下载数据模式。
	 */
	private int mode = DOWNLOAD_MODE;
	
	public FTPSyncService(StatusModel model, EventQueue eventQueue, JFrame win, Configuration cfg,
			ThreadPoolExecutor threadPool){
		this.model = model;
		this.eventQueue = eventQueue;
		this.win = win;
		this.cfg = cfg;
		this.syncThread = threadPool;
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
	
	private void initFTPConnection(){
		
	}
	
	/**
	 * 简单下载处理流程。
	 */
	public void simpleDownload(){
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
		//updateStatusBar("文件下载完毕.");
		
	}
	
	private void downLoadFile(UpgradeModel file) throws IOException{
		status.fileBytes = file.dstSize;
		status.fileBytes = 0;
		status.curFile = file.dst;
		File dstFile = new File(this.localRoot, file.dst + ".cache");
		long offset = 0;
		if(dstFile.isFile()){
			offset = dstFile.length();
		}
		updateProgressBar();
		ftp.setRestartOffset(offset);
		curOs = new FileOutputStream(dstFile, true);
		ftp.retrieveFile(file.dst, curOs);
		
		curOs.close();
		curOs = null;
		status.doneFiles++;
		status.doneFileByptes+= file.dstSize;
		
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
			for(FTPFile f: files){
				if(!f.isFile()) continue;
				updateFTPFileInfo(f);
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
		
		if(tmp != null && (tmp.sourceSize != tmp.dstSize)){
			tmp.isUpdate = true;
			//如果是下载模式，FTP上没有文件。不需要更新。
			if(mode == DOWNLOAD_MODE && tmp.dst == null){
				tmp.isUpdate = false;
			}else if(mode == UPLOAD_MODE && tmp.source == null){ //上传的时候，如果本地没有文件不上传。
				tmp.isUpdate = false;
			}
		}
	}
	
	
	private void updateStatusBar(String msg){
		Map<String, Object> param = new HashMap<String, Object>();
		param.put(Events.STATUS_PARAM, msg);
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
	public void upgrade(){
		simpleDownload();
		/*
		JOptionPane.showMessageDialog(win,
			    "功能还没有实现！",
			    "Error",
			    JOptionPane.ERROR_MESSAGE);
		*/
	}
	
	public void scanLocalPath(){
		//for()
		File root = new File(".");
		log.info("Scan local path:" + root.getAbsolutePath());
		updateStatusBar("当前目录：" + root.getAbsolutePath());
		
		UpgradeModel m = null;
		for(File f: root.listFiles()){
			if(!f.isFile())continue;
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
            //bytesTransferred(event.getTotalBytesTransferred(), event.getBytesTransferred(), event.getStreamSize());
        	if(status != null){
        		//status.doneBytes += event.getBytesTransferred();
        		status.doneFileByptes += event.getBytesTransferred();
        		
        		//至少0.2秒才更新一次进度条，避免影响界面操作影响效率。
        		//if(System.currentTimeMillis() - lastUpdateUI > 200){
        			lastUpdateUI = System.currentTimeMillis(); 
        			updateProgressBar();
        		//}
        	}
        }

        public void bytesTransferred(long totalBytesTransferred,
                int bytesTransferred, long streamSize) {
            long megs = totalBytesTransferred / 1000000;
            for (long l = megsTotal; l < megs; l++) {
                System.err.print("#");
            }
            
            megsTotal = megs;
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
}
