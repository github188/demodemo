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
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JFrame;

import org.demo.five.FiveChess;
import org.demo.five.Player;
import org.demo.five.player.LocalPlayer;
import org.demo.five.player.RobotPlayer;

public class MainPlane extends JFrame {
	private ChessBoard board = null;
	private FiveChess chess = null;
	private Player localPlayer = null;
	
	public MainPlane(){
		  super("Java五子棋");
		  setLayout(new BorderLayout());
		  board = new ChessBoard();
		  chess = new FiveChess();
		  localPlayer = new LocalPlayer("anonymity", ChessMan.WHITE);
		  board.setPlayer(localPlayer);
		  
		  chess.addPlayer(localPlayer);
		  chess.addPlayer(new RobotPlayer("robot", ChessMan.BLACK));
		  chess.addModelListener(board);
		  
		  this.getContentPane().add("Center", board);
		  this.init();
		  pack();
		  setSize(670,548);
		  //setVisible(true);
		  setResizable(false); 
	}
	
	public void start(){
		
	}
	
	public void init() {
		
		this.chess.start(this.localPlayer);
		
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
}
