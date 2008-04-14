package com.sigma.demo;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Collection;
import java.util.Properties;
import java.util.Vector;

public class ServerLauncher {
	// private static final Logger log =
	// LogManager.getLogger(ServerLauncher.class);
	private static final String DEFAULT_DS = "com.sigma.demo.DefaultServer";

	public static void main(String[] args) {

		ServerLauncher launcher = new ServerLauncher();
		// initParam = initConfig();

		String action = null;
		if (args.length == 1) {
			action = args[0];
		}

		if (action == null || "start".equals(action)) {
			launcher.startDemoServer();
		} else {
			echo("ServerLauncher [start|stop]");
		}
	}

	private void startDemoServer() {
		Thread.currentThread().setName("ServerLauncher");
		echo("Starting up DemoServer....");
		File current = new File(".");
		echo("$DS_HOME=" + current.getAbsolutePath());
		// Set class loader.
		Collection<URL> jars = new Vector<URL>();
		// searchJarfiles(new File(current, "lib/"), jars);
		// searchJarfiles(new File(current, "tomcat-lib/"), jars);
		if (jars.size() > 0) {
			ClassLoader cl = new URLClassLoader(jars.toArray(new URL[] {}),
					Thread.currentThread().getContextClassLoader());
			Thread.currentThread().setContextClassLoader(cl);
		} else {
			echo("Not found libs!");
		}
		try {
			Properties initParam = initConfig();
			Class dsClass = Thread.currentThread().getContextClassLoader()
					.loadClass(System.getProperty("server", DEFAULT_DS));
			Server server = (Server) dsClass.newInstance();
			server.init(initParam);
			server.launcher();
		} catch (Exception e) {
			echo(e.toString());
		}
	}

	private void searchJarfiles(File dir, Collection<URL> list) {
		if (dir.exists() && dir.isDirectory()) {
			echo("$DS_LIB=" + dir.getAbsolutePath());
			for (File jar : dir.listFiles()) {
				if (jar.getName().endsWith(".jar")) {
					try {
						list.add(jar.toURL());
					} catch (MalformedURLException e) {
						echo(e.toString());
					}
				}
			}
		}
	}

	private Properties initConfig() throws IOException {
		Properties initParam = new Properties();
		String config = System.getProperty("config");
		InputStream in = null;
		if (config == null) {
			echo("Load default demo server config.");
			in = Thread.currentThread().getContextClassLoader()
					.getResourceAsStream("DemoServer.cfg");
		} else {
			echo("Load demo server config from url:" + config);
			URL url = new URL(config);
			in = url.openStream();
		}

		if (in != null) {
			initParam.load(in);
		} else {
			echo("Not found init config resource.");
		}
		return initParam;
	}

	private static void echo(String message) {
		System.out.println(message);
	}

}
