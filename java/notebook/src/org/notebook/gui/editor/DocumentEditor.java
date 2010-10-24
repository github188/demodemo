/*  
  * Copyright [2008] DeonWu@gmail.com 
  *  
  * Licensed under the Apache License, Version 2.0 (the "License");  
  * you may not use this file except in compliance with the License.  
  * You may obtain a copy of the License at  
  *      http://www.apache.org/licenses/LICENSE-2.0  
  * Unless required by applicable law or agreed to in writing, software  
  * distributed under the License is distributed on an "AS IS" BASIS,  
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
  * See the License for the specific language governing permissions and  
  * limitations under the License. 
  *  
  * $ Name LastChangeRevision LastChangeDate LastChangeBy $ 
  * $Id$ 
  */ 
 
package org.notebook.gui.editor;

import static org.notebook.gui.MenuToolbar.icon;

import java.awt.Font;
import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.JComponent;
import javax.swing.JTextArea;
import javax.swing.JToggleButton;

import org.notebook.cache.NoteMessage;

public class DocumentEditor extends JTextArea {
	public NoteMessage msg = null;

	public DocumentEditor(){
		super("");

		//this.setFont(new Font("Dialog", Font.ITALIC, 24));
		//this.setFont(new Font("Courier New", Font.PLAIN, 16));
		this.setFont(new Font("Courier", Font.PLAIN, 14));
		this.wrapLine.putValue(Action.SELECTED_KEY, false);
		this.wrapLine.actionPerformed(null);
		//this.setLineWrap(false);
		this.setWrapStyleWord(true);
		this.setEditable(false);
	}
	
	public void openDocument(NoteMessage msg){
		this.msg = msg;
		this.setText(msg.text);
		this.msg.isDirty = false;
		if(msg.getCategory().isExpired){
			this.setEditable(false);
		}else {
			this.setEditable(true);
		}
	}
	
	public void reloadDocument(NoteMessage newMsg){
		if(this.msg != null && newMsg != null){
			if(this.msg.messageId.equals(newMsg.messageId)){
				this.setText(newMsg.getText());
			}
		}
	}
	
	public NoteMessage currentDocuemnt(){
		if(msg != null){
			if(!msg.getText().equals(this.getText())){
				msg.setText(this.getText());
				msg.isDirty = true;
			}
		}
		return msg;
		//this.setw
	}
	
	public JComponent[] getMenuBar(){
		return new JComponent[]{
			new JToggleButton(this.wrapLine)
		};
	}
	
	public JComponent[] getToolBar(){
		return new JComponent[]{
			new JToggleButton(this.wrapLine)
		};		
	}
	
	private AbstractAction wrapLine = new AbstractAction("", 
			icon("org/notebook/gui/images/wrap.png")){
		private static final long serialVersionUID = 1L;
		//public AbstractAction(){}
		@Override
		public void actionPerformed(ActionEvent e) {
			Boolean x = (Boolean)this.getValue(SELECTED_KEY);
			setLineWrap(x.booleanValue());
		}
	};	
}

