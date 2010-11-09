package org.notebook.gui;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;

import org.notebook.cache.NoteBook;
import org.notebook.services.BookController;

public class NoteBookSettings extends JDialog {
	protected static final String textFieldString = "JTextField";
	private NoteBook book = null;
	private BookController controller = null;
	//private boolean c
	
	public NoteBookSettings(JFrame parent, BookController controller){
		this(parent, null, controller);
	}
	public NoteBookSettings(JFrame parent, NoteBook book, BookController controller){
		super(parent, true);
		if(book == null){
			this.book = controller.getNoteBook();
		}else {
			this.book = book;
		}
		
		this.controller = controller;
		
		this.setTitle("笔记本设置");
		setContentPane(createSettingJPanel());
		//this.setPreferredSize(new Dimension(400, 210));
		this.pack();
		this.setResizable(false);
	}
	
	private JPanel createSettingJPanel(){
		JPanel p = new JPanel(new BorderLayout());
		
        final JTextField name = new JTextField(book.name);
        final JTextField username = new JTextField(book.user);
        final JPasswordField password = new JPasswordField(book.password);
        final JTextField endpoint = new JTextField(book.endpoint);
        final JTextField proxy = new JTextField(book.proxy);
        
        if(!book.isDev){
        	endpoint.setEditable(false);
        }
                
        JLabel nameLabel = new JLabel("笔记本名: ");
        JLabel usernameLabel = new JLabel("Gmail: ");
        JLabel usernamePassword = new JLabel("密码: ");
        JLabel endpointLabel = new JLabel("服务器地址: ");
        JLabel proxyLabel = new JLabel("HTTP代理: ");
        
        nameLabel.setLabelFor(name);
        usernameLabel.setLabelFor(username);
        endpointLabel.setLabelFor(endpoint);
        proxyLabel.setLabelFor(proxy);

        JPanel textControlsPane = new JPanel();
        GridBagLayout gridbag = new GridBagLayout();
        //GridBagConstraints c = new GridBagConstraints();

        textControlsPane.setLayout(gridbag);

        JLabel[] labels = {nameLabel, usernameLabel, usernamePassword, endpointLabel, proxyLabel};
        JTextField[] textFields = {name, username, password, endpoint, proxy};
        addLabelTextRows(labels, textFields, gridbag, textControlsPane);

        //textControlsPane.add(actionLabel, c);
        textControlsPane.setBorder(BorderFactory.createCompoundBorder(
                                	BorderFactory.createTitledBorder("设置信息"),
                                	BorderFactory.createEmptyBorder(5,5,5,5)));
        
        JPanel buttons = new JPanel();
        
        JButton save = new JButton("保存");
        JButton close = new JButton("取消");
        
        final JDialog dailog = this;
        save.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				book.name = name.getText();
				book.user = username.getText();
				book.setEndPoint(endpoint.getText());
				book.proxy = proxy.getText();
				if(controller != null){
					controller.dispatchEvent(MenuToolbar.UPDATEDSETTINGS);
				}
				dailog.setVisible(false);
				dailog.dispose();
			}
		});
        
        close.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				dailog.setVisible(false);
				dailog.dispose();
			}
		});        
        
        buttons.add(save);
        buttons.add(close);        
        p.add(textControlsPane, BorderLayout.CENTER);
        p.add(buttons, BorderLayout.SOUTH);
                        
		return p;
	}
	
    private void addLabelTextRows(JLabel[] labels, JTextField[] textFields,
            		GridBagLayout gridbag, Container container) {
		GridBagConstraints c = new GridBagConstraints();
		c.anchor = GridBagConstraints.EAST;
		int numLabels = labels.length;		
		for (int i = 0; i < numLabels; i++) {
			c.gridwidth = GridBagConstraints.RELATIVE; //next-to-last
			c.fill = GridBagConstraints.NONE;      //reset to default
			c.weightx = 0.0;                       //reset to default
			container.add(labels[i], c);
			
			c.gridwidth = GridBagConstraints.REMAINDER;     //end row
			c.fill = GridBagConstraints.HORIZONTAL;
			c.weightx = 1.0;
			container.add(textFields[i], c);
		}
    }
    
    private static void createAndShowGUI() {
        //Create and set up the window.
        JFrame frame = new JFrame("TextSamplerDemo");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        //Add content to the window.
        final NoteBookSettings s = new NoteBookSettings(frame, new NoteBook(), null);
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
    	
    	
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                 //Turn off metal's use of bold fonts
            	//UIManager.put("swing.boldMetal", Boolean.FALSE);
				createAndShowGUI();
            }
        });
    }    
	
}
