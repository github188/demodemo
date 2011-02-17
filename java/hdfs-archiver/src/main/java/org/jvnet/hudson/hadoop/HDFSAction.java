package org.jvnet.hudson.hadoop;

import hudson.model.Action;
import hudson.model.AbstractModelObject;

public class HDFSAction extends AbstractModelObject implements Action{

	@Override
	public String getSearchUrl() {
		return "hdfs";
	}

	@Override
	public String getDisplayName() {
		return "HDFS Archiver";
	}

	@Override
	public String getIconFileName() {
		return "/plugin/hadoop/24x24/hadoop.png";
	}

	@Override
	public String getUrlName() {
		return "/hadoop";
	}

}
