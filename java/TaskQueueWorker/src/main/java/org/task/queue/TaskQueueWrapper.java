package org.task.queue;

import hudson.Launcher;
import hudson.model.BuildListener;
import hudson.model.AbstractBuild;
import hudson.model.Cause;
import hudson.tasks.BuildWrapper;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import org.kohsuke.stapler.DataBoundConstructor;
import org.task.queue.topic.Message;

/**
 * 在build 开始前,准备环境变量.在结束后更新Queue状态.
 * @author Administrator
 *
 */
public class TaskQueueWrapper extends BuildWrapper{

	@DataBoundConstructor
	public TaskQueueWrapper(){
		
	}
	
    @Override
    public Environment setUp(final AbstractBuild build, Launcher launcher, final BuildListener listener) throws IOException, InterruptedException {
    	Cause cause = build.getCause(TaskQueueCause.class);
    	
    	Environment msgEnv = null;
    	if(cause != null){
    		TaskQueueCause c = (TaskQueueCause)cause;
    		msgEnv = new MsgEnvironment(c.msg);
    	}else {
    		msgEnv = new Environment(){};
    	}
    	
    	return msgEnv;
    }
    
    class MsgEnvironment extends Environment{
    	private Message msg = null;
    	public MsgEnvironment(Message msg){
    		this.msg =msg;
    		this.buildEnvVars(getMsgEnv());
    	}
    	
    	private Map<String, String> getMsgEnv(){
    		Map<String, String> data = new HashMap<String, String>();
    		for(Map.Entry<String, String> entry: data.entrySet()){
    			if(entry.getValue().indexOf('\n') < 0 && entry.getValue().length() < 1024){
    				data.put(entry.getKey(), entry.getValue());
    			}
    		}    		
    		return data;
    	}
    	
    	public boolean tearDown(AbstractBuild build, BuildListener listener){
    		return true;
    	} 
    }
}
