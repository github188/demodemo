package com.nsn.reporting.chart.http;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.lang.ref.SoftReference;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.ParseException;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.jfree.chart.ChartUtilities;
import org.mortbay.jetty.Server;
import org.mortbay.jetty.servlet.ServletHandler;

import com.nsn.reporting.chart.ChartReport;
import com.nsn.reporting.data.ChartInfomation;


public class SimpleHTTPServer {
	static{
		if(System.getProperty("org.apache.commons.logging.simplelog.defaultlog") == null){
			System.setProperty("org.apache.commons.logging.simplelog.defaultlog", "trace");	
		}
	}
	
	private Log log = LogFactory.getLog(SimpleHTTPServer.class);
	
	public Map<String, String> chartParam = new HashMap<String, String>();
	private static SimpleHTTPServer ins = null;
	private int httpPort = 8081;
	private Server server = null;
	private String outExt = ".png";
	private PersistenceCache pm = null;
	private ChartData notFoundChart = null;
	
	public Map<String, SoftReference<ChartData>> cache = new HashMap<String, SoftReference<ChartData>>();
	
	private int hitCount = 0;
	
	public SimpleHTTPServer(int httpPort, Map<String, String> param, 
			CommandLine options){
		this.httpPort = httpPort;
		if(param != null) this.chartParam = param;
		if(options != null){
			outExt = options.getOptionValue(ChartReport.OUTEXT, ".png");
			if(options.hasOption(ChartReport.ROOT)){
				pm = new PersistenceCache(options.getOptionValue(ChartReport.ROOT));
			}
		}
		
		ins = this;
	}
	
	public static SimpleHTTPServer curInstance(){
		if (ins == null) ins = new SimpleHTTPServer(8081, null, null);
		return ins;
	}
	
	public void run(){
		server = new Server(this.httpPort);
		
		//log = LogFactory.getLog("ss");
		log.trace("loging with trace level!");
		log.info("Listening HTTP port:" + this.httpPort);
		
        ServletHandler handler=new ServletHandler();
        server.setHandler(handler);
        handler.addServletWithMapping("com.nsn.reporting.chart.http.StatusServlet", "/status");
        handler.addServletWithMapping("com.nsn.reporting.chart.http.ChartServlet", "/*");
        try {
			server.start();
	        server.join();
	        System.out.println("Shutdown HTTP service..");
		} catch (Exception e) {
			System.err.println("Stop HTTP service, with error:" + e.toString());
		}
	}
	
	public static void main(String[] args) throws ParseException{		
		SimpleHTTPServer server = new SimpleHTTPServer(8081, null, null);
		server.run();
		System.exit(0);
	}
	
	public ChartData processChart(String sid, String data, boolean sync){
		ChartData chart = new ChartData();
		chart.sid = sid;
		chart.data = data;
		ChartWorker w = new ChartWorker(chart);
		if(sync) {
			server.getThreadPool().dispatch(w);
		}else {
			w.run();
		}
		if(sid != null){
			cache.put(sid, new SoftReference<ChartData>(chart));
		}
		
		return chart;
	}
	
	public ChartData getChart(String sid, boolean showNotFound){
		ChartData chart = null;
		if (cache.get(sid) != null){
			chart = cache.get(sid).get();
			if(chart != null){
				synchronized(chart){
					if(!chart.updated){
						try {
							chart.wait();
						} catch (InterruptedException e) {
							chart.error = "InterruptedException to wait charting.";
						}
					}
				}
			}
		}
		if(chart == null && pm != null){
			chart = pm.load(sid);
			if(chart != null){
				cache.put(sid, new SoftReference<ChartData>(chart));
			}
		}
		
		if(chart == null && showNotFound){
			if(this.notFoundChart == null){
				this.initNotFound();
			}
			chart = this.notFoundChart;
		}
		
		//clean up cache per 100 query.
		hitCount = hitCount++ % 100;
		if(hitCount == 0){
			server.getThreadPool().dispatch(new CleanCache());
		}
		
		return chart;
	}
	
	class CleanCache implements Runnable{
		public void run() {
			Collection<String> conn = new ArrayList<String>();
			conn.addAll(cache.keySet());
			for(String k : conn){
				if(cache.get(k).get() == null){
					cache.remove(k);
				}
			}
		}
	}
	
	class PersistenceTask implements Runnable{
		private ChartData data = null;
		public PersistenceTask(ChartData data){this.data = data; }
		@Override
		public void run() {
			pm.save(data);
		}
	}
	
	class ChartWorker implements Runnable{
		private ChartData data = null;
		public ChartWorker(ChartData data){
			this.data = data;
		}
		@Override
		public void run() {
			synchronized(this.data){
				try{
					this.processChart();
					if(pm != null && data.sid != null){
						server.getThreadPool().dispatch(new PersistenceTask(data));
					}
				}catch(Exception e){
					ByteArrayOutputStream buffer = new ByteArrayOutputStream(100 * 1024);
					e.printStackTrace(new PrintStream(buffer));
					this.data.error = e.toString() + "\n" + buffer.toString();
					try{ buffer.close(); } catch(Exception e1){}
					buffer = null;
					log.error(e.toString(), e);
				}finally{
					this.data.updated = true;
					this.data.notifyAll();
				}
			}
		}
		
		private void processChart() throws IOException{
			ChartInfomation info = null;
			String[] inputLine = this.data.data.split("\n");
			String line = null;
			for(int lineCount = 0; lineCount < inputLine.length; lineCount++){
				line = inputLine[lineCount];
				line = line.trim();
				if(line.startsWith("#") || line.equals("")) continue;
				//a single '.' for split chart data in one file.
				if(line.endsWith(".")){break;}
				
				if(info == null){
					info = ChartReport.createChartInformation(line);
				}else {
					info.parseData(line, lineCount);
				}
			}
			
			for(String header : chartParam.keySet()){
				info.processHeader(header, chartParam.get(header));
			}
			
			this.data.chart = info.createChart();
			
			String[] size = info.getSize().split("x", 2);
			
			ByteArrayOutputStream buffer = new ByteArrayOutputStream(100 * 1024);
			String output = info.getOutput();
			if(output != null){
				if(output.endsWith(".png")){
					this.data.outExt = ".png";
				}else {
					this.data.outExt = ".jpg";
				}
			}else {
				this.data.outExt = outExt;
			}	
			
			int width = Integer.parseInt(size[0]);
			int height = Integer.parseInt(size[1]);
			float quality = info.getQuality();
			
		    if(this.data.outExt.equals(".png")){
		    	//response.setContentType("image/png");
		    	ChartUtilities.writeChartAsPNG(buffer, 
		    			data.chart, width, height, null);
		    }else {
		    	ChartUtilities.writeChartAsJPEG(buffer, 
		    			quality, data.chart, width, 
		    			height, null);
		    }		    
		    data.rawData = buffer.toByteArray();
		    buffer = null;
		}
	}
	
	private void initNotFound(){
		String notFound = "Chart:group_stack\n" +
						 "Title: Not found chart\n" +
						 "Label:Not found, Not found\n" +
						 "Fixed Length:30\n" + 
						 "Data:0\n";
		this.notFoundChart = this.processChart("notfound", notFound, false);
	}
}
