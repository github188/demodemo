package org.socialnet.core;

public class SearchException extends Exception {
	public static final String NO_SESSION = "1001:Session达到最大限制。";
	public SearchException(String msg){
		super(msg);
	}
}
