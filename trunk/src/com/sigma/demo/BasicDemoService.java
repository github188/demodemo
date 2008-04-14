package com.sigma.demo;

import static com.sigma.demo.cfg.ServerConfig.CFG_DEMO_SINGLE_SHELL;
import static com.sigma.demo.cfg.ServerConfig.CFG_SHELL_NAME;
import static com.sigma.demo.cfg.ServerConfig.CFG_SHELL_ROOT_PATH;
import static com.sigma.demo.cfg.ServerConfig.CFG_SNMP_SERVICE;
import static com.sigma.demo.cfg.ServerConfig.CFG_THREAD_POOL;
import static com.sigma.demo.mbean.NotificationType.SNMP_UPDATED;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.security.AccessControlContext;
import java.security.AccessControlException;
import java.security.AccessController;
import java.security.Principal;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import javax.management.InstanceNotFoundException;
import javax.management.MBeanException;
import javax.management.Notification;
import javax.management.NotificationListener;
import javax.management.ObjectName;
import javax.management.ReflectionException;
import javax.security.auth.Subject;

import org.snmp4j.PDU;

import com.sigma.demo.acl.DemoFacadeService;
import com.sigma.demo.acl.DemoGroup;
import com.sigma.demo.data.DemoData;
import com.sigma.demo.data.DemoStep;
import com.sigma.demo.data.Series;
import com.sigma.demo.deploy.DeployEntity;
import com.sigma.demo.event.DMCallEvent;
import com.sigma.demo.event.DemoMsgEvent;
import com.sigma.demo.event.GraphEvent;
import com.sigma.demo.mbean.NotificationType;
import com.sigma.demo.service.AbstractService;
import com.sigma.demo.service.FailServiceException;
import com.sigma.demo.service.SNMPWatcher;
import com.sigma.demo.service.Shell;
import com.sigma.demo.service.ShellService;
import com.sigma.demo.service.ShellServiceMBean;
import com.sigma.demo.util.ProcessFilter;

