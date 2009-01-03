package org.jmf.rtp.demo;

import java.io.File;
import java.io.IOException;

import javax.media.DataSink;
import javax.media.Format;
import javax.media.Manager;
import javax.media.MediaLocator;
import javax.media.Player;
import javax.media.Processor;
import javax.media.ProcessorModel;
import javax.media.format.AudioFormat;
import javax.media.protocol.ContentDescriptor;
import javax.media.protocol.DataSource;

public class RTPServer {
	private static final Format[] FORMATS = new Format[] {
		new AudioFormat(AudioFormat.MPEG_RTP)};
	
	private static final ContentDescriptor CONTENT_DESCRIPTOR =
	new ContentDescriptor(ContentDescriptor.RAW_RTP);	
	
	private static MediaLocator mediaLocator = null;
	private static DataSink dataSink = null;
	private static Processor mediaProcessor = null;

	/**
	 * @param args
	 */
	public static void main(String[] args) throws Exception {
		if (args.length != 1){
			System.out.println("usage: java RTPServer [File]");
			System.exit(-1);
		}
		
		File f = new File(args[0]);
		echo("init play:" + f.getAbsolutePath());
		
		DataSource source = Manager.createDataSource(new MediaLocator(f.toURI().toURL()));
		
		//Player p = Manager.createPlayer(source);
		//p.realize();
		//p.start();
		//Thread.sleep(1000 * 20);
		
		mediaLocator = new MediaLocator("rtp://192.168.1.104:5050/audio/");
		
		mediaProcessor = Manager.createRealizedProcessor(
				new ProcessorModel(source, FORMATS, CONTENT_DESCRIPTOR));
		
		dataSink = Manager.createDataSink(mediaProcessor.getDataOutput(),
										  mediaLocator);
		
		echo("start rtp:" + mediaLocator.toString());
		startTransmitting();
		
		Thread.sleep(1000 * 20);
		
		stopTransmitting();
		
		echo("done");
		//System.exit(0);
	}
	
	public static void startTransmitting() throws Exception{
		echo("realize..");
		mediaProcessor.realize();
		while (mediaProcessor.getState() != Player.Realized) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		echo("start..");
		
		mediaProcessor.start();
		dataSink.open();
		dataSink.start();
	}	
	
	public static void stopTransmitting() throws IOException {
		dataSink.stop();
		dataSink.close();
		mediaProcessor.stop();
		mediaProcessor.close();
	}	
	
	public static void echo(String msg){
		System.out.println(msg);
	}	

}
