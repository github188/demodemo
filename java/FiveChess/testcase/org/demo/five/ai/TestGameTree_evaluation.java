package org.demo.five.ai;

import static org.junit.Assert.assertEquals;

import org.junit.Test;


public class TestGameTree_evaluation extends BaseTestGameTree{

	@Test
	public void test_evaluation_one_point() {
		this.initChessStatus(15, new int[][]{{0,7,RC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子
		assertEquals(RE(1, 7), 160);
		assertEquals(RE(2, 7), 160);
		assertEquals(RE(3, 7), 160);
		assertEquals(RE(4, 7), 160);
		assertEquals(RE(5, 7), 80);
		
		//assertEquals(R(0, 8), "BB2B");
		assertEquals(RE(0, 8), 230);
	}	
	
	
}
