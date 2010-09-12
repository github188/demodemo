package com.nsn.reporting.chart;

import java.io.File;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class DirTest {

	/**
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		File root = new File("C:/USERS/_doc/../cc/");
		root = new File(root, "atm");
		System.out.println(root.getAbsolutePath());
		
		Pattern pa = Pattern.compile("sid/([[^\\./]]+)");
		
		Matcher ma = pa.matcher("aa/sid/xx-_xaaa");
		if(ma.find()){
			String xx = ma.group(1);
			System.out.println("group:" + xx);
		}else {
			System.out.println("not found");
		}
		
		//ma.
		//Matcher ma;
		//pa = 
		//ma = pa.		
		
		//"xxxx/sid/sss.jpg".matches("sid/(\\w+)[^\\./]")
		//"".m

		System.out.println("".split("\\.")[0]);
	}

}
