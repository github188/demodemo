package org.notebook.gui.editor;

import java.awt.Color;
import java.util.Vector;

import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultStyledDocument;
import javax.swing.text.Element;
import javax.swing.text.SimpleAttributeSet;
import javax.swing.text.StyleConstants;

public class NoteDocument extends DefaultStyledDocument {
	private SimpleAttributeSet bold = new SimpleAttributeSet(); 
	private SimpleAttributeSet normal = new SimpleAttributeSet();
	private int currentPos = 0; 
	//private int wordStart = 0;
	private String word = ""; 
	private Vector<String> keywords = new Vector<String>(); 
	
	public NoteDocument(){
		StyleConstants.setForeground(bold, Color.red);		
		keywords.add("public");
		keywords.add("import");
	}
	
	public void insertString(int offs, 
              String str, 
              AttributeSet a) throws BadLocationException{ 
		super.insertString(offs, str, normal);
		//currentPos = offs;
		currentPos = offs; 
	    char  strChar = str.charAt(0); 
	    switch (strChar){ 
	        case ('{'):case ('}'):case (' '): case('\n'): 
	        case ('('):case (')'):case (';'):case ('.'):{ 
	          checkForKeyword(); 
	          //wordStart = offs; 
	        } 
	   }
	}

	private void checkForKeyword() {
		int offs = this.currentPos;
		Element element = this.getParagraphElement(offs);
		String elementText = "";
		try {
			// this gets our chuck of current text for the element we’re on
			elementText = this.getText(element.getStartOffset(),
					element.getEndOffset() - element.getStartOffset());
		} catch (Exception ex) {
			// whoops!
			System.out.println("no text");
		}
		
		int strLen = elementText.length();
		if (strLen == 0) {
			return;
		}
		
		int i = 0;
		if (element.getStartOffset() > 0) {
			// translates backward if neccessary
			offs = offs - element.getStartOffset();
		}
		if ((offs >= 0) && (offs <= strLen - 1)) {
			i = offs;
			while (i > 0) {
				// the while loop walks back until we hit a delimiter
				i--;
				char charAt = elementText.charAt(i);
				if ((charAt == ' ') | (i == 0) | (charAt == '(') | (charAt == ')') 
					| (charAt == '{') | (charAt == '}')) { 
					if (i != 0) {
						i++;
					}
					word = elementText.substring(i, offs);// skip the period

					String s = word.trim().toLowerCase();
					// this is what actually checks for a matching keyword
					if (keywords.contains(s)) {
						try {
							// remove the old word and formatting
							this.remove(currentPos - word.length(),
									word.length());
							/*
							 * replace it with the same word, but new formatting
							 * we MUST call the super class insertString method
							 * here, otherwise wewould end up in an infinite
							 * loop !!!!!
							 */
							super.insertString(currentPos - word.length(),
									word, bold);
						} catch (Exception ex) {
							ex.printStackTrace();
						}
					}
					break;
				}
			}
		}
	}
}
