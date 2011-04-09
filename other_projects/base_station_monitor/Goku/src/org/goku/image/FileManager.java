package org.goku.image;

import java.io.File;
import java.util.Calendar;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Timer;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.goku.core.model.AlarmRecord;
import org.goku.db.DataStorage;
import org.goku.db.QueryParameter;
import org.goku.db.QueryResult;
import org.goku.settings.Settings;

public class FileManager {
	public String pattern = null;

	private static final String DEFAULT = "${yyyy}-${mm}/" +
			"${UUID}-${ALARM_TYPE}-" +
			"${USER}-${mm}${dd}-" +
			"${HH}${MM}${SS}.${SSS}.h264";
	private Log log = LogFactory.getLog("video.recorder");
	private File rootPath = null;
	private DataStorage storage = null;
	private Timer timer = new Timer();
	
	//单位秒。
	private long alarmTimeOut = 5;
	
	private Map<String, AlarmRecord> runningRecorder = new HashMap<String, AlarmRecord>();
	
	public FileManager(Settings settings, DataStorage storage){
		this.storage = storage;
		initRootPath(settings.getString(Settings.FILE_ROOT_PATH, "data"));	
		pattern = settings.getString(Settings.FILE_NAME_PATTERN, DEFAULT);
		
		//alarmTimeOut = settings.getInt(Settings.AUTO_CONFIRM_TIME, 5);
	}
	
	public AlarmRecord saveImageFile(ASC100Client client, ImageInfo image){	
		return null;
	}
	
	/**
	 * 通过告警ID，查询图片列表。 
	 */
	@SuppressWarnings("unchecked")
	public Collection<AlarmRecord> getImageListByAlaram(String alarmUUID, int last, int status, String baseUUID){
		Map<String, Object> filter = new HashMap<String, Object>();
		QueryParameter param = new QueryParameter();
		param.qsid = null;
		param.limit = 500;
		param.offset = 0;
		param.order = "startTime";
		
		if(alarmUUID != null && !"".equals(alarmUUID)){
			filter.put("combineUuid__=", alarmUUID);
		}
		if(baseUUID != null && !"".equals(baseUUID)){
			filter.put("baseStation__=", baseUUID);
		}
		if(status != 0){
			filter.put("alarmStatus__=", status + "");
		}
		if(last != 0){
			param.order = "-startTime";
			param.limit = last;
		}
		
		param.param = filter;
		QueryResult alarms = storage.queryData(AlarmRecord.class, param);
		
		if(log.isDebugEnabled()){
			log.debug(String.format("image files:%s, alarmID:%s, last:%s, status:%s, uuid:%s", alarms.data.size(), 
					alarmUUID, last, status, baseUUID));
		}		
		return alarms.data;
	}
	
	public File getRealPath(AlarmRecord alarm){
		File f = new File(this.rootPath, alarm.videoPath);
		return f.isFile() ? f: null;
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
		log.info("Image root path:" + this.rootPath.getAbsolutePath());
	}	
	
	protected String getSavePath(Date startDate, String clientId, String user,
			String alarmType, String ch) {
		String path = pattern;
		Calendar now = Calendar.getInstance();
		now.setTimeInMillis(startDate.getTime());

		path = path.replaceAll("\\$\\{yyyy\\}",
				String.format("%04d", now.get(Calendar.YEAR)));
		path = path.replaceAll("\\$\\{mm\\}",
				String.format("%02d", now.get(Calendar.MONTH) + 1));
		path = path.replaceAll("\\$\\{dd\\}",
				String.format("%02d", now.get(Calendar.DAY_OF_MONTH)));

		path = path.replaceAll("\\$\\{HH\\}",
				String.format("%02d", now.get(Calendar.HOUR_OF_DAY)));
		path = path.replaceAll("\\$\\{MM\\}",
				String.format("%02d", now.get(Calendar.MINUTE)));
		path = path.replaceAll("\\$\\{SS\\}",
				String.format("%02d", now.get(Calendar.SECOND)));
		path = path.replaceAll("\\$\\{SSS\\}",
				String.format("%03d", now.get(Calendar.MILLISECOND)));

		path = path.replaceAll("\\$\\{UUID\\}", clientId);
		path = path.replaceAll("\\$\\{USER\\}", user);
		path = path.replaceAll("\\$\\{ALARM_TYPE\\}", alarmType);
		path = path.replaceAll("\\$\\{CHANNEL\\}", ch);

		path = path.replaceAll("\\\\/", File.separator);

		File pathFile = new File(rootPath, path);
		File dirFile = pathFile.getParentFile();
		if (!dirFile.exists()) {
			if (!dirFile.mkdirs()) {
				log.error("Failed to create directory:"
						+ dirFile.getAbsolutePath());
			}
		}

		return path;
	}	
	

}
