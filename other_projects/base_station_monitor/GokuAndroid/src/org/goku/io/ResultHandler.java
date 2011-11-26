package org.goku.io;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import android.util.Log;

public abstract class ResultHandler {
	private final static String TAG = "areaci.http";
	public boolean processInputStream(InputStream ins, GokuResult r) throws IOException{
		BufferedReader in = new BufferedReader(new InputStreamReader(ins, "utf8"));		
		boolean headerDone = false;
        String line = null;
        while ((line = in.readLine()) != null) {
        	if(line.trim().length() == 0) continue;
        	Log.d(TAG, line);
        	if(!headerDone && line.indexOf(":") > 0){
        		this.processHeader(line, r);
        		headerDone = true;
        	}else if(headerDone){
        		processBody(line, r);
        	}else {
        		processUnkownData(line, r);
        	}
        }
        in.close();
        
        return true;
	}
		
	private void processHeader(String l, GokuResult r){
		String[] d = l.split("\\$", 2);
		switch(d.length){
			case 2:
				extraHeader(d[1], r);
			case 1:
				String[] st = d[0].split(":", 2);
				r.status = Integer.parseInt(st[0].trim());
				r.message = st[1];
		}		
	}
	
	protected void extraHeader(String l, GokuResult r){
		
	}
	
	protected void processBody(String l, GokuResult r){		
	}

	protected void processUnkownData(String l, GokuResult r){
	}
}
