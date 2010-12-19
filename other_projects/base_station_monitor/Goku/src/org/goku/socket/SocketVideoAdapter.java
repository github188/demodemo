package org.goku.socket;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.video.VideoRouteServer;
import org.goku.video.odip.MonitorClient;
import org.goku.video.odip.VideoDestination;

/**
 * 处理Socket的Video相关的命令。
 * 
 * 命令格式：
 * 
 * video>real?uuid=1111
 * video>replay?uuid=1111
 * video>seek?pos=1111
 * video>ack
 * 
 * @author deon
 */
public class SocketVideoAdapter {
	private Log log = LogFactory.getLog("client.socket.video");
	private VideoRouteServer server = null;
	public SocketVideoAdapter(){
		server = VideoRouteServer.getInstance();		
	}
	
	public void runCommand(String command, SocketClient client)  throws IOException{
		Map<String, String> param = parseCommand(command);
		String cmd = param.get("q");
		if(cmd.equals("real")){
			doRealPlay(client, param.get("uuid"));
		}else if(cmd.equals("replay")){
			doRePlay(client, param.get("uuid"));
		}else if(cmd.equals("seek")){
			doSeek(client, param.get("pos"));
		}else if(cmd.equals("ack")){
			doOk(client);
		}
	}
	
	protected void doRealPlay(SocketClient client, String uuid){
		if(client.connectionMode == SocketClient.MODE_HTTP){
			MonitorClient mc = server.getMonitorClient(uuid);
			if(mc != null){
				log.info("Start real play client id " + uuid);
				mc.route.addDestination(new SocketVideoPlayer(client));
				client.connectionMode = SocketClient.MODE_REALLPLY;
			}else {
				client.closeSocket();
			}
		}else {
			client.closeSocket();
		}
	}
	
	protected void doRePlay(SocketClient client, String uuid) throws IOException{
		if(client.connectionMode == SocketClient.MODE_HTTP){
			File videoPath = server.recordManager.getAlarmRecordFile(uuid);
			if(videoPath != null){
				log.info("Start replay uuid " + uuid + ", path:" + videoPath.getAbsolutePath());
				client.replay = new FileReplayController(client, videoPath);
				client.replay.nextFrame();
				client.connectionMode = SocketClient.MODE_REPLAY;
			}else {
				client.closeSocket();
			}
		}else {
			client.closeSocket();
		}		
	}	
	
	protected void doSeek(SocketClient client, String pos) throws IOException{
		int posLong = 0;
		try{
			posLong = Integer.parseInt(pos);
		}catch(Throwable e){
			log.error("doSeek error, err:" + e.toString(), e);
		}
		if(client.replay != null){
			client.replay.seekPos(posLong);
			client.replay.nextFrame();
		}
	}
	
	protected void doOk(SocketClient client) throws IOException{
		if(client.replay != null){
			client.replay.nextFrame();
		}
	}
	
	class SocketVideoPlayer implements VideoDestination{
		private SocketClient client = null;
		public SocketVideoPlayer(SocketClient client){
			this.client = client;
		}

		@Override
		public boolean accept(int sourceType) {
			return true;
		}

		@Override
		public void write(byte[] data) throws IOException {
			client.write(data);
		}

		@Override
		public void close() {
			this.client.closeSocket();
		}

		@Override
		public boolean isClosed() {
			return !(client.socket.isOpen() && client.socket.isConnected());
		}
		
	}
	
	protected Map<String, String> parseCommand(String command){
		Map<String, String> p = new HashMap<String, String>();
		command = command.replace("video>", "");
		String[] data = command.split("\\?", 2);
		p.put("q", data[0]);
		if(data.length > 1 && data[1] != null){
			for(String para: data[1].split("&")){
				if(para.indexOf('=') > 0){
					String[] aParam = para.split("=", 2);
					if(aParam.length == 2){
						p.put(aParam[0].trim(), aParam[1].trim());
					}else {
						p.put(aParam[0].trim(), "");
					}
				}
			}
		}
		return p;
	};	
}