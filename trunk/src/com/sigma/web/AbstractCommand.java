package com.sigma.web;

import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.beanutils.BeanUtils;
import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

/*
import org.hibernate.Session;
import com.sigma_rt.demo.admin.model.User;
import com.sigma_rt.demo.admin.model.UserGroup;
import com.sigma_rt.demo.admin.service.UserManager;
*/

public abstract class AbstractCommand implements Command, Cloneable{
	protected Logger log = null; //LogManager.getLogger(AbstractCommand.class);
	protected Stack<Command> clones = new Stack<Command>();
		
	protected static final String GROUP_ADMIN = "admin";
	protected static final String AUTH_KEY = "auth";
	
	protected ServletContext ctx = null;
	protected HttpServletRequest req = null;
	protected HttpServletResponse resp = null;
	/*
	protected Session s = null;
	protected UserManager um = null;
	*/
	protected Map<String, String> defaultParameter = null;
	
	private boolean isRedirect = false;
	/*
	private User auth = null;
	*/	
	
	public AbstractCommand(){
		log = LogManager.getLogger(this.getClass());
		/**
		 * Can't push original object into stack, The mutable object of subclass will not initialization.
		 */
		//initMutableObject();
		//clones.push(this);
	}
	
	public void init(ServletContext ctx){
		this.ctx = ctx;
	}
	
	public void initRequest(HttpServletRequest req,
			HttpServletResponse resp)throws ServletException, IOException{
	//	auth = null;
		this.req = req;
		this.resp = resp;
		this.isRedirect = false;
		
		req.setCharacterEncoding("utf-8");
		resp.setContentType("text/html; charset=UTF-8");

	}
	
	public boolean checkAuthorization(){
		return true;
	}
	
	public void setParameter(Map<String,String> param){
		this.defaultParameter = param;
	}
	
	public boolean isRedirect() {
		return isRedirect;
	}
	
	public synchronized void close(){
		log.debug("Closing the command. ref=" + this.toString());

		defaultParameter = null;
		clones.push(this);
	}
	public synchronized Command copy(){
		if(clones.empty()){
			log.debug("The clones is empty, clone a new obj.");
			try {
				AbstractCommand cmd = (AbstractCommand)this.clone();
				log.debug("Cloned new object:" + cmd.toString());	
				cmd.initMutableObject();
				clones.push(cmd);
			} catch (CloneNotSupportedException e) {
				log.error(e,e);
			}
		}
		log.debug("Get a command copy. ref=" + clones.peek());
		return clones.pop();
	}
	
	/**
	 * populate the show page!
	 * the data can access by jstl.
	 * @param name
	 * @param data
	 */
	protected void populatePage(String name, Object data){
		req.setAttribute(name, data);
	}
	
	protected void populateBean(Object bean){
        HashMap<String,String[]> properties = new HashMap<String,String[]>();
        Enumeration names = req.getParameterNames();

        while (names.hasMoreElements()) {
            String name = (String) names.nextElement();
            String stripped = name;
            String[] parameterValue = null;
            parameterValue = req.getParameterValues(name);
            properties.put(stripped, parameterValue);
        }

        try {
            BeanUtils.populate(bean, properties);
            if(defaultParameter != null){
            	BeanUtils.populate(bean, defaultParameter);
            }
        } catch (Exception e) {
            log.error(e.toString(), e);
        }
		//req.setAttribute(name, data);
	}
	
	protected int getIntParameter(String name, int def){
		String value = getStringParameter(name, null);
		if(value != null){
			try{
				def = Integer.parseInt(value);
			}catch(NumberFormatException e){}
		}
		return def;
	}
	
	protected String getStringParameter(String name, String def){
		String value = req.getParameter(name);
		if(value == null && defaultParameter != null){
			value = defaultParameter.get(name);
		}
		
		if(value == null || "".equals(value)){
			value = def;
		}
		return value;
	}
	
	protected boolean getBooleanParameter(String name, boolean def){
		String value = getStringParameter(name, null);
		if(value != null ){
			try{
				def = Boolean.parseBoolean(value);
			}catch(Error e){}
		}
		return def;
	}	
	
	//java.text.SimpleDateFormat
	protected Date getDateParameter(String name, String pattern, Date def){
		String value = getStringParameter(name, "");
		if(value != null ){
			try{
				def = new SimpleDateFormat(pattern).parse(value);
			}catch (ParseException e) {
			}catch (Error e){}
		}
		return def;
	}	

	
	protected void saveAuth(String name){
		req.getSession(true).setAttribute(AUTH_KEY,name);
	}
	
	protected String getAuth(){
		return (String)req.getSession(true).getAttribute(AUTH_KEY);
	}


	protected void initMutableObject(){
		//um = new UserManager();
	}
	
	protected void isRedirect(boolean is){
		this.isRedirect = is;
	}
	
	
}

