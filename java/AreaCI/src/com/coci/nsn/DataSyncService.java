package com.coci.nsn;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.json.JSONException;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.IBinder;
import android.util.Log;

import com.coci.client.CoCiClient;
import com.coci.provider.AreaCI;
import com.coci.provider.AreaCI.TaskInfo;

public class DataSyncService extends Service {
	private final static String SYNC_TASK = "sync_task";
	private final static String INIT_LOCAL_DATA = "init_local_data";
	private final static String REMOVE_EXPIRED_TASK = "remove_expired_task";
	private final static DateFormat format= new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
	
	private final static String TAG = "areaci.sync";
	//private WorkerThread worker = null; 
	private ThreadPoolExecutor threadPool = null;
	private Timer timer = new Timer();
	private CoCiClient client = null;
	private long lastTaskUpdatedTime = 0;
	private long lastCleanUpTime = 0;
	private long lastTryConnectTime = 0;
	
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
				int queueSize = threadPool.getQueue().size(); 
				Log.d(TAG, "Current task queue size:" + queueSize);
				if(queueSize <= 1){
					threadPool.execute(new TaskWorker(SYNC_TASK));
				}else if(queueSize > 10) {
					Log.w(TAG, "Clean up padding task queue, current size:" + queueSize);
					threadPool.purge();
				}
				/**
				 * 每隔5小时清理一次过期数据。
				 */
				if(System.currentTimeMillis() - lastCleanUpTime > 1000 * 60 * 60 * 5){
					lastCleanUpTime = System.currentTimeMillis();
					threadPool.execute(new TaskWorker(REMOVE_EXPIRED_TASK));
				}
				
			}
		}, 100, 60 * 1000);		
		
		/**
		 * 初始化数据如果本地数据库记录没有数据。
		 */
		Cursor taskList = getContentResolver().query(TaskInfo.TASK_LIST_URI, null, null, null, null);
		if(taskList.getCount() < 50){
			threadPool.execute(new TaskWorker(INIT_LOCAL_DATA));		
		}
		taskList.close();
    }
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.i("areaci.service", "start to sync data from networking...");
		
		//sendNotification("Network111", "Failed to connect to areaci, id:" + startId);
		threadPool.execute(new TaskWorker(SYNC_TASK));
	    return START_STICKY;
	}

	@Override
	public IBinder onBind(Intent paramIntent) {
		// TODO Auto-generated method stub
		return null;
	}
	
	@Override
	public void onDestroy() {
	    //Toast.makeText(this, "service done", Toast.LENGTH_SHORT).show();
		Log.i("areaci.service", "Stop the sync service");
		timer.cancel();
		threadPool.shutdownNow();
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
	
	public void syncTask(boolean initData){
		long cur = System.currentTimeMillis();
		long lastTime = (cur - lastTaskUpdatedTime) / 1000 / 60;
		List<ContentValues> taskList = null;
		
		//如果是第一次初始化数据，查询最近1个月的数据。
		if(initData){
			lastTime = 60 * 24 * 30;
		}else {
			lastTime = Math.min(lastTime + 1, 60 * 24);
		}
		
		if(lastTime <= 1) {
			Log.d(TAG, String.format("ignore sync task action, last update time in %s seconds.", (cur - lastTaskUpdatedTime) / 1000));
			return;
		}
		
		try {
			taskList = client.updatedTask(lastTime, 50);
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
		if(taskList.size() > 0){
        	SharedPreferences settings = getSharedPreferences(AreaCI.PREFS_NAME, 0);
        	SharedPreferences.Editor editor = settings.edit();
        	editor.putLong(AreaCI.PREFS_LAST_SYNC_TIME, System.currentTimeMillis());        	
        	Date now = new Date(System.currentTimeMillis());
        	editor.putString(AreaCI.PREFS_LAST_SYNC_TIME_STR, format.format(now));        	
        	editor.commit();			
			getContentResolver().notifyChange(TaskInfo.CONTENT_URI, null);
		}
	}
	
	private void deleteExpriedData(){
		getContentResolver().delete(AreaCI.EXPRIED_DATA_URI, null, null);
	}	
	
	private void sendNotification2(String title, String message){
		int icon = R.drawable.ipaci;        // icon from resources
		CharSequence tickerText = "Hello";              // ticker-text
		long when = System.currentTimeMillis();         // notification time
		Context context = getApplicationContext();      // application Context
		//CharSequence contentTitle = "My notification";  // message title
		//CharSequence contentText = "Hello World!";      // message text

		Intent notificationIntent = new Intent(this, TaskQueueActivity.class);
		PendingIntent contentIntent = PendingIntent.getActivity(this, 0, notificationIntent, 0);

		// the next two lines initialize the Notification, using the configurations above
		Notification notification = new Notification(icon, tickerText, when);
		notification.setLatestEventInfo(context, title, message, contentIntent);
		
		notification.flags |= Notification.FLAG_SHOW_LIGHTS;
		
		String ns = Context.NOTIFICATION_SERVICE;
		NotificationManager mNotificationManager = (NotificationManager) getSystemService(ns);		
		mNotificationManager.notify(1, notification);		
	}
	
	private synchronized boolean connectoToAreaCI(){
		
		if(System.currentTimeMillis() - this.lastTryConnectTime < 5 * 60 * 1000){
			Log.d(TAG, String.format("ignore connection action, last connected in %s seconds.", 
					(System.currentTimeMillis() - this.lastTryConnectTime) / 1000));
			return false;
		}
		
		this.lastTryConnectTime = System.currentTimeMillis();
		
		String[][] connection = new String[][]{
			{"http://proxy-nsn.deonwu84.com:8080/coci/areaci/api/", null},
			{"http://10.0.2.2:8000/areaci/api/", null},	
			{"http://10.56.117.81/coci/areaci/api/", null},
			//{"http://10.56.117.81/coci/areaci/", "http://10.144.1.10:8080"},
			//{"http://proxy.deonwu84.com/coci/areaci/", "http://10.144.1.10:8080"},
		};
		for(int i = 0; i < connection.length; i++){
			if(client.connect(connection[i][0], connection[i][1])){
				Log.d(TAG, String.format("Connect to '%s', proxy:'%s'", connection[i][0],
						 connection[i][1]));				
				return true;
			}
		}
		
		//sendNotification("Network", "Failed to connect to areaci.");
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
					syncTask(false);
				}else if (task.equals(REMOVE_EXPIRED_TASK)){
					deleteExpriedData();
				}else if(task.equals(INIT_LOCAL_DATA)){
					syncTask(true);
				}
			}
		}
	}
		
}
