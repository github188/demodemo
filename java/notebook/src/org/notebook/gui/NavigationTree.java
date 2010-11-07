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

import java.awt.Component;
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
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

import javax.swing.JTree;
import javax.swing.event.TreeExpansionEvent;
import javax.swing.event.TreeWillExpandListener;
import javax.swing.tree.DefaultTreeCellEditor;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.ExpandVetoException;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;

import org.notebook.cache.Category;
import org.notebook.cache.Comparators;

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
		super(new BookTreeModel(root));
		this.menu = menu;
		this.addMouseListener(this);
		this.setEditable(true);
		editor = new DefaultTreeCellEditor(this, (DefaultTreeCellRenderer) this.getCellRenderer());
		this.setCellEditor(editor);
		this.addTreeWillExpandListener(treeWillExpandListener);
		this.setCellRenderer(new BookTreeRenderer());
		//this.setCellRenderer(new DefaultTreeCellRenderer());
		
		TreeDragSource ds = new TreeDragSource(this, DnDConstants.ACTION_MOVE);
		TreeDropTarget dt = new TreeDropTarget(this);	
	}
	
	@Override
	public void mouseClicked(MouseEvent e) {
		if (e.getButton() == MouseEvent.BUTTON3) {
			//右键选择
			int row = getClosestRowForLocation(e.getX(), e.getY());  
			setSelectionRow(row);
			menu.getNavigationContextMenu(this).show(e.getComponent(), e.getX(), e.getY());
		}else if(e.getClickCount() > 1 && e.getButton() == MouseEvent.BUTTON1){
			if(getSelectionPath() != null){
				Category node = (Category)getSelectionPath().getLastPathComponent();
				if(node.isLeaf()){
					menu.$(MenuToolbar.OPEN).actionPerformed(null);
				}
			}
		}
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
	
	public void setRoot(Category root){
		DefaultTreeModel model = (DefaultTreeModel)getModel();
		model.setRoot(root);
		root.treeModel = model;
		model.reload();
	}
	
	TreeWillExpandListener treeWillExpandListener = new TreeWillExpandListener() {
		public void treeWillCollapse(TreeExpansionEvent event)
		      throws ExpandVetoException {
				TreePath path = event.getPath();
		}
	
		public void treeWillExpand(TreeExpansionEvent event)
		      throws ExpandVetoException {
		    TreePath path = event.getPath();
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
		}
	
		public void dragOver(DragSourceDragEvent dsde) {
			  //System.out.println("TreeDragSource: dragOver" + dsde);
		}
	
		public void dropActionChanged(DragSourceDragEvent dsde) {
		}
	
		public void dragDropEnd(DragSourceDropEvent dsde){
		    /*
		     * to support move or copy, we have to check which occurred:
		     */
			//dsde.
			if(dsde.getDropSuccess()){
				oldNode.remove(false);
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
			  TreePath target = getEventTreePath(dtde);
			  Category node = (Category)target.getLastPathComponent();
			  if(node.isLeaf()){
				  dtde.rejectDrag();
			  }else{
				Category source = (Category)getDragSource(dtde).getLastPathComponent();
				//System.out.println("")
		    	if(!isSubTree(getDragSource(dtde), target) && //不能拖放到子节点。		    	    
		    		node.getIndex(source) == -1 //不能拖放到当前父节点。
		    	   ){
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
			  if(node.isLeaf()){
				  dtde.rejectDrag();
			  }else{
				 Category source = (Category)getDragSource(dtde).getLastPathComponent();
		    	if(!isSubTree(getDragSource(dtde), target) &&
		    		node.getIndex(source) == -1
		    	   ){
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
		  }

		  public void dropActionChanged(DropTargetDragEvent dtde) {
		  }

		  public void drop(DropTargetDropEvent dtde) {
		    Point pt = dtde.getLocation();
		    DropTargetContext dtc = dtde.getDropTargetContext();
		    JTree tree = (JTree) dtc.getComponent();
		    TreePath parentpath = tree.getClosestPathForLocation(pt.x, pt.y);
		    Category parent = (Category) parentpath.getLastPathComponent();
		    if (parent.isLeaf()) {
		      dtde.rejectDrop();
		      return;
		    }

		    try {
		      Transferable tr = dtde.getTransferable();
		      DataFlavor[] flavors = tr.getTransferDataFlavors();
		      for (int i = 0; i < flavors.length; i++) {
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
	
	static class BookTreeModel extends DefaultTreeModel{
		public BookTreeModel(TreeNode root) {
			super(root);
		}

		public void valueForPathChanged(TreePath path, Object newValue) {
			Category c = (Category)path.getLastPathComponent();
			c.setName((String)newValue);
	        nodeChanged(c);
	    }
	}
	
	class BookTreeRenderer extends DefaultTreeCellRenderer{
		private static final long serialVersionUID = -2215063329307646081L;
		protected DateFormat format= new SimpleDateFormat("MM/dd hh:mm");
		
		public BookTreeRenderer(){
			super();
			setOpenIcon(MenuToolbar.icon("org/notebook/gui/images/folder_open.gif"));
			setClosedIcon(MenuToolbar.icon("org/notebook/gui/images/folder_close.gif"));
			setLeafIcon(MenuToolbar.icon("org/notebook/gui/images/file_obj.gif"));
		}
		
		public Component getTreeCellRendererComponent(
                JTree tree,
                Object value,
                boolean sel,
                boolean expanded,
                boolean leaf,
                int row,
                boolean hasFocus) {
			Component componet = super.getTreeCellRendererComponent(
			                	tree, value, sel, expanded, leaf, row,
			                	hasFocus);
			DefaultTreeCellRenderer renderer = (DefaultTreeCellRenderer)componet;
			if(value instanceof Category){
				Category c = (Category)value;
				if(c.isLeaf() && c.lastUpdated != null){
					StringBuilder label = new StringBuilder(c.getName());
					if(c.isDirty){
						label.append("*");
					}
					if(c.isExpired){
						label.append("$");
					}
					label.append(" ");
					label.append(format.format(c.lastUpdated));
					renderer.setText(label.toString());
				}else {
					StringBuilder label = new StringBuilder(c.getName());
					if(Comparators.shortName(c.getOrderBy()) != 'P'){
						label.append(':');
						label.append(Comparators.shortName(c.getOrderBy()));
					}
					if(c.isDirty){
						label.append("*");
					}
					renderer.setText(label.toString());					
				}
			}
			return componet;
		}
	}
	
}

