package com.sigma.demo.util;


import java.io.IOException;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;
import org.snmp4j.PDU;
import org.snmp4j.smi.VariableBinding;

import com.sigma.demo.data.Series;
import com.sigma.demo.data.SeriesData;
import com.sigma_rt.asn1.DerOutputStream;

public class DemoSeriesProducer extends Thread{
	private static final Logger log = LogManager.getLogger(DemoSeriesProducer.class);
	//private DemoContext ctx = null;
	private Series series = null;
	private DerOutputStream out = null;
	private SeriesData data = null;
	private boolean isRunning = true;
	private SnmpRequest snmp = null;
	
	public DemoSeriesProducer( Series s, DerOutputStream customerOut){
		//this.ctx = ctx;
		this.series = s;
		this.out = customerOut;
		this.data = new SeriesData();
		//data.setDemoFullName(ctx.getFullname());
		data.setName(s.getName());
		this.setName("Series_" + s.getName());
		initSnmp();
		this.setDaemon(true);
		isRunning =true;
	}
	
	public void close(){
		isRunning = false;
		log.debug("close a Series productor..");
	}
	
	public void run(){
		
		while(isRunning){
			//data.getDataX()
			data.setDataX(System.currentTimeMillis());
			if(snmp != null){
				log.debug("sendPDU...");
				snmp.sendPDU();
				VariableBinding[] result = snmp.getVarBindings();
				if(result != null && result.length > 0 && result[0] != null){
					data.setDataY(result[0].getVariable().toLong());
				}else {
					data.setDataY(0);
				}
				log.debug("snmp result:" + data.getDataY());
				//data.setDataY(Math.random() * 100);
			}else if(log.isDebugEnabled()){
				data.setDataY(Math.random() * 100);
			}else {
				break;
			}
			try {
				if(out != null){
					synchronized(out){
						data.derEncode(out);
						out.flush();
					}
				}
			}catch (IOException e) {
				log.error(e,e);
				//Get a other outputStream, when current output Stream occur exception.
				//out = ctx.getGraphOutPutStream();
			}
			
			try {
				Thread.sleep(series.getPoll() * 1000);
			}catch (InterruptedException e) {
				log.error(e,e);
			}
		}
		
		log.debug("Stoped a Series productor...");
	}
	
	private void initSnmp(){
		try{
			if(log.isDebugEnabled()){
				log.debug("Init Snmp IP:" + series.getIp() + ", Community:" + series.getCommunity() + ", OID:" + series.getOID());
			}
			snmp = new SnmpRequest(series.getIp(), series.getCommunity(), PDU.GET, series.getOID());
			snmp.sendPDU();
			if(snmp.getErrorIndex() != 0){
				log.error("Init Snmp Error Index:" +snmp.getErrorStatus()+ ", Error Text:" + snmp.getErrorStatusText());
				log.debug("A random number will instead of the snmp data.(only in debug mode)");
				snmp = null;
			}
			
		}catch(Throwable e){
			log.error(e,e);
		}
	}
	
	public void start(){
		log.debug("Start a Series productor...");
		super.start();
	}

	public Series getSeries() {
		return series;
	}
	
}
