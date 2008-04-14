package com.sigma.demo.acl.security;

import java.security.Permission;
import java.security.PermissionCollection;
import java.util.StringTokenizer;

import com.sigma.demo.DemoService;

/**
 * Actions:
 * 	list
 * 	run-stop
 * 	lock-release
 *  view
 *  create
 *  
 * @author user
 */
public class DemoPermission extends Permission {
	
	private static final long serialVersionUID = 1L;
	
	static private int LIST    		= 0x01;
	static private int EXECUTE   	= 0x02;
	static private int LOCK 		= 0x04;
	static private int VIEW  		= 0x08;
	static private int CREATE  		= 0x10;
	
	static final public String LIST_ACTION    	= "list";
	static final public String EXECUTE_ACTION   = "execute";
	static final public String LOCK_ACTION 		= "lock";
	static final public String VIEW_ACTION  	= "view";
	static final public String CREATE_ACTION  	= "create";
	protected int mask;
	
	private DemoService demo = null;

	public DemoPermission(String name) {
		this(name, null, null);
	}
	
	public DemoPermission(String name, String action) {
		this(name, action, null);
	}

	public DemoPermission(String name, String action, DemoService demo) {
		super(name);
		this.demo = demo;
		parseActions(action);
		
		System.out.println("Create Permission:" + toString());
	}	
	
	public DemoService getDemoService(){
		return this.demo;
	}

	

	@Override
	public boolean equals(Object object) {
		if( !(object instanceof DemoPermission) )		
			return false;
			
		DemoPermission p = (DemoPermission) object;
		
		return ( (p.getName().equals(getName())) && (p.mask == mask) );
	}

	@Override
	public int hashCode() {
		StringBuffer value = new StringBuffer(getName());
		return value.toString().hashCode() ^ mask;
	}
	
//	public boolean implies(Permission permission){
//		return true;
//	}

	@Override
	public boolean implies(Permission permission) {
		System.out.println("implies:" + permission.toString() + " in " + this.toString());
		
		if( !(permission instanceof DemoPermission) ){
			System.out.println("implies false," + permission.toString());
			return false;
		}
		
		// The resource name must be the same.
		if( !(permission.getName().equals(getName())) ){
			System.out.println("implies false," + permission.toString());
			return false;
		}
			
		DemoPermission p = (DemoPermission) permission;
		String user = SecurityContext.contextUsername();
		
		if( (p.mask & EXECUTE) == EXECUTE &&
			(user == null ||
			 p.getDemoService() == null ||
			 p.getDemoService().state().locked() == null ||
			 !user.equals(p.getDemoService().state().locked())) ){
			
			System.out.println("implies false," + permission.toString());
			return false;
		}
						
		if( (p.mask & LOCK) == LOCK && p.getDemoService().state().locked() != null ){
				System.out.println("implies false," + permission.toString());
				return false;
		}
		
		// The action bits in p (permission) 
		// must be set in the current mask permission.
		if( (mask & p.mask) != p.mask){
			System.out.println("implies false," + permission.toString());
			return false;
		}
		
		System.out.println("implies true," + permission.toString());
		return true;
	}
	
	public String getActions() {
		StringBuffer buf = new StringBuffer();

		if( (mask & LIST) == LIST ){
			buf.append(LIST_ACTION);
			buf.append(", ");
		}
		if( (mask & EXECUTE) == EXECUTE ) {
			buf.append(EXECUTE_ACTION);
			buf.append(", ");
		}
		if( (mask & LOCK) == LOCK ) {
			buf.append(LOCK_ACTION);
			buf.append(", ");
		}
		if( (mask & VIEW) == VIEW ) {
			buf.append(VIEW_ACTION);
			buf.append(", ");
		}
		if( (mask & CREATE) == CREATE ) {
			buf.append(CREATE_ACTION);
			buf.append(", ");
		}
		
		if(buf.length() > 2){
			buf.delete(buf.length() -2, buf.length());
		}

		return buf.toString();
	}	
	
	private void parseActions(String actions) {
		mask = 0;		
		if(actions != null) {
			StringTokenizer tokenizer = new StringTokenizer(actions, ",\t ");		
			while(tokenizer.hasMoreTokens()) {
				String token = tokenizer.nextToken();
				if(token.equals(LIST_ACTION))
					mask |= LIST;
				else if(token.equals(EXECUTE_ACTION))
					mask |= EXECUTE;
				else if(token.equals(LOCK_ACTION))
					mask |= LOCK;
				else if(token.equals(CREATE_ACTION))
					mask |= CREATE;
				else if(token.equals(VIEW_ACTION))
					mask |= VIEW;
				else 
					throw new IllegalArgumentException("Unknown action: " + token);
			}
		}
	}
	
	//	public PermissionCollection newPermissionCollection() {
	//		System.out.println("newPermissionCollection:" + this.toString());
	//		return new DemoPermissionCollection();
	//	}
	
	public String toString(){
		return "DemoPermission Name:" + this.getName() + ", Actions:" + this.getActions();
	}
	

}
