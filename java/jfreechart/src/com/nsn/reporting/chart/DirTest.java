package com.nsn.reporting.chart;

import java.io.File;
import java.io.IOException;

public class DirTest {

	/**
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		File root = new File("C:/USERS/_doc/../cc/");
		root = new File(root, "atm");
		System.out.println(root.getAbsolutePath());
		
		root = new File(root, "a/Temp");

		System.out.println(root.getCanonicalPath());
	}

}
