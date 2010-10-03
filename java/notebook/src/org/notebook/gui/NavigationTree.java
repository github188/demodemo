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

import java.awt.Point;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DragGestureEvent;
import java.awt.dnd.DragGestureListener;
import java.awt.dnd.DragGestureRecognizer;
import java.awt.dnd.DragSource;
import java.awt.dnd.DragSourceContext;
import java.awt.dnd.DragSourceDragEvent;
import java.awt.dnd.DragSourceDropEvent;
import java.awt.dnd.DragSourceEvent;
import java.awt.dnd.DragSourceListener;
import java.awt.dnd.DropTarget;
import java.awt.dnd.DropTargetContext;
import java.awt.dnd.DropTargetDragEvent;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.dnd.DropTargetListener;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.IOException;

import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JTree;
import javax.swing.event.TreeExpansionEvent;
import javax.swing.event.TreeWillExpandListener;
import javax.swing.tree.DefaultTreeCellEditor;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.ExpandVetoException;
import javax.swing.tree.TreePath;

import org.notebook.cache.Category;

/*
JOptionPane.showMessageDialog(frame,
	    "Eggs are not supposed to be green.",
	    "Inane warning",
	    JOptionPane.WARNING_MESSAGE);
*/

public class NavigationTree extends JTree implements MouseListener {
	private static final long serialVersionUID = -9163354911246378721L;
	private MenuToolbar menu = null;
	private DefaultTreeCellEditor editor = null;

	public NavigationTree(Category root, MenuToolbar menu){
		setModel(root);
		//menu = this.createContextMenu();
		this.menu = menu;
		this.addMouseListener(this);
		this.setEditable(true);
		editor = new DefaultTreeCellEditor(this, (DefaultTreeCellRenderer) this.getCellRenderer());
		this.setCellEditor(editor);
		this.addTreeWillExpandListener(treeWillExpandListener);
		
		TreeDragSource ds = new TreeDragSource(this, DnDConstants.ACTION_MOVE);
		TreeDropTarget dt = new TreeDropTarget(this);		
	}

	private JPopupMenu createContextMenu(){
	    final JPopupMenu menu = new JPopupMenu();
	    //menu.
	    JMenuItem newCategory = new JMenuItem("新建目录");	
	    newCategory.addActionListener(new ActionListener() {
	      public void actionPerformed(ActionEvent e) {
	    	  if(getSelectionModel() != null){
	    		  Category obj = (Category)getSelectionModel().getSelectionPath().getLastPathComponent();
	    		  Category xx = obj.addCategory("新建目录");
	    	  }
	      }
	    });	
	    menu.add(newCategory);
	    
	    JMenuItem newNote = new JMenuItem("新建文件");
	    newNote.addActionListener(new ActionListener() {
	      public void actionPerformed(ActionEvent e) {
	    	  if(getSelectionModel() != null){
	    		  Category obj = (Category)getSelectionModel().getSelectionPath().getLastPathComponent();
			  	obj.addMessage("记事本");
	    	  }
	      }
	    });	
	    menu.add(newNote);
	    
	    menu.addSeparator();

	    JMenuItem remove = new JMenuItem("Remove");	
	    remove.addActionListener(new ActionListener() {
	    	public void actionPerformed(ActionEvent e) {
		    	  if(getSelectionModel() != null){
		    		  Category obj = (Category)getSelectionModel().getSelectionPath().getLastPathComponent();
		    		  obj.remove();
		    	  }
	    	}
	    });	
	    menu.add(remove);	    
	    //menu.add(JMenuItem.s);

	    
	    return menu;
	}
	
	@Override
	public void mouseClicked(MouseEvent e) {
		if (e.getButton() == e.BUTTON3) {
			System.out.println("mouseClicked..." + e.getComponent());
			//右键选择
			int row = getClosestRowForLocation(e.getX(), e.getY());  
			setSelectionRow(row);
			menu.getNavigationContextMenu(this).show(e.getComponent(), e.getX(), e.getY());
		}
		//this.s
	}
	@Override
	public void mouseEntered(MouseEvent arg0) {			
	}
	@Override
	public void mouseExited(MouseEvent arg0) {			
	}
	@Override
	public void mousePressed(MouseEvent arg0) {
	}
	@Override
	public void mouseReleased(MouseEvent arg0) {
		
	}
	
