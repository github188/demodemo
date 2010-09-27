package org.notebook.gui;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import org.notebook.cache.Category;
import org.notebook.cache.NoteMessage;
import org.notebook.cache.SimpleObjectCache;

public class BookController implements MouseListener {
	private NavigationTree tree = null;
	private DocumentEditor editor = null;
	private SimpleObjectCache cache = null;
	
	private NoteMessage curMessage = null;
	
	public BookController(NavigationTree tree, DocumentEditor editor,
			SimpleObjectCache cache){
		this.tree = tree;
		this.editor = editor;
		this.cache = cache;
		
		this.tree.addMouseListener(this);
	}

	@Override
	public void mouseClicked(MouseEvent e) {
		if(e.getSource().equals(tree) && e.getClickCount() > 1){
			if(tree.getSelectionPath() != null){
				Category node = (Category)tree.getSelectionPath().getLastPathComponent();
				if(node.isLeaf()){
					editor.openDocument(node);
					curMessage = node.getMessage();
				}
			}
		}
	}

	@Override
	public void mousePressed(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseEntered(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseExited(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}
	
	public void save(){
		saveCurMessage();
	}
	
	protected void saveCurMessage(){
		if(curMessage != null){
			curMessage.text = this.editor.getText();
			this.cache.save(curMessage);
		}
	}

}
