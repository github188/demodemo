package org.notebook.services;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.print.PageFormat;
import java.awt.print.Printable;
import java.awt.print.PrinterException;
import java.util.List;

public class ImagePrintable implements Printable {
	private List<Image> pages = null;
	public ImagePrintable(List<Image> view){
		this.pages = view;
	}

	@Override
	public int print(Graphics g, PageFormat pageFormat, int pageIndex)
			throws PrinterException {
		if(pageIndex > pages.size() - 1){
			return NO_SUCH_PAGE;
		}else {
		      Graphics2D g2d = (Graphics2D)g;
		      g2d.translate(pageFormat.getImageableX(), pageFormat.getImageableY());
		      g2d.drawImage(pages.get(pageIndex), 0, 0, null);
		      return PAGE_EXISTS;			
		}
	}
	
}
