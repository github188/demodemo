package org.notebook.gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;

import org.notebook.cache.NoteBook;
import org.notebook.services.GmailAuthCallback;
import org.notebook.services.GmailAuthencation;

public class LoginDailog extends JDialog {
	private NoteBook book = null;
	//private boolean c
	private GmailAuthencation gmail = new GmailAuthencation();
	private JLabel errorLabel = new JLabel("");
	private JLabel captchaLabel = new JLabel("验证码: ");
	private JTextField captcha = new JTextField("");
	
	private JPanel textControlsPane = new JPanel();
	private boolean captchaVisible = false;
	
	public LoginDailog(JFrame parent, NoteBook book){
		super(parent, true);
		this.book = book;		
		this.setTitle("用户登录");
		setContentPane(createSettingJPanel());
		//this.setPreferredSize(new Dimension(340, 200));
		setMinimumSize(new Dimension(340, 150));
		this.pack();
		this.setResizable(false);
	}	
	
	private JPanel createSettingJPanel(){
		JPanel p = new JPanel(new BorderLayout());
		
        final JTextField username = new JTextField(book.getUser() + "@gmail.com");
        final JPasswordField password = new JPasswordField(book.password);               
        JLabel nameLabel = new JLabel("Gmail: ");
        JLabel passwordLabel = new JLabel("密码: ");
        nameLabel.setLabelFor(username);
        passwordLabel.setLabelFor(password);
        captchaLabel.setLabelFor(captcha);
        
        nameLabel.setMinimumSize(new Dimension(140, 10));
        username.setMinimumSize(new Dimension(120, 10));
        
        errorLabel.setForeground(Color.red);
        password.setToolTipText("输入Gmail密码!");

        JPanel inputPane = new JPanel();
        GridBagLayout gridbag = new GridBagLayout();
        inputPane.setLayout(gridbag);

        JLabel[] labels = {new JLabel(""), nameLabel, passwordLabel, captchaLabel };
        Component[] textFields = {errorLabel, username, password, captcha, };
        addLabelTextRows(labels, textFields, gridbag, inputPane);
        
        textControlsPane.setLayout(new BorderLayout());
        textControlsPane.add(inputPane, BorderLayout.CENTER);
        final JLabel xxx = new JLabel("");
        xxx.setPreferredSize(new Dimension(200, 70));
        xxx.setBorder(BorderFactory.createEtchedBorder());
        JPanel captcaPancel = new JPanel(); 
        textControlsPane.add(captcaPancel, BorderLayout.SOUTH);
        captcaPancel.add(xxx);
        xxx.setVisible(false);
        captchaLabel.setVisible(false);
        captcha.setVisible(false);
        
        textControlsPane.setBorder(BorderFactory.createCompoundBorder(
                                	BorderFactory.createTitledBorder("Gmail登录"),
                                	BorderFactory.createEmptyBorder(5,5,5,5)));
        
        JPanel buttons = new JPanel();
        
        final JButton login = new JButton("登录");
        JButton close = new JButton("取消");
        
        final JDialog dailog = this;
        login.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				//String user = username.getText();
				login.setEnabled(false);
				username.setEnabled(false);
				password.setEnabled(false);
				dailog.validate();
				dailog.pack();
				errorLabel.setText("");
				new Thread(){
					public void run(){
						boolean result = gmail.login(username.getText(), password.getPassword(), 
								captcha.getText(), 
								new GmailAuthCallback(){
									public void showCaptcha(Image image) {
										xxx.setIcon(new ImageIcon(image));
								        xxx.setVisible(true);
								        captchaLabel.setVisible(true);
								        captcha.setVisible(true);
								        dailog.validate();
								        dailog.pack();
									}
									@Override
									public void error(String message) {
										errorLabel.setText(gmail.getErrorMsg(message));
										dailog.validate();
										dailog.pack();
									}
								}
							);
						if(result){
							book.user = username.getText();
							book.password = new String(password.getPassword());
							book.authToken = gmail.authToken;
							book.authSID = gmail.authSID;
							dailog.setVisible(false);
							dailog.dispose();
						}else {
							login.setEnabled(true);
							username.setEnabled(true);
							password.setEnabled(true);
						}
					}
				}.start();
			}
		});
        
        close.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				dailog.setVisible(false);
				dailog.dispose();
			}
		});        
        
        buttons.add(login);
        buttons.add(close);        
        p.add(textControlsPane, BorderLayout.CENTER);
        p.add(buttons, BorderLayout.SOUTH);
                        
		return p;
	}
	
    private void addLabelTextRows(JLabel[] labels, Component[] textFields,
            		GridBagLayout gridbag, Container container) {
		GridBagConstraints c = new GridBagConstraints();
		c.anchor = GridBagConstraints.EAST;
		int numLabels = labels.length;		
		for (int i = 0; i < numLabels; i++) {
			c.gridwidth = GridBagConstraints.RELATIVE; //next-to-last
			c.fill = GridBagConstraints.NONE;      //reset to default
			c.weightx = 1.0;                       //reset to default
			container.add(labels[i], c);
			
			c.gridwidth = GridBagConstraints.REMAINDER;     //end row
			c.fill = GridBagConstraints.HORIZONTAL;
			c.weightx = 3.0;
			container.add(textFields[i], c);
		}
    }
    
    private static void createAndShowGUI() {
        //Create and set up the window.
        JFrame frame = new JFrame("TextSamplerDemo");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        NoteBook book = new NoteBook();
        book.user = "deonwu";
        book.password = "ite,rt1201"; //.toCharArray();
        

        //Add content to the window.
        final LoginDailog s = new LoginDailog(frame, book);
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
    	org.apache.commons.logging.impl.SimpleLog logx = null;
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
