package org.goku.image;

import java.io.IOException;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.channels.DatagramChannel;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ThreadPoolExecutor;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 如果SR+通道ID,在数据库不存在就丟掉。
 * 
 * 1. SR 标识ID， 低位在前，高位在后。
 * 2. 循环累加， 表示UDP包的顺序。
 * 
 * 
 * 
 * 1. 有2种通信协议都需要同时支持， 主要是压缩格式不同。 
 * 1. 老格式图片时间，在图片内。 新的在协议内。
 * 1. 图片数据里面的x0FF没有转义。 
 * 1. 所有告警都是查询上传
 * 1. 告警和点播同时传的时候，优先传告警结束后才传点播图片。
 * 1. 10秒没有响应，触发连接超时告警。
 * 
 * 1. 图片浏览功能, 每页50～100张可设置
 * 1. 需要每张图片都确认
 * 
 * 时间读取时，需要检查时间的合法性，如果出现了非法时间，使用当前服务器的时间。
 * 
 * 图片重传次数是整体确认次数，需要后台可以设置. 异常终止是只当前操作。
 * 
 * 告警查询一般间隔10秒, 每张告警图片最多传一次。 
 *  
 * 
 * @author deon
 *
 */
public class ASC100MX implements Runnable{
	private Log log = LogFactory.getLog("mx");
	
	/**
	 * SR和MX地址的映射关系，用于发送数据的时候，查询向那个MX发UPD包。
	 * 例如：
	 * ea.10 --> 192.168.1.1
	 */
	private Map<String, String> srRoute = new HashMap<String, String>();	
	private Map<String, IncomeQueue> mxIncomeQueue = new HashMap<String, IncomeQueue>();	
	/**
	 * 地址标识符和设备的映射关系，在收到UDP包解析后，查询是否有客户端可以处理当前串口数据。
	 * 例如：
	 * ea.10.1 --> <Object>
	 */
	private Map<String, ASC100Client> clientTable = Collections.synchronizedMap(new HashMap<String, ASC100Client>());	
	private int remotePort = 0;	
	private int localPort = 0;
	
	private boolean isRunning = true;
	private boolean inProcessing = false;
	//发送包的序列
	private int outputCount = 0;
	private DatagramChannel channel = null;
	
	private ThreadPoolExecutor threadPool = null;
	private ByteBuffer writeBuffer = null;
	
	public ASC100MX(int remotePort, int localPort, ThreadPoolExecutor threadPool){
		this.localPort = localPort; 
		this.remotePort = remotePort;
		this.threadPool = threadPool;
	}
	
	/**
	 * 所有数据处理都在同一个线程完成，简化Buffer的管理。
	 */
	public void run(){
	    ByteBuffer readBuffer = ByteBuffer.allocate(65535);	  
	    readBuffer.order(ByteOrder.LITTLE_ENDIAN);
	    
	    writeBuffer = ByteBuffer.allocate(65535);
	    writeBuffer.order(ByteOrder.LITTLE_ENDIAN);
	    
	    SocketAddress address = new InetSocketAddress(this.localPort);
	    DatagramSocket socket = null; //channel.socket();
	    try{
		    channel = DatagramChannel.open();
		    socket = channel.socket();
		    socket.bind(address);
	    }catch(Exception e){
	    	log.error("Failed open local UDP port " + this.localPort + ", error:" + e.toString());
	    }
	    
	    log.info("Started UPD server at port " + this.localPort);
	    while (this.isRunning) {
			try {
				readBuffer.clear();
				InetSocketAddress client = (InetSocketAddress)channel.receive(readBuffer);
				readBuffer.flip();
				log.info(String.format("Recevive from MX %s, size %s", client.toString(), readBuffer.remaining()));
				this.process(client, readBuffer);
			} catch (Exception ex) {
				log.error("Process Error:" + ex.toString(), ex);
			}
	    }
	    
	    try {
	    	if(channel != null){
	    		channel.close();
	    	}
		} catch (IOException e) {
			log.warn("Exception at shutdown UDP server, error:" + e.toString());
		}
	    
	}
	
