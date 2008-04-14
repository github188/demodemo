package com.sigma.demo.acl.security;

import java.security.AccessControlContext;
import java.security.AccessController;
import java.security.Principal;
import java.util.HashSet;
import java.util.Set;

import javax.security.auth.Subject;

import com.sigma.demo.acl.security.auth.RolePrincipal;
import com.sigma.demo.acl.security.auth.UserPrincipal;

public class SecurityContext {
	public static String contextUsername(){
		AccessControlContext context = AccessController.getContext();
		Subject user = Subject.getSubject(context);
		if(user != null){
			for(Principal prin : user.getPrincipals()){
				if(prin instanceof UserPrincipal){
					return prin.getName();
				}
			}
		}
		
		return null;
	}
	
	public static Set<String> contextRoles(){
		AccessControlContext context = AccessController.getContext();
		Subject user = Subject.getSubject(context);
		Set<String> roles = new HashSet<String>();
		
		if(user != null){
			for(Principal prin : user.getPrincipals()){
				if(prin instanceof RolePrincipal){
					roles.add(prin.getName());
				}
			}		
		}
		
		return roles;
	}

	public static boolean isRole(String role){
		
		AccessControlContext context = AccessController.getContext();
		Subject user = Subject.getSubject(context);
		
		if(role == null  || "".equals(role) || user == null) return false;
		
		for(Principal prin : user.getPrincipals()){
			if(prin instanceof RolePrincipal &&
				prin.getName().equals(role)
			  ){
				return true;
			}
		}
		
		return false;
	} 
	
}
