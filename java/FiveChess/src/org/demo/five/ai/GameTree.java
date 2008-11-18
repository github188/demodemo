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
	private static final Position DUMMY = new Position(0, 0, 0);
	private static final int MAX_WEIGHT = 10000;
	public static class Position {
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
	byte[][] dir = {{1, 0}, {1, -1}, {0, -1}, {-1, -1},
			{-1, 0}, {-1, 1}, {0, 1}, {1, 1}};
	/**
	 * 创建一个博弈树。
	 * @param value 博弈树的当前状态。
	 * @param robot 在博弈树中代表机器的值。
	 * @param human 在博弈树中代表人的值。
	 * @param width 搜索宽度，在博弈树的每层选择进行递归模拟的数量。越大搜索的时间越长。
	 */
	public GameTree(DataModel value, byte robot, byte human, int width){
		this.data = value.data;
		this.robot = robot;
		this.human = human;
		this.searchWidth = width;
	}
	
	public GameTree(DataModel value, int robot, int human){
		this(value, (byte)robot, (byte)human, 5);
	}	
	
	/**
	 * 在博弈树中搜索一个最优的下棋点。
	 * @param depth 模拟博弈的深度。
	 * @param robot 当前模拟的对象（人或机器)
	 * @return
	 */
	public Position search(int depth, final boolean robot){

		SortedSet<Position> list = new TreeSet<Position>(new Comparator<Position>(){
			@Override
			public int compare(Position arg0, Position arg1) {
				//if (arg0.weight == arg1.weight) return 1;
				//避免删除同值得节点。
				if (Math.abs(arg0.weight) > Math.abs(arg1.weight)){
					return robot ? 1 : -1;
				}else {
					return robot ? -1 : 1;
				}
			}});
		Position min, max, index;
		
		//遍历当前棋盘中的所有可行方案，并对每点进行评估值， 对评估的结果进行排序，选择最优的方案。
		Iterator<Position> iter = findAllPosition(robot);
		for(; iter.hasNext(); ){
			index = iter.next();
			index.weight = this.evaluation(index, robot);
			//this.evaluation(index, robot);
			list.add(index);
			
			//当前棋子是必胜子。不在进行更深的模拟，直接推出搜索。
			if ((robot && index.weight >= MAX_WEIGHT) || 
				 index.weight <= -1 * MAX_WEIGHT){
				depth = -1;
				break;
			}
		}
		
		max = list.isEmpty() ? DUMMY : list.first();
		if (depth > 0){
			//max = GameTree.MIN_DUMMY;
			Iterator<Position> optimize = list.iterator();
			for(int i = 0; i < searchWidth && optimize.hasNext(); i++){
				index = optimize.next();
				data[index.x][index.y] = robot ? this.robot : this.human;
				//模拟搜索对方的最优方案。
				min = search(depth - 1, !robot);
				index.weight += min.weight;
				if(robot && index.weight > max.weight || index.weight < max.weight){
					max = index;
				}
				data[index.x][index.y] = 0;
			}
		}
		
		return max;
	}
	
	protected Iterator<Position> findAllPosition(final boolean robot){
		return new Iterator<Position>(){
			int index = -1;
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
					//p.weight = evaluation(p, robot);
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
	 * 
	 * 
	 * 1x00000 
	 * 0xx0000 2
	 * 0x0x000 2
	 * 0x0000x 2
	 */
	public int evaluation(Position p, boolean robot){
		byte[] status = this.checkResult(p.x, p.y, robot ? this.robot : this.human);
		int result = 0;
		for (byte i : status){
			if(i >= 'A'){
				result += (int)Math.pow(10, i - 'A');
			}else if(i >= '0'){
				result += (int)Math.pow(10, i - '0') * 2;
			}
		}
		
		return (robot) ? result : -1 * result;
	}
	
	
	public byte[] checkResult(int x, int y, byte chess){
		//if(this.data.isEmpty(x, y)) return false;
		//byte[][] data = this.data.data;		
		byte[] result = new byte[4];
		byte[] r1, r2;
		int dirCount = dir.length / 2;
		for(int i = 0; i < dirCount; i++){
			r1 = this.checkStatusOnDirection(x, y, chess, i);
			r2 = this.checkStatusOnDirection(x, y, chess, i + dirCount);
			
			result[i] = (byte)(r1[0] + r2[0] + 1); //
			
			if(r1[2] + r2[2] < 4){ //当前的活动空间为死棋。
				result[i] = '0';
			}else{
				result[i] += (r1[1] == 0 || r2[1] == 0) ? 'A' : '0';
			}
		}
		
		return result;
	}
	
	protected byte[] checkStatusOnDirection(int x, int y, byte chess, int dirIndex){
		//计算可以搜索的范围。
		int dx = dir[dirIndex][0], dy = dir[dirIndex][1];
		
		int ix = x, iy = y;
		byte count = 0, space = 0;
		byte i;
		for(i = 1; i <= 4; i++){
			ix += dx;
			iy += dy;
			if(ix >= data.length || ix < 0 || 
			   iy >= data[0].length || iy < 0 ) break;
			if(data[ix][iy] == chess){
				count++;
				space = 0;
			}else if(data[ix][iy] == 0){
				space++;
			}else {
				break;
			}
		}
		//一共有多少自己的棋子；是否是活棋；在这个方向上搜索的长度，
		return new byte[]{count, space, i};
	}
	
}
