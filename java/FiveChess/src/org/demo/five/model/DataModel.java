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
	protected byte[][] data = new byte[18][18];
	private Collection<ModelListener> listener = new Vector<ModelListener>();
	public DataModel(){
		
	}
	
	public void addListener(ModelListener l){
		this.listener.add(l);		
	}
	
	protected void fireEvent(ModelEvent e){
		for (ModelListener l : this.listener){
			l.update(e);
		}
	}
}
