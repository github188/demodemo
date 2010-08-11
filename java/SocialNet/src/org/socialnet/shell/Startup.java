package org.socialnet.shell;

public class Startup {
	public static final String VERSION = "0.1.0";
	private String conf = "sns.conf";

	public Startup(){		
	}

	public Startup(String conf){
		this.conf = conf;		
	}
	
	public static void main(String[] args){
		Startup st = null;
		if(args.length == 0){
			st = new Startup();
		}else if (args.length == 1){
			if(args[0].equals("--version")){
				System.out.println("SocialNet " + VERSION);
				System.exit(0);
			}else if(args[0].equals("--help")){
				help();
			}else if(args[0].endsWith(".conf")){
				st = new Startup(args[0]);
			}else {
				help();
			}
		}else if(args.length > 1){
			help();
		}
		st.initDirectory();
		st.initLogging();
		st.startHttpServer();
	}
	
	private void initLogging(){
		
	}
	
	private void initDirectory(){
		
	}
	
	private void startHttpServer(){
		
	}	
	
	private static void help(){
		System.out.println("SocialNet.jar <config>");
		System.exit(0);
	}
}
