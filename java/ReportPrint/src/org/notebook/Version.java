package org.notebook;

public final class Version {
	private static final String version = "1.0.10";
	public static String getVersion(){
		return version;
	}
	
	public static String getName(){
		return "蓝星打印";
	}
	
	public static String getVerName(){
		return getName() + " " + getVersion();
	}	
}
