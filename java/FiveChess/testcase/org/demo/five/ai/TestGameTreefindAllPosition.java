package org.demo.five.ai;

import static org.junit.Assert.*;

import java.util.Iterator;

import org.demo.five.model.DataModel;
import org.junit.Test;

public class TestGameTreefindAllPosition {
	private GameTree game = null;
	private DataModel data = null;
	
	private String initState = "0**0" +
							   "*00x" +
							   "x0x0" +
							   "0*x*";
	
	@Test
	public void test_findAllPosition_9() {
		this.initStatus(4, initState);
		int[][] position = {{0,0},{3,0}, {1,1}, {2,1}, {1,2}, {3,2}, {0, 3}};
		Iterator<GameTree.Position> iter = game.findAllPosition(true);
		
		int count = 0;
		for(; iter.hasNext();) {
			GameTree.Position p = iter.next();
			assertEquals(count + ": " + p.x + "!=" + position[count][0], p.x, position[count][0]);
			assertEquals(count + ": " + p.y + "!=" + position[count][1], p.y, position[count][1]);
			count++;
		}
		assertEquals(count, 7);

	}
	
	@Test
	public void test_findAllPosition_0() {
		this.initStatus(4, initState);
		int[][] position = {{0,0},{3,0}, {1,1}, {2,1}, {1,2}, {3,2}, {0, 3}};
		Iterator<GameTree.Position> iter = game.findAllPosition(true);
		
		int count = 0;
		iter.hasNext();
		iter.hasNext();
		iter.hasNext();
		for(; iter.next() != null;) {
			count++;
		}
		assertEquals(count, 7);

	}	
	
	private void initStatus(int size, String st){
		data = new DataModel(size);
		game = new GameTree(data, 1, 2);
		
		int length = data.data.length;
		for(int i = 0; i < length * length; i++){
			if(st.charAt(i) == '*'){
				data.set(i % length, i / length, 1);
			}else if(st.charAt(i) == 'x'){
				data.set(i % length, i / length, 2);
			}
		}
	}
}
