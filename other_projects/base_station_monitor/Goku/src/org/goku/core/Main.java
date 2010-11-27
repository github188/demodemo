package org.goku.core;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.master.CenterServer;
import org.goku.settings.Settings;
import org.goku.video.VideoRouteServer;


public class Main {
	static{
		if(System.getProperty("org.apache.commons.logging.simplelog.defaultlog") == null){
			System.setProperty("org.apache.commons.logging.simplelog.defaultlog", "trace");	
		}
		if(System.getProperty("org.apache.commons.logging.simplelog.showdatetime") == null){
			System.setProperty("org.apache.commons.logging.simplelog.showdatetime", "true");	
		}
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) throws Exception{
		LogFactory.getLog("main");
		if (args.length == 1){
			if(args[0].equals("--version")){
				System.out.println(Version.getName() + " " + Version.getVersion());
				System.exit(0);
			}else if(args[0].equals("--help")){
				help();
			}else if(args[0].equals("--master")){
				new Main().startAsMaster();
			}else if(args[0].equals("--video")){
				new Main().startAsVideoRoute();
			}else if(args[0].equals("--image")){
				new Main().startAsImageRoute();
			}else {
				help();
			}
		}else{
			help();
		}
	}
	
	private void startAsMaster() throws Exception{
		Settings settings = new Settings("master.conf");
		new CenterServer(settings).startUp();
	}
	
	private void startAsVideoRoute() throws Exception {
		Settings settings = new Settings("video.conf");
		new VideoRouteServer(settings).startUp();
	}
	
	private void startAsImageRoute() throws Exception {
		Settings settings = new Settings("image.conf");
		//new VideoRouteServer(settings).startUp();
	}

	private static void help(){
		System.out.println("java -jar Goku.jar <Option>\n" +
				"    --master              Run as master server.\n" +
				"    --video               Run as video routing server.\n" +
				"    --image               Run as image routing server.\n" +
				"    --version             Show version.\n");
		System.exit(0);
	}
}