	TreeWillExpandListener treeWillExpandListener = new TreeWillExpandListener() {
		public void treeWillCollapse(TreeExpansionEvent event)
		      throws ExpandVetoException {
				TreePath path = event.getPath();
				//System.out.println("Collapse:" + path.toString());
		}
	
		public void treeWillExpand(TreeExpansionEvent event)
		      throws ExpandVetoException {
		    TreePath path = event.getPath();
		    //System.out.println("Collapse:" + path.toString());
		}
	};	

	class TreeDragSource implements DragSourceListener, DragGestureListener {
		DragSource source;
		DragGestureRecognizer recognizer;
		TransferableTreeNode transferable;
		Category oldNode;
		JTree sourceTree;
	
		public TreeDragSource(JTree tree, int actions) {
		    sourceTree = tree;
		    source = new DragSource();
		    recognizer = source.createDefaultDragGestureRecognizer(sourceTree,
		        actions, this);
		    System.out.println("actions:" + actions);
		}
	
		  /*
		   * Drag Gesture Handler
		   */
		public void dragGestureRecognized(DragGestureEvent dge) {
			  oldNode = null;
		    TreePath path = sourceTree.getSelectionPath();
		    if ((path == null) || (path.getPathCount() <= 1)) {
		      // We can't move the root node or an empty selection
		      return;
		    }
		    oldNode = (Category) path.getLastPathComponent();
		    System.out.println("dragGestureRecognized:" + oldNode.toString());
		    transferable = new TransferableTreeNode(path);
		    source.startDrag(dge, DragSource.DefaultMoveDrop, transferable, this);
	
		    // If you support dropping the node anywhere, you should probably
		    // start with a valid move cursor:
		    //source.startDrag(dge, DragSource.DefaultMoveDrop, transferable,
		    // this);
		}
	
		  /*
		   * Drag Event Handlers
		   */
		public void dragEnter(DragSourceDragEvent dsde) {
			System.out.println("TreeDragSource: dragEnter:" + dsde.getDropAction());
		    DragSourceContext context = dsde.getDragSourceContext();
		    int dropAction = dsde.getDropAction();
		    /*
		    if ((dropAction & DnDConstants.ACTION_COPY) != 0) {
		    	context.setCursor(DragSource.DefaultCopyDrop);
		    } else if ((dropAction & DnDConstants.ACTION_MOVE) != 0) {
		    	context.setCursor(DragSource.DefaultMoveDrop);
		    } else {
		    	context.setCursor(DragSource.DefaultCopyNoDrop);
		    }*/			
		}
	
		public void dragExit(DragSourceEvent dse) {
			System.out.println("TreeDragSource: dragExit-->" + dse);
		}
	
		public void dragOver(DragSourceDragEvent dsde) {
			  //System.out.println("TreeDragSource: dragOver" + dsde);
		}
	
		public void dropActionChanged(DragSourceDragEvent dsde) {
		    System.out.println("Action: " + dsde.getDropAction());
		    System.out.println("Target Action: " + dsde.getTargetActions());
		    System.out.println("User Action: " + dsde.getUserAction());
		}
	
		public void dragDropEnd(DragSourceDropEvent dsde){
		    /*
		     * to support move or copy, we have to check which occurred:
		     */
			System.out.println("Drop Action: " + dsde.getDropAction() + 
					"\tDropSuccess:" + dsde.getDropSuccess());
			//dsde.
			if(dsde.getDropSuccess()){
				oldNode.remove();
			}
		}
	}
	
	class TreeDropTarget implements DropTargetListener {
		  DropTarget target;
		  JTree targetTree;

		  public TreeDropTarget(JTree tree) {
		    targetTree = tree;
		    target = new DropTarget(targetTree, this);
		  }

		  private TreePath getEventTreePath(DropTargetDragEvent dtde){
			Point p = dtde.getLocation();
			DropTargetContext dtc = dtde.getDropTargetContext();
			JTree tree = (JTree) dtc.getComponent();
			TreePath path = tree.getClosestPathForLocation(p.x, p.y);
			return path;
		  }
		  
		  private TreePath getDragSource(DropTargetDragEvent dtde){
				Transferable tr = dtde.getTransferable();
				DataFlavor[] flavors = tr.getTransferDataFlavors();
				TreePath p = null;
				try {
					p = (TreePath) tr.getTransferData(flavors[0]);
				} catch (Exception e){
					e.printStackTrace();
				}
				return p;
		  }
		  
		  /**
		   * 判断P2是否是p1的子节点.
		   * @param p1
		   * @param p2
		   * @return
		   */
		  private boolean isSubTree(TreePath p1, TreePath p2){
			  Object[] path1 = p1.getPath();
			  Object[] path2 = p2.getPath();
			  if(path1.length <= path2.length){
				  for(int i = 0; i < path1.length; i++){
					  if(!path1[i].equals(path2[i])){
						  return false;
					  }
				  }
				  return true;
			  }
			  return false;
		  }

