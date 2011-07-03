package org.notebook.gui.editor;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.BorderFactory;
import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.border.Border;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.notebook.cache.Document;
import org.notebook.cache.DocumentDefine;
import org.notebook.cache.InputDataField;
import org.notebook.gui.StatusBar;

public class SimplePrintPanel extends JPanel {
	//private Image bg = null;
	public StatusBar bar = null;
	private JTextField input = null;
	private Log log = LogFactory.getLog("print");
	private ImageObserver bgObserver = null;
	private DocumentDefine doc = null;
	private Document data = null;
	
	private JComponent focusOn = null;
	
	/*
	 * 设置是否显示背景，用来区分是打印，还是屏幕显示。如果屏幕显示需要看到背景，
	 * 打印的时候。不需要背景，实现套打功能。
	 */
	private boolean isShowBG = true;	
	
	public void loadDocument(DocumentDefine doc){
		this.setLayout(null);
		//无背景
		this.setOpaque(false);
		this.doc = doc;
		this.bgObserver = new BgImageObserver();
		
		LayoutFocus f1 = new LayoutFocus();
		LayoutContorl l = new LayoutContorl();
		
		JTextField jText = null;
		for(InputDataField f: doc.fields){
			jText = new JTextField();
			jText.setBounds(f.x, f.y, f.w, f.h);
			jText.addFocusListener(f1);
			jText.addKeyListener(l);
			jText.setName(f.name);
			Border border = BorderFactory.createEmptyBorder(2, 2, 2, 2);
			jText.setBorder(border);
			this.add(jText);
		}
		if(doc.bgImage.getHeight(this.bgObserver) > 0){
			this.updateSize();
		}
	}
	
	public void showDocument(Document doc){
		this.data = doc;
		JTextField f = null;
		for(Component c : this.getComponents()){
			if(c instanceof JTextField){
				f = (JTextField) c;
				f.setText(data.get(f.getName()));
			}
		}
	}
	
	public Document getDocumentData(){
		if(this.data == null){
			this.data = new Document();
		}
		
		JTextField f = null;
		for(Component c : this.getComponents()){
			if(c instanceof JTextField){
				f = (JTextField) c;
				data.put(f.getName(), f.getText());
			}
		}
		
		return this.data.copy();
	}
	
	public DocumentDefine saveLayout(){
		Map<String, Component> sub = new HashMap<String, Component>(); 
		for(Component c : this.getComponents()){
			sub.put(c.getName(), c);
		}
		
		Component child = null;
		for(InputDataField f: doc.fields){
			child = sub.get(f.name);
			if(child == null)continue;
			f.x = child.getX();
			f.y = child.getY();
			f.h = child.getSize().height;
			f.w = child.getSize().width;
		}
		
		return this.doc;
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
	public List<Image> getPrintScreen(){
		Image page = new BufferedImage(this.getWidth(), this.getHeight(), BufferedImage.TYPE_INT_ARGB);
		//page.getGraphics()
		boolean o = this.setShowBackGroup(false);
		//this.setBackground('');		
		this.paintAll(page.getGraphics());
		this.setShowBackGroup(o);
		List<Image> l = new ArrayList<Image>();
		l.add(page);
		return l;
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
	    if(this.getParent() != null){
	    	//在界面线程更新布局。
			SwingUtilities.invokeLater(new Runnable() {
				public void run(){	    	
					getParent().validate();
				}
			});
	    }
	    //repaint();
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
				return false;				
			}
			return true;
		}		
	}
	
	class LayoutFocus  implements FocusListener{

		@Override
		public void focusGained(FocusEvent event) {
			focusOn = (JComponent)event.getComponent();
			bar.setText("输入-" + focusOn.getName());
		}

		@Override
		public void focusLost(FocusEvent event) {
			focusOn = null;			
		}
		
	}
	
	class LayoutContorl implements KeyListener{
		int updateSizeMask = KeyEvent.CTRL_DOWN_MASK;
		int updateLocalMask = KeyEvent.ALT_DOWN_MASK;
		@Override
		public void keyPressed(KeyEvent event) {
			if(focusOn != null){
				if((event.getModifiersEx() & updateSizeMask) == updateSizeMask){

					switch(event.getKeyCode()){
						case KeyEvent.VK_DOWN:
							focusOn.setSize(focusOn.getWidth(), focusOn.getHeight() + 2);
							break;
						case KeyEvent.VK_UP:
							focusOn.setSize(focusOn.getWidth(), focusOn.getHeight() - 2);
							break;
						case KeyEvent.VK_LEFT:
							focusOn.setSize(focusOn.getWidth() -2 , focusOn.getHeight());
							break;
						case KeyEvent.VK_RIGHT:
							focusOn.setSize(focusOn.getWidth() + 2, focusOn.getHeight());
							break;
					}
				}else if ((event.getModifiersEx() & updateLocalMask) == updateLocalMask){
					int val = 0;
					switch(event.getKeyCode()){
						case KeyEvent.VK_DOWN:
							focusOn.setLocation(focusOn.getX(), focusOn.getY() + 2);
							break;
						case KeyEvent.VK_UP:
							focusOn.setLocation(focusOn.getX(), focusOn.getY() - 2);
							break;
						case KeyEvent.VK_LEFT:
							focusOn.setLocation(focusOn.getX() -2 , focusOn.getY());
							break;
						case KeyEvent.VK_RIGHT:
							focusOn.setLocation(focusOn.getX() + 2, focusOn.getY());
							break;
					}
				}
			}
		}

		@Override
		public void keyReleased(KeyEvent arg) {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void keyTyped(KeyEvent arg0) {
			// TODO Auto-generated method stub
			
		}
		
	}
}
