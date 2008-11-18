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

package org.demo.five.model;

import java.util.*;

public class DataModel {
	public byte[][] data = null;
	private Collection<ModelListener> listener = null;
	public DataModel(){
		this(18);
	}

	public DataModel(int length){
		data = new byte[length][length];
		listener = new Vector<ModelListener>();
	}
	
	public void addListener(ModelListener l){
		this.listener.add(l);		
	}
	
	protected void fireEvent(ModelEvent e){
		for (ModelListener l : this.listener){
			l.update(e);
		}
	}
	
	public void set(int x, int y, int data){
		this.set(x, y, (byte)data);
	}	
	
	public void set(int x, int y, byte data){
		this.data[x][y] = data;
		fireEvent(new ModelEvent(ModelEvent.ADD, x, y, data));
	}
	
	public void remove(int x, int y){
		byte oldData = this.data[x][y];
		this.data[x][y] = 0;
		fireEvent(new ModelEvent(ModelEvent.REMOVE, x, y, oldData));
	}
	
	public boolean isEmpty(int x, int y){
		return this.data[x][y] == 0;
	}
	
}
