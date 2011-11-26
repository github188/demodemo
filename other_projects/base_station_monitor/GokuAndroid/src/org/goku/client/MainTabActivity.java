package org.goku.client;

import android.app.TabActivity;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.widget.TabHost;

public class MainTabActivity extends TabActivity{
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	    
	    setContentView(R.layout.tab_main);
	    
	    Resources res = getResources(); // Resource object to get Drawables
	    TabHost tabHost = getTabHost();  // The activity TabHost
	    TabHost.TabSpec spec;  // Resusable TabSpec for each tab
	    Intent intent;  // Reusable Intent for each tab
	    
	    intent = new Intent().setClass(this, BTSActivity.class);
	    spec = tabHost.newTabSpec("prb").setIndicator("基站列表",
	                      res.getDrawable(R.drawable.ic_launcher))
	                  .setContent(intent);
	    tabHost.addTab(spec);	    

	    intent = new Intent().setClass(this, RealAlarmActivity.class);
	    spec = tabHost.newTabSpec("real_alarm").setIndicator("实时告警",
	                      res.getDrawable(R.drawable.ic_launcher))
	                  .setContent(intent);
	    tabHost.addTab(spec);
	    
	    intent = new Intent().setClass(this, QueryAlarmActivity.class);
	    spec = tabHost.newTabSpec("query").setIndicator("告警查询",
	                      res.getDrawable(R.drawable.ic_launcher))
	                  .setContent(intent);
	    tabHost.addTab(spec);	   
	    
	    tabHost.setCurrentTab(0);
	}
}
