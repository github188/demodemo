package org.notebook.gui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.SwingUtilities;
import javax.swing.table.AbstractTableModel;

import org.notebook.Version;
import org.notebook.cache.Configuration;

public class SettingsDialog extends JDialog {
	private String name = "";
	public Configuration config = null;
	
	public SettingsDialog(JFrame parent, Configuration config){
		super(parent, true);
		this.config = config;
		this.name = Version.getName();
		this.setTitle("系统参数配置");
		setContentPane(createAboutJPanel());
		this.setPreferredSize(new Dimension(400, 250));
		setMinimumSize(new Dimension(400, 250));
		this.pack();
		this.setResizable(true);
	}
	
	/**
	 * 
	 * 	 * @return
	 */
	private JPanel createAboutJPanel(){
		JPanel p = new JPanel(new BorderLayout());
		
		JPanel aboutPanel = new JPanel(new BorderLayout());
		
		JTable table = new JTable(new ConfigModel());
		table.getColumnModel().getColumn(0).setPreferredWidth(80);
		table.setEditingColumn(1);
		//table.getColumnModel().getColumn(0).s
		
		JScrollPane jsPanel = new JScrollPane();
		aboutPanel.add(jsPanel, BorderLayout.CENTER);	
		jsPanel.getViewport().add(table);
		
        JPanel buttons = new JPanel();
        JButton save = new JButton("保存");
        buttons.add(save);
        
        JButton close = new JButton("取消");
        buttons.add(close);
        
        
        p.add(buttons, BorderLayout.NORTH);
        
        p.add(aboutPanel, BorderLayout.CENTER);
        p.add(buttons, BorderLayout.SOUTH);
        
        final JDialog dailog = this;
        save.addActionListener(
        		new ActionListener(){
        			@Override
        			public void actionPerformed(ActionEvent e) {
        				config.saveRegistry();
        				dailog.setVisible(false);
        				dailog.dispose();   				
        			}
		});

        close.addActionListener(
        		new ActionListener(){
        			@Override
        			public void actionPerformed(ActionEvent e) {
        				dailog.setVisible(false);
        				dailog.dispose();        				
        			}
		});
		
		return p;
	}
	
	class ConfigModel extends AbstractTableModel{
		String[] columns = new String[]{"参数名称", "参数值"};
		//Configuration config = null;
		
		public String getColumnName(int column){
			 return columns[column];
		}
		
		@Override
		public int getColumnCount() {
			return columns.length;
		}

		@Override
		public int getRowCount() {
			return config.PARAMS.length;
		}

		@Override
		public Object getValueAt(int row, int col) {
			// TODO Auto-generated method stub
			if(col == 0){
				return config.PARAMS[row];
			}else if(col == 1) {
				return config.param.get(config.PARAMS[row]);
			}
			return "";
		}		
		
	    public boolean isCellEditable(int row, int col) {
	        //Note that the data/cell address is constant,
	        //no matter where the cell appears onscreen.
	        if (col == 1) {
	            return true;
	        } else {
	            return false;
	        }
	    }
	    
	    public void setValueAt(Object value, int row, int col) {
	    	if(col == 1){
	    		config.param.put(config.PARAMS[row], value.toString());
	    		fireTableCellUpdated(row, col);
	    	}
	    }
	}  
	
	   private static void createAndShowGUI() {
	        //Create and set up the window.
	        JFrame frame = new JFrame("TextSamplerDemo");
	        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);	        

	        //Add content to the window.
	        final SettingsDialog s = new SettingsDialog(frame, new Configuration());
	        //s.config = new Configuration();
	        JButton a = new JButton("test");
	        
	        a.addActionListener(new ActionListener(){
				@Override
				public void actionPerformed(ActionEvent e) {
					s.setVisible(true);				
				}});
	        frame.setLayout(new FlowLayout());
	        frame.add(a);

	        frame.setPreferredSize(new Dimension(400, 150));
	        //Display the window.
	        frame.pack();
	        frame.setVisible(true);
	    }
	    
	    public static void main(String[] args) {
	        //Schedule a job for the event dispatching thread:
	        //creating and showing this application's GUI.
	    	//MsgBox message = new MsgBox(null, "Hey you user, are you sure ?", true);
	    	//Dialog.
			//System.setProperty("http.proxyHost", "10.144.1.10");
			//System.setProperty("http.proxyPort", "8080");
	    	
	        SwingUtilities.invokeLater(new Runnable() {
	            public void run() {
	                 //Turn off metal's use of bold fonts
	            	//UIManager.put("swing.boldMetal", Boolean.FALSE);
					createAndShowGUI();
	            }
	        });
	    }   	

}
