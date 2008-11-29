/* 
 * Copyright [2008] DeonWu@gmail.com
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at 
 *	http://www.apache.org/licenses/LICENSE-2.0 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 * 
 * $ Name LastChangeRevision LastChangeDate LastChangeBy $
 * $Id$
 */

package org.demo.five.gui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.DefaultComboBoxModel;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import org.demo.five.ChessListener;
import org.demo.five.FiveChess;
import org.demo.five.Player;
import org.demo.five.player.LocalPlayer;
import org.demo.five.player.RobotPlayer;

public class MainPlane extends JFrame implements ChessListener {
	private ChessBoard board = null;
	private FiveChess chess = null;
	private Player localPlayer = null;
	private RobotPlayer robot = null; 
	private DefaultComboBoxModel levelModel = null;
	private DefaultListModel stepData = null;
	
	private JCheckBox robotFirst = null; 
	
	private JButton startButton = null;
	
	
	public MainPlane(){
		  super("Java五子棋");
		  setLayout(new BorderLayout());
		  board = new ChessBoard(15);
		  chess = new FiveChess(15);
		  localPlayer = new LocalPlayer("玩家-Human", ChessMan.WHITE);
		  board.setPlayer(localPlayer);
		  
		  chess.addPlayer(localPlayer);
		  this.robot = new RobotPlayer("电脑-Robot", ChessMan.BLACK);
		  chess.addPlayer(this.robot);
		  chess.addModelListener(board);
		  
		  this.getContentPane().add("Center", board);
		  
		  this.initControlPanel();
		  this.init();
		  pack();
		  setSize(670,548);
		  //setVisible(true);
		  setResizable(false); 
	}
	
	public void startChess(){
		this.startButton.setEnabled(false);
		this.robotFirst.setEnabled(false);
		this.board.reset();
		
		int level = 1;
		Object o = levelModel.getSelectedItem();
		if (o != null){
			level += levelModel.getIndexOf(o);
		}
		
		this.robot.setLevel(level);
		this.chess.start(robotFirst.isSelected() ? this.robot : this.localPlayer);
		
	}
	
	public void init() {
		this.chess.addChessListener(this);
		
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				System.exit(0);
			}
		});
	}
	
	public static void main(String[] args){
		MainPlane main = new MainPlane();
		main.setVisible(true);
	}
	
	protected void initControlPanel(){
		JPanel cPanel = new JPanel();
		
		cPanel.setPreferredSize(new Dimension(150, 80));
		this.getContentPane().add("East", cPanel);
		
		JPanel cAction = new JPanel();
		
		levelModel = new DefaultComboBoxModel();
		levelModel.addElement("初级-1");
		levelModel.addElement("中级-2");
		levelModel.addElement("高级-3");
		levelModel.addElement("专家级-4");
		
		JComboBox level = new JComboBox(levelModel);
		
		startButton = new JButton("玩家开始");
		startButton.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent arg0) {
				startChess();				
			}});
		
		cAction.setLayout(new BoxLayout(cAction, BoxLayout.X_AXIS));
		
		cAction.add(new JLabel("等级:"));
		cPanel.add(Box.createHorizontalStrut(5));
		cAction.add(level);
		cPanel.add(Box.createVerticalGlue());
		//cAction.add(start);
		cAction.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));
		
		stepData = new DefaultListModel();
		JList steps = new JList(stepData);	
		//stepData = steps.getModel();
		
		JScrollPane listScrollPane = new JScrollPane(steps);
		
		steps.setPreferredSize(new Dimension(80, 380));
		
		robotFirst = new JCheckBox("电脑先手", false);
		
		cPanel.add(cAction);
		cPanel.add(Box.createVerticalGlue());
		
		cPanel.add(robotFirst);
		
		cPanel.add(startButton);
		
		//cPanel.add(listScrollPane);

	}

	@Override
	public void end(Player p, int result) {
		if(result == FiveChess.DRAW){
			JOptionPane.showMessageDialog(this,
					"和棋！", 
				    "Message", JOptionPane.WARNING_MESSAGE);				
		}else if(result == FiveChess.WIN){
			//String winer = (p.getChessMan() == ChessMan.WHITE) ? ""
			JOptionPane.showMessageDialog(this,
					p.name() + " 赢！",
				    "Message", JOptionPane.WARNING_MESSAGE);			
		}
		
		this.robotFirst.setEnabled(true);
		this.startButton.setEnabled(true);
	}

	@Override
	public void handover(Player p) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void play(Player p, int x, int y) {
		String s = p.getChessMan() == ChessMan.WHITE ? "W" : "B";
		stepData.add(0, "test");
		stepData.addElement(String.format("%s:%s,%s", s, x, y));
		//stepData.
		System.out.println(String.format("%s:%s,%s", s, x, y));
	}

	@Override
	public void redo(int step) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void start(Player p) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void undo(int step) {
		stepData.removeElement(stepData.lastElement());
	}
}
