package org.task.queue;

import hudson.Extension;
import hudson.Launcher;
import hudson.model.BuildListener;
import hudson.model.BuildableItem;
import hudson.model.AbstractBuild;
import hudson.model.AbstractProject;
import hudson.model.Cause;
import hudson.tasks.BuildWrapper;
import hudson.tasks.BuildWrapperDescriptor;

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
    		listener.getLogger().println(String.format("----message %s----", c.msg.id));
    		for(Map.Entry<String, String> entry: ((MsgEnvironment)msgEnv).data.entrySet()){
    			listener.getLogger().println(String.format("%s='%s'", entry.getKey(), entry.getValue()));
    		}
    	}else {
    		msgEnv = new Environment(){};
    	}
    	
    	return msgEnv;
    }
    
    class MsgEnvironment extends Environment{
    	public Map<String, String> data = null;
    	private Message msg = null;

    	public MsgEnvironment(Message msg){
    		this.msg =msg;
    		this.data = getMsgEnv();
    		this.buildEnvVars(data);
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
    		//build successful.
    		if(build.getResult() == null){
    			if(msg.mapping != null){
    				listener.getLogger().println("Ack message done, url:" + msg.mapping.queue);
    				msg.mapping.ackMessageDone(msg);
    			}else {
    				listener.getLogger().println("Not found message queue address, can't " +
    						"ack message.");
    			}
    		}else {
    			listener.getLogger().println("the build not successful.");
    		}
    		return true;
    	} 
    }
    
	@Extension
	public static class DescriptorImpl extends BuildWrapperDescriptor {
		/**
		 * {@inheritDoc}
		 */
		@Override
		public boolean isApplicable(AbstractProject<?, ?> item) {
		  return item instanceof BuildableItem;
		}
		
		/**
		 * {@inheritDoc}
		 */
		@Override
		public String getDisplayName() {
		  return "Message queue wrapper...";
		}
	}
}
