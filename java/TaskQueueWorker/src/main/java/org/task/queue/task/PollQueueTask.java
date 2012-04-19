package org.task.queue.task;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URLEncoder;
import java.util.Iterator;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.task.queue.TaskQueuePlugin;
import org.task.queue.topic.Message;
import org.task.queue.topic.MessageTaskMapping;

/**
 * 具体的查询某一个task queue.
 * @author deon
 */
public class PollQueueTask implements Runnable {
	public transient static Log log = LogFactory.getLog("queue");
	private MessageTaskMapping mapping = null;
	
	public PollQueueTask(MessageTaskMapping m){
		this.mapping = m;
	}
	
	
	@SuppressWarnings("unchecked")
	@Override
	public void run() {
		if(mapping.queueLock.tryLock()){
			try{
				poll();
			}finally{
				mapping.lastPoll = System.currentTimeMillis();
				mapping.queueLock.unlock();
			}
		}
	}
	
	private void poll(){
		JSONObject result = getMessages();
		JSONArray data = null;
		if(result != null){
			data = result.getJSONArray("data");
		}
		if(data == null){
			log.info("failed to get message data.");
			return;
		}
		
		Message m = null;
		JSONObject obj = null;
		for(Iterator<JSONObject> iter = data.listIterator(); iter.hasNext(); ){
			obj = iter.next();
			m = new Message();
			m.id = obj.getInt("id");
			if(m.id > 0){
				m.data = obj;
				mapping.addMessage(m);
			}
		}
		
		if(mapping.pendingMessage().size() > 0){
			TaskQueuePlugin.getInstance().threadPool.execute(new TriggerTaskWorkerTask(mapping));
		}		
	}
	
	private JSONObject getMessages(){
		HttpURLConnection conn = null;
		JSONObject obj = null;
		InputStream ins = null;
		String data = null;
		try{
			// TODO Auto-generated method stub
			log.info("Get task queue info from url:" + mapping.queue.toString());
			conn = (HttpURLConnection)mapping.queue.openConnection();
			/**
			 * Use post method to avoid request caching in http proxy.
			 */
			conn.setRequestMethod("POST");
			conn.setRequestProperty("Content-Type",
                    "application/x-www-form-urlencoded");
			conn.setDoInput(true);
			conn.setDoOutput(true);
			
			/**
			 * Write a dummy string, the django server will error if no data with post request.
			 */
			OutputStream os = conn.getOutputStream();
			os.write("format=json".getBytes());
			if(mapping.filter != null && mapping.filter.trim().length() > 0){
				os.write(("&filter=" + URLEncoder.encode(mapping.filter, "utf8")).getBytes());
			}
			os.close();
			
			byte[] buffer = new byte[1024 * 1024];
			ins = conn.getInputStream();
			int offset = 0;
			for(int len = 0; len >= 0; ){
				len = ins.read(buffer, offset, buffer.length - offset);
				if(len > 0){
					offset += len;
				}
			}
			data = new String(buffer, 0, offset, "utf8");
			obj = JSONObject.fromObject(data);
		}catch(Exception e){
			String msg = "load queue task error.";
			if(data != null){
				msg += " data:" + data;
			}
			log.error(msg, e);
		} finally{
			if(ins != null){
				try {
					ins.close();
				} catch (IOException e) {
					log.error("close error:" + e.toString(), e);
				}
			}
			if(conn != null){
				conn.disconnect();
			}
		}		
		return obj;
	}

}
