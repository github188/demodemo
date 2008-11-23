package org.demo.five.ai;

import static org.junit.Assert.*;

import java.util.Iterator;

import org.demo.five.model.DataModel;
import org.junit.Test;

public class TestGameTreefindAllPosition extends BaseTestGameTree{
	
	private String initState = "0**0" +
							   "*00x" +
							   "x0x0" +
							   "0*x*";
	
	@Test
	public void test_findAllPosition_9() {
		this.initChessStatus(4, initState);
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
		this.initChessStatus(4, initState);
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
	
	@Test
	public void test_findAllPosition_2() {
		this.initChessStatus(4, new int[][]{{0, 0, RC}, {0, 1, HC}});
		Iterator<GameTree.Position> iter = game.findAllPosition(true);
		
		int count = 0;
		iter.hasNext();
		iter.hasNext();
		iter.hasNext();
		for(; iter.next() != null;) {
			count++;
		}
		assertEquals(count, 14);

	}		
	

}
