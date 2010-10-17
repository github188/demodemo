package org.notebook.services;

import javax.jnlp.ServiceManager;
import javax.jnlp.SingleInstanceListener;
import javax.jnlp.SingleInstanceService;
import javax.jnlp.UnavailableServiceException;

import org.notebook.gui.MainFrame;
import org.notebook.gui.MenuToolbar;

public class JNLPSingleInstance implements SingleInstance {

	@Override
	public boolean checkRunning(final MainFrame app) {
		// TODO Auto-generated method stub
		try {
			SingleInstanceService  ss = (SingleInstanceService )ServiceManager.lookup("javax.jnlp.SingleInstanceService");
			ss.addSingleInstanceListener(new SingleInstanceListener(){
				@Override
				public void newActivation(String[] arg0) {
					app.menu.$(MenuToolbar.SHOWWINDOW).actionPerformed(null);
				}});
			log.info("register JNLP SingleInstance service..");
		}catch (UnavailableServiceException e) {
			//e.printStackTrace();
		}
		return false;
	}

}
