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

import java.awt.Color;
import java.awt.Font;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JTextPane;
import javax.swing.JToggleButton;
import javax.swing.KeyStroke;
import javax.swing.text.AbstractDocument;
import javax.swing.text.BoxView;
import javax.swing.text.ComponentView;
import javax.swing.text.Element;
import javax.swing.text.IconView;
import javax.swing.text.LabelView;
import javax.swing.text.ParagraphView;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyledEditorKit;
import javax.swing.text.View;
import javax.swing.text.ViewFactory;
import javax.swing.undo.CannotRedoException;
import javax.swing.undo.CannotUndoException;
import javax.swing.undo.UndoManager;

import org.notebook.cache.NoteMessage;

public class DocumentEditor extends JTextPane {
	public NoteMessage msg = null;
	public UndoManager undoManager = null;

	public DocumentEditor(){
		super(new NoteDocument());
		this.setFont(new Font("Courier", Font.PLAIN, 14));
		//this.wrapLine.putValue(Action.SELECTED_KEY, false);
		//this.wrapLine.actionPerformed(null);
		//this.setLineWrap(false);
		//this.setWrapStyleWord(true);
		this.setEditable(true);
		this.setBackground(Color.white);
		//this.setEditorKit(new WrapEditorKit());
		//this.setSize(1024, 800);
		undoManager = new UndoManager();
		this.getDocument().addUndoableEditListener(undoManager);
		registerKeyboardAction(undo, 
							   KeyStroke.getKeyStroke(
		        					KeyEvent.VK_Z, 
		        					InputEvent.CTRL_MASK), 
		        		 	   JComponent.WHEN_IN_FOCUSED_WINDOW);
		registerKeyboardAction(redo, 
		   						KeyStroke.getKeyStroke(
		   							KeyEvent.VK_Y, 
		   							InputEvent.CTRL_MASK), 
		   						JComponent.WHEN_IN_FOCUSED_WINDOW);	
	}
	
	public void setLineWrap(boolean xx){};
	
	public void setText(String text){
		super.setText(text);
		undoManager.discardAllEdits();
	};
	
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
			new JButton(undo),
			new JButton(redo),
			new JToggleButton(this.wrapLine)
		};
	}
	
	public JComponent[] getToolBar(){
		return new JComponent[]{
			new JButton(undo),
			new JButton(redo),
			new JToggleButton(this.wrapLine)
		};		
	}
	
	private AbstractAction wrapLine = new AbstractAction("", 
			icon("org/notebook/gui/images/wrap.png")){
		private static final long serialVersionUID = 1L;
		//public AbstractAction(){}
		@Override
		public void actionPerformed(ActionEvent e) {
		}
	};
	
	private AbstractAction undo = new AbstractAction("", 
			icon("org/notebook/gui/images/undo.png")){
		private static final long serialVersionUID = 2L;
		@Override
		public void actionPerformed(ActionEvent e) {
			try {
				if(undoManager.canUndo()){
					undoManager.undo();
				}
		    } catch (CannotUndoException e1) {
		    	Toolkit.getDefaultToolkit().beep();
		    }
		}
	};
	
	private AbstractAction redo = new AbstractAction("", 
			icon("org/notebook/gui/images/redo.png")){
		private static final long serialVersionUID = 3L;
		//public AbstractAction(){}
		@Override
		public void actionPerformed(ActionEvent e) {
			try {
				if(undoManager.canRedo()){
					undoManager.redo();
				}				
		    } catch (CannotRedoException  e1) {
		    	Toolkit.getDefaultToolkit().beep();
		    }
		}
	};	
	
	
	public boolean getScrollableTracksViewportWidth()
	{	  
		return true;
	}	
	
	class WrapEditorKit extends StyledEditorKit {
	    ViewFactory defaultFactory=new WrapColumnFactory();
	    public ViewFactory getViewFactory() {
	        return defaultFactory;
	    }
	}
	
	class NoWrapBoxView extends BoxView {
	    public NoWrapBoxView(Element elem, int axis) {
	        super(elem, axis);
	    }
	    public void layout(int width, int height) {
	        super.layout(Short.MAX_VALUE, height);
	    }
	}
	
	class WrapColumnFactory implements ViewFactory {
	    public View create(Element elem) {
	        String kind = elem.getName();
	        System.out.println("Element name:" + kind);
			if (kind.equals(AbstractDocument.ContentElementName))
			{
				return new LabelView(elem);
			}
			else if (kind.equals(AbstractDocument.ParagraphElementName))
			{
				return new ParagraphView(elem);
			}
			else if (kind.equals(AbstractDocument.SectionElementName))
			{
				return new NoWrapBoxView(elem, View.Y_AXIS);
			}
			else if (kind.equals(StyleConstants.ComponentElementName))
			{
				return new ComponentView(elem);
			}
			else if (kind.equals(StyleConstants.IconElementName))
			{
				return new IconView(elem);
			}	        
	        return new LabelView(elem);
	    }
	 }	
}

