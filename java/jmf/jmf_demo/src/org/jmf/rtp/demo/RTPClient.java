package org.jmf.rtp.demo;

import javax.media.Format;
import javax.media.Manager;
import javax.media.MediaLocator;
import javax.media.Player;
import javax.media.format.AudioFormat;
import javax.media.protocol.ContentDescriptor;
import javax.media.protocol.DataSource;

public class RTPClient {

	private static final Format[] FORMATS = new Format[] {
		new AudioFormat(AudioFormat.MPEGLAYER3)};
	
	private static final ContentDescriptor CONTENT_DESCRIPTOR =
	new ContentDescriptor(ContentDescriptor.RAW_RTP);	
	
	public static void main(String[] args) throws Exception {
		echo("init..");
		//Player audioPlayer = Manager.createRealizedPlayer(new MediaLocator("rtp://192.168.1.104:5050/audio"));
		String url = "rtp://192.168.1.104:5050/audio/";
		echo("rtp:" + url);
		MediaLocator ml = new MediaLocator(url);
		echo("create datasource..");
		DataSource source = Manager.createDataSource(ml);
		echo("createPlayer..");
		Player p = Manager.createPlayer(source);
		echo("realize..");
		p.realize();
		while (p.getState() != Player.Realized) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		echo("start..");
		Thread.sleep(1000 * 5);
		p.start();
		//Thread.sleep(1000 * 20);
				
		Thread.sleep(1000 * 20);
		
		echo("done");
	}
	
	public static void echo(String msg){
		System.out.println(msg);
	}
	
}
