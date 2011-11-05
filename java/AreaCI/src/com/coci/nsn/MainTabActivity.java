package com.coci.nsn;

import android.app.TabActivity;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.widget.TabHost;

public class MainTabActivity extends TabActivity {
	
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	    
	    setContentView(R.layout.tab_main);

	    Resources res = getResources(); // Resource object to get Drawables
	    TabHost tabHost = getTabHost();  // The activity TabHost
	    TabHost.TabSpec spec;  // Resusable TabSpec for each tab
	    Intent intent;  // Reusable Intent for each tab
	    
	    intent = new Intent().setClass(this, PRBActivity.class);
	    spec = tabHost.newTabSpec("prb").setIndicator("PRB",
	                      res.getDrawable(R.drawable.sub_component))
	                  .setContent(intent);
	    tabHost.addTab(spec);	    

	    intent = new Intent().setClass(this, TaskPolicyActivity.class);
	    spec = tabHost.newTabSpec("policy").setIndicator("Policy",
	                      res.getDrawable(R.drawable.policy))
	                  .setContent(intent);
	    tabHost.addTab(spec);

	    // Do the same for the other tabs
	    intent = new Intent().setClass(this, TaskQueueActivity.class);
	    spec = tabHost.newTabSpec("queue").setIndicator("queue",
	                      res.getDrawable(R.drawable.ic_table))
	                  .setContent(intent);
	    tabHost.addTab(spec);

	    intent = new Intent().setClass(this, TaskResultActivity.class);
	    spec = tabHost.newTabSpec("result").setIndicator("result",
	                      res.getDrawable(R.drawable.ic_table))
	                  .setContent(intent);
	    tabHost.addTab(spec);

	    intent = new Intent().setClass(this, ReportActivity.class);
	    spec = tabHost.newTabSpec("report").setIndicator("Report",
	                      res.getDrawable(R.drawable.report))
	                  .setContent(intent);
	    tabHost.addTab(spec);
	    
	    tabHost.setCurrentTab(2);
	    
	    intent = new Intent().setClass(this, DataSyncService.class);
	    this.startService(intent);
	}
}
