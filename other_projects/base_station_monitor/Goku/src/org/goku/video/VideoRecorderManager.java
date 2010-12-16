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
		this.storage = storage;
		initRootPath(settings.getString(Settings.FILE_ROOT_PATH, "data"));	
		pattern = settings.getString(Settings.FILE_NAME_PATTERN, DEFAULT);		
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
	
	/**
	 * 根据录像ID查询文件路径，
	 * @param uuid
	 * @return 录像文件路径，如果没有找到或文件不存在，返回null;
	 */
	public File getAlarmRecordFile(String uuid){
		AlarmRecord record = (AlarmRecord)storage.load(AlarmRecord.class, uuid);
		File path = null;
		if(record != null){
			path = new File(this.rootPath, record.videoPath);
			if(!path.isFile()){
				log.warn("Not found video file:" + path + ", by id:" + uuid);
			}
		}else {
			log.warn("Not found video record by id:" + uuid);
		}
		return path;
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
		
		path = path.replaceAll("\\$\\{yyyy\\}", String.format("%04d", now.get(Calendar.YEAR)));
		path = path.replaceAll("\\$\\{mm\\}", String.format("%02d", now.get(Calendar.MONTH)));
		path = path.replaceAll("\\$\\{dd\\}", String.format("%02d", now.get(Calendar.DAY_OF_MONTH)));
		
		path = path.replaceAll("\\$\\{HH\\}", String.format("%02d", now.get(Calendar.HOUR_OF_DAY)));
		path = path.replaceAll("\\$\\{MM\\}", String.format("%02d", now.get(Calendar.MINUTE)));
		path = path.replaceAll("\\$\\{SS\\}", String.format("%02d", now.get(Calendar.SECOND)));
		path = path.replaceAll("\\$\\{SSS\\}", String.format("%03d", now.get(Calendar.MILLISECOND)));
		
		path = path.replaceAll("\\$\\{UUID\\}", clientId);
		path = path.replaceAll("\\$\\{USER\\}", user);
		path = path.replaceAll("\\$\\{ALARM_TYPE\\}", alarmType);
		
		path = path.replaceAll("\\\\/", File.separator);
		
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
