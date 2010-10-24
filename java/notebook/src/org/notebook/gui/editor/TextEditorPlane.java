package org.notebook.gui.editor;

import java.awt.Color;
import java.awt.Font;

import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.Element;

public class TextEditorPlane extends JScrollPane{
    private DocumentEditor editor = null;
    private JTextArea lines = null;

    public TextEditorPlane(DocumentEditor editor){
    	this.editor = editor;
        lines = new JTextArea("1");
        this.initRowNumber();
    }    
	
    public TextEditorPlane(JTextArea editor){
    	//this.editor = editor;
        lines = new JTextArea("1");
        this.initRowNumber();
    }
    
    protected void initRowNumber(){
        lines.setBackground(Color.LIGHT_GRAY);
        lines.setEditable(false);
        lines.setFont(new Font("Courier", Font.PLAIN, 14));
        
        editor.getDocument().addDocumentListener(new DocumentListener(){
            public String getText(){
                int caretPosition = editor.getDocument().getLength();
                Element root = editor.getDocument().getDefaultRootElement();
                String text = "1" + System.getProperty("line.separator");
                for(int i = 2; i < root.getElementIndex( caretPosition ) + 2; i++){
                    text += i + System.getProperty("line.separator");
                }
                return text;
            }
            @Override
            public void changedUpdate(DocumentEvent de) {
                lines.setText(getText());
            }

            @Override
            public void insertUpdate(DocumentEvent de) {
                lines.setText(getText());
            }

            @Override
            public void removeUpdate(DocumentEvent de) {
                lines.setText(getText());
            }
            
        });
        getViewport().add(editor);
        setRowHeaderView(lines);
        setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);   
        //this.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);      
    }
}
