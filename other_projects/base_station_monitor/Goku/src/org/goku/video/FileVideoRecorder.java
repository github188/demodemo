package org.goku.video;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.video.odip.VideoDestination;

public class FileVideoRecorder implements VideoDestination{
	private Log log = LogFactory.getLog("recorder.file");
	
	private File path = null;
	private OutputStream os = null;
	
	//private 
	public FileVideoRecorder(File path){
		this.path = path;
	}

	@Override
	public boolean accept(int sourceType) {
		// TODO Auto-generated method stub
		return true;
	}

	@Override
	public void write(byte[] data, int type, int channel) throws IOException {
		log.info("File " + path.getName() + " write:" + data.length + " bytes.");
		if(os == null){
			os = new FileOutputStream(path);
		}
		os.write(data);
		os.flush();
	}
	
	public boolean isClosed(){
		return false;
	}

	@Override
	public void close() {
		if(os != null){
			try{
				os.close();
			}catch(IOException e){				
			}
		}		
	}

}
