package com.sigma.demo.acl.security.auth;

import java.security.InvalidParameterException;
import java.security.Principal;

public class UserPrincipal implements Principal {

	private String name;

	public UserPrincipal(String name) {
		if (name == null)
	 	   throw new InvalidParameterException("name cannot be null");
	
		this.name = name;
	}


    public String getName() {
		return name;
    }

    public String toString() {
		return("UserPrincipal: " + name);
    }

    public boolean equals(Object object) {
		if(object == null)
			return false;
			
		if(!(object instanceof UserPrincipal))
            return false;			

        if(object == this)
        	return true;
 
        UserPrincipal principal = (UserPrincipal) object;
		if (getName().equals(principal.getName()))
		    return true;
		    
		return false;
    }
	 
    public int hashCode() {
		return name.hashCode();
    }

}
