package org.http.channel.proxy;

import java.util.HashMap;
import java.util.Map;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

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
	private Queue<String> newState = new ConcurrentLinkedQueue<String>();
	
	/**
	 * 保存等待下载HTTP请求的连接。
	 */
	private Queue<Continuation> clients = new ConcurrentLinkedQueue<Continuation>();
	
	public ProxyClient(){
		
	}
	
	/**
	 * 返回一个处于Waiting状态的Session，如果没有等待的Session返回null.
	 * @return
	 */
	public ProxySession waitingSession(){
		String id = newState.poll();
		if (id != null){
			return sessions.get(id);
		}
		return null;
	}

	public void newSession(ProxySession s){
		sessions.put(s.sid, s);
		newState.add(s.sid);
		schedule(null);
	}	
	
	public void closeSession(String sid){
		sessions.remove(sid);
	}
	
	/**
	 * 有新的下载队列。
	 * @param s
	 */
	public void activeContinuation(Continuation s){
		//clients.add(s);
		schedule(s);
	}
	
	/**
	 * 调度一次把需要转发的请求写到对应的下载队列中。
	 * 
	 * @param s -- 如果为空,从队列中去一个。
	 */
	public void schedule(Continuation s){
		//如果为空
		clients.add(s);
		
	}
}
