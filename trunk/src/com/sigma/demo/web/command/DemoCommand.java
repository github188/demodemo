package com.sigma.demo.web.command;

import java.io.IOException;
import java.io.PrintStream;
import org.json.simple.JSONObject;

import com.sigma.demo.DemoException;
import com.sigma.demo.DemoService;
import com.sigma.demo.acl.DemoFacade;
import com.sigma.demo.util.JSONEncoder;

public class DemoCommand extends AbstractDemoCommand {
	protected PrintStream out = null;
	private JSONEncoder jsonEncoder = new JSONEncoder();
	private DemoFacade facade = null; 
	
	@SuppressWarnings("unchecked")
	public String doCommand() throws IOException {
		JSONObject result = new JSONObject();
		out = new PrintStream(resp.getOutputStream());		
		String op = this.getStringParameter("op", "@");
		String demoName = this.getStringParameter("dn", null);
		
		facade = lookupDemoFacade();
		if(demoName != null){
			DemoService service = facade.lookupDemo(demoName); //deployService.lookup(demoName);
			if(service != null){
				try{
					if(op.equals("load")) {
						getDemoData(service, result);
					}else if(op.equals("lock")) {
						lockDemo(service, result);
					}else if(op.equals("start")) {
						startDemo(service, result);
					}else if(op.equals("stop")) {
						stopDemo(service, result);
					}else if(op.equals("unlock")) {
						unLockDemo(service, result);
					}else if(op.equals("startAction")) {
						startDemoAction(service, result);
					}else if(op.equals("stopAction")) {
						stopDemoAction(service, result);
					}
				}catch(Exception e){
					result.put("error", e.getMessage());
					log.error(e,e);
				}
			}else {
				result.put("error", "Not found demo by name, name:" + demoName);
			}
		}else {
			result.put("error", "Demo name not defined.");
		}
		
		out = new PrintStream(resp.getOutputStream());
		out.println(result);
		return null;
	}
	
	
	@SuppressWarnings("unchecked")
	private void getDemoData(DemoService service,JSONObject result){
		result.put("data", jsonEncoder.encodeObject(service.getDemoData()));
	}

	private void lockDemo(DemoService service, JSONObject result) throws DemoException{
		
		this.facade.lockDemo(service);
		result.put("data", "true");
	}
	
	private void unLockDemo(DemoService service, JSONObject result) throws DemoException{
		
		this.facade.unlockDemo(service, this.getBooleanParameter("force", false));
		result.put("data", "true");
	}
	
	private void startDemo(DemoService service, JSONObject result) throws Exception{
		String pushLetId = this.getStringParameter("pId", null);
		if(pushLetId == null){
			throw new Exception("The pushlet session ID not supply.");
		}
		service.startDemo();
		log.debug(String.format("Start demo, name=%1$s, pushletId=%2$s, auth=%3$s",
				service.getDemoName(),
				pushLetId,
				this.getAuth()
				));
		service.runTimeListener(new PushletDemoListener(pushLetId));
		//service.addListener(new PushletDemoListener(pushLetId));
		result.put("data", "true");
	}

	private void stopDemo(DemoService service, JSONObject result) throws DemoException{
		service.stopDemo();
		result.put("data", "true");
	}
	
	private void startDemoAction(DemoService service, JSONObject result) throws Exception{
		String action = this.getStringParameter("ac", null);
		if(action == null){
			throw new Exception("The action name of start can't emtpy!");
		}
		service.startDemoAction(action);
		result.put("data", "true");
	}

	private void stopDemoAction(DemoService service, JSONObject result) throws Exception{
		String action = this.getStringParameter("ac", null);
		if(action == null){
			throw new Exception("The action name of stop can't emtpy!");
		}
		service.stopDemoAction(action);
		result.put("data", "true");
	}

}
