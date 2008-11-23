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
import java.util.List;
import java.util.SortedSet;
import java.util.TreeSet;
import java.util.Vector;

import org.demo.five.model.DataModel;

public class GameTree {
	private static final Position DUMMY = new Position(0, 0, 0);
	private static final int MAX_WEIGHT = 100000;
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
	
	private boolean random = false;
	/**
	 * 创建一个博弈树。
	 * @param value 博弈树的当前状态。
	 * @param robot 在博弈树中代表机器的值。
	 * @param human 在博弈树中代表人的值。
	 * @param width 搜索宽度，在博弈树的每层选择进行递归模拟的数量。越大搜索的时间越长。
	 */
	public GameTree(DataModel value, byte robot, byte human, int width, boolean random){
		this.data = value.data;
		this.robot = robot;
		this.human = human;
		this.searchWidth = width;
		this.random = random;
	}
	
	public GameTree(DataModel value, byte robot, byte human, int width){
		this(value, robot, human, width, false);
	}	
	
	public GameTree(DataModel value, int robot, int human){
		this(value, (byte)robot, (byte)human, 5000000);
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
					return -1;
				}else {
					return 1;
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
		if (!list.isEmpty())
			this.filter(list.iterator(), list.size());
		
		max = list.isEmpty() ? DUMMY : list.first();
		List<Position> equals = new Vector<Position>();
		Iterator<Position> optimize = list.iterator();
		
		if(false)
		System.out.println(String.format("Search for %s, length: %s", robot ? "R" : "H", list.size()));
		if (depth > 0 && list.size() > 1){
			//max = GameTree.MIN_DUMMY;
			for(int i = 0; i < searchWidth && optimize.hasNext(); i++){
				index = optimize.next();
				//System.out.println(String.format("%s, %s,%s,%s", robot ? "R" : "H",
				//		index.x, index.y, index.weight));
				data[index.x][index.y] = robot ? this.robot : this.human;
				//模拟搜索对方的最优方案。
				min = search(depth - 1, !robot);
				if(false && index.x == 7 && index.y == 8){
					System.out.println(String.format("Min, %s,%s,%s", 
							min.x, min.y, min.weight));
				}
				index.weight += min.weight;				
				if((robot && index.weight > max.weight) || (!robot && index.weight < max.weight)){
					max = index;
				}else if(this.random && max.weight == index.weight) {
					equals.add(index);
				}
				data[index.x][index.y] = 0;
			}
		}else if(this.random && list.size() > 1) {
			while(optimize.hasNext()) {
				index = optimize.next();
				if(index.weight == max.weight){
					equals.add(index);
				}else {
					break;
				}
			}
		}
		
		if(this.random && !equals.isEmpty()){ //在等值棋力中随机选择一个点。
			equals.add(max);
			int i = (int)(Math.random() * equals.size());
			max = equals.get(i);
		}
		
		return max;
	}
	
	protected void filter(Iterator<Position> iter, int length){
		//开局第一颗。	
		if (length == this.data.length * this.data.length){
			for (Position p = null; iter.hasNext();){
				p = iter.next();
				if(p.x != this.data.length /2 || p.y != this.data.length /2){
					iter.remove();
				}
			}
		}else if(length + 1 == this.data.length * this.data.length) { //开局第二颗。
			for (Position p = null; iter.hasNext();){
				p = iter.next();
				if(Math.abs(p.weight) != 70){
					iter.remove();
				}
			}	
		}
		
		//删除孤立一点的棋子。
		for (Position p = null; iter.hasNext();){
			p = iter.next();
			if(Math.abs(p.weight) == 80){
				iter.remove();
			}
		}
		
	}
	
	protected Iterator<Position> findAllPosition(final boolean robot){
		return new Iterator<Position>(){
			int index = -1;
			int end = data.length * data.length;
			int next = -1;
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
	 * @param robot 定义robot为先手，先后主要进攻，以活棋为最大值。后手为防守以冲型为最大值。
	 * 
	 * 
	 * 1x00000 
	 * 0xx0000 2
	 * 0x0x000 2
	 * 0x0000x 2
	 */
	public int evaluation(Position p, boolean robot){ 
		byte[][] status = this.checkResult(p.x, p.y, robot ? this.robot : this.human);
		int result = 0;
		int data = 0;
		for (int i = 0; i < status[0].length; i++){
			data = status[0][i];
			if(data >= 'A'){
				result += (int)Math.pow(10, data - 'A'); // * (robot ? 1 : 2);
			}else if(data >= '0'){
				//如果有多子相连的情况，中间的空格将大大的减弱力度。
				if (data >= '3') data -= status[1][i];
				result += (int)Math.pow(10, data - '0') * 2; //(robot ? 2 : 1);
			}
		}
		for (byte i : status[1]){
			result -= i;
		}
		
		return (robot) ? result : -1 * result;
	}
	
	
	public byte[][] checkResult(int x, int y, byte chess){
		//if(this.data.isEmpty(x, y)) return false;
		//byte[][] data = this.data.data;		
		byte[][] result = new byte[2][4];
		byte[] r1, r2;
		int dirCount = dir.length / 2;
		for(int i = 0; i < dirCount; i++){
			r1 = this.checkStatusOnDirection(x, y, chess, i);
			r2 = this.checkStatusOnDirection(x, y, chess, i + dirCount);
			
			result[0][i] = (byte)(r1[0] + r2[0] + 1); //
			
			if(r1[2] + r2[2] < 4){ //当前的活动空间为死棋。
				result[0][i] = '0';
			}else{
				result[0][i] += (r1[1] == 0 || r2[1] == 0) ? 'A' : '0';
				//棋子间的空格数=搜索宽度 - 棋子数 - 外延空格数
				result[1][i] += r1[2] - r1[0] - r1[1] + r2[2] - r2[0] - r2[1]; 
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
			   iy >= data[0].length || iy < 0 ){
				i++;
				space++; //边界上定义为活棋。这样可以简化后手防守算法。
				break;
			}
			if(data[ix][iy] == chess){
				count++;
				space = 0;
			}else if(data[ix][iy] == 0){
				space++;
			}else {
				break;
			}
		}
		
		if (i == 5){ //?
			i++;
			space++;
		}		
		//一共有多少自己的棋子；是否是活棋；在这个方向上搜索的长度，
		return new byte[]{count, space, --i};
	}
	
}
