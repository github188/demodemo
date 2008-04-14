package com.sigma.web;

import java.io.IOException;
import java.util.Map;

import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

//import org.hibernate.Session;

public interface Command {

	public void init(ServletContext ctx);
	
	public void initRequest(HttpServletRequest req,
			HttpServletResponse resp)throws ServletException, IOException;
	
	public boolean checkAuthorization();
	
	public String doCommand() throws IOException ;
	
	//public void setDAOSession(Session s);
	
	public boolean isRedirect();
	
	public void setParameter(Map<String,String> param);
	
	public void close();
	public Command copy();
}
