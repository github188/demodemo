package org.demo.five.ai;

import static org.junit.Assert.assertEquals;

import org.demo.five.ai.GameTree.Position;
import org.demo.five.model.DataModel;
import org.junit.Test;

public class TestGameTree_search_simple extends BaseTestGameTree {
	private GameTree.Position p = null;
	
	@Test
	public void test_search_first_step() {
		this.initChessStatus(8, new int[][]{});
		
		p = S(0, true);
		assertEquals(80, p.weight);
		assertEquals(4, p.x);
		assertEquals(4, p.y);
		
		p = S(0, false);
		assertEquals(-80, p.weight);
		assertEquals(4, p.x);
		assertEquals(4, p.y);	
	}
	
	@Test
	public void test_search_sec_step() {
		this.initChessStatus(8, new int[][]{{4, 4, HC}});
		
		p = S(0, true);
		assertEquals(70, p.weight);
		assertEquals(3, p.x);
		assertEquals(3, p.y);
		
		this.initChessStatus(8, new int[][]{{4, 4, RC}});
		
		p = S(0, false);
		assertEquals(-70, p.weight);
		assertEquals(3, p.x);
		assertEquals(3, p.y);		
		
	}	
	
	//@Test
	public void test_search_one_point() {
		this.initChessStatus(10, new int[][]{{5, 5, RC}, {0, 0, HC}});
		
		p = S(0, true);
		assertEquals(260, p.weight);
		assertEquals(p.x, 4);
		assertEquals(p.y, 4);
		
		
	}
	
	//@Test
	public void test_search_two_point_one_side() {
		this.initChessStatus(10, new int[][]{{4, 5, RC}, {5, 5, RC}});
		
		p = S(0, true);
		assertEquals(p.weight, 2060);
		assertEquals(p.x, 3);
		assertEquals(p.y, 5);

		p = S(0, false);
		assertEquals(p.weight, -60);
		assertEquals(p.x, 4);
		assertEquals(p.y, 4);
		
	}	
	
	//@Test
	public void test_search_two_point_two_side() {
		this.initChessStatus(10, new int[][]{{4, 5, RC}, {5, 5, HC}});
		
		p = S(0, true);
		assertEquals(p.weight, 260);
		assertEquals(p.x, 3);
		assertEquals(p.y, 4);

		p = S(0, false);
		assertEquals(p.weight, -230);
		assertEquals(p.x, 6);
		assertEquals(p.y, 5);
	}		
	
	protected GameTree gameInstance(DataModel data){
		return new TestGameTree(data, RC, HC);
	}	
	
	class TestGameTree extends GameTree{

		public TestGameTree(DataModel value, byte robot, byte human, int width) {
			super(value, robot, human, width);
		}
		
		public TestGameTree(DataModel value, int robot, int human) {
			super(value, robot, human);
		}
		
		public int evaluation(Position p, boolean robot){
			int r = super.evaluation(p, robot);
			System.out.println("X=" + p.x + ",Y=" + p.y + " V:" + r);
			return r;
		}
	}
	
}
