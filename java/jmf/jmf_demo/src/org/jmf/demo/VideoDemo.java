/* 
 * Copyright (C) 2008 DeonWu@gmail.com
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at 
 * http://www.apache.org/licenses/LICENSE-2.0 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 *
 * $ Name LastChangeRevision LastChangeDate LastChangeBy $
 * $Id$
 */

package org.jmf.demo;

import java.awt.BorderLayout;
import java.awt.Component;
import java.io.IOException;
import java.util.Vector;

import javax.media.CannotRealizeException;
import javax.media.CaptureDeviceInfo;
import javax.media.CaptureDeviceManager;
import javax.media.Format;
import javax.media.Manager;
import javax.media.NoPlayerException;
import javax.media.Player;
import javax.media.format.AudioFormat;
import javax.media.format.VideoFormat;
import javax.media.protocol.DataSource;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class VideoDemo extends JPanel{
	
	DataSource ds = null;
	Player mediaPlayer = null;
	
	public VideoDemo(){
		setLayout(new BorderLayout());
		
		init();
	}
	

	/**
	 * @param args
	 * @throws IOException 
	 * @throws CannotRealizeException 
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		
		JFrame jframe = new JFrame("Video Demo");
		
		jframe.setSize(400, 600);
		
		VideoDemo videoDemo = new VideoDemo();
		jframe.getContentPane().add(videoDemo);
		
		videoDemo.playDataSource(videoDemo.ds);
		videoDemo.mediaPlayer.start();
		jframe.setVisible(true);
	}
	
	public void init(){
		Vector captureDevices = null;
		captureDevices = CaptureDeviceManager.getDeviceList(null);
		System.out.println("- number of capture devices: " + captureDevices.size());
		CaptureDeviceInfo video = this.getVideoDevice(captureDevices);
		CaptureDeviceInfo audio = this.getAudioDevice(captureDevices);
		
		this.ds = createDataSource(audio, video);
	}
	
	public void playDataSource(DataSource ds) throws NoPlayerException, CannotRealizeException, IOException{
		mediaPlayer = Manager.createRealizedPlayer(ds);
		Component video = mediaPlayer.getVisualComponent();
		Component controls = mediaPlayer.getControlPanelComponent();
		
		if(video != null){
			add(video, BorderLayout.CENTER );
		}
		
		if (controls != null){
			add(controls, BorderLayout.SOUTH );
		}
		
		//mediaPlayer.start(); 
	}
	
	public CaptureDeviceInfo getVideoDevice(Vector<CaptureDeviceInfo> captureDevices){
		for (CaptureDeviceInfo cdi : captureDevices){
			System.out.println("    - name of the capture device: " + cdi.getName());
			for(Format format: cdi.getFormats()){
				if (format instanceof VideoFormat) {
					System.out.println("         - format accepted by this VIDEO device: "
										+ format.toString().trim());
					return cdi;
				}
			}
		}
		return null;
	}

	public CaptureDeviceInfo getAudioDevice(Vector<CaptureDeviceInfo> captureDevices){
		for (CaptureDeviceInfo cdi : captureDevices){
			System.out.println("    - name of the capture device: " + cdi.getName());
			for(Format format: cdi.getFormats()){
				if (format instanceof AudioFormat) {
					System.out.println("         - format accepted by this Audio device: "
										+ format.toString().trim());
					return cdi;
				}
			}
		}
		return null;		
	}
	
	public DataSource createDataSource(CaptureDeviceInfo audio, CaptureDeviceInfo video){
		DataSource audioDS = null;
		DataSource videoDS = null;
		if (audio != null){
			try {
				audioDS = javax.media.Manager.createDataSource(audioDS.getLocator());
			} catch (Exception e) {
				System.out.println("-> Couldn't connect to audio capture device");
			}
		}
		if (video != null){
			try {
				videoDS = javax.media.Manager.createDataSource(video.getLocator());
			} catch (Exception e) {
				System.out.println("-> Couldn't connect to video capture device");
			}
		}
		
		if (audioDS != null && videoDS != null){
			try {
				//create the 'audio' and 'video' DataSource
				return javax.media.Manager.createMergingDataSource(
						new DataSource[] { audioDS, videoDS });
			}catch (Exception e) {
				System.out.println("-> Couldn't connect to audio or video capture device");
			}
		}else if(audioDS != null){
			return audioDS;
		}else if(videoDS != null){
			return videoDS;
		}
		return null;
	}
	
}
