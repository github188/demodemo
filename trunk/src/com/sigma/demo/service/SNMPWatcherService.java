/* SNMPWatcherService.java
 *
 * Sigma Resources & Technologies, Inc.
 * Copyright (C) 2005-2008, sigma-rt.com
 *
 * PROPRIETARY RIGHTS of Sigma Resources & Technologies are involved in
 * the subject matter of this material. All manufacturing, reproduction,
 * use, and sales rights pertaining to this subject matter are governed
 * by the license agreement. The recipient of this software implicitly
 * accepts the terms of the license.
 *
 * Created on 2008-3-14, by Alexandre Tsu.
 * 
 * Project:	DemoServer
 * Author :	James Che
 * Auditor: 
 * 
 * # any description here # 
 *
 * For more information, visit:
 * http://www.sigma-rt.com
 * 
 * Change History
 * ---------------------------------------------------
 * REVISION          DATE/TIME        AUTHOR
 * ---------------------------------------------------
 * $Log$
 * ---------------------------------------------------
 *
 */
package com.sigma.demo.service;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import javax.management.Notification;

import org.apache.log4j.Logger;
import org.snmp4j.CommunityTarget;
import org.snmp4j.PDU;
import org.snmp4j.Snmp;
import org.snmp4j.TransportMapping;
import org.snmp4j.event.ResponseEvent;
import org.snmp4j.event.ResponseListener;
import org.snmp4j.mp.SnmpConstants;
import org.snmp4j.smi.Address;
import org.snmp4j.smi.GenericAddress;
import org.snmp4j.smi.OID;
import org.snmp4j.smi.OctetString;
import org.snmp4j.smi.VariableBinding;
import org.snmp4j.transport.DefaultUdpTransportMapping;

import com.sigma.demo.cfg.ServerConfig;
import com.sigma.demo.mbean.NotificationType;

public class SNMPWatcherService extends ThreadPoolSupportService implements SNMPWatcherServiceMBean{
	
    private Snmp snmp = null;
    private TransportMapping transport = null;
    private Map<String, SNMPCell> watches = null;
    private boolean isRunning = true;
    private long notificationSequence = 0;
    private Logger eventLogger = null;

	public void shutDown() throws FailServiceException {
		isRunning = false;
		snmp = null;
		try {
			if (transport != null) {
				transport.close();
			}
		} catch (IOException e) {
			throw new FailServiceException(e.getMessage(), e);
		}
	}

	public void start() throws FailServiceException {
		try {
			eventLogger = this.getLogger(ServerConfig.LOGGER_NOTIFICATION);
			watches = new HashMap<String, SNMPCell>();
			transport = new DefaultUdpTransportMapping();
			snmp = new Snmp(transport);			
			transport.listen(); 
		} catch (IOException e) {
			log.error(e ,e);
			throw new FailServiceException(e.getMessage(), e);
		}
		super.start();
	}

	public void addWatcher(String name, String oid, String ip, String community, int poll, int type) {
		synchronized(watches){ //void the key set modified in iterater.
			if(watches.get(name) != null){
				log.warn("The snmp watcher is exsit. cell:" + watches.get(name)); 
			} else {
				watches.put(name, new SNMPCell(name, oid, ip, community, poll, type));
			}
		}
	}

	public void removeWatcher(String name) {
		synchronized(watches){ //void the key set modified in iterater.
			if(watches.get(name) == null) {
				log.warn("The snmp watch isn't exsit. name:" + name);
			}else {
				watches.remove(name);
			}
		}
	}

	public void updateWatcher(String name, String oid, String ip, String community, int poll, int type) {
		if(watches.get(name) == null) {
			log.warn("The snmp watch isn't exsit. name:" + name);
		}else {			
			watches.get(name).update(name, oid, ip, community, poll, type);
		}
	}
	
	class SNMPCell {
		static final long pollUnit = 1000;
		
		String name;
		String oid;
		String ip;
		String community;
		long poll;
		int type;
		long lastSent;
		boolean error;
		String errorMsg;
		CommunityTarget target = null;
		PDU pdu = null;
		ResponseListener listener = null;
		SNMPCell(String name, String oid, String ip, String community, int poll, int type) {
			this.name = name;
			this.oid = oid;
			this.ip = ip;
			this.community = community;
			this.poll = poll * pollUnit;
			this.type = type;
			this.lastSent = 0;
			this.initPDU();
		}
		
		public void update(String name, String oid, String ip, String community, int poll, int type) {
			this.name = (name == null) ? this.name : name;
			this.oid = (oid == null) ? this.oid : oid;
			this.ip = (ip == null) ? this.ip : ip;
			this.community = (community == null) ? this.community : community;
			this.poll = (poll == 0) ? this.poll : poll * pollUnit;
			this.type = (type == 0) ? this.type : type;
			this.initPDU();
		}
		
		void initPDU () {
			this.error = true;
			/*
	        Address targetAddress = GenericAddress.parse("udp:"+ this.ip + "/161");      
	        target = new CommunityTarget();
	        target.setCommunity(new OctetString(community));
	        target.setAddress(targetAddress);
	        target.setRetries(2);
	        target.setTimeout(1500);
	        target.setVersion(SnmpConstants.version1);
	        pdu = new PDU();
	        pdu.add(new VariableBinding(new OID(oid)));
	        pdu.setType(this.type);
	        listener = new ResponseListener(){
				public void onResponse(ResponseEvent event) {
					if(event.getResponse().getErrorIndex() != 0){
						SNMPCell.this.error = true;
						SNMPCell.this.errorMsg = event.getResponse().getErrorStatusText();
						log.error(String.format("SNMP Error[name=%1$s,oid=%2$s,ip=%3$s,error=%4$s]",
								SNMPCell.this.name,
								SNMPCell.this.oid,
								SNMPCell.this.ip,
								SNMPCell.this.errorMsg								
								));
					}else {
						if(!event.getResponse().getVariableBindings().isEmpty()){
							VariableBinding data = (VariableBinding) event.getResponse().getVariableBindings().get(0);
							sentResponseData(data.getVariable().toLong());
						}
					}
				}
			};
			*/
		}
		
		void sendPDU() throws IOException {
			if(error && log.isDebugEnabled()) { //if the snmp is error. generate random data.
				sentResponseData(Math.random() * 1000);
			} else {
				snmp.send(pdu, target, null, listener);
			}
		}
		void sentResponseData(double data) {
			Notification n = new Notification(
					NotificationType.SNMP_UPDATED,
					SNMPWatcherService.this,
					notificationSequence++,
					System.currentTimeMillis(),
					SNMPCell.this.name);
			n.setUserData(data);
			if (eventLogger.isDebugEnabled()) {
				eventLogger.debug("Start:" + n);
			}
			sendNotification(n);
		}
	}

	/**
	 * poll all the snmp watcher. 
	 */
	public void run() {
		isRunning = true;
		long time = 0;
		if(log.isDebugEnabled()) {
			log.debug("Start snmp poll thread.");
		}
		
		while(isRunning) {
			time = System.currentTimeMillis();
			synchronized(watches) {
				for(SNMPCell c : watches.values()) {
					if(time - c.lastSent > c.poll) {
						try {
							c.lastSent = time;
							c.sendPDU();
						} catch (IOException e) {
							log.error(e, e);
						}
					}
				}
			}
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
			}
		}
		if(log.isDebugEnabled()) {
			log.debug("shuting down snmp poll thread.");
		}
	}

}

