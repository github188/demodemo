package org.socialnet.core;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 查询 会话管理：
 * 当多线程查询时，需要给当前查询一个唯一的标识。Session ID是一个0~Max Session
 * 的整数。Session ID释放后可以重新分配给下一个查询。
 * @author Lenovo
 *
 */
public class SessionManager {
	private BlockingQueue<Integer> sessionQueue = null;
	private Log log = LogFactory.getLog(SessionManager.class);
	
	public SessionManager(int maxSession){
		sessionQueue = new ArrayBlockingQueue<Integer>(maxSession);
		for(int i = 0; i < maxSession; i++){
			try {
				sessionQueue.put(i);
			} catch (InterruptedException e) {
				log.error(e);
			}
		}
	}
	
	/**
	 * 分配一个可用的 Session ID, 如果没有可用的Session者返回-1,或等待。
	 * @param block
	 * @return
	 */
	public int newSession(boolean block){
		int sessionId = -1;
		
		if(block){
			try {
				sessionId = this.sessionQueue.take();
			} catch (InterruptedException e) {
				log.error(e);
			}
		}else if(this.sessionQueue.size() > 0){
			sessionId = this.sessionQueue.poll();
		}
		log.debug("create new session, id=" + sessionId);
		
		return sessionId;
	}
	
	public void releaseSession(int sessionId){
		log.debug("release session, id=" + sessionId);
		try {
			sessionQueue.put(sessionId);
		} catch (InterruptedException e) {			
			log.error(e);
		}
	}

}

