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
import java.awt.GridBagLayout;
import java.awt.Image;

import javax.swing.DefaultListModel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JMenu;
import javax.swing.JToolBar;
import javax.swing.ImageIcon;
import javax.swing.JScrollPane;
import javax.swing.JList;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.ListModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import udn.buddiz.Buddiz;
import udn.buddiz.FatalBuddizException;
import udn.buddiz.events.BuddizEvent;
import udn.buddiz.events.BuddizEventListener;
import udn.buddiz.events.ErrorEvent;
import udn.buddiz.events.MessageEvent;
import udn.buddiz.events.NotifyEvent;
import udn.buddiz.events.RegisteredEvent;
import udn.buddiz.sip.SipEngine;

import java.awt.FlowLayout;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.image.ImageObserver;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Enumeration;
import java.util.List;
import java.util.Properties;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.awt.GridLayout;
import java.awt.GridBagConstraints;

public class BuddizWindow extends JFrame implements BuddizEventListener {

	private JPanel jContentPane = null;
	private JPanel topPanel = null;
	private JMenuBar mainMenuBar = null;
	private JMenu fileMenu = null;
	private JMenu helpMenu = null;
	private JMenuItem fileExitItem = null;
	private JToolBar mainToolBar = null;
	private JMenuItem addBuddyItem = null;
	private JMenuItem helpAboutItem = null;
	private JMenuItem helpAboutItem2 = null;
	private JScrollPane buddyListScrollPane = null;
	private JList buddyList = null;
	private JPanel bottomPanel = null;
	private JButton callBtn = null;
	private JComboBox statusBox = null;
	private JLabel statusLabel = null;
	private JPanel toolbarPanel = null;
	private ExecutorService taskQueue;
    private Properties properties=new Properties();
	private JLabel imageLabel = null;
	private JButton chatButton = null;
	protected Buddy currentBuddy;
	private SipEngine engine = null;

	/**
	 * This method initializes titlePanel	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getTopPanel() {
		if (topPanel == null) {
			GridBagLayout gridBagLayout = new GridBagLayout();
			topPanel = new JPanel();
			topPanel.setLayout(gridBagLayout);
			GridBagConstraints gridBagConstraints2 = new GridBagConstraints();
			gridBagConstraints2.gridx = 0;
			gridBagConstraints2.gridwidth= 2;
			gridBagConstraints2.gridy = 0;
			gridBagConstraints2.fill = GridBagConstraints.HORIZONTAL;
			topPanel.add(getToolbarPanel(), gridBagConstraints2);

			statusLabel = new JLabel();
			statusLabel.setText("Status: ");
			GridBagConstraints gridBagConstraints = new GridBagConstraints();
			gridBagConstraints.gridx = 0;
			gridBagConstraints.gridy = 1;
			gridBagConstraints.fill = GridBagConstraints.NONE;
			topPanel.add(statusLabel, gridBagConstraints);
			GridBagConstraints gridBagConstraints1 = new GridBagConstraints();
			gridBagConstraints1.gridx = 1;
			gridBagConstraints1.gridy = 1;
			gridBagConstraints1.fill = GridBagConstraints.HORIZONTAL;
			topPanel.add(getStatusBox(), gridBagConstraints1);
			GridBagConstraints gridBagConstraints3 = new GridBagConstraints();
			gridBagConstraints3.gridx = 0;
			gridBagConstraints3.gridy = 2;
		}
		return topPanel;
	}

	/**
	 * This method initializes mainMenuBar	
	 * 	
	 * @return javax.swing.JMenuBar	
	 */
	private JMenuBar getMainMenuBar() {
		if (mainMenuBar == null) {
			mainMenuBar = new JMenuBar();
			mainMenuBar.add(getFileMenu());
			mainMenuBar.add(getHelpMenu());
		}
		return mainMenuBar;
	}

	/**
	 * This method initializes fileMenu	
	 * 	
	 * @return javax.swing.JMenu	
	 */
	private JMenu getFileMenu() {
		if (fileMenu == null) {
			fileMenu = new JMenu();
			fileMenu.setText("File");
			fileMenu.setMnemonic(java.awt.event.KeyEvent.VK_F);
			fileMenu.add(getFileExitItem());
		}
		return fileMenu;
	}

