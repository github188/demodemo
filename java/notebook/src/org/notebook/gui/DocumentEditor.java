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
 
package org.notebook.gui;

import java.awt.Font;

import javax.swing.JTextArea;

import org.notebook.cache.Category;
import org.notebook.cache.NoteMessage;

public class DocumentEditor extends JTextArea {
	public NoteMessage msg = null;

	public DocumentEditor(){
		super("");

		//this.setFont(new Font("Dialog", Font.ITALIC, 24));
		//this.setFont(new Font("Courier New", Font.PLAIN, 16));
		this.setFont(new Font("Courier", Font.PLAIN, 14));
		this.setLineWrap(false);
		this.setWrapStyleWord(true);
		//this.==
	}
	
	public void openDocument(NoteMessage msg){
		this.msg = msg;
		this.setText(msg.text);
		this.msg.isDirty = false;
	}
	
	public NoteMessage currentDocuemnt(){
		if(msg != null){
			if(!msg.getText().equals(this.getText())){
				msg.setText(this.getText());
				msg.isDirty = true;
			}
		}
		return msg;
	}
}

