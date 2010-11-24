package org.goku.core;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.route.RouteServer;
import org.goku.server.CenterServer;
import org.goku.settings.Settings;


public class Main {
	private Log log = LogFactory.getLog("main");
	/**
	 * @param args
	 */
	public static void main(String[] args){
		if (args.length == 1){
			if(args[0].equals("--version")){
				System.out.println(Version.getName() + " " + Version.getVersion());
				System.exit(0);
			}else if(args[0].equals("--help")){
				help();
			}else if(args[0].equals("--master")){
				new Main().startAsMaster();
			}else if(args[0].equals("--route")){
				new Main().startAsRoute();
			}else {
				help();
			}
		}else{
			help();
		}
	}
	
	private void startAsMaster(){
		Settings settings = new Settings("master.conf");
		new CenterServer(settings).startUp();
	}
	
	private void startAsRoute(){
		Settings settings = new Settings("master.conf");
		new RouteServer(settings).startUp();
	}

	private static void help(){
		System.out.println("Goku.jar [--master] [--route]");
		System.exit(0);
	}
}
