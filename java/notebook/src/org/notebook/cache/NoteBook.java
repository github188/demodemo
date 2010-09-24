package org.notebook.cache;

import java.io.Serializable;

public class NoteBook implements Serializable{

	private static final long serialVersionUID = -4458662555796010998L;
	public String name = "Deon的记事本";
	public String endpoint = "";
	public Category root = null;
}
