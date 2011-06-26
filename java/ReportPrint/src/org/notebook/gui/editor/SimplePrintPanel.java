package org.notebook.gui.editor;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;
import java.net.URL;

import javax.swing.JPanel;
import javax.swing.JTextField;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.DocumentDefine;
import org.notebook.cache.InputDataField;

public class SimplePrintPanel extends JPanel {
	//private Image bg = null;
	private JTextField input = null;
	private Log log = LogFactory.getLog("print");
	private ImageObserver bgObserver = null;
	private DocumentDefine doc = null;
	
	/*
	 * 设置是否显示背景，用来区分是打印，还是屏幕显示。如果屏幕显示需要看到背景，
	 * 打印的时候。不需要背景，实现套打功能。
	 */
	private boolean isShowBG = true;	
	
	public void loadDocument(DocumentDefine doc){
		this.setLayout(null);
		this.doc = doc;
		this.bgObserver = new BgImageObserver();
		
		JTextField jText = null;
		for(InputDataField f: doc.fields){
			jText = new JTextField();
			jText.setBounds(f.x, f.y, f.w, f.h);
			this.add(jText);
		}
		
		this.updateSize();
		this.repaint();
		this.doLayout();
	}
	
	//private void 
	
	private boolean setShowBackGroup(boolean isShow){
		boolean o = this.isShowBG;
		this.isShowBG = isShow;
		return o;
	}
	
	/**
	 * 返回一个打印对象。
	 */
	public Image getPrintScreen(){
		Image page = new BufferedImage(this.getWidth(), this.getHeight(), BufferedImage.TYPE_INT_ARGB);
		//page.getGraphics()
		boolean o = this.setShowBackGroup(false);
		this.paintAll(page.getGraphics());
		this.setShowBackGroup(o);		
		return page;
	}
	
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		if(this.isShowBG && doc != null && doc.bgImage != null){
			g.drawImage(doc.bgImage, 0, 0, bgObserver);
		}
	}
	
	protected void updateSize(){
		//this.setSize(d);
		if(doc == null || doc.bgImage == null) return;
	    Dimension size = new Dimension(doc.bgImage.getWidth(null), doc.bgImage.getHeight(null));
	    setPreferredSize(size);
	    setMinimumSize(size);
	    setMaximumSize(size);
	    setSize(size);	 
	    
	}
	
	/**
	 * 更新背景时，有可能图片没有加载完成。等图片加载完成后，重新刷新面板。
	 * @author deon
	 *
	 */
	class BgImageObserver implements ImageObserver{
		@Override
		public boolean imageUpdate(Image arg0, int arg1, int arg2, int arg3,
				int arg4, int arg5) {
			//log.info("xxxxxxxxxxxxx");
			if(arg1 == ImageObserver.ALLBITS){
				updateSize();
				repaint();
				doLayout();
				return false;				
			}
			return true;
		}		
	}
}
