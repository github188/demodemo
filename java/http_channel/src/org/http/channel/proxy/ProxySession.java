package org.http.channel.proxy;

import java.io.Serializable;
import java.util.Map;

import org.mortbay.util.ajax.Continuation;

/**
 * 一次HTTP的会话数据.
 * @author deon
 */
public class ProxySession  implements Serializable{
	private static final long serialVersionUID = 8989783162442987982L;
	
	public String sid = null;
	public Map<String, String> header = null;
	public String method = null;
	public String queryURL = null;
	public byte[] content = null;
	
	public int status = 0;
	
	/**
	 * 一个Jetty6的特有对象，Jetty7后已经成为Servlet3的标准。实现异步Servlet的一个机制。
	 * http://docs.codehaus.org/display/JETTY/Continuations
	 * 
	 * 里面有一个HttpResponse的引用。
	 */
	public transient Continuation continuation = null;
	public long createTime = 0;
	
	public String toString(){
		return "sid:" + this.sid + ", url:" + this.queryURL;
	}
}
