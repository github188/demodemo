package org.notebook.services;

import java.awt.Image;

public interface GmailAuthCallback {
	public void showCaptcha(Image capcha);
	public void error(String message);

}
