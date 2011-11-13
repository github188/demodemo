package org.http.channel.client.gui;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;

public class LoginPanel extends JPanel{	
	public void initPanel(){
		//private JPanel createSettingJPanel(){
		//JPanel p = new JPanel(new BorderLayout());		
        final JTextField name = new JTextField("");
        final JTextField username = new JTextField("");
        final JPasswordField password = new JPasswordField("");
        //final JTextField endpoint = new JTextField("");
        final JTextField proxy = new JTextField("");
        
                
        JLabel nameLabel = new JLabel("外部域名: ");
        JLabel usernameLabel = new JLabel("注册密码: ");
        JLabel usernamePassword = new JLabel("内部地址: ");
        //JLabel endpointLabel = new JLabel("服务器地址: ");
        JLabel proxyLabel = new JLabel("HTTP代理: ");
        
        nameLabel.setLabelFor(name);
        usernameLabel.setLabelFor(username);
        //endpointLabel.setLabelFor(endpoint);
        proxyLabel.setLabelFor(proxy);

        JPanel textControlsPane = new JPanel();
        GridBagLayout gridbag = new GridBagLayout();
        //GridBagConstraints c = new GridBagConstraints();

        textControlsPane.setLayout(gridbag);

        JLabel[] labels = {nameLabel, usernameLabel, usernamePassword, proxyLabel};
        JTextField[] textFields = {name, username, password, proxy};
        addLabelTextRows(labels, textFields, gridbag, textControlsPane);

        //textControlsPane.add(actionLabel, c);
        textControlsPane.setBorder(BorderFactory.createCompoundBorder(
                                	BorderFactory.createTitledBorder("设置信息"),
                                	BorderFactory.createEmptyBorder(5,5,5,5)));
        
        JPanel buttons = new JPanel();
        
        JButton save = new JButton("保存");
        JButton close = new JButton("取消");
                
        buttons.add(save);
        buttons.add(close);
        
        add(textControlsPane, BorderLayout.CENTER);
        add(buttons, BorderLayout.SOUTH);
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
}