		  public void dragEnter(DropTargetDragEvent dtde) {
			  System.out.println("TreeDropTarget: dragEnter-->" + dtde);
			  TreePath target = getEventTreePath(dtde);
			  Category node = (Category)target.getLastPathComponent();
			  if(node.isLeaf(node)){
				  dtde.rejectDrag();
			  }else{
		    	if(!isSubTree(getDragSource(dtde), target)){
		    		dtde.acceptDrag(dtde.getDropAction());
		    	}else {
		    		dtde.rejectDrag();
		    	}
		    }
		  }
		  
		  public void dragOver(DropTargetDragEvent dtde) {
			  // System.out.println("TreeDropTarget: dragOver-->" + dtde);
			  TreePath target = getEventTreePath(dtde);
			  Category node = (Category)target.getLastPathComponent();
			  if(node.isLeaf(node)){
				  dtde.rejectDrag();
			  }else{
		    	if(!isSubTree(getDragSource(dtde), target)){
		    		dtde.acceptDrag(dtde.getDropAction());
		    	}else {
		    		dtde.rejectDrag();
		    	}
			  }
			Point p = dtde.getLocation();
			DropTargetContext dtc = dtde.getDropTargetContext();
			JTree tree = (JTree) dtc.getComponent();
			int row = tree.getClosestRowForLocation(p.x, p.y);
			setSelectionRow(row);
			
		    /*
		    if ((dropAction & DnDConstants.ACTION_COPY) != 0) {
		    	context.setCursor(DragSource.DefaultCopyDrop);
		    } else if ((dropAction & DnDConstants.ACTION_MOVE) != 0) {
		    	context.setCursor(DragSource.DefaultMoveDrop);
		    } else {
		    	context.setCursor(DragSource.DefaultCopyNoDrop);
		    }*/					
			  
		  }

		  public void dragExit(DropTargetEvent dte) {
			  System.out.println("TreeDropTarget: dragExit-->" + dte);
		  }

		  public void dropActionChanged(DropTargetDragEvent dtde) {
			  System.out.println("TreeDropTarget: dropActionChanged-->" + dtde);
		  }

		  public void drop(DropTargetDropEvent dtde) {
			System.out.println("TreeDropTarget,drop-->" + dtde);
		    Point pt = dtde.getLocation();
		    DropTargetContext dtc = dtde.getDropTargetContext();
		    JTree tree = (JTree) dtc.getComponent();
		    TreePath parentpath = tree.getClosestPathForLocation(pt.x, pt.y);
		    Category parent = (Category) parentpath.getLastPathComponent();
		    if (parent.isLeaf(parent)) {
		      dtde.rejectDrop();
		      return;
		    }

		    try {
		      Transferable tr = dtde.getTransferable();
		      DataFlavor[] flavors = tr.getTransferDataFlavors();
		      for (int i = 0; i < flavors.length; i++) {
		    	  System.out.println("drop:" + flavors[i]);
					if (tr.isDataFlavorSupported(flavors[i])) {
					  dtde.acceptDrop(dtde.getDropAction());
					  TreePath p = (TreePath) tr.getTransferData(flavors[i]);
					  Category node = (Category) p.getLastPathComponent();
					  parent.addCategory(node);
					  dtde.dropComplete(true);
					  return;
					}
		      }
		      dtde.rejectDrop();
		    } catch (Exception e) {
		      e.printStackTrace();
		      dtde.rejectDrop();
		    }
		  }
		}

	public static DataFlavor TREE_PATH_FLAVOR = new DataFlavor(TreePath.class,
	      "Tree Path");
	class TransferableTreeNode implements Transferable {
	  DataFlavor flavors[] = { TREE_PATH_FLAVOR };

	  TreePath path;

	  public TransferableTreeNode(TreePath tp) {
	    path = tp;
	  }

	  public synchronized DataFlavor[] getTransferDataFlavors() {
	    return flavors;
	  }

	  public boolean isDataFlavorSupported(DataFlavor flavor) {
	    return (flavor.getRepresentationClass() == TreePath.class);
	  }

	  public synchronized Object getTransferData(DataFlavor flavor)
	      throws UnsupportedFlavorException, IOException {
	    if (isDataFlavorSupported(flavor)) {
	      return (Object) path;
	    } else {
	      throw new UnsupportedFlavorException(flavor);
	    }
	  }
	}
}

