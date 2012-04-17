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
import hudson.FilePath;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import org.kohsuke.stapler.DataBoundConstructor;
import org.task.queue.topic.Message;

/**
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
    		Map<String, String> newEnvs = ((MsgEnvironment)msgEnv).data;
    		for(Map.Entry<String, String> entry: newEnvs.entrySet()){
    			listener.getLogger().println(String.format("%s='%s'", entry.getKey(), entry.getValue()));
    		}
    		listener.getLogger().println(String.format("--------------------"));
    		if(build.getWorkspace() != null){
    			this.writeDataToWorkspace(build.getWorkspace(), ((MsgEnvironment)msgEnv).fileData, listener);
    		}
    		
    	}else {
    		msgEnv = new Environment(){};
    	}
    	
    	return msgEnv;
    }
    
    private void writeDataToWorkspace(FilePath path, Map<String, String> data, final BuildListener listener){
    	for(Map.Entry<String, String> entry: data.entrySet()){
    		try{
    			new FilePath(path, entry.getKey()).write(entry.getValue(), "utf-8");
    		}catch(Exception e){
    			listener.getLogger().println("Failed to write file:" + e.toString());
    		}
    	}
    }
    
    class MsgEnvironment extends Environment{
    	public Map<String, String> data = null;
    	public Map<String, String> fileData = new HashMap<String, String>();
    	private Message msg = null;

    	public MsgEnvironment(Message msg){
    		this.msg =msg;
    		this.data = getMsgEnv();
    		//this.buildEnvVars(data);
    	}
    	
        @Override
        public void buildEnvVars(Map<String, String> env) {
            env.putAll(data);
        }    	
    	
    	private Map<String, String> getMsgEnv(){
    		Map<String, String> data = new HashMap<String, String>();
    		String value = null;
    		for(Map.Entry<String, Object> entry: msg.data.entrySet()){
    			if(entry.getValue() == null)continue;
    			value = entry.getValue().toString().trim();
    			if(value.indexOf('\n') < 0 && value.length() < 1024){
    				data.put("MQ_" + entry.getKey(), value);
    			}else {
    				fileData.put(String.format("MQ_%s.data", entry.getKey()), value);
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
