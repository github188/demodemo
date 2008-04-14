package com.sigma.web;

import java.io.IOException;
import java.security.PrivilegedActionException;
import java.security.PrivilegedExceptionAction;
import java.util.Collection;
import java.util.Vector;

import javax.security.auth.Subject;
import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

@SuppressWarnings("serial")
public class DispatcherServlet extends HttpServlet {
	protected static final Logger log = LogManager.getLogger(DispatcherServlet.class);
	
	private static Collection<UrlMapping> urlMaps = new  Vector<UrlMapping>();
	private static Command unknow = new IndexList();
	private boolean inited = false;
	
	@SuppressWarnings("unchecked")
	public static void addUrlHanlder(Class cl, String url){
		log.debug("addUrlHanlder,class:" + cl.toString() + ", url:" + url);
		if(IndexList.class.isAssignableFrom(cl)){
			log.debug("Set unknow handler:" + cl.toString());
			try {
				unknow = (Command) cl.newInstance();
			}catch (Exception e) {
				log.error(e,e);
			}
		}else if(url != null){
			urlMaps.add(new UrlMapping(url,cl));
		}else {
			log.debug("Error Handler,class:" + cl.toString() + ", url:" + url);
		}
	}
	
	public void init(ServletConfig config) throws ServletException{
		//Of cause, this code is better moving to ClientLoader.
		//But It's better closed with url-mapping in other side.
		if(!inited){
			inited = true;
			unknow.init(config.getServletContext());
			for(UrlMapping mapping: urlMaps){
				mapping.getURLHandler().init(config.getServletContext());
			}
		}
	}
	
	protected void doPost(final HttpServletRequest req,
			final HttpServletResponse resp) throws ServletException, IOException{
		jaasManagerHandle(req, resp);
	}
	
	protected void doGet(HttpServletRequest req,
			HttpServletResponse resp) throws ServletException, IOException{
		jaasManagerHandle(req, resp);
	}
	
	protected void requestHandle(HttpServletRequest req,
			HttpServletResponse resp) throws ServletException, IOException{
		
		log.debug("Dispathing Action....");
		Command cmd = dispatchToCommand(req);
		log.debug("Dispathed Action:" + cmd.toString());
				
		//Session session = HibernateUtil.getSessionFactory().getCurrentSession(); 
		try {
			//session.beginTransaction();
			
			cmd.initRequest(req, resp);
			//cmd.setDAOSession(session);
			log.debug("Check authoriztion....");
			if(cmd.checkAuthorization()){
				log.debug("Do Action....");
				String next = cmd.doCommand();
				if(log.isDebugEnabled()){
					log.debug("Forward next page:" + next + ", redirect:" + cmd.isRedirect());
				}
				// this.getServletContext()
				if (next != null) {
					if (cmd.isRedirect()) {
						if(next.startsWith("/")){
							next = req.getContextPath() + next;
						}
						resp.sendRedirect(next);
					} else {
						req.getRequestDispatcher(next).forward(req, resp);
					}
				} else {
					log.debug("Have not return next page!");
				}
			}else {
				log.debug("Failed to check authoriztion...");
				resp.sendRedirect(req.getContextPath()+ "/login.cgi");
			}
			/*
			if(session.isOpen()){
				session.getTransaction().commit();
			}
			*/
		} catch (Exception ex) {
			log.error(ex,ex);
			//session.getTransaction().rollback();
			if(resp.isCommitted()){
				resp.sendRedirect(req.getContextPath() + "/error.jsp");
			}else {
				req.getRequestDispatcher("/error.jsp").forward(req, resp);
			}
			//throw new ServletException(ex);
		} finally{
			cmd.close();
			/*
			if(session != null && session.isOpen())session.close();
			//HibernateUtil.getSessionFactory().get
			session = null;
			*/
		}
	}	
	
	protected void jaasManagerHandle(final HttpServletRequest req,
			final HttpServletResponse resp) throws ServletException, IOException{
		Subject subject = (Subject)req.getSession(true).getAttribute("_jaas_subject_");
		if(subject == null) {
			subject = new Subject();
			req.getSession(true).setAttribute("_jaas_subject_", subject);
		}
		//subject
		try {
			Subject.doAs(subject, new PrivilegedExceptionAction(){
				public Object run() throws Exception {
					requestHandle(req, resp);
					return null;
				}});
		} catch (PrivilegedActionException e) {
			log.error(e, e);
		}		
	}
	
	
	protected Command dispatchToCommand(HttpServletRequest req){
		//String action = req.getParameter("p");
		Command cmd = null;
		String url = req.getRequestURI();
		//log.error("ctxPath:" + ctxPath);
		//url = url.replaceFirst(ctxPath, "");
		if(req.getQueryString() != null){
			url += "?" + req.getQueryString();
		}
		
		for(UrlMapping mapping: urlMaps){
			if(mapping.matched(url)){
				cmd = mapping.getURLHandler().copy();
				cmd.setParameter(mapping.parseUrlParameter(url));
				break;
			}
		}
		
		return (cmd == null) ? unknow.copy() : cmd;
	}

}
