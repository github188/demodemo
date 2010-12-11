package org.goku.core.model;

import java.util.Date;

public class User {
	public static final String ORM_TABLE = "user_account";
	public static final String[] ORM_FIELDS = new String[]{"name", "password",
		"display", "status", "lastActive", 
		};
	public static final String[] ORM_PK_FIELDS = new String[]{"name"};
	
	public String name;
	public String password;

	public String display;
	
	public String status = "ok";
	public Date lastActive;
}
