package com.sigma.demo.acl.security;

import java.security.Permission;
import java.security.PermissionCollection;
import java.util.Collection;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

public class DemoPermissionCollection extends PermissionCollection {

	private Hashtable permissions;
	
	public DemoPermissionCollection(){
		//
		permissions = new Hashtable();
	}
	
	@Override
	public void add(Permission permission) {
		System.out.println("Add Permission:" + permission.toString());
		
		if(isReadOnly())
			throw new IllegalArgumentException("Read only collection");
		
		if( !(permission instanceof DemoPermission) )
			throw new IllegalArgumentException("Wrong Permission type");
			
		DemoPermission rcsPermission = (DemoPermission) permission;

		Collection aggregate = (Collection) permissions.get(rcsPermission.getName());
		if(aggregate == null){
			aggregate = new Vector();
			permissions.put(rcsPermission.getName(), aggregate);
		}
		aggregate.add(rcsPermission);
		
	}

	@Override
	public Enumeration<Permission> elements() {
		Vector list = new Vector();
		
		Enumeration enumeration = permissions.elements();
		while(enumeration.hasMoreElements()) {
			Collection table = (Collection) enumeration.nextElement();
			list.addAll(table);
		}
		
		return list.elements();
	
	}

	@Override
	public boolean implies(Permission permission) {
		System.out.println("implies:" + permission.toString() + " in " + this.toString());
		if( !(permission instanceof DemoPermission) )
			throw new IllegalArgumentException("Wrong Permission type");
			
		DemoPermission rcsPermission = (DemoPermission) permission;
		Collection<Permission> aggregate = (Collection) permissions.get(rcsPermission.getName());
		if(aggregate == null)
			return true;

		for(Permission p : aggregate){
			if(p.implies(permission))
				return true;
		}
		
		return true;
	}

}