	public void register(ASC100Client client){
		log.info("Register image client:" + client.getClientId());
		this.clientTable.put(client.getClientId(), client);
		//如果已经存在SR和MX的映射关系则不改变。
		if(!this.srRoute.containsKey(client.getSrId())){
			this.srRoute.put(client.getSrId(), client.defaultMx());
		}
		client.setASC100MX(this);
	}
	
	public synchronized void send(ASC100Client client, ByteBuffer data) throws IOException{
		String locationId = client.getClientId();
		String mxAddr = srRoute.get(client.getSrId());
		if(mxAddr != null){
			writeBuffer.clear();
			InetSocketAddress addr = new InetSocketAddress(mxAddr, remotePort);
			writeBuffer.put((byte)0);
			writeBuffer.put((byte)1);
			//数据填充结束后再计算校验和。
			writeBuffer.putShort((short)(outputCount++ % 65535)); 
			String[] channelAddr = locationId.split("\\.");
			
			writeBuffer.put(Byte.parseByte(channelAddr[0], 16));
			writeBuffer.put(Byte.parseByte(channelAddr[1], 16));
			writeBuffer.put(Byte.parseByte(channelAddr[2], 16));
			writeBuffer.put((byte)0);
			
			//数据长度。
			writeBuffer.putShort((short)data.limit());
			
			writeBuffer.put(data);
			
			ByteBuffer sub = writeBuffer.asReadOnlyBuffer();
			sub.position(4);
			writeBuffer.position(2);
			writeBuffer.putShort(dataSumCheck(sub));
			
			writeBuffer.position(0);
			channel.send(writeBuffer, addr);
			log.info(String.format("Send to MX %s->%s size %s", client.getClientId(), addr.toString(), data.limit()));
		}else {
			log.info("Not found MX server for node:" + locationId);
		}
	}
	
	public void close(){
		this.isRunning = false;
	}
	
	/**
	 * 根据MX地址，把接受到的数据放入一个缓冲队列，进行排队处理。对乱序的包进行重组合。
	 * @param client
	 * @param data
	 */
	protected void process(InetSocketAddress client, ByteBuffer data){
		byte cmd = data.get();
		if(cmd == 0xc5){
			this.processRouteTable(client, data);
		}else if(cmd == 0x00){
			ByteBuffer tmp = ByteBuffer.allocate(data.limit()); 
			tmp.put(data);
			
			ByteBuffer copy = tmp.duplicate();
			copy.get();
			int order = unsignedShort(copy);
			String ipAddr = client.getAddress().getHostAddress();
			IncomeQueue queue = this.mxIncomeQueue.get(ipAddr);
			if(queue == null){
				queue = new IncomeQueue();
				this.mxIncomeQueue.put(ipAddr, queue);
			}
			queue.put(order, tmp);
			//开始在另一个线程处理
			if(!this.inProcessing){
				this.threadPool.execute(this.queueWorker);
			}
		}else {
			log.info(String.format("Unkown package, Control data:0x%x.", cmd));
		}
	}
	
	protected void clientRoute(byte node1, byte node2, byte channel, ByteBuffer data){
		String channelId = String.format("%x.%x.%x", node1, node2, channel);
		ASC100Client client = this.clientTable.get(channelId);
		if(client != null){
			log.info("Process data client:" + channelId + ", size:" + data.remaining());
			client.process(data);
		}else {
			log.debug("Unkown client id:" + channelId);
		}
	}
	
	protected void processRouteTable(InetSocketAddress mx, ByteBuffer data){
		byte channelCount = data.get();
		data.getShort();
		
		String ipAddr = mx.getAddress().getHostAddress();
		for(int i = 0; i < channelCount; i++){
			byte node2 = data.get();
			byte node1 = data.get();
			String srID = String.format("%x.%x", node1, node2);
			String oldMx = srRoute.get(srID);
			if(oldMx == null || !oldMx.equals(ipAddr)){
				srRoute.put(srID, ipAddr);
				updateASC100Data(srID, ipAddr);
				log.info(String.format("Update Mx table:%s->%s", srID, ipAddr));
			}
		}
	}
	
