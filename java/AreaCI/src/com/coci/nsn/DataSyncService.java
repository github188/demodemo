package com.coci.nsn;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class DataSyncService extends Service {
	
	@Override
    public void onCreate() {
		Log.i("areaci.service", "start new service");
        //mNM = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        // Display a notification about us starting.  We put an icon in the status bar.
        //showNotification();
    }
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.i("areaci.service", "start to sync data from networking...");
	    //handleCommand(intent);
	    // We want this service to continue running until it is explicitly
	    // stopped, so return sticky.
	    return START_STICKY;
	}	

	@Override
	public IBinder onBind(Intent paramIntent) {
		// TODO Auto-generated method stub
		return null;
	}

}
