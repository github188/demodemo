package org.http.channel.client;

import static org.http.channel.client.Main.LOCAL;
import static org.http.channel.client.Main.REMOTE;

import java.io.EOFException;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.OutputStream;
import java.net.ConnectException;
import java.net.HttpURLConnection;
import java.net.Proxy;
import java.net.URL;
import java.util.List;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.http.channel.proxy.ProxySession;
import org.http.channel.settings.Settings;

public class ProxyClient {
	private Log log = LogFactory.getLog("gate");
	
	private Settings settings = null;
	private ThreadPoolExecutor proxyWorkerPool = null;
	private ThreadPoolExecutor proxyCommandPool = null;
	private String remote = null;
	private String local = null;
	private Timer timer = new Timer();
	private long commandCount = 0;
	
	public ProxyClient(Settings s){
		this.settings = s;
		
		this.remote = s.getString(REMOTE, "");
		this.local = s.getString(LOCAL, "");
	}
	
	public void run(){
		int core_thread_count = 5;
		proxyWorkerPool = new ThreadPoolExecutor(
				core_thread_count,
				settings.getInt(Settings.MAX_ROUTE_THREAD_COUNT, 500),
				60, 
				TimeUnit.SECONDS, 
				new LinkedBlockingDeque<Runnable>(core_thread_count * 2)
				);

		proxyCommandPool = new ThreadPoolExecutor(
				1, 3, 60, TimeUnit.SECONDS, 
				new LinkedBlockingDeque<Runnable>(50)
				);
		proxyCommandPool.execute(new RequestTracker());		
		log.info(String.format("Start proxy client, remote:%s, local:%s", remote, local));
		
		timer.scheduleAtFixedRate(new TrackerScheduler(), 100, 60 * 1000);		
	}
	
	/**
	 * 判断需要多少个下载命令的线程。
	 * @author deon
	 */
	class TrackerScheduler extends TimerTask {
		@Override
		public void run() {
			if(proxyCommandPool.getActiveCount() < 1) {
				proxyCommandPool.execute(new RequestTracker());
			}else if(commandCount > 10 && proxyCommandPool.getActiveCount() < 3){
				proxyCommandPool.execute(new RequestTracker());
			}
			log.info(String.format("Active thread:%s, executed proxy command count:%s", 
					proxyCommandPool.getActiveCount(),
					commandCount));
			commandCount = 0;
		}
	}
	
	/**
	 * 用来从远程服务器，下载需要请求的HTTP命令。
	 * @author deon
	 */
	class RequestTracker implements Runnable{
		@Override
		public void run() {
			ObjectInputStream ios = null;
			HttpURLConnection connection = null;
			try {
				URL request = new URL(remote + "/~/request");
				connection = (HttpURLConnection )request.openConnection();
				connection.setReadTimeout(1000 * 60 * 5);
				connection.setConnectTimeout(1000 * 30);

				connection.setRequestMethod("POST");
				connection.setDoInput(true);
				connection.connect();
				
				ios = new ObjectInputStream(connection.getInputStream());
				for(Object obj = ios.readObject(); obj != null; obj = ios.readObject()){
					commandCount++;
					log.debug("Request:" + obj.toString());
					if(obj instanceof ProxySession){
						proxyWorkerPool.execute(new TaskWorker((ProxySession)obj));
					}
				}
				
			}catch(ConnectException conn){
				log.info(String.format("Failed connection to '%s', msg:%s", remote, conn.toString()));
			}catch(IOException eof){
				log.info(String.format("EOF read proxy command. msg:%s", eof.toString()));
			}catch (Exception e) {
				log.error(e.toString(), e);
			} finally {
				if (ios != null)
					try {
						ios.close();
					} catch (IOException e) {
						log.error(e.toString(), e);
					}
				if (connection != null) connection.disconnect();
				//threadPool.execute(new RequestTracker());
			}
		}
	}

	/**
	 * 用来访问本地的最终服务，并把结果转发回代理服务器。
	 * @author deon
	 */
	class TaskWorker implements Runnable{
		private ProxySession request = null;
		public TaskWorker(ProxySession request){
			this.request = request;
		}

		@Override
		public void run() {
			log.info(String.format("Local:%s, session:%s", request.queryURL, request.sid));
			HttpURLConnection connection = null;
			InputStream ins = null;
			try {
				URL localURL = new URL(local + request.queryURL);
				connection = (HttpURLConnection) localURL.openConnection(Proxy.NO_PROXY);
				connection.setRequestMethod(request.method);
				
				/**
				 * Set default header.
				 */
				connection.setRequestProperty("Content-type", "text/html");
				
				for(String key: request.header.keySet()){
					//log.info(key + "-->" + request.header.get(key));
					if(key.equals("Host")){
						continue;
					}
					connection.setRequestProperty(key, request.header.get(key));
				}
				/*
				if(!contentType){
					connection.setRequestProperty("Content-type", "text/html");
				}*/
				
				connection.setDoInput(true);
				if(this.request.content != null){
					OutputStream os = null;
					connection.setDoOutput(true);
					os = connection.getOutputStream();
					os.write(this.request.content);
					os.close();
				}
				connection.connect();
				ins = connection.getInputStream();
				
				log.debug(String.format("Local Done [%s] '%s' length:%s, type:%s",
						connection.getResponseCode(),
						request.queryURL,
						connection.getContentLength(),
						connection.getContentType()
						));
				this.uploadResponse(ins, connection.getHeaderFields(), connection.getResponseCode());
			}catch(ConnectException conn){
				log.info(String.format("Failed connection to '%s', msg:%s", local, conn.toString()));
			} catch (Exception e) {
				log.error(e.toString(), e);
			}
		}
		
		private void uploadResponse(InputStream in, Map<String, List<String>> header, int code) throws IOException{
			log.debug(String.format("Proxy return:%s, sid:%s", this.request.queryURL, this.request.sid));

			HTTPForm form = new HTTPForm(new URL(remote + "/~/reponse"));
			form.setParameter("sid", this.request.sid);
			form.setParameter("status", code + "");
			
			
			String values = "";
			for(String k: header.keySet()){
				List<String> val = header.get(k);
				values = val.get(0);
				for(int i = 1; i < val.size(); i++){
					values += "," + val.get(i);
				}
				//log.info(String.format("%s->%s", k, values));
				if(k != null){
					form.setParameter(k, values);
				}
			}
			form.startFileStream("file0", "file", in);
			String data = form.read();
			form.close();
			log.debug(String.format("Proxy done:%s, msg:%s", this.request.queryURL, data));
		}
	}
}
