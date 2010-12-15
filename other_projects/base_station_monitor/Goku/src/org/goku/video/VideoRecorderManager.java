package org.goku.video;

import java.io.File;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.AlarmRecord;
import org.goku.db.DataStorage;
import org.goku.settings.Settings;
import org.goku.video.odip.MonitorClient;

/**
 * 管理视频录像。
 * @author deon
 */
public class VideoRecorderManager implements Runnable{
	private static final String DEFAULT = "${yyyy}-${mm}/" +
			"${UUID}-${ALARM_TYPE}-" +
			"${USER}-${mm}${dd}-" +
			"${HH}${MM}${SS}.${SSS}.h264";
	private Log log = LogFactory.getLog("video.recorder");
	private File rootPath = null;
	private String pattern = null;
	private DataStorage storage = null;
	private Timer timer = new Timer();
	
	private Map<String, AlarmRecord> runningRecorder = new HashMap<String, AlarmRecord>();
	
	public VideoRecorderManager(Settings settings, DataStorage storage){
		initRootPath(settings.getString(Settings.FILE_ROOT_PATH, "data"));		
		pattern = settings.getString(Settings.FILE_ROOT_PATH, DEFAULT);		
	}
	
	/**
	 * 返回当前录像的会话ID.
	 * @param client
	 * @param alarm
	 * @return
	 */
	public String startAlarmRecord(MonitorClient client, AlarmRecord alarm){
		Date startTime = new Date();
		String path = getSavePath(startTime, client.info.uuid, alarm.user, alarm.alarmType);
		alarm.videoPath = path;
		alarm.generatePK();
			
		alarm.recorder = new FileVideoRecorder(new File(rootPath, path));
		
		log.info(String.format("Start record, sid:%s, path:%s", alarm.uuid, alarm.videoPath));
		client.route.addDestination(alarm.recorder);		
		storage.save(alarm);
		
		runningRecorder.put(alarm.uuid, alarm);
		
		return alarm.uuid;
		
		//return alarm.uuid;
	}
	
	public String startManualRecord(MonitorClient client, String user){
		AlarmRecord alarm = new AlarmRecord();
		alarm.user = user;
		alarm.baseStation = client.info.uuid;
		alarm.startTime = new Date();
		alarm.alarmType = "none";
		
		return startAlarmRecord(client, alarm);
	}
	
	public void stoptRecord(String sid){		
		AlarmRecord alarm = runningRecorder.get(sid);
		if(alarm != null){
			alarm.recorder.close();
			alarm.endTime = new Date();
			storage.save(alarm);
			
			runningRecorder.remove(sid);
			log.info(String.format("Closed record, sid:%s", alarm.uuid));
		}else {
			log.warn("Try to stop record, but Not found session by id " + sid);
		}
	}
	
	/**
	 * 关闭超时的录像。
	 */
	protected void closeTimeOutRecord(){
		
	}
	
	/**
	 * 模式类型:
	 * ${yyyy} -- 年份
	 * ${mm} -- 月
	 * ${dd} -- 天
	 * 
	 * ${HH} -- 24时
	 * ${MM} -- 分钟
	 * ${SS} -- 秒
	 * 
	 * ${USER} -- 
	 * ${UUID} --
	 *  
	 * ${ALARM_TYPE} -- 
	 * 
	 * like: ${yyyy}-${mm}/${UUID}-${ALARM_TYPE}-${USER}-${mm}${dd}-${HH}${MM}${SS}.${SSS}.h264
	 * 
	 * @param startDate
	 * @param clientId
	 * @param user
	 * @param alarmType
	 * @return
	 */
	protected String getSavePath(Date startDate, String clientId, 
							   String user, String alarmType){
		String path = pattern;
		Calendar now = Calendar.getInstance();
		now.setTimeInMillis(startDate.getTime());
		
		path = path.replaceAll("\\$\\{yyyy\\}", now.get(Calendar.YEAR) + "");
		path = path.replaceAll("\\$\\{mm\\}", now.get(Calendar.MONTH) + "");
		path = path.replaceAll("\\$\\{dd\\}", now.get(Calendar.DAY_OF_MONTH) + "");
		
		path = path.replaceAll("\\$\\{HH\\}", now.get(Calendar.HOUR_OF_DAY) + "");
		path = path.replaceAll("\\$\\{MM\\}", now.get(Calendar.MINUTE) + "");
		path = path.replaceAll("\\$\\{SS\\}", now.get(Calendar.SECOND) + "");
		path = path.replaceAll("\\$\\{SSS\\}", now.get(Calendar.MILLISECOND) + "");
		
		path = path.replaceAll("\\$\\{UUID\\}", clientId);
		path = path.replaceAll("\\$\\{USER\\}", user);
		
		path = path.replaceAll("\\/", File.separator);
		
		File pathFile = new File(rootPath, path);
		File dirFile = pathFile.getParentFile();
		if(!dirFile.exists()){
			if(!dirFile.mkdirs()){
				log.error("Failed to create directory:" + dirFile.getAbsolutePath());
			}
		}
		
		return path;
	}
	
	private void initRootPath(String path){
		path = path.replaceAll("\\/", File.separator);
		File dirFile = new File(path);
		if(!dirFile.exists()){
			if(!dirFile.mkdirs()){
				log.error("Failed to create directory:" + dirFile.getAbsolutePath());
			}
		}
		this.rootPath = dirFile;
		log.info("Data root path:" + this.rootPath.getAbsolutePath());
	}

	@Override
	public void run() {
		timer.scheduleAtFixedRate(new TimerTask(){
			@Override
			public void run() {
				try{
					closeTimeOutRecord();
				}catch(Throwable e){
					log.error(e.toString(), e);
				}
			}
		}, 100, 1000 * 60);		
	}
}
