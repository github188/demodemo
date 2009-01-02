/*
 * Copyright (c) 2006 Ubiquity Software Corporation plc. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 *  
 * - Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution. 
 * 
 * - The name of Ubiquity Software Corporation may not be used to endorse or 
 * promote products derived from this software without specific prior written 
 * permission.
 * 
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITH ALL FAULTS AND WITHOUT
 * ANY TECHNICAL SUPPORT OR WARRANTIES OF ANY KIND. YOU ASSUME ALL RISK OF USE
 * OF THIS SOFTWARE. TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW,
 * UBIQUITY, ITS SUPPLIERS AND RESELLERS DISCLAIM ALL WARRANTIES WITH RESPECT TO
 * THE TO THE SOFTWARE WHETHER EXPRESS, IMPLIED, STATUTORY OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * NONINFRINGEMENT, QUALITY OF PERFORMANCE AND FITNESS FOR A PARTICULAR PURPOSE.
 * TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, IN NO EVENT WILL UBIQUITY,
 * OR ITS SUPPLIERS OR RESELLERS BE LIABLE FOR ANY INDIRECT, SPECIAL, INCIDENTAL
 * OR CONSEQUENTIAL DAMAGES ARISING OUT OF OR RELATED TO THE USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF ADVISED OF THE POSSIBILITY THEREOF, AND
 * REGARDLESS OF THE LEGAL OR EQUITABLE THEORY (CONTRACT, TORT OR OTHERWISE)
 * UPON WHICH THE CLAIM IS BASED. TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE
 * LAW, IN NO EVENT WILL UBIQUITY, OR ITS SUPPLIERS OR RESELLERS BE LIABLE FOR
 * DAMAGES FOR LOSS OF GOODWILL, LOST DATA, WORK STOPPAGE, COMPUTER FAILURE OR
 * MALFUNCTION, OR ANY AND ALL OTHER COMMERCIAL DAMAGES OR LOSSES, EVEN IF
 * ADVISED OF THE POSSIBILITY THEREOF, AND REGARDLESS OF THE LEGAL OR EQUITABLE
 * THEORY (CONTRACT, TORT OR OTHERWISE) UPON WHICH THE CLAIM IS BASED.
 * UBIQUITY'S TOTAL LIABILITY FOR THE SOFTWARE WILL BE LIMITED TO THE LICENSE
 * FEES PAID BY YOU TO UBIQUITY FOR THE SOFTWARE.
 * 
 * Contact: Ubiquity Software Corporation plc, Building 3, Eastern Business
 * Park, Wern Fawr Lane, St Mellons, Cardiff CF3 5EA United Kingdom, Attention:
 * Technical Services and Operation Manager.
 */
package udn.buddiz.windows;

import java.awt.BorderLayout;

import javax.sip.InvalidArgumentException;
import javax.sip.SipException;
import javax.swing.JPanel;
import javax.swing.JDialog;
import javax.swing.JButton;
import javax.swing.JLabel;
import java.awt.GridLayout;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.io.Closeable;
import java.net.UnknownHostException;
import java.text.ParseException;

import javax.swing.JTextField;
import javax.swing.JTextArea;

import udn.buddiz.FatalBuddizException;
import udn.buddiz.events.BuddizEvent;
import udn.buddiz.events.BuddizEventListener;
import udn.buddiz.events.ErrorEvent;
import udn.buddiz.events.MessageEvent;
import udn.buddiz.sip.SipEngine;

public class ChatDialog extends JDialog implements BuddizEventListener {

	private JPanel jContentPane = null;
	private JPanel buttonPanel = null;
	private JButton sendButton = null;
	private JButton closeButton = null;
	private JPanel mainPanel = null;
	private JLabel messageLabel = null;
	private JTextField messageField = null;
	private BuddizWindow parent;
	private JTextArea chatTextArea = null;
	private Buddy buddy;  //  @jve:decl-index=0:
	private SipEngine engine = null;

	/**
	 * This is the default constructor
	 */
	public ChatDialog(BuddizWindow parent, Buddy buddy) {
		super();
		this.buddy = buddy;
		this.parent = parent;
		initialize();
	}

	/**
	 * This method initializes this
	 * @return void
	 */
	private void initialize() {
		this.setSize(316, 200);
		this.setTitle("Chatting with " + buddy);
		this.setContentPane(getJContentPane());
		
		this.engine = SipEngine.getInstance();
	}

	/**
	 * This method initializes jContentPane
	 * @return javax.swing.JPanel
	 */
	private JPanel getJContentPane() {
		if (jContentPane == null) {
			jContentPane = new JPanel();
			jContentPane.setLayout(new BorderLayout());
			jContentPane.add(getButtonPanel(), java.awt.BorderLayout.SOUTH);
			jContentPane.add(getMainPanel(), java.awt.BorderLayout.CENTER);
		}
		return jContentPane;
	}

