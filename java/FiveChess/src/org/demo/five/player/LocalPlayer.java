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

package org.demo.five.player;

import org.demo.five.FiveChess;
import org.demo.five.gui.ChessMan;

public class LocalPlayer extends AbstractPlayer{
	//private 
	
	public LocalPlayer(String name, ChessMan c) {
		super(name, c);
	}
	
	
	public void setResult(int r){
		super.setResult(r);
		if (r == FiveChess.WIN){
			System.out.println("I WIN!!!");
		}
	}	

}