public class BasicDemoService extends AbstractService
	implements BasicDemoServiceMBean {
	
	public static final String INFO  = "[info]";
	public static final String ALERT = "[alert]";
	public static final String DEBUG = "[debug]";
	public static final String ERROR = "[error]";
	public static final String WARN  = "[warn]";
	
	public static final String DM_CALL = "DM::";
	public static final String DS_PROVISION = "DS::demoProvision";	
	
	private DemoData demoContext = null;
	//private UserPrincipal lockedUser = null;
	private Collection<DemoListener> listeners = new Vector<DemoListener>();
	private DemoListener runtime = null;
	//private ThreadPool threadPool = null; 
	//private ExecContext shellExec = null;
	private DeployEntity deployInfo = null;
	private DefaultDemoState state = new DefaultDemoState();
	private ObjectName snmpService = null;
	private SNMPListener snmpListener = null;
	private SNMPWatcher snmpWatcher = null;
	private ObjectName shellService = null;
	private Shell shell = null; 
	private boolean singleShellThread = false;
	private Collection<String> provisions = new Vector<String>();

	public void shutDown() throws FailServiceException {
		
	}
	
	/**
	 * Start the demo service. it's invoked by deployer service. 
	 */
	public void start() throws FailServiceException {
		if(this.demoContext == null){
			throw new FailServiceException("Not initialize demo context.");
		}
	}

	public void addListener(DemoListener l) {
		this.listeners.add(l);
	}

	public void initDemoContext(DemoData context) {
		this.demoContext = context;
	}

	public boolean lockDemo(Principal user) throws DemoException {
		checkLockPermission();
		log.info(String.format("lock demo [user=%1$s, demo name = %2$s]", user, getDemoName()));
		if(this.state.isIdel()) {
			if(user == null) {
				throw new DemoException("Can't locked by null user.");
			}
			state.lock(user);
		} else {
			throw new DemoException("The demo status must idle when the lock is invoked.");			
		}
		
		log.info(String.format("locked demo successfully. [user=%1$s, demo name = %2$s]", user, getDemoName()));
		
		return true;
	}
	
	public boolean unlockDemo(boolean isForce) throws DemoException {
		log.info(String.format("unlock demo [demo name = %1$s]", getDemoName()));
		
		checkRunPermission();
		checkLockPermission();
		if(isForce && this.state.isRunning()) {
			this.stopDemo();
		}
		
		if(this.state.name.equals(DemoState.STOPED)){
			state.lock(null);
		}else {
			throw new DemoException("The demo status must stoped when the unlock is invoked.");
		}
		
		log.info(String.format("unlocked demo successfully. [demo name = %1$s]", getDemoName()));
		
		return true;
	}
	
	public void removeListener(DemoListener l) {
		this.listeners.remove(l);		
	}

	public void startDemo() throws DemoException {
		log.info("start demo, demo name=" + this.getDemoName());
		
		checkRunPermission();
		
		state.setName(DemoState.RUNNING);
		
		snmpListener = new SNMPListener();
		try {
			String serviceName = context.getStringParameter(CFG_SNMP_SERVICE, null);
			if(serviceName != null) {
				snmpService = new ObjectName(serviceName);
				log.info("initialize snmp watches, demo name=" + this.getDemoName());
				context.addNotificationListener(snmpService, snmpListener, null,
						null);
				snmpWatcher = (SNMPWatcher) context.getService(
						snmpService.getCanonicalName(), 
						SNMPWatcher.class);
				
				for (Series s : this.demoContext.getSerieses()) {
					snmpWatcher.addWatcher(genSeriesKey(s.getName()),
							s.getOID(),
							s.getIp(),
							s.getCommunity(), 
							s.getPoll(), 
							PDU.GET);
				}
			}else {
				log.warn("Not defined snmp service name. The demo can't get snmp data.");
			}
		} catch (Exception e) {
			log.error(e, e);
			throw new DemoException(e.getMessage(), e.getCause());
		}
		
		try {
			log.info("initialize demo shell service, demo name=" + this.getDemoName());
			Map<String, String> shellParam = new HashMap<String,String> ();
			shellParam.put(CFG_THREAD_POOL, context.getStringParameter(CFG_THREAD_POOL, null));
			shellParam.put(CFG_SHELL_ROOT_PATH, this.deployInfo.getServiceRoot().getAbsolutePath());
			shellParam.put(CFG_SHELL_NAME, context.getStringParameter(CFG_SHELL_NAME, null));
			shellService = new ObjectName(this.getName() + ",comp=shell");
			shell = (Shell) context.createService(shellService, ShellService.class.getName(), Shell.class, shellParam);
			shell.start();
			shell.setState(STATE_RUNNING);
			context.addNotificationListener(shellService, new ShellListener(), null,
					null);
		} catch (Exception e) {
			log.error(e, e);
			throw new DemoException(e.getMessage(), e.getCause());
		}
		
		singleShellThread = context.getBooleanParameter(CFG_DEMO_SINGLE_SHELL, false);
		
		log.info("started demo successfully, demo name=" + this.getDemoName());
	}

	public void stopDemo() throws DemoException {
		log.info("stop demo, demo name=" + this.getDemoName());		
		checkRunPermission();
		/*
		if(this.state.activeAction() != null) {
			String error = "The demo action isn't stopped before stopping the demo. [actvie action=" + this.state.activeAction() + "]";
			log.error(error);
			throw new DemoException(error);
		}
		*/
		if(shell != null) {	
			if(shell.isRunning() && !this.singleShellThread) {
				shell.destory();
			}
			
			String[] args = new String[this.provisions.size() + 1];
			args[0] = this.demoContext.getProvision();
			int i = 1;
			String _tmp = args[0] + " ";
			for(String device : this.provisions) {
				args[i++] = device;
				_tmp += device + " ";
			}
			log.debug("Provision:" + _tmp);
			if (this.singleShellThread) {
				shell.write(_tmp);
			} else {
				//shell.exeCommand(args, true);
				try {
					context.invoke(shellService, ShellServiceMBean.OP_EXEC, 
							new Object[]{args, true}, 
							new String[]{}
					);
				} catch (Exception e) {
				}
			}
			
			log.info("clean demo shell, demo name=" + this.getDemoName());
			try {
				shell.shutDown();
				context.removeService(shellService.getCanonicalName());
			} catch (FailServiceException e) {
				log.error(e, e);
			}
		}
		
		if(snmpService != null) {
			log.info("clean snmp watches, demo name=" + this.getDemoName());
			try {
				context.removeNotificationListener(snmpService, snmpListener);
				for (Series s : this.demoContext.getSerieses()) {
					snmpWatcher.removeWatcher(genSeriesKey(s.getName()));
				}
			} catch (Exception e) {
				log.error(e, e);
			}
		}else {
			log.warn("not initialize snmp watches, demo name=" + this.getDemoName());
		}
		//provisions
		
		snmpListener = null;
		state.setName(DemoState.STOPED);
		log.info("stoped demo successfully, demo name=" + this.getDemoName());
	}


	public DemoData getDemoData() {
		return this.demoContext;
	}
	
	public String getDemoName(){
		return this.demoContext.getName();
	}

	public void startDemoAction(String actionName) throws DemoException {
		log.info(String.format("start demo action, [action=%1$s, demo=%2$s]", actionName, this.getDemoName()));		
		checkRunPermission();
		
		if(this.state.activeAction() != null) {
			String error = "Other demo action is activing. [actvie action=" + this.state.activeAction() + "]";
			log.error(error);
			throw new DemoException(error);
		}
		
		state.runAction(actionName);
		DemoStep step = this.demoContext.getStep(actionName);
		if(step != null) {
			if(singleShellThread) {
				shell.write(step.getFunction());
			}else {
				shell.exeCommand(step.getFunction());
			}
		}else {
			log.error(String.format("Not defined the demo action, [action=%1$s]", actionName));
			throw new DemoException(String.format("Not defined the demo action, [action=%1$s]", actionName));
		}
		//shellExec.exec(actionName);
		
		log.info(String.format("started demo action successfully, [action=%1$s, demo=%2$s]", actionName, this.getDemoName()));		
	}

	public void stopDemoAction(String actionName) throws DemoException {
		log.info(String.format("stop demo action, [action=%1$s, demo=%2$s]", actionName, this.getDemoName()));
		
		checkRunPermission();
		state.runAction(null);
		//shellExec.exec(actionName);
		
		log.info(String.format("stoped demo action successfully, [action=%1$s, demo=%2$s]", actionName, this.getDemoName()));		
	}
	
	public void clearSchedule() throws DemoException {
		state.schedule(null, null);
		
	}

	public void schedule(Principal user, Date time) throws DemoException {
		state.schedule(user, time);		
	}

	public DemoState state() {
		return this.state;
	}
	
	protected void fireInfoEvent(DemoMsgEvent msg){
		
		for(DemoListener l : this.listeners){
			l.info(msg);
		}
		if(this.runtime != null){
			this.runtime.info(msg);
		}
	}
	
	protected void fireDMCallEvent(DMCallEvent evt){
		
		for(DemoListener l : this.listeners){
			l.dmCall(evt);
		}
		if(this.runtime != null){
			this.runtime.dmCall(evt);
		}
	}
	
	protected void fireGraphUpdateEvent(GraphEvent evt){
		
		for(DemoListener l : this.listeners){
			l.updateGraph(evt);
		}
		if(this.runtime != null){
			this.runtime.updateGraph(evt);
		}
	}		

	public void runTimeListener(DemoListener l) throws DemoException {
		this.runtime = l;
	}
	
	public DemoGroup getDemoGroup() {
		
		return null;
	}

	public DeployEntity getDeployEntity() {
		return this.deployInfo;
	}

	public void setDeployEntity(DeployEntity entity) {
		this.deployInfo = entity;
	}
	
	/**
	 * JMX Management 
	 */
	public String getDeployer() {
		return (this.deployInfo != null) ? this.deployInfo.getDeployer() : "";
	}

	/**
	 * JMX Management 
	 */	
	public String getServiceFile() {
		return (this.deployInfo != null) ? this.deployInfo.getServiceFile().getAbsolutePath() : "";	
	}
	
	/**
	 * JMX Management 
	 */		
	public String getDemoState() {
		return this.state.toString();
	}
	
	/**
	 * JMX Management 
	 */		
	public String getActiveAction() {
		return this.state.activeAction();
	}
	
	/**
	 * JMX Management 
	 */		
	public String getSchedule() {
		if(this.state.isSchedule()){
			return String.format("[%1$s/%2$tD %2$tT]",
					this.state.scheduler().getName(),
					this.state.scheduleTime());
		}else {
			return "None";
		}
	}
	

	/**
	 * valid status.
	 * 1. locked.
	 * 2. locked by self
	 * 3. caller is admin
	 * </code></pre>
	 */
	protected void checkRunPermission(){
		AccessControlContext context = AccessController.getContext();
		Subject subject = Subject.getSubject(context);
		if(log.isDebugEnabled()) {
			log.debug("check RunPermission, status=" + this.state.toString());
		}
		if(subject != null && this.state.isLocked()) {
			for(Principal prin : subject.getPrincipals()){
				if(log.isDebugEnabled()) {
					log.debug("check RunPermission, Principals=" + prin.getName());
				}
				if(this.state.locked().equals(prin) ||
				  prin.getName().equals("admin")
				) {
					return;
				}
			}
		}else if(subject == null) {
			log.warn("checkRunPermission, Not find security subject.");
		}
		
		throw new AccessControlException("Must locked the demo before running by user.");
	}

	/**
	 * valid status.
	 * 1. the call stack have demo facde service. 
	 * </code></pre>
	 */	
	protected void checkLockPermission() {
		for(StackTraceElement st :	Thread.currentThread().getStackTrace()){
			ClassLoader cl = Thread.currentThread().getContextClassLoader();				
			try {
				if(DemoFacadeService.class.isAssignableFrom(cl.loadClass(st.getClassName()))){
					return;
				}
			} catch (ClassNotFoundException e) {
				log.error(e, e);
			}
		}
		throw new AccessControlException("lock/unlock must invoked by demo facade service.");
	}
	
	private String genSeriesKey(String name) {
		return this.hashCode() + "$" + name + "$" + this.getDemoName(); 
	}
		
	private class DefaultDemoState implements DemoState {
		private String action = null;
		private Principal user = null;
		private String name = null;
		private Principal scheduler = null;
		private Date scheduleTime = null;
		private Date activeTime = null;
		//private 
		DefaultDemoState() {
			this.name = IDLE;
			activeTime = new Date(System.currentTimeMillis());
		}

		public String activeAction() {
			return action;
		}

		public Date activeTime() {
			return activeTime;
		}

		public Principal locked() {
			return user;
		}

		public String name() {
			return name;
		}

		public boolean isIdel() {
			return user == null;
		}

		public boolean isLocked() {
			return user != null;
		}

		public boolean isRunning() {			
			return name.equals(RUNNING);
		}

		public boolean isSchedule() {
			return scheduler != null;
		}

		public Date scheduleTime() {
			return scheduleTime;
		}

		public Principal scheduler() {
			return scheduler;			
		}
		
		void lock(Principal user){
			this.user = user;			
			name = (this.user == null) ? IDLE : LOCKED;
			this.updated();
			
			//Clear the schedule when it's actived.
			if(this.scheduler != null && user != null &&
					this.scheduler.equals(user) &&
					this.activeTime.after(this.scheduleTime)
			){
				this.scheduler = null;
				this.scheduleTime = null;
			}
		}
		
		void schedule(Principal user, Date time){
			if (this.scheduler == null || this.scheduleTime.after(time)) {
				this.scheduler = user;
				this.scheduleTime = time;
				name = (this.scheduler != null && name.equals(IDLE)) ? SCHEDULE
						: name;
			}
		}
		
		void runAction(String action) {
			this.action = action;
			this.updated();
		}
		
		void setName(String name) {
			this.name = name;
			this.updated();
		}
		
		void updated(){
			this.activeTime = new Date(System.currentTimeMillis());
		}
		
		public String toString(){
			return String.format("[%1$s/%2$tT, user=%3$s,action=%4$s, schedule=%5$s/%6$tD %6$tT]",
					name(),
					activeTime,
					(user == null) ? "None" : user.getName(),
					(action == null) ? "None" : action,
					(!isSchedule()) ? "None" : scheduler.getName(), 
					scheduleTime
			);
		}
	}
	
	/**
	 * listen the snmp update data, and forward the demo listener.
	 */
	class SNMPListener implements NotificationListener{	
		public void handleNotification(Notification notify, Object arg1){
			if(notify.getType().equals(SNMP_UPDATED)){
				String msg = notify.getMessage();
				String[] tmp = msg.split("\\$");
				if(tmp.length > 2 && Long.parseLong(tmp[0]) == BasicDemoService.this.hashCode()){
					GraphEvent evt = new GraphEvent(tmp[1], notify.getTimeStamp(), (Double)notify.getUserData());
					fireGraphUpdateEvent(evt);
				}
			}
		}
	}
	
	/**
	 * listen the snmp update data, and forward the demo listener.
	 */
	class ShellListener implements NotificationListener{
		ProcessFilter[] filters = new ProcessFilter[]{
				new DMCallProcessFilter(),
				new DMInfoProcessFilter(INFO),
				new DMInfoProcessFilter(DEBUG),
				new LoggerProcessFilter()
		};
		
		@SuppressWarnings("unchecked")
		public void handleNotification(Notification notify, Object arg1){
			if(notify.getSource() instanceof ShellService){				
				if(notify.getType().equals(NotificationType.SHELL_OUT)){
					Collection<String> msgs = (Collection<String>)notify.getUserData();
					for(String msg : msgs) {
						for(ProcessFilter p : filters) {
							try {
								p.filter(msg, null, null);
							} catch (IOException e) {
								log.error(e, e);
							}
						}
					}
				}
			}
		}
		
		private class DMCallProcessFilter extends ProcessFilter{
			public DMCallProcessFilter() {				
				super(DM_CALL, ALERT);
			}
			protected int handle(String line, BufferedReader reader, PrintWriter out) throws IOException {
				DMCallEvent event = new DMCallEvent(line);
				fireDMCallEvent(event);
				return NEXT;
			}
		}
		
		private class DMInfoProcessFilter extends ProcessFilter{
			String level = null;
			public DMInfoProcessFilter(String level) {
				super(level, "");
				this.level = level;
			}
			@Override
			protected int handle(String line, BufferedReader reader, PrintWriter out) throws IOException {
				DemoMsgEvent event = new DemoMsgEvent(line, level);
				fireInfoEvent(event);
				return NEXT;
			}
		}
		
		private class ProvisionFilter extends ProcessFilter{
			public ProvisionFilter() {
				super(DS_PROVISION, ALERT);
			}
			@Override
			protected int handle(String line, BufferedReader reader, PrintWriter out) throws IOException {
				BasicDemoService.this.provisions.add(line);
				return 0;
			}
		}		
		
		private class LoggerProcessFilter extends ProcessFilter{
			public LoggerProcessFilter() {
				super("*", "");
			}
			@Override
			protected int handle(String line, BufferedReader reader, PrintWriter out) throws IOException {
				log.error("Shell Out:" + line);
				return 0;
			}
		}		
	}
}
