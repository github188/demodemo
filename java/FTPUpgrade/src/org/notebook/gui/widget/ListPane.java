/*
 * ListPane.java
 *
 * Created on June 8, 2007, 9:17 PM
 */

package org.notebook.gui.widget;

import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.Action;
import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.border.Border;

public class ListPane extends JPanel {
    /**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = 6807890033041814417L;
	
	private static final int HORZ_PAD=12;
    private static final int VERT_PAD=6;
    /**
     * Creates new form ListPane
     */
    public ListPane() {
        initComponents();
        Border b=BorderFactory.createEmptyBorder(VERT_PAD,HORZ_PAD,VERT_PAD,HORZ_PAD);
        setBorder(b);
    }
    private void initComponents() {
        FlowLayout layout=new FlowLayout(FlowLayout.LEADING, 5, 10);
        setLayout(layout);
    }
    public void addItem(String text, ImageIcon icon){
    	addItem(text,icon,null);
    }
    public void addItem(String text, ImageIcon icon, final Action action){
    	LinkLabel lblItem = new LinkLabel();
   
        lblItem.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				if(action != null){action.actionPerformed(event);}
				else
					{System.out.println(((LinkLabel)event.getSource()).getText());}
			}
		});
        if(icon!=null){
            lblItem.setIcon(icon);
            lblItem.setForeground(GuiUtils.getLookAndFeelColor("foregroundColor"));
        }else
            lblItem.setForeground(GuiUtils.getLookAndFeelColor("foregroundColor"));
        lblItem.setText(text);
        add(lblItem);
    }
    public void addButtonItem(String text, ImageIcon icon){
        JButton btnItem=new JButton();
       // btnItem.setBorderPainted(false);
        btnItem.setBorder(BorderFactory.createCompoundBorder());
        btnItem.setBackground(GuiUtils.getLookAndFeelColor("backgroundFillColor"));
        if(icon!=null){
        	btnItem.setIcon(icon);
        }
        	btnItem.setForeground(GuiUtils.getLookAndFeelColor("foregroundColor"));
        btnItem.setText(text);
        add(btnItem);
    }
    
    public void addListItem(){
    	String[] data = {"one", "two", "three", "four"};
    	JList lstItem = new JList(data);
    	add(lstItem);
    }
}
