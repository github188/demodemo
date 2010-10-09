package org.notebook.io;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.notebook.cache.Category;
import org.notebook.cache.NoteBook;
import org.notebook.cache.NoteMessage;

public class NoteBookClient{
	private static Log log = LogFactory.getLog(NoteBookClient.class);
	public NoteBook book = null;
	protected DateFormat format= new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
	
	public NoteBookClient(NoteBook book){
		this.book = book;
	}
	
	public List<Category> listCategory(Category parent) throws IOException{
		return listOrGetCategory(parent.id, "/list_category");
	}
	
	private List<Category> listOrGetCategory(String cateId, String func) throws IOException{
		URL url = new URL(book.endpoint + func);
		Map<String, String> param = new HashMap<String, String>();
		
		param.put("user", this.book.user);
		param.put("cate_id", cateId);
		
		InputStream in = ClientHttpRequest.post(url, param);
		JSONParser parser = new JSONParser();
		
		List<Category> result = new ArrayList<Category>();
		try {
			List<Map<String, Object>> jsonObject = (List<Map<String, Object>>)parser.parse(new InputStreamReader(in, "utf-8"));
			Category c = null;
			for(Map<String, Object> item : jsonObject){
				c = new Category();
				result.add(c);
				c.id = (String)item.get("cate_id");
				c.nodeType = (Integer)item.get("nodeType");
				c.name = (String)item.get("name");

				try {
					Date updateTime = format.parse((String)item.get("update_date"));
					//It's a bug, the GAE always use GTM+0.
					//c.lastUpdated = new Date(updateTime.getTime() + (60 * 60 * 8));
				} catch (java.text.ParseException e) {
					log.error(e.toString(), e);
				}
			}
		} catch (ParseException e) {
			log.error(e.toString(), e);
		}
		return result;
	}
	
	public void updateCategory(Category cate) throws IOException, ClientException{		
		URL url = new URL(book.endpoint + "/update_category");
		Map<String, String> param = new HashMap<String, String>();
		
		
		param.put("user", this.book.user);
		if(cate.parent != null){
			param.put("parent", cate.parent.id);
		}else {
			param.put("parent", "");
		}
		param.put("cate_id", cate.id);
		param.put("name", cate.name);
		param.put("nodeType", cate.nodeType + "");
		param.put("updateDate", format.format(cate.lastUpdated));
		
		InputStream in = ClientHttpRequest.post(url, param);
		JSONParser parser = new JSONParser();
		Map<String, String> jsonObject = null;
		try {
			jsonObject = (Map<String, String>)parser.parse(new InputStreamReader(in, "utf-8"));
			if(!jsonObject.get("status").equals("OK")){
				throw new ClientException("Failed to upload category, id=" + cate.id);
			}			
		} catch (ParseException e) {
			log.error(e.toString(), e);
		}
	}
	
	public NoteMessage downLoadMessage(String msgId) throws IOException{
		URL url = new URL(book.endpoint + "/sync_message");
		Map<String, String> param = new HashMap<String, String>();
		
		param.put("user", this.book.user);
		param.put("message_id", msgId);
		
		InputStream in = ClientHttpRequest.post(url, param);
		JSONParser parser = new JSONParser();
		Map<String, String> jsonObject = null;
		
		NoteMessage message = new NoteMessage(msgId);
		try {
			jsonObject = (Map<String, String>)parser.parse(new InputStreamReader(in, "utf-8"));
			message.text = jsonObject.get("text");
			try {
				Date updateTime = format.parse((String)jsonObject.get("update_date"));
				//message.lastUpdated = new Date(updateTime.getTime() + (60 * 60 * 8));
			} catch (java.text.ParseException e) {
				log.error(e.toString(), e);
			}
		} catch (ParseException e) {
			log.error(e.toString(), e);
		}		
		return message;
	}
	
	public void uploadMessage(NoteMessage message) throws IOException, ClientException{
		URL url = new URL(book.endpoint + "/sync_message");
		Map<String, String> param = new HashMap<String, String>();
		
		param.put("user", this.book.user);
		param.put("message_id", message.messageId);
		param.put("text", message.text);
		param.put("updateDate", format.format(message.lastUpdated));
		
		InputStream in = ClientHttpRequest.post(url, param);
		JSONParser parser = new JSONParser();
		Map<String, String> jsonObject = null;
		try {
			jsonObject = (Map<String, String>)parser.parse(new InputStreamReader(in, "utf-8"));
			if(!jsonObject.get("status").equals("OK")){				
				throw new ClientException("Failed to upload message, id=" + message.messageId);
			}
		} catch (ParseException e) {
			log.error(e.toString(), e);
		}
	}
	
	public Category getCategory(String id) throws IOException, ClientException{
		//get_category
		List<Category> rs = listOrGetCategory(id, "/get_category");
		return (rs.size() > 0) ? rs.get(0) : null;
	}
	
	public void removeCategory(Category cate) throws IOException, ClientException{		
		URL url = new URL(book.endpoint + "/delete");
		Map<String, String> param = new HashMap<String, String>();
		
		param.put("user", this.book.user);
		param.put("cate_id", cate.id);
		
		InputStream in = ClientHttpRequest.post(url, param);
		JSONParser parser = new JSONParser();
		Map<String, String> jsonObject = null;
		try {
			jsonObject = (Map<String, String>)parser.parse(new InputStreamReader(in, "utf-8"));
			if(!jsonObject.get("status").equals("OK")){				
				throw new ClientException("Failed to remove category, id=" + cate.id);
			}
			if(cate.isLeaf()){
				removeMessage(cate);
			}
		} catch (ParseException e) {
			log.error(e.toString(), e);
		}
	}
	
	public void removeMessage(Category cate) throws IOException, ClientException{		
		URL url = new URL(book.endpoint + "/delete");
		Map<String, String> param = new HashMap<String, String>();
		
		param.put("user", this.book.user);
		param.put("cate_id", cate.id);
		param.put("nodeType", "msg");
		
		InputStream in = ClientHttpRequest.post(url, param);
		JSONParser parser = new JSONParser();
		Map<String, String> jsonObject = null;
		try {
			jsonObject = (Map<String, String>)parser.parse(new InputStreamReader(in, "utf-8"));
			if(!jsonObject.get("status").equals("OK")){
				throw new ClientException("Failed to remove message, id=" + cate.id);
			}
		} catch (ParseException e) {
			log.error(e.toString(), e);
		}
	}
}