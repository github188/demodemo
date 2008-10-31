package com.nsn.bugTracker;

import java.io.*;
import java.util.*;

/**
 * Tools of executing a shell commands.
 * 
 * It's include a system shell runtime for different OS(window is "CMD.exe", linux 
 * is "bash"), and a root path of every executed commands.
 * 
 * @author user
 *
 */
public class ExecContext {
	protected static final Logger log = new Logger();

	public static final int RET_OK = 0;

	protected Collection<ProcessFilter> filters = new Vector<ProcessFilter>();
	private File pwd = null;
	private Runtime tools = Runtime.getRuntime();

	private String[] shell = null; //"cmd.exe ";
	private String[] env = null; //new String[] {};

	public ExecContext() {

	}

	public void setShell(String shell) {
		if (shell != null) {
			this.shell = shell.split("\\s");
		}
	}

	public boolean setRootDir(String path) {
		pwd = new File(path);
		if (pwd.exists() && pwd.isDirectory()) {
			return true;
		} else {
			return false;
		}
	}

	public void addFilter(ProcessFilter filter) {
		filters.add(filter);
	}

	public void removeFilter(ProcessFilter filter) {
		filters.remove(filter);
	}

	public Process exec(String command) {
		return exec(new String[] { command });
	}

	public Process exec(String[] command) {
		Process process = null;
		try {
			String[] tmp = new String[shell.length + command.length];
			System.arraycopy(shell, 0, tmp, 0, shell.length);
			System.arraycopy(command, 0, tmp, shell.length, command.length);
			if (log.isDebugEnabled()) {
				log.debug("cmds: " + listEnv(tmp));
			}
			process = tools.exec(tmp, env, pwd);
		} catch (IOException e) {
			log.error("Execute shell commands error! " + "cmds: "
					+ listEnv(command) + "\npwd:" + pwd.getAbsolutePath()
					+ "\n env:" + listEnv(env));
			log.error(e.toString(), e);
		}

		return process;
	}

	public int execProcess(String[] command) {
		return execProcess(command, filters);
	}

	public int execProcess(String[] command, ProcessFilter filter) {
		//Process p = exec(command);
		Vector<ProcessFilter> fs = new Vector<ProcessFilter>();
		fs.add(filter);

		return execProcess(command, fs);
	}

	public int execProcess(String[] command, Collection<ProcessFilter> filter) {
		Process p = exec(command);

		return process(p, filter);
	}

	public int process(final Process p, Collection<ProcessFilter> filters) {

		PrintWriter stdIn = new PrintWriter(p.getOutputStream());
		ReadPipe stdOut = new ReadPipe(p.getInputStream(), stdIn,
				"Script STDOUT:", filters);
		ReadPipe errOut = new ReadPipe(p.getErrorStream(), stdIn,
				"Script STDERR:", filters);

		Timer t = new Timer();
		t.schedule(new TimerTask() {
			public void run() {
				//current.interrupt();
				p.destroy();
			}
		}, 1000 * 60 * 10);

		try {
			log.info("waiting script......");
			p.waitFor();
			t.cancel();
			stdOut.close();
			errOut.close();
			stdIn.close();
			stdOut.join();
			errOut.join();
		} catch (InterruptedException e) {
			log.fatal("The script can't exits!:" + p.toString());
		}

		return p.exitValue();
	}

	private String listEnv(String[] arg) {
		StringBuffer buf = new StringBuffer();
		if( arg != null){
			for (String s : arg) {
				buf.append(s + ",");
			}
		}
		return buf.toString();
	}

	class ReadPipe extends Thread {
		private BufferedReader pipeOut = null;
		private boolean running = false;
		private PrintWriter stdIn = null;
		private Collection<ProcessFilter> filters = null;

		//private String logPropmt = null;
		public ReadPipe(InputStream in, PrintWriter stdIn, String logPropmt,
				Collection<ProcessFilter> filters) {
			pipeOut = new BufferedReader(new InputStreamReader(in));
			this.stdIn = stdIn;
			//this.logPropmt = logPropmt;
			this.setName(logPropmt);
			this.filters = filters;
			running = true;
			this.start();
		}

		public void run() {
			String s = null;
			try {
				while (running || s != null) {
					s = pipeOut.readLine();
					if (s != null) {
						log.debug(s);
						for (ProcessFilter f : this.filters) {
							if (f.filter(s, pipeOut, stdIn) == ProcessFilter.RETURN) {
								break;
							}
						}
					} else if (running) {
						synchronized (this) {
							try {
								this.wait(100);
								continue;
							} catch (InterruptedException e) {
								continue;
							}
						}
					}
				}
			} catch (IOException e) {
				log.error(e.toString(), e);
			} finally {
				try {
					if (pipeOut != null)
						pipeOut.close();
				} catch (IOException e) {
					log.error(e.toString(), e);
				}
			}
		}

		public void close() {
			running = false;
		}
	}
}
