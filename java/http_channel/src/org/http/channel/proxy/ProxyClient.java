package org.http.channel.proxy;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.locks.ReentrantLock;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.mortbay.util.ajax.Continuation;

/**
 * 一个ProxyClient, 表示一个代理的转发规则。
 * 
 * 1. 请求属于转发到什么Client.
 * 
 * 2. Clinet下面所有的代理会话和状态。 
 * 
 * @author deon
 *
 */
public class ProxyClient {
	private Log log = LogFactory.getLog("client");
	
	/**
	 * 属性设置为Pulbic,为了显示Status时可以读到内部数据
	 */
	public Map<String, ProxySession> sessions = new HashMap<String, ProxySession>();
	public Queue<String> waiting = new ConcurrentLinkedQueue<String>();
	public Queue<String> blocking = new ConcurrentLinkedQueue<String>();
	/**
	 * 保存等待下载HTTP请求的连接。
	 */
	public Queue<Continuation> clients = new ConcurrentLinkedQueue<Continuation>();
	
	public Queue<ProxySession> doneSession = new ConcurrentLinkedQueue<ProxySession>();
	
	public String accessKey = null;
	/**
	 * 最后响应时间，用来计算超时客户端。
	 */
	public Date lastActive = new Date();
	/**
	 * 确保只有一个线程在作Schedule操作.
	 */
	private final ReentrantLock lock = new ReentrantLock();
	
	/**
	 * 用来应答代理客户端，表示连接建立成功。
	 */
	private final RemoteStatus echo = new RemoteStatus();
	public ProxyClient(){		
		echo.connection = RemoteStatus.CONNEDTED;
	}

	public void newSession(ProxySession s){
		sessions.put(s.sid, s);
		waiting.add(s.sid);
		schedule();
	}	
	
	/*
	public void closeSession(String sid){
		sessions.remove(sid);
	}*/
	
	public ProxySession doneSession(String sid){
		blocking.remove(sid);
		schedule();
		
		ProxySession s = sessions.remove(sid);
		if (s != null){
			doneSession.add(s);
			while(doneSession.size() > 20){
				doneSession.poll();
			}
		}
		return s;
	}
	
	/**
	 * 有新的下载队列。
	 * @param s
	 */
	public void activeContinuation(Continuation s){
		log.debug("new Task tracker:" + s.toString());
		clients.add(s);
		try {
			ObjectOutputStream o = (ObjectOutputStream)s.getObject();
			o.writeObject(echo);
			o.flush();
			//log.info("write command...");
		} catch (IOException e) {
			log.error(e.toString(), e);
		}
		schedule();
	}
	
	public int activeClient(){
		return this.clients.size();
	}
		
	
	/**
	 * 调度一次把需要转发的请求写到对应的下载队列中。
	 * @param s -- 如果为空,从队列中去一个。
	 */
	private void schedule(){
		if(lock.tryLock()){
			cleanUpTimeoutSession();
			try{
				ObjectOutputStream os = null;
				ProxySession session = nextSession();
				for(Continuation s = clients.peek(); s != null; ){
					//会话已经不在阻塞等待状态。
					s = clients.poll();
					if(s == null || s.isResumed() || (!s.isNew() && !s.isPending())) continue;
					if(session != null){
						try {
							os = (ObjectOutputStream)s.getObject();
							os.writeObject(session);
							os.flush();
							blocking.add(session.sid);
							session = nextSession();
						} catch (Throwable e) {
							log.debug("Task tracker:" + s.toString());
							log.debug("Write session Error:" + e.toString(), e);
							continue;
						}
					}
					clients.add(s);
					if(session == null)break;
				}
				//被取出来了，但是没有发送成功，需要从新放回waiting的队列里面。
				if(session != null){
					waiting.add(session.sid);
				}
			}finally{
				lock.unlock();
			}
		}
	}
	
	private void cleanUpTimeoutSession(){
		long cur = System.currentTimeMillis();
		for(ProxySession s: new ArrayList<ProxySession>(sessions.values())){
			if(cur - s.createTime > 1000 * 60 * 3){
				sessions.remove(s.sid);
				waiting.remove(s.sid);
				blocking.remove(s.sid);
				log.info(String.format("Remove time out session:%s, waiting:%s", s.sid, (cur - s.createTime) / 1000));
			}
		}
	}
	
	
	private ProxySession nextSession(){
		ProxySession s = null;
		for(int i = waiting.size(); i > 0 && s == null; i--){
			String key = waiting.poll();
			if(key == null)break;
			s = sessions.get(key);
			if(s == null)continue;
			break;
		}
		return s;
	}
}
