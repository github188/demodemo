package com.nsn.reporting.chart.http;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.json.simple.JSONValue;

public class ChartServlet extends HttpServlet{
	//private Map<String, String> param = new HashMap<String, String>();
	private Log log = LogFactory.getLog(ChartServlet.class);
	private SimpleHTTPServer service = null;
    public ChartServlet(){
    	service = SimpleHTTPServer.curInstance();
    }
    
    protected void doGet(HttpServletRequest request, HttpServletResponse response) 
    	throws ServletException, IOException {
    	String sid = getSID(request);
    	String data = request.getParameter("data");
    	String dummy = request.getParameter("notfound");
    	if(dummy == null){
    		dummy = (String)request.getAttribute("notfound");
    	}
    	long st = System.currentTimeMillis();
    	Map<String, String> status = new HashMap<String, String>(); 
    	status.put("status", "ERR");
    	
    	if(sid == null){
    	    response.setContentType("text/html");
    	    response.setStatus(HttpServletResponse.SC_OK);
    	    response.getWriter().println(QUERY_FORM);
    	}else {
    		ChartData chart = null;
    		chart = service.getChart(sid, dummy != null);
    		if(chart == null){
    			status.put("error", "Not found session, " + sid);
    			this.outputJSON(status, response);
    		}else if(data != null && "Y".equals(data.trim())){
    			this.outputMessage(chart.data, response);
    		}else if(chart.error != null){
    			status.put("error", chart.error);
    			this.outputJSON(status, response);
    		}else {
    			this.outputChart(chart, response);
    		}
    	}
    	
    	st = System.currentTimeMillis() - st;
    	if(st > 300){
    		log.warn(String.format("slow query, sid=%s, elapsed time=%s ms", sid, st));
    	}
    }
    
    protected void doPost(HttpServletRequest request, HttpServletResponse response) 
	throws ServletException, IOException {
    	String sid = getSID(request);
    	String data = request.getParameter("data");
    	ChartData chart = null;
    	long st = System.currentTimeMillis();

    	Map<String, String> status = new HashMap<String, String>(); 
    	status.put("status", "ERR");    	
    	if(sid == null){
    		chart = service.processChart(sid, data, false); 
    		if(chart.error != null){
    			status.put("error", chart.error);
    			this.outputJSON(status, response);
    		}else {
    			this.outputChart(chart, response);
    		}
    	}else {
    		chart = service.processChart(sid.trim(), data, true);
    		status.put("status", "OK");
    		status.put("sid", sid);
    		this.outputJSON(status, response);
    	}

    	st = System.currentTimeMillis() - st;
    	if(st > 300){
    		log.warn(String.format("slow query, sid=%s, elapsed time=%s ms", sid, st));
    	}
    }
    
    private void outputMessage(String msg, HttpServletResponse response) throws IOException{
	    response.setContentType("text/plain");
	    response.setStatus(HttpServletResponse.SC_OK);
	    response.getWriter().println(msg);
    }
    private void outputJSON(Map<String, String> data, HttpServletResponse response) throws IOException{
	    response.setContentType("text/plain");
	    response.setStatus(HttpServletResponse.SC_OK);
	    JSONValue.writeJSONString(data, response.getWriter());
    }
    
    private void outputChart(ChartData chart, HttpServletResponse response) throws IOException{
	    response.setStatus(HttpServletResponse.SC_OK);
	    response.setHeader("Cache-Control", "no-cache");
	    response.setHeader("Expires", "Thu, 01 Dec 1994 16:00:00 GMT"); 
	    if(chart.outExt.equals(".png")){
	    	response.setContentType("image/png");
	    }else {
	    	response.setContentType("image/jpeg");
	    }
	    response.getOutputStream().write(chart.rawData);
    }
    
    private String getSID(HttpServletRequest request){
    	//S request.getPathInfo()
    	String sid = request.getParameter("sid");
    	if(sid != null){
    		sid = sid.trim().split("\\.")[0];
    		if ("".equals(sid)) sid = null;
    	}else {
    		Pattern pa = Pattern.compile("sid/([[^\\./]]+)");
    		Matcher ma = pa.matcher(request.getPathInfo());
    		if(ma.find()){
    			sid = ma.group(1);
    		}
    		request.setAttribute("notfound", "Y");
    	}
    	return sid;
    }
    
    public static final String QUERY_FORM = "<html><body>" +
    "<form action='?' method='POST'> " +
    "SID:<input type='input' name='sid' value=''><br/>" +
    "Chart Data:<br/><textarea cols='80' rows='30' name='data'></textarea><br/>" +
    "<input type='submit' name='submit' value='Submit'>" +
    "</form>" + 
    "</body></html>";
}
