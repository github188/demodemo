package org.notebook.services;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
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
import org.notebook.cache.UpgradeModel;
import org.notebook.events.EventQueue;
import org.notebook.gui.Events;
import org.notebook.gui.MainTable.StatusModel;

public class FTPSyncService {
	private static Log log = LogFactory.getLog("ftp.sync");
	
	private ThreadPoolExecutor syncThread = null;
	private StatusModel model = null;
	private EventQueue eventQueue = null;
	private JFrame win = null;
	private FTPClient ftp = null;
	private Configuration cfg = null;
	private boolean logined = false;
	private FTPStreamListener listener = new FTPStreamListener();
	
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
			status("FTP连接成功,正在登录...");
		} catch (Exception e) {
			status("FTP连接失败, 服务器" + cfg.getHost());
			log.error(e.toString(), e);
		}
		if(ftp.isConnected()){
			try {
				if(ftp.login(cfg.getUsername(), cfg.getPassword())){
					ftp.setCopyStreamListener(this.listener);
					status("FTP登录成功!");
					logined = true;
					syncThread.execute(new Runnable(){
						@Override
						public void run() {
							updateFTPFilelist();							
						}});
				}else {
					status("FTP登录失败, 用户名:" + cfg.getUsername());					
				}
			} catch (IOException e) {
				status("FTP网络连接出错");
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
	
	public void updateFTPFilelist(){
		if(!logined)return;
		try {
			status("读取FTP服务文件列表");
			FTPFile[] files = ftp.listFiles();
			for(FTPFile f: files){
				if(!f.isFile()) continue;
				updateFTPFileInfo(f);
			}
			model.fireTableDataChanged();
			status("文件列表读取完成，准备更新文件...");
		} catch (IOException e) {
			status("FTP网络连接出错");
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
		}
	}
	
	
	public void status(String msg){
		Map<String, Object> param = new HashMap<String, Object>();
		param.put(Events.STATUS_PARAM, msg);
		eventQueue.fireEvent(Events.STATUS_EVENT, this, param);
		
	}
	/**
	 * 由界面触发，开始执行升级操作。
	 */	
	public void upgrade(){
		JOptionPane.showMessageDialog(win,
			    "功能还没有实现！",
			    "Error",
			    JOptionPane.ERROR_MESSAGE);		
	}
	
	public void scanLocalPath(){
		//for()
		File root = new File(".");
		log.info("Scan local path:" + root.getAbsolutePath());
		status("当前目录：" + root.getAbsolutePath());
		
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
        public void bytesTransferred(CopyStreamEvent event) {
            bytesTransferred(event.getTotalBytesTransferred(), event.getBytesTransferred(), event.getStreamSize());
        }

        public void bytesTransferred(long totalBytesTransferred,
                int bytesTransferred, long streamSize) {
            long megs = totalBytesTransferred / 1000000;
            for (long l = megsTotal; l < megs; l++) {
                System.err.print("#");
            }
            megsTotal = megs;
        }
    };
}
