package org.task.queue;

import hudson.Extension;
import hudson.model.BuildableItem;
import hudson.model.Item;
import hudson.triggers.Trigger;
import hudson.triggers.TriggerDescriptor;

import java.net.URL;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.kohsuke.stapler.DataBoundConstructor;

import antlr.ANTLRException;


public class TaskQueueTrigger extends Trigger<BuildableItem>{
	private transient static Log log = LogFactory.getLog("queue");
	
	private URL taskQueue = null;
	
    @DataBoundConstructor
    public TaskQueueTrigger(String taskQueue)
    throws ANTLRException {
      super("");
      
      this.taskQueue = new URL(taskQueue);
    }
    
    public void start(BuildableItem project, boolean newInstance) {
    	super.start(project, newInstance);
    	log.info("start message queue trigger, job:" + this.job.getName());
    	
    	TaskQueuePlugin.getInstance().registerTopic(this.taskQueue, this.job);
    }
  
    @Override
    public void run() {
    	log.info("run task queue.......");
    	//this.
    }
    
    @Override
    public void stop() {
    	log.info("remove message queue trigger, project:" + this.job.getName());
    	TaskQueuePlugin.getInstance().unregisterTopic(this.taskQueue, this.job);
    }	
    

	@Extension
	public static class DescriptorImpl extends TriggerDescriptor {
		/**
		 * {@inheritDoc}
		 */
		@Override
		public boolean isApplicable(Item item) {
		  return item instanceof BuildableItem;
		}
		
		/**
		 * {@inheritDoc}
		 */
		@Override
		public String getDisplayName() {
		  return "Task queue trigger.";
		}
	}
}
