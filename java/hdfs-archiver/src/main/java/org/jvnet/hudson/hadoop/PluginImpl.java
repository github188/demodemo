package org.jvnet.hudson.hadoop;

import hudson.Plugin;
import hudson.model.Hudson;
//import hudson.plugins.hadoop.HadoopPage;

public class PluginImpl extends Plugin{
    /*package*/ HDFSAction page = new HDFSAction();
    private String masterHostName;

    @Override
    public void start() throws Exception {
        Hudson.getInstance().getActions().add(page);
    }

}
