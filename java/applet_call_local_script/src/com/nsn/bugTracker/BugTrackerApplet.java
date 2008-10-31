package com.nsn.bugTracker;

import java.applet.Applet;
import java.awt.Graphics;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.URL;
import java.security.AccessController;
import java.security.PrivilegedAction;

public class BugTrackerApplet extends Applet {
	protected static final Logger log = new Logger();

	private static final long serialVersionUID = 1L;
	private ExecContext exec = new ExecContext();
	private File tempDir = null;
	
	public BugTrackerApplet(){
	}
	
	public void paint(Graphics g) {
		String user = System.getenv("USERNAME");
		g.drawString("Bug Tracker 0.1", 10, 30);
		g.drawString(user, 10, 50);
	}

	@SuppressWarnings("unchecked")
	public String getCurrentUser(){
		return (String)AccessController.doPrivileged(
				new PrivilegedAction() {
					public Object run(){
						return System.getenv("USERNAME");
					}
				});	
	}
	
	@SuppressWarnings("unchecked")
	public void updateBugStatue(final String bug){
		if (bug == null || bug.trim().equals("")) {
			showStatus("have not bug status to update!");
			return;
		}
		
		showStatus("Update bug status, bug list:" + bug);
		
		AccessController.doPrivileged(
				new PrivilegedAction() {
					public Object run(){
						execScript("bug_updater_mini.py", 
							  new String[]{"-r", getParameter("UpdateUrl"), "-b", bug},
							  new ProcessFilter("*", ""){
								@Override
								protected int handle(String line, 
													 BufferedReader reader,
													 PrintWriter out) throws IOException {
									log.debug(line);
									return 0;
								}
							  }
						);
						return "";
					}
				});	
	}
	
	public void execScript(final String script, 
						   final String[] args, 
						   final ProcessFilter filter){
		new Thread(){
			public void run(){
				if (tempDir == null) {
					initWorkDir();
				}
				File scriptFile = downloadScript(script);
				if (scriptFile != null){
					exec.setShell("python.exe " + scriptFile.getAbsolutePath());
					int ret = exec.execProcess(args, filter);
					log.info("script " + script + "done ret:" + ret);
					scriptFile.delete();
				}
			}
		}.start();
	}

	public File downloadScript(String script){
		byte[] buffer = new byte[1024 * 4];
		File scriptFile = null;
		OutputStream fout = null;
		
		InputStream fin = this.getClass().getClassLoader().getResourceAsStream(script);
		if (fin == null) {
			log.info("Not found script:" + script);
			return null;
		}

		try{
			scriptFile = File.createTempFile("script", null, this.tempDir);
			fout = new FileOutputStream(scriptFile);
			for(int len = buffer.length; len == buffer.length;) {
				len = fin.read(buffer);
				fout.write(buffer, 0, len);
			}
		}catch(IOException e){
			scriptFile = null;
			log.error(e.toString(), e);
		}finally {
			try {
				fin.close();
			} catch (IOException e) {
				log.error(e.toString(), e);
			}
			if(fout != null)
				try {
					fout.close();
				} catch (IOException e) {
					log.error(e.toString(), e);
				}
		}
		
		return scriptFile;
	}
	
	private void initWorkDir(){
	    try {
	    	tempDir = File.createTempFile("bug", "");
	    	tempDir.delete();
	    	tempDir.mkdirs();
		} catch (IOException e) {
			log.error(e.toString(), e);
			tempDir = new File(".");
		}
		log.info("temp:" + tempDir.getAbsolutePath());
		exec.setRootDir(tempDir.getAbsolutePath());		
	}
}
