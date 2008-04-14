package com.sigma.demo.acl.security.auth;

import java.security.InvalidParameterException;
import java.security.Principal;

public class RolePrincipal implements Principal {

	private String name;

	public RolePrincipal(String name) {
		if (name == null)
	 	   throw new InvalidParameterException("Rolename cannot be null");
	
		this.name = name;
	}


    public String getName() {
		return name;
    }

    public String toString() {
		return("RolePrincipal: " + name);
    }

    public boolean equals(Object object) {
		if(object == null)
			return false;
			
		if(!(object instanceof RolePrincipal))
            return false;			

        if(object == this)
        	return true;
 
        RolePrincipal principal = (RolePrincipal) object;
		if (getName().equals(principal.getName()))
		    return true;
		    
		return false;
    }
	 
    public int hashCode() {
		return name.hashCode();
    }

}
