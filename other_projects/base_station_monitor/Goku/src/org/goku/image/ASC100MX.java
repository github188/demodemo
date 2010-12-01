package org.goku.image;

import java.io.IOException;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.settings.Settings;

public class ASC100MX implements Runnable{
	private Log log = LogFactory.getLog("mx");
	
	private Map<String, String> routeTable = new HashMap<String, String>();	
	private Map<String, ASC100Client> route = Collections.synchronizedMap(new HashMap<String, ASC100Client>());	
	private int remotePort = 0;	
	private int localPort = 0;
	
	private boolean isRunning = true;
	private DatagramChannel channel = null;
	
	public ASC100MX(Settings settings){
		this.localPort = settings.getInt(Settings.UDP_LOCAL_PORT, 0);
		this.remotePort = settings.getInt(Settings.UDP_REMOTE_PORT, 0);
	}
	
	/**
	 * 所有数据处理都在同一个线程完成，简化Buffer的管理。
	 */
	public void run(){
	    ByteBuffer buffer = ByteBuffer.allocate(65535);
	    SocketAddress address = new InetSocketAddress(this.localPort);
	    DatagramSocket socket = channel.socket();
	    DatagramChannel channel = null;
	    try{
		    channel = DatagramChannel.open();
		    socket.bind(address);
	    }catch(Exception e){
	    	log.error("Failed open local UDP port " + this.localPort + ", error:" + e.toString());
	    }
	    
	    log.info("Started UPD server at port " + this.localPort);
	    while (this.isRunning) {
			try {
				buffer.clear();
				SocketAddress client = channel.receive(buffer);
				buffer.flip();
				this.process(client, buffer);
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
		this.route.put(client.getClientId(), client);
	}
	
	public void send(ASC100Client client, ByteBuffer data){
		
	}
	
	public void close(){
		this.isRunning = false;
	}
	
	protected void process(SocketAddress client, ByteBuffer data){
		byte cmd = data.get();
		if(cmd == 0xc5){
			this.processRouteTable(client, data);
		}
		
		byte channelCount = data.get();
		short sumCheck = data.getShort();
		short curSum = dataSumCheck(data.asReadOnlyBuffer());
		if(sumCheck == curSum){
			for(int i = 0; i < channelCount; i++){
				short node2 = data.get();
				short node1 = data.get();
				short channelId = data.get();
				data.get();
				short len = data.getShort();
				ByteBuffer sub = data.asReadOnlyBuffer();
				sub.limit(data.position() + len);
				this.clientRoute(node1, node2, channelId, sub);
			}
		}else {
			log.info("Failed to data sum check, the package is dropped.");
		}
	}
	
	protected void clientRoute(short node1, short node2, short channel, ByteBuffer data){
		String channelId = String.format("%s.%s.%s", node1, node2, channel);
		ASC100Client client = this.route.get(channelId);
		if(client != null){
			log.info("Process data client:" + channelId);
			client.process(data);
		}
	}	
	
	protected void processRouteTable(SocketAddress client, ByteBuffer data){
		byte channelCount = data.get();
		short reversed = data.getShort();
	}
	
	protected short dataSumCheck(ByteBuffer data){
		short sum = 0;
		while(data.hasRemaining()){
			sum += data.get();
		}
		return sum;
	}
}
