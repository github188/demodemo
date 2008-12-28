/*
 * MediaManager.java
 *
 * Created on December 2, 2003, 8:56 AM
 */

package org.jmf.demo;

import javax.media.*;
import javax.media.protocol.*;
import javax.media.protocol.DataSource;
import javax.media.format.*;
import javax.media.control.TrackControl;

import java.util.List;
import java.util.Iterator;
import java.util.Vector;
import java.util.Random;
import java.io.IOException;

/**
 * This class will handle the media part of a call
 * Opening the receiver and transmitter, close them,...
 * 
 * @author Jean Deruelle <jean.deruelle@nist.gov>
 *
 * <a href="{@docRoot}/uncopyright.html">This code is in the public domain.</a>
 */
public class MediaManager {
	//Codec supported by the user agent
	public static List audioCodecSupportedList = null;
	public static List videoCodecSupportedList = null;
	//Remote Address to connect to
	private String remoteAddress;
	//Ports chosen
	private int remoteAudioPort;
	private int localAudioPort = -1;
	private int remoteVideoPort;
	private int localVideoPort = -1;
	//Codecs chosen
	private String negotiatedAudioCodec;
	private String negotiatedVideoCodec;
	//flag to know if the session has been started
	private boolean started;
	//flag to know if the media session is going through the proxy
	private boolean proxyEnabled;

	/**
	 * Detects the supported codecs of the user agent depending of 
	 * the devices connected to the computer
	 */
	public static void detectSupportedCodecs() {
		audioCodecSupportedList = new Vector();
		videoCodecSupportedList = new Vector();
		MediaLocator audioLocator = null;
		MediaLocator videoLocator = null;
		CaptureDeviceInfo videoCDI = null;
		CaptureDeviceInfo audioCDI = null;
		Vector captureDevices = null;
		captureDevices = CaptureDeviceManager.getDeviceList(null);
		System.out.println(
			"- number of capture devices: " + captureDevices.size());
		CaptureDeviceInfo cdi = null;
		for (int i = 0; i < captureDevices.size(); i++) {
			cdi = (CaptureDeviceInfo) captureDevices.elementAt(i);
			System.out.println(
				"    - name of the capture device: " + cdi.getName());
			Format[] formatArray = cdi.getFormats();
			for (int j = 0; j < formatArray.length; j++) {
				Format format = formatArray[j];
				if (format instanceof VideoFormat) {
					System.out.println(
						"         - format accepted by this VIDEO device: "
							+ format.toString().trim());
					if (videoCDI == null) {
						videoCDI = cdi;
					}
				} else if (format instanceof AudioFormat) {
					System.out.println(
						"         - format accepted by this AUDIO device: "
							+ format.toString().trim());
					if (audioCDI == null) {
						audioCDI = cdi;
					}
				} else
					System.out.println("         - format of type UNKNOWN");
			}
		}
		if (videoCDI != null)
			videoLocator = videoCDI.getLocator();
		if (audioCDI != null)
			audioLocator = audioCDI.getLocator();

		DataSource audioDS = null;
		DataSource videoDS = null;
		DataSource mergeDS = null;
		//create the DataSource
		//it can be a 'video' DataSource, an 'audio' DataSource
		//or a combination of audio and video by merging both
		if (videoLocator == null && audioLocator == null)
			return;
		if (audioLocator != null) {
			try {
				//create the 'audio' DataSource
				audioDS = javax.media.Manager.createDataSource(audioLocator);
			} catch (Exception e) {
				System.out.println(
					"-> Couldn't connect to audio capture device");
			}
		}
		if (videoLocator != null) {
			try {
				//create the 'video' DataSource
				videoDS = javax.media.Manager.createDataSource(videoLocator);
			} catch (Exception e) {
				System.out.println(
					"-> Couldn't connect to video capture device");
			}
		}
		Processor processor = null;
		if (videoDS != null && audioDS != null) {
			try {
				//create the 'audio' and 'video' DataSource
				mergeDS =
					javax.media.Manager.createMergingDataSource(
						new DataSource[] { audioDS, videoDS });
			} catch (Exception e) {
				System.out.println(
					"-> Couldn't connect to audio or video capture device");
			}
			try {
				//Create the processor from the merging DataSource
				processor = javax.media.Manager.createProcessor(mergeDS);
			} catch (NoProcessorException npe) {
				npe.printStackTrace();
				return;
			} catch (IOException ioe) {
				ioe.printStackTrace();
				return;
			}
		}
		//if the processor has not been created from the merging DataSource
		if (processor == null) {
			try {
				if (audioDS != null)
					//Create the processor from the 'audio' DataSource
					processor = javax.media.Manager.createProcessor(audioDS);
				else
					//Create the processor from the 'video' DataSource
					processor = javax.media.Manager.createProcessor(videoDS);
			} catch (NoProcessorException npe) {
				npe.printStackTrace();
				return;
			} catch (IOException ioe) {
				ioe.printStackTrace();
				return;
			}
		}

		// Get the tracks from the processor
		TrackControl[] tracks = processor.getTrackControls();

		// Do we have atleast one track?
		if (tracks == null || tracks.length < 1) {
			System.out.println("Couldn't find tracks in processor");
			return;
		}
		// Set the output content descriptor to RAW_RTP
		// This will limit the supported formats reported from
		// Track.getSupportedFormats to only valid RTP formats.
		ContentDescriptor cd = new ContentDescriptor(ContentDescriptor.RAW_RTP);
		processor.setContentDescriptor(cd);

		Format supported[];
		Format chosen = null;
		boolean atLeastOneTrack = false;

		// Program the tracks.
		for (int i = 0; i < tracks.length; i++) {
			Format format = tracks[i].getFormat();
			if (tracks[i].isEnabled()) {
				supported = tracks[i].getSupportedFormats();
				// We've set the output content to the RAW_RTP.
				// So all the supported formats should work with RTP.            
				if (supported.length > 0) {
					for (int j = 0; j < supported.length; j++) {
						System.out.println(
							"Supported format : "
								+ supported[j].toString().toLowerCase());
						//Add to the list of video supported codec
						if (supported[j] instanceof VideoFormat) {
							videoCodecSupportedList.add(supported[j]);
						}
						//Add to the list of audio supported codec
						else {
							audioCodecSupportedList.add(supported[j]);
						}
					}
				}
			}
		}
		processor.stop();
		processor.close();
	}


	/*PCMU 		javax.media.format.AudioFormat.ULAW_RTP;
	1016
	G721
	GSM 		javax.media.format.AudioFormat.GSM_RTP;
	G723		javax.media.format.AudioFormat.G723_RTP
	DVI4_8000           javax.media.format.AudioFormat.DVI_RTP;
	DVI4_16000          javax.media.format.AudioFormat.DVI_RTP;
	LPC
	PCMA		javax.media.format.AudioFormat.ALAW;
	G722		javax.media.format.AudioFormat.ALAW;
	L16_2CH
	L16_1CH
	QCELP
	CN
	MPA
	G728		javax.media.format.AudioFormat.G728_RTP;
	DVI4_11025
	DVI4_22050
	G729		javax.media.format.AudioFormat.G729_RTP
	CN_DEPRECATED
	H263		javax.media.format.VideoFormat.H263_RTP
	CelB
	JPEG		javax.media.format.VideoFormat.JPEG_RTP
	nv
	H261		javax.media.format.VideoFormat.H261_RTP
	MPV*/
}
