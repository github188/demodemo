package com.sigma.demo.test;

import java.io.InputStream;

public class DemoClient {
	public static void main(String[] s){
		InputStream in = DemoClient.class.getResourceAsStream("json_simple.jar");
		System.out.println("json_simple.jar:" + in);
		in = DemoClient.class.getResourceAsStream("DemoClient");
		System.out.println("DemoClient:" + in);
		in = DemoClient.class.getResourceAsStream("DemoClient.class");
		System.out.println("DemoClient.class:" + in);

	}
}