	/**
	 * This method initializes helpMenu	
	 * 	
	 * @return javax.swing.JMenu	
	 */
	private JMenu getHelpMenu() {
		if (helpMenu == null) {
			helpMenu = new JMenu();
			helpMenu.setText("Help");
			helpMenu.setMnemonic(java.awt.event.KeyEvent.VK_H);
			helpMenu.add(getHelpAboutItem2());
		}
		return helpMenu;
	}

	/**
	 * This method initializes fileExitItem	
	 * 	
	 * @return javax.swing.JMenuItem	
	 */
	private JMenuItem getFileExitItem() {
		if (fileExitItem == null) {
			fileExitItem = new JMenuItem();
			fileExitItem.setText("Exit");
			fileExitItem.setMnemonic(java.awt.event.KeyEvent.VK_X);
			fileExitItem.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					dispose();
					close();
				}
			});
		}
		return fileExitItem;
	}

	private void close() {
		engine.stopSubscriptions();
		unsubscribe();
		
		try { //Wait a few seconds for the messages to go through...
		Thread.sleep(2000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		engine.unregister();

		try { //Wait a few seconds for the messages to go through...
			Thread.sleep(2000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		} 
		
		taskQueue.shutdown();
        URL propURL = Buddiz.class.getClassLoader().getResource("buddiz.properties");
        if(propURL==null)
        	propURL = Buddiz.class.getResource("buddiz.properties");
        if(propURL==null) {
			JOptionPane.showMessageDialog(null, "The file buddiz.properties doesn't exist.");
			return;
        }
        File file;
		try {
			file = new File(propURL.toURI());
	        FileOutputStream os = new FileOutputStream(file);
			properties.store(os, null);
		} catch (Throwable e) {
			e.printStackTrace();
			JOptionPane.showMessageDialog(null, "Problem while saving to the file buddiz.properties:" + e.getMessage());
		}
		try { //wait a bit for unregistration and unsubscriptions to be done
			Thread.sleep(2000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		} 
		System.exit(0);
	}
	
	/**
	 * This method initializes mainToolBar	
	 * 	
	 * @return javax.swing.JToolBar	
	 */
	private JToolBar getMainToolBar() {
		if (mainToolBar == null) {
			mainToolBar = new JToolBar();
			mainToolBar.setComponentOrientation(java.awt.ComponentOrientation.UNKNOWN);
			mainToolBar.add(getAddBuddyItem());
			mainToolBar.add(getHelpAboutItem());

			String l = mainToolBar.getLayout().toString();
			
		}
		return mainToolBar;
	}

	/**
	 * This method initializes addBuddyItem	
	 * 	
	 * @return javax.swing.JMenuItem	
	 */
	private JMenuItem getAddBuddyItem() {
		if (addBuddyItem == null) {
			addBuddyItem = new JMenuItem();
			addBuddyItem.setText("Add Buddy");
			addBuddyItem.setMnemonic(java.awt.event.KeyEvent.VK_A);
			addBuddyItem.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
			addBuddyItem.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);
			addBuddyItem.setPressedIcon(new ImageIcon(getClass().getResource("/icons/user_add_pressed.png")));
			addBuddyItem.setRolloverIcon(new ImageIcon(getClass().getResource("/icons/user_add_over.png")));
			addBuddyItem.setDisabledIcon(new ImageIcon(getClass().getResource("/icons/user_add_disabled.png")));
			addBuddyItem.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
			addBuddyItem.setToolTipText("Add a buddy to the list.");
			addBuddyItem.setPreferredSize(new java.awt.Dimension(90,49));
			addBuddyItem.setIcon(new ImageIcon(getClass().getResource("/icons/user_add.png")));
			addBuddyItem.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					menuAddBuddy();
				}
			});
		}
		return addBuddyItem;
	}

	/**
	 * This method initializes helpAboutItem	
	 * 	
	 * @return javax.swing.JMenuItem	
	 */
	private JMenuItem getHelpAboutItem() {
		if (helpAboutItem == null) {
			helpAboutItem = new JMenuItem();
			helpAboutItem.setText("About");
			helpAboutItem.setMnemonic(java.awt.event.KeyEvent.VK_B);
			helpAboutItem.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
			helpAboutItem.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);
			helpAboutItem.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
			helpAboutItem.setToolTipText("Get information about this product.");
			helpAboutItem.setPreferredSize(new java.awt.Dimension(59,49));
			helpAboutItem.setIcon(new ImageIcon(getClass().getResource("/icons/help.png")));
			helpAboutItem.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					menuHelpAbout(); 
				}
			});
		}
		return helpAboutItem;
	}

	private void menuHelpAbout() {
		JOptionPane.showMessageDialog(this, "<HTML><BODY><H2>This app is a simple IM and VoIP client using SIP</H2><p>Offered with no warranties, use at your own risk.</p></BODY></HTML>", "About Buddiz", JOptionPane.INFORMATION_MESSAGE);
	}
	
	private void menuAddBuddy() {
		AddBuddyDialog addBuddyDialog = new AddBuddyDialog(this);
		addBuddyDialog.setVisible(true);
	}
	
	/**
	 * This method initializes helpAboutItem2	
	 * 	
	 * @return javax.swing.JMenuItem	
	 */
	private JMenuItem getHelpAboutItem2() {
		if (helpAboutItem2 == null) {
			helpAboutItem2 = new JMenuItem();
			helpAboutItem2.setText("About");
			helpAboutItem2.setMnemonic(java.awt.event.KeyEvent.VK_B);
			helpAboutItem2.setHorizontalTextPosition(javax.swing.SwingConstants.LEFT);
			helpAboutItem2.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);
			helpAboutItem2.setIcon(new ImageIcon(getClass().getResource("/icons/help.png")));
			helpAboutItem2.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					menuHelpAbout(); 
				}
			});
		}
		return helpAboutItem2;
	}

	/**
	 * This method initializes buddyListScrollPane	
	 * 	
	 * @return javax.swing.JScrollPane	
	 */
	private JScrollPane getBuddyListScrollPane() {
		if (buddyListScrollPane == null) {
			buddyListScrollPane = new JScrollPane();
			buddyListScrollPane.setViewportView(getBuddyList());
		}
		return buddyListScrollPane;
	}

	/**
	 * This method initializes buddyList	
	 * 	
	 * @return javax.swing.JList	
	 */
	private JList getBuddyList() {
		if (buddyList == null) {
			buddyList = new JList(new DefaultListModel());
			buddyList.setCellRenderer(new BuddyListCellRenderer());
			buddyList.addListSelectionListener(new ListSelectionListener() {

				public void valueChanged(ListSelectionEvent event) {
					int index = event.getFirstIndex();
					if(index==-1) {
						currentBuddy = null;
						return;
					}
					DefaultListModel list = (DefaultListModel) buddyList.getModel();
					Buddy buddy = (Buddy) list.get(index);
					currentBuddy = buddy;
				}
				
			});
		}
		return buddyList;
	}

	/**
	 * This method initializes bottomPanel	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getBottomPanel() {
		if (bottomPanel == null) {
			bottomPanel = new JPanel();
			bottomPanel.add(getChatButton(), null);
			bottomPanel.add(getCallBtn(), null);
		}
		return bottomPanel;
	}

	/**
	 * This method initializes callBtn	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getCallBtn() {
		if (callBtn == null) {
			callBtn = new JButton();
			callBtn.setText("Call");
		}
		return callBtn;
	}

	/**
	 * This method initializes statusBox	
	 * 	
	 * @return javax.swing.JComboBox	
	 */
	private JComboBox getStatusBox() {
		if (statusBox == null) {
			statusBox = new JComboBox(new Object [] {Mood.Offline, Mood.Happy, Mood.Sad});
			statusBox.setToolTipText("Indicates your current status.");
			statusBox.addItemListener(new ItemListener() {
				public void itemStateChanged(final ItemEvent e) {
					taskQueue.execute(new Runnable(){
						public void run() {
							changeStatus(e); 
						}
					});
				}
			});
		}
		return statusBox;
	}

	private void changeStatus(ItemEvent event) {
		//Only process selections.
		if(event.getStateChange() == ItemEvent.DESELECTED)
			return;

		Mood mood = (Mood)event.getItem();

		if(mood == Mood.Offline) {
			if(!engine.isRegistered())
				return;
			engine.setRegistrationPayload(null);
			engine.unregister();
			unsubscribe();
			return;
		}
		if( (mood == Mood.Happy) || (mood == Mood.Sad) ) {
			engine.setRegistrationPayload(mood.toString());
			engine.register();
			return;
		}
		
	}

	/**
	 * This method initializes toolbarPanel	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getToolbarPanel() {
		if (toolbarPanel == null) {
			imageLabel = new JLabel();
			imageLabel.setText("Icon");
			loadImageLabel();
			FlowLayout flowLayout = new FlowLayout();
			flowLayout.setAlignment(java.awt.FlowLayout.LEFT);
			toolbarPanel = new JPanel();
			toolbarPanel.setLayout(flowLayout);
			toolbarPanel.add(getMainToolBar(), null);
			toolbarPanel.add(imageLabel, null);
		}
		return toolbarPanel;
	}

	private void loadImageLabel() {
		Runnable loadIcon = new Runnable() {
			public void run() {
				try {
					URL iconURL = new URL(properties.getProperty("icon"));
				    if (iconURL == null) {
				    	return;
				    }
				    ImageIcon icon = new ImageIcon(iconURL);
				    ImageObserver o=null;
				    icon.setImageObserver(new ImageObserver() {
						public boolean imageUpdate(Image img, int infoflags, int x, int y, int width, int height) {
							if( (infoflags & ImageObserver.ABORT) > 0 ) {
								imageLabel.setText("Error loading icon");
								return false;
							}
							if( (infoflags & ImageObserver.ABORT) > 0 ) {
								imageLabel.setText("Error loading icon");
								return false;
							}
							return true;
						}
				    });
				    icon.getImage();
				    imageLabel.setText(null);
				    imageLabel.setIcon(icon);
				} catch (Throwable e) {
					imageLabel.setText("Error loading icon");
					e.printStackTrace();
				}
			}
		};
		taskQueue.execute(loadIcon);
	}
	
	/**
	 * This is the default constructor
	 * @param username 
	 * @throws FatalBuddizException 
	 */
	public BuddizWindow(final String username) {
		super();
        InputStream is = Buddiz.class.getClassLoader().getResourceAsStream("buddiz.properties");
        if(is==null)
        	is = Buddiz.class.getResourceAsStream("buddiz.properties");
        if(is==null) {
			JOptionPane.showMessageDialog(null, "The file buddiz.properties isn't in the classpath.");
			return;
        }
    	try {
			properties.load(is);
		} catch (IOException e) {
			JOptionPane.showMessageDialog(null, "Problem loading file buddiz.properties: " + e.getMessage());
			return;
		}
        
		taskQueue = Executors.newSingleThreadExecutor();

		initialize(username);
		
		final BuddizWindow window = this;
		
		Runnable initSipStackTask = new Runnable() {
			public void run() {
				engine = SipEngine.getInstance();
				engine.setUsername(username);
				engine.setRegistrationPayload(Mood.Happy.toString());
				engine.startup(window);
			}
		};
		taskQueue.execute(initSipStackTask);

		Runnable fillBuddyListTask = new Runnable() {
			public void run() {
				DefaultListModel list = (DefaultListModel) buddyList.getModel();
				Enumeration keys = properties.keys();
				while (keys.hasMoreElements()) {
					
					String key = (String) keys.nextElement();
					if(!key.startsWith("buddy."))
						continue;
					
					String buddyString = properties.getProperty(key);
					addBuddy(buddyString, -1);
				}
			}
		};
		taskQueue.execute(fillBuddyListTask);
	}

	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize(String username) {
		this.setSize(314, 298);
		this.setJMenuBar(getMainMenuBar());
		this.setName("buddizWindow");
		this.setDefaultCloseOperation(javax.swing.JFrame.DISPOSE_ON_CLOSE);
		this.addWindowListener(new java.awt.event.WindowAdapter() {
			   public void windowClosing(java.awt.event.WindowEvent e) {
				   close();
			   }
			});
		this.setContentPane(getJContentPane());
		this.setTitle("Buddiz - " + username);
	}

	/**
	 * This method initializes jContentPane
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJContentPane() {
		if (jContentPane == null) {
			jContentPane = new JPanel();
			jContentPane.setLayout(new BorderLayout());
			jContentPane.add(getTopPanel(), java.awt.BorderLayout.NORTH);
			jContentPane.add(getBuddyListScrollPane(), java.awt.BorderLayout.CENTER);
			jContentPane.add(getBottomPanel(), java.awt.BorderLayout.SOUTH);
		}
		return jContentPane;
	}

	public void addBuddy(String displayName, String uri, int index) {
		String buddyString; 
		if(displayName != null)
			buddyString = displayName + " <" + uri + ">'";
		else
			buddyString = uri;
		addBuddy(buddyString, index);
	}

	public void addBuddy(String buddyString, int index) {
		DefaultListModel list = (DefaultListModel) buddyList.getModel();
		Buddy buddy = new Buddy(buddyString);
		if(index == -1)
			list.addElement(buddy);
		else 
			list.add(index, buddy);
		index = list.indexOf(buddy);
		properties.setProperty("buddy." + index, buddyString);
		
		engine.subscribe(buddy);
	}

	public void unsubscribe() {
		DefaultListModel list = (DefaultListModel) buddyList.getModel();
		for (int i = 0; i < list.getSize(); i++) {
			Buddy buddy = (Buddy) list.get(i);
			engine.unsubscribe(buddy);
		}
	}

	public void processEvent(BuddizEvent event) {
		EventTask task = new EventTask(event);
		taskQueue.execute(task);
	}

	class EventTask implements Runnable {

		public BuddizEvent event;
		
		public EventTask(BuddizEvent event) {
			this.event = event;
		}

		public void run() {
			if(event instanceof RegisteredEvent) {
				RegisteredEvent registered = (RegisteredEvent) event;
				statusBox.setSelectedItem(Mood.valueOf(registered.getMood()));
				return;
			}

			if(event instanceof ErrorEvent) {
				ErrorEvent errorEvent = (ErrorEvent) event;
				String message = "Error: ";
				if(errorEvent.getCode() != -1)
					message += "Status code = " + errorEvent.getCode() + ", ";
				message += errorEvent.getMessage();
				JOptionPane.showMessageDialog(null, message);
				return;
			}
			
			if(event instanceof MessageEvent) {
				MessageEvent message = (MessageEvent) event;
				Buddy buddy = new Buddy(message.getSender());
				ChatDialog dialog = chat(buddy);
				dialog.processEvent(message);
			}

			if(event instanceof NotifyEvent) {
				NotifyEvent notify = (NotifyEvent) event;
				DefaultListModel list = (DefaultListModel) buddyList.getModel();
				for (int i = 0; i < list.size(); i++) {
					Buddy buddy = (Buddy) list.elementAt(i);
					if(buddy.getSipAddress().equals(notify.getSubscription())) {
                        String status = notify.getStatus();
                        Mood mood = Mood.Unknown;
                        if(status == null)
                            status="";
                        status = status.trim();
                        if(status.equals("")) {
                            mood = Mood.Offline;
                        }
                        try {
                            mood = Mood.valueOf(notify.getStatus());
                        }
                        catch(Throwable e) {
                            //Do nothing
                        }
						buddy.setMood(mood);
                        BuddizWindow.this.repaint();
						return;
					}
				}

			}
		}

	}

	/**
	 * This method initializes chatButton	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getChatButton() {
		if (chatButton == null) {
			chatButton = new JButton();
			chatButton.setText("Chat");
			chatButton.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					chat(currentBuddy); // TODO Auto-generated Event stub actionPerformed()
				}
			});
		}
		return chatButton;
	}

	protected ChatDialog chat(Buddy buddy) {
		ChatDialog chatDialog = new ChatDialog(this, buddy);
		chatDialog.setVisible(true);
		return chatDialog;
	}	
}  //  @jve:decl-index=0:visual-constraint="10,10"
