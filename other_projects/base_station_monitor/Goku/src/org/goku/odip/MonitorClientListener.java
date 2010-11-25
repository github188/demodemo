package org.goku.odip;

public interface MonitorClientListener {
	public void connected(MonitorClientEvent event);
	
	public void disconnected(MonitorClientEvent event);
	
	public void timeout(MonitorClientEvent event);
	
	public void alarm(MonitorClientEvent event);

}
