package org.http.channel.proxy;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.util.HashMap;
import java.util.Map;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.locks.ReentrantLock;

import org.mortbay.log.Log;
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
	private Map<String, ProxySession> sessions = new HashMap<String, ProxySession>();
	private Queue<String> waiting = new ConcurrentLinkedQueue<String>();
	private Queue<String> blocking = new ConcurrentLinkedQueue<String>();
	
	private final ReentrantLock lock = new ReentrantLock();
	
	/**
	 * 保存等待下载HTTP请求的连接。
	 */
	private Queue<Continuation> clients = new ConcurrentLinkedQueue<Continuation>();
	
	public ProxyClient(){
		
	}

	public void newSession(ProxySession s){
		sessions.put(s.sid, s);
		waiting.add(s.sid);
		schedule();
	}	
	
	public void closeSession(String sid){
		sessions.remove(sid);
	}
	
	/**
	 * 有新的下载队列。
	 * @param s
	 */
	public void activeContinuation(Continuation s){
		clients.add(s);
		schedule();
	}
	
	/**
	 * 调度一次把需要转发的请求写到对应的下载队列中。
	 * @param s -- 如果为空,从队列中去一个。
	 */
	private void schedule(){
		if(lock.tryLock()){
			try{
				ObjectOutputStream os = null;
				ProxySession session = nextSession();
				for(Continuation s = clients.peek(); s != null; s = clients.peek()){
					//会话已经不在阻塞等待状态。
					if(s.isResumed()) continue;
					if(session != null){
						try {
							os = (ObjectOutputStream)s.getObject();
							os.writeObject(session);
							blocking.add(session.sid);
							session = nextSession();
						} catch (IOException e) {
							Log.debug("Write session Error:" + e.toString());
							continue;
						}
					}
					clients.add(s);
					if(blocking.size() > 10 || session == null)break;
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
	
	
	private ProxySession nextSession(){
		ProxySession s = null;
		for(String key = waiting.poll(); key != null; key = waiting.poll()){
			s = sessions.get(key);
			if (s != null)break;
		}
		return s;
	}
}