	/**
	 * This method initializes buttonPanel	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getButtonPanel() {
		if (buttonPanel == null) {
			buttonPanel = new JPanel();
			buttonPanel.add(getCloseButton(), null);
		}
		return buttonPanel;
	}

	/**
	 * This method initializes sendButton	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getSendButton() {
		if (sendButton == null) {
			sendButton = new JButton();
			sendButton.setText("Send");
			sendButton.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					send(); 
				}
			});
		}
		return sendButton;
	}

	protected void send() {
		
		try {
			this.engine.sendMessage(buddy, messageField.getText(), this);
		} catch (Throwable e) {
			e.printStackTrace();
			chatTextArea.append("\nError while sending: "+ e.getMessage());
		}
		
	}

	private void close() {
		this.engine.removeMessageListener(buddy);
		dispose();
	}
	
	/**
	 * This method initializes closeButton	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getCloseButton() {
		if (closeButton == null) {
			closeButton = new JButton();
			closeButton.setText("Close");
			closeButton.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					close();
				}
			});
		}
		return closeButton;
	}

	/**
	 * This method initializes mainPanel	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getMainPanel() {
		if (mainPanel == null) {
			GridBagConstraints gridBagConstraints = new GridBagConstraints();
			gridBagConstraints.gridx = 3;
			gridBagConstraints.gridy = 1;
			GridBagConstraints gridBagConstraints1 = new GridBagConstraints();
			gridBagConstraints1.fill = GridBagConstraints.BOTH;
			gridBagConstraints1.gridy = 0;
			gridBagConstraints1.weightx = 1.0;
			gridBagConstraints1.weighty = 1.0;
			gridBagConstraints1.gridwidth = 4;
			gridBagConstraints1.ipadx = 5;
			gridBagConstraints1.ipady = 5;
			gridBagConstraints1.gridx = 0;
			GridBagConstraints gridBagConstraints2 = new GridBagConstraints();
			gridBagConstraints2.fill = java.awt.GridBagConstraints.HORIZONTAL;
			gridBagConstraints2.gridy = 1;
			gridBagConstraints2.weightx = 1.0;
			gridBagConstraints2.gridwidth = 1;
			gridBagConstraints2.ipadx = 5;
			gridBagConstraints2.ipady = 5;
			gridBagConstraints2.gridx = 2;
			GridBagConstraints gridBagConstraints3 = new GridBagConstraints();
			gridBagConstraints3.gridx = 0;
			gridBagConstraints3.anchor = java.awt.GridBagConstraints.EAST;
			gridBagConstraints3.gridheight = 4;
			gridBagConstraints3.ipadx = 5;
			gridBagConstraints3.ipady = 5;
			gridBagConstraints3.gridy = 1;
			messageLabel = new JLabel();
			messageLabel.setText("Message:");
			mainPanel = new JPanel();
			mainPanel.setLayout(new GridBagLayout());
			mainPanel.add(messageLabel, gridBagConstraints3);
			mainPanel.add(getMessageField(), gridBagConstraints2);
			mainPanel.add(getChatTextArea(), gridBagConstraints1);
			mainPanel.add(getSendButton(), gridBagConstraints);
		}
		return mainPanel;
	}

	/**
	 * This method initializes messageField	
	 * 	
	 * @return javax.swing.JTextField	
	 */
	private JTextField getMessageField() {
		if (messageField == null) {
			messageField = new JTextField();
			messageField.setText("");
		}
		return messageField;
	}

	/**
	 * This method initializes chatTextArea	
	 * 	
	 * @return javax.swing.JTextArea	
	 */
	private JTextArea getChatTextArea() {
		if (chatTextArea == null) {
			chatTextArea = new JTextArea();
		}
		return chatTextArea;
	}

	public Buddy getBuddy() {
		return buddy;
	}

	public void setBuddy(Buddy currentBuddy) {
		this.buddy = currentBuddy;
	}

	public void processEvent(BuddizEvent event) {
		if(event instanceof ErrorEvent) {
			ErrorEvent error = (ErrorEvent) event;
			chatTextArea.append("\nError while sending: "+ error.getMessage() + "\n");
			return;
		}
		if(event instanceof MessageEvent) {
			MessageEvent message = (MessageEvent) event;
			Buddy buddy = new Buddy(message.getSender());
			this.engine.addMessageListener(buddy, this);
			chatTextArea.append("From "+ buddy + ": " +
					message.getMessage() + "\n");
			return;
		}

		//Here we could use a SuccessEvent but it's not useful
	}

}  //  @jve:decl-index=0:visual-constraint="10,10"
