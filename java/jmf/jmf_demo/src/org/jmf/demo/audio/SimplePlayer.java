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


package org.jmf.demo.audio;

import java.io.File;

import javax.media.Manager;
import javax.media.Player;

public class SimplePlayer {

	/**
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {
		if (args.length != 1){
			System.out.println("usage: java SimplePlayer [File]");
			System.exit(-1);
		}
		File f = new File(args[0]);
		
		echo("init play:" + f.getAbsolutePath());
		Player audioPlayer = Manager.createRealizedPlayer(f.toURI().toURL());
		
		echo("start..");
		audioPlayer.start();
		
		Thread.sleep(1000 * 20);
		
		audioPlayer.stop();
		echo("done");
	}
	
	public static void echo(String msg){
		System.out.println(msg);
	}

}
