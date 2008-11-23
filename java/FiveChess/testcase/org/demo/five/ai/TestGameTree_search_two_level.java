package org.demo.five.ai;

import static org.junit.Assert.assertEquals;

import org.demo.five.ai.GameTree.Position;
import org.demo.five.model.DataModel;
import org.junit.Test;

public class TestGameTree_search_two_level extends BaseTestGameTree {
	private GameTree.Position p = null;
	
	@Test
	public void test_search_one_point() {
		this.initChessStatus(15, new int[][]{{5, 5, RC},{6, 5, RC},
						{4, 8, HC},{5, 8, HC},{6, 8, HC}});
		
		
		p = S(0, true);
		assertEquals(p.weight, 2060);
		assertEquals(p.x, 4);
		assertEquals(p.y, 5);
		
		p = S(0, false);
		assertEquals(p.weight, -20060);
		assertEquals(p.x, 3);
		assertEquals(p.y, 8);
		
		
		p = S(1, true);
		assertEquals(p.weight, -9812);
		assertEquals(3, p.x);
		assertEquals(8, p.y);	

		
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
			//System.out.println("X=" + p.x + ",Y=" + p.y + " V:" + r);
			return r;
		}
	}
	
}
