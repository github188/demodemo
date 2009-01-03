package org.jmf.rtp.demo;

import java.io.File;
import java.io.IOException;
import java.util.Vector;

import javax.media.CaptureDeviceInfo;
import javax.media.CaptureDeviceManager;
import javax.media.DataSink;
import javax.media.Manager;
import javax.media.MediaLocator;
import javax.media.NoProcessorException;
import javax.media.Processor;
import javax.media.control.FormatControl;
import javax.media.control.TrackControl;
import javax.media.format.AudioFormat;
import javax.media.protocol.ContentDescriptor;
import javax.media.protocol.DataSource;

public class SimpleVoiceTransmiter {

	/**
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {
		//		 First find a capture device that will capture linear audio
		// data at 8bit 8Khz 
		AudioFormat format = new AudioFormat(AudioFormat.LINEAR, 8000, 8, 1);

		Vector devices = CaptureDeviceManager.getDeviceList(format);

		CaptureDeviceInfo di = null;

		if (devices.size() > 0) {
			di = (CaptureDeviceInfo) devices.elementAt(0);
		} else {
			// exit if we could not find the relevant capturedevice. 
			System.exit(-1);
		}

		// Create a processor for this capturedevice & exit if we 
		// cannot create it 
		Processor processor = null;
		try {
			
			//processor = Manager.createProcessor(new MediaLocator(new File("C:\\USERS\\My Music\\03.mp3").toURI().toURL()));
			processor = Manager.createProcessor(di.getLocator());
		} catch (IOException e) {
			System.exit(-1);
		} catch (NoProcessorException e) {
			System.exit(-1);
		}

		// configure the processor  
		processor.configure();

		while (processor.getState() != Processor.Configured) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		processor.setContentDescriptor(new ContentDescriptor(
				ContentDescriptor.RAW));

		TrackControl track[] = processor.getTrackControls();

		boolean encodingOk = false;

		// Go through the tracks and try to program one of them to
		// output gsm data. 
		
		for (int i = 0; i < track.length; i++) {
			if (!encodingOk && track[i] instanceof FormatControl) {
				if (((FormatControl) track[i]).setFormat(new AudioFormat(
						AudioFormat.GSM_RTP, 8000, 8, 1)) == null) {

					track[i].setEnabled(false);
				} else {
					encodingOk = true;
				}
			} else {
				// we could not set this track to gsm, so disable 
				track[i].setEnabled(false);
			}
		}

		// At this point, we have determined where we can send out 
		// gsm data or not. 
		// realize the processor 
		if (encodingOk) {
			processor.realize();
			while (processor.getState() != Processor.Realized) {
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			// get the output datasource of the processor and exit 
			// if we fail 
			DataSource ds = null;

			ds = processor.getDataOutput();

			// hand this datasource to manager for creating an RTP 
			// datasink our RTP datasink will multicast the audio 
				String url = "rtp://192.168.1.104:5050/audio/";

				MediaLocator m = new MediaLocator(url);
				echo("open..");
				DataSink d = Manager.createDataSink(ds, m);
				d.open();
				d.start();
				processor.start();
				echo("start..");
		}
		
		//Thread.sleep(1000 * 20);
		echo("done..");
	}
	
	public static void echo(String msg){
		System.out.println(msg);
	}		

}
