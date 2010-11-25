package org.goku.odip;

public interface MonitorClientListener {
	public void connected(MonitorClientEvent event);
	
	public void alarm(MonitorClientEvent event);

}