	protected void updateASC100Data(String sr, String mx){
		Collection<ASC100Client> clients = new ArrayList<ASC100Client>();
		clients.addAll(clientTable.values());
		for(ASC100Client c: clients){
			if(c.getSrId().equals(sr)){
				c.info.locationId = mx + ":" + c.getClientId();
				ImageRouteServer.getInstance().storage.save(c.info, new String[]{"locationId"});
			}
		}
	}
	
	protected short dataSumCheck(ByteBuffer data){
		short sum = 0;
		while(data.hasRemaining()){
			sum += data.get();
		}
		return sum;
	}
	
	public static int unsignedShort(ByteBuffer buffer){
		byte d1 = buffer.get();
		byte d2 = buffer.get();
		int d = ((d2 << 8)  | 0x00ff) & 
				 (d1         | 0xff00);
		return d;
	}
	
	public static int unsignedShort3(ByteBuffer buffer){
		byte d1 = buffer.get();
		byte d2 = buffer.get();
		byte d3 = buffer.get();
		int d = ((d3 << 16)  | 0x00ffff) & 
				((d2 << 8)   | 0xff00ff) & 
				 (d1         | 0xffff00);
		return d;
	}
	
	/**
	 * 遍历所有输入队列，查询满足顺序的包全部处理。
	 */
	private Runnable queueWorker = new Runnable() {
		@Override
		public synchronized void run() {
			try{
				inProcessing = true;
				Collection<IncomeQueue> qList = new ArrayList<IncomeQueue>();
				qList.addAll(mxIncomeQueue.values());
				for(IncomeQueue q: qList){
					for(ByteBuffer buff = q.getNext(); buff != null; buff = q.getNext()){
						int count = buff.get();
						buff.getShort(); //去掉UDP包序号。
						for(; count > 0; count--){
							byte node2 = buff.get();
							byte node1 = buff.get();
							byte channelId = buff.get();
							buff.get();
							int len = unsignedShort(buff); // i.getShort();
							ByteBuffer sub = buff.asReadOnlyBuffer();
							sub.limit(buff.position() + len);
							clientRoute(node1, node2, channelId, sub);
						}
					}
				}
			}catch(Throwable e){
				log.error("Process in-come queue error:" + e.toString(), e);
			}finally{
				inProcessing = false;
			}
		}
		
	};
	
	/**
	 * 一个简单的队列，修正UPD的接收乱序问题。每次接收到一个包先放到哟个队列，让后在
	 * 判断下一个应处理的包。如果下一个包是需要处理的包开始处理。如果不是则等待1秒钟。
	 * 如果超时则跳过丢失的包。
	 */
	class IncomeQueue {
		public int next_index = -1;
		public long last_get_time = 0;
		public int size = 0;
		public ByteBuffer[] queue = new ByteBuffer[100];
		
		public void put(int order, ByteBuffer data){
			queue[order % queue.length] = data;
			if(next_index == -1) next_index = order % queue.length;
			size++;
		}
		
		public ByteBuffer getNext(){
			ByteBuffer d = null;
			if(size <= 0) return null;
			if(queue[next_index] != null){
				d = queue[next_index];
				queue[next_index] = null;
				next_index = (next_index + 1) % queue.length;
				last_get_time = System.currentTimeMillis();
			}else if(System.currentTimeMillis() - last_get_time > 1000) { //超时等待，跳过丢失的包。
				for(int i = queue.length; i > 0; i--){
					next_index = (next_index + 1) % queue.length;
					if(queue[next_index] != null) {
						d = queue[next_index];
						queue[next_index] = null;
						next_index = (next_index + 1) % queue.length;
						last_get_time = System.currentTimeMillis();
						break;
					}
				}
			}
			if(d != null)size--;
			return d;
		}
	}
}
