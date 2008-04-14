package com.sigma.demo.web.command;

import java.io.IOException;
import java.io.PrintStream;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import com.sigma.demo.DemoService;
import com.sigma.demo.acl.DemoFacade;
import com.sigma.demo.acl.DemoGroup;

public class DemoListCommand extends AbstractDemoCommand {
	protected PrintStream out = null;
	private DemoFacade facade = null;
	
	public String doCommand() throws IOException {
		// TODO Auto-generated method stub
		//resp.setContentType("text/plain");
		out = new PrintStream(resp.getOutputStream());
		facade = lookupDemoFacade();
		String group = this.getStringParameter("node", "group");
		if(log.isDebugEnabled()){
			log.debug("Get Demo list, group:" + group);
		}
		if(group.equals("group")) {
			out.println(getGroupList());	
		}else {
			out.println(getDemoList(group));
		}
		
		//out.println(getDemoList(group));
		
		return null;
	}
	
	//[{"text":"STOAN","id":"STOAN","cls":"folder"},
	// {"text":"ext-all-debug.js","id":"\/ext-all-debug.js","leaf":true,"cls":"file"},
	
	@SuppressWarnings("unchecked")
	private JSONArray getGroupList(){
		JSONArray list = new JSONArray();
		for(DemoGroup g: facade.listDemoGroup(0, Integer.MAX_VALUE, null)){
			JSONObject obj=new JSONObject();
			obj.put("id", g.getName());
			obj.put("text", g.getName());
			//obj.put("leaf", true);
			obj.put("cls", "folder");
			list.add(obj);			
		} 
		return list;
	}
	
	@SuppressWarnings("unchecked")
	private JSONArray getDemoList(String group){
		JSONArray list = new JSONArray();
		for(DemoService g: facade.listDemoByGroup(group, 0, Integer.MAX_VALUE, null)){
			JSONObject obj=new JSONObject();
			obj.put("id", group + "/" + g.getDemoName());
			obj.put("text", g.getDemoName());
			obj.put("leaf", true);
			obj.put("cls", "file");			
			//obj.put("leaf", true);
			obj.put("cls", "folder");
			list.add(obj);			
		} 
		return list;
	}	
	

}
