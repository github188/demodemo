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

package org.demo.five.ai;

import java.util.Comparator;
import java.util.Iterator;
import java.util.SortedSet;
import java.util.TreeSet;

import org.demo.five.model.DataModel;

public class GameTree {
	private static final Position MIN_DUMMY = new Position(0, 0, Integer.MIN_VALUE);
	private static final int MAX_WEIGHT = 10000;
	static class Position {
		public int x;
		public int y;
		public int weight;
		Position(int x, int y, int weight){
			this.x = x; this.y = y; this.weight = weight;
		}
	}
	
	private byte[][] data;
	private byte robot, human;
	private int searchWidth;
	/**
	 * 创建一个博弈树。
	 * @param value 博弈树的当前状态。
	 * @param robot 在博弈树中代表机器的值。
	 * @param human 在博弈树中代表人的值。
	 * @param width 搜索宽度，在博弈树的每层选择进行递归模拟的数量。越大搜索的时间越长。
	 */
	public GameTree(DataModel value, byte robot, byte human, int width){
		
	}
	
	/**
	 * 在博弈树中搜索一个最优的下棋点。
	 * @param depth 模拟博弈的深度。
	 * @param robot 当前模拟的对象（人或机器)
	 * @return
	 */
	public Position search(int depth, boolean robot){

		SortedSet<Position> list = new TreeSet<Position>(new Comparator<Position>(){
			@Override
			public int compare(Position arg0, Position arg1) {
				//if (arg0.weight == arg1.weight) return 1;
				//避免删除同值得节点。
				if (Math.abs(arg0.weight) > Math.abs(arg1.weight)){
					return 1;
				}else {
					return -1;
				}
			}});
		Position min, max, index;
		
		//遍历当前棋盘中的所有可行方案，并对每点进行评估值， 对评估的结果进行排序，选择最优的方案。
		Iterator<Position> iter = findAllPosition(robot);
		for(; iter.hasNext(); ){
			index = iter.next();
			//this.evaluation(index, robot);
			list.add(index);
			
			//当前棋子是必胜子。不在进行更深的模拟，直接推出搜索。
			if (index.weight >= MAX_WEIGHT){
				depth = -1;
				break;
			}
		}
		
		if (depth <= 0){
			max = list.first();
		}else {
			max = GameTree.MIN_DUMMY;
			Iterator<Position> optimize = list.iterator();
			for(int i = 0; i < searchWidth && optimize.hasNext(); i++){
				index = optimize.next();
				data[index.x][index.y] = robot ? this.robot : this.human;
				//模拟搜索对方的最优方案。
				min = search(depth - 1, !robot);
				if(index.weight + min.weight > max.weight){
					max = index;
				}
				data[index.x][index.y] = 0;
			}
		}
		if (!robot) max.weight *= (-1);
		
		return max;
	}
	
	protected Iterator<Position> findAllPosition(final boolean robot){
		return new Iterator<Position>(){
			int index = 0;
			int end = data.length * data.length;
			int next = 0;
			@Override
			public boolean hasNext() {
				if (next <= index){
					for(next++; next < this.end; next++){
						if(data[next % data.length][next / data.length] == 0){
							break;
						}
					}
				}
				return next < this.end;
			}

			@Override
			public Position next() {
				if ((next > index && next < end) || this.hasNext()){
					Position p = new Position(next % data.length, 
											  next / data.length, 
											  0);
					evaluation(p, robot);
					index = next;
					return p;
				}else {
					return null;
				}
				
			}

			public void remove() {
			}
		};
	}
	
	/**
	 * 评估但前节点的分值。
	 * @param p
	 * @param robot
	 */
	private void evaluation(Position p, boolean robot){
		
	}
	
}
