package com.sigma.demo.test;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Properties;

import com.sigma.demo.util.ExecContext;
public class ExecTest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		ExecContext exec = new ExecContext();
		exec.setRootDir("D:/tmp");
		
		Properties prop = System.getProperties();
		System.out.println("......................");
		prop.list(System.out);
		
		Process p = exec.exec("dir /w");
		BufferedReader cmdInput = new BufferedReader(new InputStreamReader(p.getInputStream()));
		
		System.out.println("start..");
		String s = null;
		do{
			try {
				s = cmdInput.readLine();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			System.out.println(s);
		}while(s != null);
		p.destroy();
		System.out.println("exits");
	}

}
