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
import javax.swing.JPanel;
import javax.swing.JDialog;
import javax.swing.JButton;
import javax.swing.JLabel;
import java.awt.GridLayout;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.io.Closeable;

import javax.swing.JTextField;

public class AddBuddyDialog extends JDialog {

	private JPanel jContentPane = null;
	private JPanel buttonPanel = null;
	private JButton okButton = null;
	private JButton cancelButton = null;
	private JPanel mainPanel = null;
	private JLabel displayNameLabel = null;
	private JTextField displayNameField = null;
	private JLabel sipAddressLabel = null;
	private JLabel sipPrefixLabel = null;
	private JTextField usernameField = null;
	private JLabel atLabel = null;
	private JTextField domainField = null;
	private BuddizWindow parent;

	/**
	 * This is the default constructor
	 */
	public AddBuddyDialog(BuddizWindow parent) {
		super();
		this.parent = parent;
		initialize();
	}

	/**
	 * This method initializes this
	 * @return void
	 */
	private void initialize() {
		this.setSize(316, 200);
		this.setTitle("Add a Buddy");
		this.setContentPane(getJContentPane());
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
			buttonPanel.add(getOkButton(), null);
			buttonPanel.add(getCancelButton(), null);
		}
		return buttonPanel;
	}

	/**
	 * This method initializes okButton	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getOkButton() {
		if (okButton == null) {
			okButton = new JButton();
			okButton.setText("OK");
			okButton.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					addBuddy(); // TODO Auto-generated Event stub actionPerformed()
				}
			});
		}
		return okButton;
	}

	private void addBuddy() {
		String uri = "sip:" + usernameField.getText().trim() + "@" + domainField.getText().trim();
		String displayName = displayNameField.getText().trim();
		if(displayName.equals(""))
			parent.addBuddy(null, uri, -1);
		else
			parent.addBuddy(displayName, uri, -1);
		close();
	}

	private void close() {
		dispose();
	}
	/**
	 * This method initializes cancelButton	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getCancelButton() {
		if (cancelButton == null) {
			cancelButton = new JButton();
			cancelButton.setText("Cancel");
			cancelButton.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					close();
				}
			});
		}
		return cancelButton;
	}

	/**
	 * This method initializes mainPanel	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getMainPanel() {
		if (mainPanel == null) {
			GridBagConstraints gridBagConstraints6 = new GridBagConstraints();
			gridBagConstraints6.fill = java.awt.GridBagConstraints.HORIZONTAL;
			gridBagConstraints6.gridy = 1;
			gridBagConstraints6.weightx = 1.0;
			gridBagConstraints6.ipadx = 5;
			gridBagConstraints6.ipady = 5;
			gridBagConstraints6.gridx = 4;
			GridBagConstraints gridBagConstraints5 = new GridBagConstraints();
			gridBagConstraints5.ipadx = 5;
			gridBagConstraints5.ipady = 5;
			gridBagConstraints5.gridx = 3;
			gridBagConstraints5.gridy = 1;
			atLabel = new JLabel();
			atLabel.setText("@");
			GridBagConstraints gridBagConstraints4 = new GridBagConstraints();
			gridBagConstraints4.fill = java.awt.GridBagConstraints.HORIZONTAL;
			gridBagConstraints4.gridy = 1;
			gridBagConstraints4.weightx = 1.0;
			gridBagConstraints4.gridwidth = 1;
			gridBagConstraints4.ipadx = 5;
			gridBagConstraints4.ipady = 5;
			gridBagConstraints4.gridx = 2;
			GridBagConstraints gridBagConstraints2 = new GridBagConstraints();
			gridBagConstraints2.gridx = 1;
			gridBagConstraints2.gridy = 1;
			gridBagConstraints2.ipadx = 5;
			gridBagConstraints2.ipady = 5;
			sipPrefixLabel = new JLabel();
			sipPrefixLabel.setText("sip:");
			GridBagConstraints gridBagConstraints11 = new GridBagConstraints();
			gridBagConstraints11.gridx = 0;
			gridBagConstraints11.anchor = java.awt.GridBagConstraints.EAST;
			gridBagConstraints11.gridheight = 4;
			gridBagConstraints11.ipadx = 5;
			gridBagConstraints11.ipady = 5;
			gridBagConstraints11.gridy = 1;
			sipAddressLabel = new JLabel();
			sipAddressLabel.setText("SIP Address: ");
			GridBagConstraints gridBagConstraints1 = new GridBagConstraints();
			gridBagConstraints1.gridy = 0;
			gridBagConstraints1.gridwidth = 4;
			gridBagConstraints1.fill = java.awt.GridBagConstraints.HORIZONTAL;
			gridBagConstraints1.ipadx = 5;
			gridBagConstraints1.ipady = 5;
			gridBagConstraints1.gridx = 1;
			GridBagConstraints gridBagConstraints = new GridBagConstraints();
			gridBagConstraints.gridx = 0;
			gridBagConstraints.anchor = java.awt.GridBagConstraints.EAST;
			gridBagConstraints.ipadx = 5;
			gridBagConstraints.ipady = 5;
			gridBagConstraints.gridy = 0;
			displayNameLabel = new JLabel();
			displayNameLabel.setText("Display Name: ");
			mainPanel = new JPanel();
			mainPanel.setLayout(new GridBagLayout());
			mainPanel.add(displayNameLabel, gridBagConstraints);
			mainPanel.add(getDisplayNameField(), gridBagConstraints1);
			mainPanel.add(sipAddressLabel, gridBagConstraints11);
			mainPanel.add(sipPrefixLabel, gridBagConstraints2);
			mainPanel.add(getUsernameField(), gridBagConstraints4);
			mainPanel.add(atLabel, gridBagConstraints5);
			mainPanel.add(getDomainField(), gridBagConstraints6);
		}
		return mainPanel;
	}

	/**
	 * This method initializes displayNameField	
	 * 	
	 * @return javax.swing.JTextField	
	 */
	private JTextField getDisplayNameField() {
		if (displayNameField == null) {
			displayNameField = new JTextField();
			displayNameField.setText("John Smith");
		}
		return displayNameField;
	}

	/**
	 * This method initializes usernameField	
	 * 	
	 * @return javax.swing.JTextField	
	 */
	private JTextField getUsernameField() {
		if (usernameField == null) {
			usernameField = new JTextField();
			usernameField.setText("johnsmith");
		}
		return usernameField;
	}

	/**
	 * This method initializes domainField	
	 * 	
	 * @return javax.swing.JTextField	
	 */
	private JTextField getDomainField() {
		if (domainField == null) {
			domainField = new JTextField();
			domainField.setText("example.com");
		}
		return domainField;
	}

}  //  @jve:decl-index=0:visual-constraint="10,10"
