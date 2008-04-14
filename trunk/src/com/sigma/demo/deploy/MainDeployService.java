package com.sigma.demo.deploy;



import static com.sigma.demo.mbean.notification.DeployerNotification.NEW_DEPLOY;
import static com.sigma.demo.mbean.notification.DeployerNotification.REMOVE_DEPLOY;
import static com.sigma.demo.mbean.notification.DeployerNotification.UPDATED_DEPLOY;

import java.io.File;
import java.io.FileFilter;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.Vector;
import java.util.regex.Pattern;

import javax.management.MBeanNotificationInfo;
import javax.management.Notification;

import org.apache.log4j.Logger;

import com.sigma.demo.cfg.ServerConfig;
import com.sigma.demo.mbean.notification.DeployerNotification;
import com.sigma.demo.service.FailServiceException;
import com.sigma.demo.service.ThreadPoolSupportService;

public class MainDeployService extends ThreadPoolSupportService implements
	MainDeployServiceMBean {
	//public static final String DEFAULT_DOMAIN = "demo.service.deployed";
	//notifcation
	
	private long sequenceNumber = 0;
	private int scanPeriod = 0;
	private boolean scanEnabled = true;
	private int scanTimes = 0;
	private DemoFilter filter = null;
	private Object lock = new Object();
	private Collection<File> scanURL = null;
	private List<DeployEntity> deployEntitys = new ArrayList<DeployEntity>();
	private String deployUrls = null;
	
	private Logger eventLogger = null;

	public void shutDown() throws FailServiceException {
		this.setScanEnabled(false);
		
	}

	public void start() throws FailServiceException {
		
		this.scanPeriod = context.getIntParameter("ScanPeriod", 5000);
		this.scanEnabled = context.getBooleanParameter("ScanEnabled", true);
		this.filter = new DemoFilter(context.getStringParameter("Filter", ""));
		scanURL = new Vector<File>();		
		deployEntitys = new Vector<DeployEntity>();
		eventLogger = this.getLogger(ServerConfig.LOGGER_NOTIFICATION);
		
		String initUrls = context.getStringParameter("URLs", "/deploy");
		for(String url : initUrls.split(",")){
			this.addURL(url);
		}
		
		//domain = context.getStringParameter("DeployDomain", DEFAULT_DOMAIN);
		super.start();
	}

	public void run() {
		
		log.debug("Start deploy scanner....");
		while(scanEnabled){
			synchronized(lock){
				scanTimes++;
				for(File f : scanURL){
					this.scanNewDeploy(f, f);
				}
				
				Notification n = null;
			    for (int i= deployEntitys.size()-1; i>=0; i--) {
			    	DeployEntity e = deployEntitys.get(i);
					if (e.isRemoved()) {
						n = new DeployerNotification(REMOVE_DEPLOY, this,
								sequenceNumber++, e);
						if (eventLogger.isDebugEnabled()) {
							eventLogger.debug("Start:" + n);
						}
						sendNotification(n);
						deployEntitys.remove(i);
					} else if (e.isUpdated()) {
						n = new DeployerNotification(UPDATED_DEPLOY, this,
								sequenceNumber++, e);
						if (eventLogger.isDebugEnabled()) {
							eventLogger.debug("Start:" + n);
						}
						sendNotification(n);
						/*
						 * It's handled the updating event by deployer, if the directory is deployed.
						 * Otherwise scan the new files.
						 */						
						if (e.getDeployer() == null && e.getServiceFile().isDirectory()) {
							for (File file : e.getServiceFile().listFiles(this.filter)) {
								this.scanNewDeploy(file, e.getServiceRoot());
							}
						}
					}
				}
				try {
					lock.wait(scanPeriod);
				} catch (InterruptedException e) {
				}
			}
		}
		log.debug("Stoped deploy scanner....");
	}

	public void addURL(String url) {

		log.debug("adding deploy scan url:" + url);
		if(deployUrls == null || deployUrls.indexOf(url) < 0){
			File deploy = null;
			if(url.startsWith("${demo.server.root}")){
				url = url.replaceAll("\\$\\{demo\\.server\\.root\\}","");
				deploy = new File(new File("."), url);
			}else {
				deploy = new File(url);
			}
				
			if(deploy.exists() && deploy.isDirectory()){
				log.debug("added deploy scan path:" + deploy.getAbsolutePath());
				scanURL.add(deploy);
				if(deployUrls != null){
					deployUrls += ",";
				}else {
					deployUrls = "";
				}
				
				deployUrls += deploy.getAbsolutePath();
				synchronized(lock){
					lock.notify();
				}
			}else {
				log.debug("The deploy path isn't a directory. url:" + deploy.getAbsolutePath());	
			}
		}else {
			log.debug("The deploy path is exists.");
		}
	}

	public String getFilter() {
		return (filter != null) ? filter.getFilterRules() : "";
	}

	public boolean getScanEnabled() {

		return this.scanEnabled;
	}

	public int getScanPeriod() {

		return this.scanPeriod;
	}

	public String getURLs() {

		return deployUrls;
	}

	public void setFilter(String filter) {
		
		this.filter = new DemoFilter(filter);
	}

	public void setScanEnabled(boolean enabled) {
		boolean oldState = scanEnabled;		
		this.scanEnabled = enabled;
		
		//Restart the scanner.
		if(scanEnabled && !oldState){
			this.threadPool.execute(this);
		}
		
		synchronized(lock){
			lock.notify();
		}
	}

	public void setScanPeriod(int period) {
		this.scanPeriod = period;
		
		synchronized(lock){
			lock.notify();
		}
	}
	
	public int getScanTimes(){
		return this.scanTimes;
	}
	
	private class DemoFilter implements FileFilter {
		private Collection<Pattern> ignores = new Vector<Pattern>();
		private String rules = null;
		
		public DemoFilter(String rules) {
			this.rules = rules;
			this.rules = (this.rules == null) ? "" : this.rules; 
			for(String rule : this.rules.split(",")) {
				ignores.add(Pattern.compile(rule, Pattern.CASE_INSENSITIVE));
			}
		}

		public boolean accept(File f) {
			for(Pattern p : ignores){
				if(p.matcher(f.getName()).find()) {
					return false;
				}
			}
			
			return true;
		}
		
		public String getFilterRules(){
			return this.rules;
		}
		
	}

	private void scanNewDeploy(File f, File root) {
		for (DeployEntity entity : deployEntitys) {
			if (entity.getServiceFile().equals(f)) {
				return;
			}
		}
		
		
		DeployEntity entity = new DeployEntity(root, f);
		deployEntitys.add(entity);
	        
    	Notification n = new DeployerNotification(NEW_DEPLOY, this,
				sequenceNumber++, entity);
    	if (eventLogger.isDebugEnabled()) {
			eventLogger.debug("Start:" + n);
		}
        sendNotification(n); 
        
		if (f.isDirectory()) {
			for (File file : f.listFiles(this.filter)) {
				this.scanNewDeploy(file, root);
			}
		}
	}
	
    public MBeanNotificationInfo[] getNotificationInfo() { 
        String[] types = new String[] {
        		DeployerNotification.NEW_DEPLOY,
        		DeployerNotification.UPDATED_DEPLOY,
        		DeployerNotification.REMOVE_DEPLOY
        }; 
        String name = DeployerNotification.class.getName(); 
        String description = "An deploy entity state has changed,(new/updated/removed)";        
        MBeanNotificationInfo info = 
            new MBeanNotificationInfo(types, name, description); 
        
        return new MBeanNotificationInfo[] {info};
    }
	
}
