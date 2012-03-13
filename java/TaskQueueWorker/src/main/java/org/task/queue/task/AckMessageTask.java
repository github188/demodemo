package org.task.queue.task;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import net.sf.json.JSONObject;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 向task queue确认处理的结果.
 * @author deon
 *
 */
public class AckMessageTask implements Runnable {
	public transient static Log log = LogFactory.getLog("queue");
	public URL ackURL = null;
	private int id;
	
	public AckMessageTask(URL url, int msgId){
		this.ackURL = url;
		this.id = msgId;
	}

	@Override
	public void run() {
		HttpURLConnection conn = null;
		JSONObject obj = null;
		InputStream ins = null;
		String data = null;
		try{
			// TODO Auto-generated method stub
			log.info("Ack message url:" + ackURL.toString());
			conn = (HttpURLConnection)ackURL.openConnection();
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
			os.write(("msg_id=" + id).getBytes());
			os.close();
			
			byte[] buffer = new byte[512];
			ins = conn.getInputStream();
			int offset = 0;
			for(int len = 0; len >= 0; ){
				len = ins.read(buffer, offset, buffer.length - offset);
				if(len > 0){
					offset += len;
				}
			}
			data = new String(buffer, 0, offset, "utf8");
			log.info("ack message return:" + data);
		}catch(Exception e){
			String msg = "ack message error.";
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
	}

}
