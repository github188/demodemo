package com.coci.nsn;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.json.JSONException;

import android.app.Service;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.IBinder;
import android.util.Log;

import com.coci.client.CoCiClient;
import com.coci.provider.AreaCI.TaskInfo;

public class DataSyncService extends Service {
	private final static String SYNC_TASK = "sync_task";
	private final static String TAG = "areaci.sync";
	//private WorkerThread worker = null; 
	private ThreadPoolExecutor threadPool = null;
	private Timer timer = new Timer();
	private CoCiClient client = null;
	private long lastTaskUpdatedTime = 0;
	
	@Override
    public void onCreate() {
		Log.i("areaci.service", "start new service");
		client = new CoCiClient();
		threadPool = new ThreadPoolExecutor(
				1, 3, 60, TimeUnit.SECONDS, 
				new ArrayBlockingQueue<Runnable>(50));
		
		timer.scheduleAtFixedRate(new TimerTask (){
			@Override
			public void run() {
				if(threadPool.getTaskCount() == 0){
					threadPool.execute(new TaskWorker(SYNC_TASK));
					if(threadPool.getQueue().size() > 10){
						threadPool.purge();
					}
				}
			}
		}, 100, 60 * 1000);		
    }
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.i("areaci.service", "start to sync data from networking...");
		threadPool.execute(new TaskWorker(SYNC_TASK));
	    return START_STICKY;
	}

	@Override
	public IBinder onBind(Intent paramIntent) {
		// TODO Auto-generated method stub
		return null;
	}
	
	public boolean isNetworkAvailable() {
	    ConnectivityManager cm = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
	    NetworkInfo networkInfo = cm.getActiveNetworkInfo();
	    // if no network is available networkInfo will be null, otherwise check if we are connected
	    if (networkInfo != null && networkInfo.isConnected()) {
	        return true;
	    }
	    Log.d(TAG, "The network isn't available.");
	    return false;
	}	
	
	public void syncTask(){
		long cur = System.currentTimeMillis() / 1000;
		long lastTime = cur - lastTaskUpdatedTime;
		List<ContentValues> taskList = null;
		try {
			taskList = client.updatedTask(Math.min(lastTime, 60 * 60 * 24), 50);
			if(taskList != null){
				Log.d(TAG, String.format("download task size:%s", taskList.size()));
				this.saveTaskInfo(taskList);
				lastTaskUpdatedTime = cur;
			}
		} catch (JSONException e) {
			Log.e(TAG, e.toString());
		}
	}
	
	private void saveTaskInfo(List<ContentValues> taskList){
		for(ContentValues task: taskList){
			getContentResolver().update(TaskInfo.CONTENT_URI, task, null, null);
		}
	}
	
	private synchronized boolean connectoToAreaCI(){
		String[][] connection = new String[][]{
			{"http://10.0.2.2:8000/areaci/api/", null},	
			{"http://10.56.117.81/coci/areaci/api/", null},
			//{"http://10.56.117.81/coci/areaci/", "http://10.144.1.10:8080"},
			{"http://proxy.deonwu84.com/coci/areaci/api/", null},
			//{"http://proxy.deonwu84.com/coci/areaci/", "http://10.144.1.10:8080"},
		};
		for(int i = 0; i < connection.length; i++){
			if(client.connect(connection[i][0], connection[i][1])){
				Log.d(TAG, String.format("Connect to '%s', proxy:'%s'", connection[i][0],
						 connection[i][1]));
				return true;
			}
		}
		return false;
	}
	
	class TaskWorker implements Runnable{
		private String task = null;
		public TaskWorker(String task){
			this.task = task;
		}
		
		@Override
		public void run() {
			if(this.task != null && isNetworkAvailable()){
				synchronized(client){
					if(!client.isConnected && !connectoToAreaCI()){
						Log.d(TAG, "The AreaCI server is disconnected.");						
						return;
					}
				}
				if(task.equals(SYNC_TASK)){
					syncTask();
				}
			}
		}
	}
}
