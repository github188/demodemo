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


/**
 * @author deon
 */
public class ModelEvent {
	public static final int ADD = 1;
	public static final int REMOVE = 2;
	
	private int action;
	private int x;
	private int y;
	private int data;
	public ModelEvent(int action, int x, int y, int data){
		this.action = action;
		this.x = x;
		this.y = y;
		this.data = data;
	}
	
	public int getAction() {
		return action;
	}

	public int getData() {
		return data;
	}

	public int getX() {
		return x;
	}

	public int getY() {
		return y;
	}

}
