package org.goku.core.model;

public class Location {
	public static final String ORM_TABLE = "location";
	public static final String[] ORM_FIELDS = new String[]{"uuid", "name",
		"parent", };
	public static final String[] ORM_PK_FIELDS = new String[]{"uuid"};
	
	public String uuid = null;	
	public String name = null;
	public String parent = null;
}
