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

import javax.swing.*;
import javax.swing.tree.*;
import java.awt.*;
import java.awt.event.*; 
import javax.swing.event.*;
import java.net.*;

public class NavigationTree extends JTree {

	public NavigationTree(){
		DefaultMutableTreeNode root = new DefaultMutableTreeNode("JTree");
		DefaultMutableTreeNode parent = new DefaultMutableTreeNode("colors");
		root.add(parent);
		parent.add(new DefaultMutableTreeNode("blue"));
		parent.add(new DefaultMutableTreeNode("violet"));
		parent.add(new DefaultMutableTreeNode("red"));
		parent.add(new DefaultMutableTreeNode("yellow"));

		DefaultTreeModel model = new DefaultTreeModel(root);

		setModel(model);
	}



}

