package org.demo.five.ai;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

/**
 * 简单的只有某一方的棋力测试。
 * @author dalwu
 *
 */
public class TestGameTree_evaluation extends BaseTestGameTree{

	@Test
	public void test_evaluation_one_point_in_bound() {
		this.initChessStatus(15, new int[][]{{0,7,RC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子
		assertEquals(RE(1, 7), 260);
		assertEquals(RE(2, 7), 259);
		assertEquals(RE(3, 7), 258);
		assertEquals(RE(4, 7), 157);
		assertEquals(RE(5, 7), 80);
		
		//assertEquals(R(0, 8), "BB2B");
		assertEquals(RE(0, 8), 260);
	}
	
	@Test
	public void test_evaluation_one_point_in_center() {
		this.initChessStatus(15, new int[][]{{7,7,RC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子		
		assertEquals(R(6, 7), "2111");
		assertEquals(RE(6, 7), 260);
		
		this.initChessStatus(15, new int[][]{{7,7,HC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子		
		assertEquals(H(6, 7), "2111");
		assertEquals(HE(6, 7), -130);
	}	

	@Test
	public void test_evaluation_two_point_in_center() {
		this.initChessStatus(15, new int[][]{{7,7,RC}, {6,7,RC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子		
		assertEquals(R(8, 7), "3111");
		assertEquals(RE(8, 7), 2060);
		
		assertEquals(R(7, 6), "1221");
		assertEquals(RE(7, 6), 440);

		this.initChessStatus(15, new int[][]{{7,7,RC}, {5,7,RC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子		
		assertEquals(R(8, 7), "3111");
		assertEquals(RE(8, 7), 2059);
		
		this.initChessStatus(15, new int[][]{{7,7,HC}, {6,7,HC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子		
		assertEquals(H(8, 7), "3111");
		assertEquals(HE(8, 7), -1030);
		
		assertEquals(H(7, 6), "1221");
		assertEquals(HE(7, 6), -220);

		this.initChessStatus(15, new int[][]{{7,7,HC}, {5,7,HC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子		
		assertEquals(H(8, 7), "3111");
		assertEquals(HE(8, 7), -1029);		
	}	
	
	@Test
	public void test_evaluation_three_point_in_center() {
		this.initChessStatus(15, new int[][]{{7,7,RC}, {6,7,RC}, {5,7,RC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子		
		assertEquals(R(8, 7), "4111");
		assertEquals(RE(8, 7), 20060);
		
		this.initChessStatus(15, new int[][]{{7,7,HC}, {6,7,HC}, {5,7,HC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子		
		assertEquals(H(8, 7), "4111");
		assertEquals(HE(8, 7), -10030);		

	}		

	@Test
	public void test_evaluation_four_point_in_center() {
		this.initChessStatus(15, new int[][]{{7,7,RC}, {6,7,RC}, {5,7,RC}, {4,7,RC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子		
		assertEquals(R(8, 7), "F111");
		assertEquals(RE(8, 7), 100060);
		
		this.initChessStatus(15, new int[][]{{7,7,HC}, {6,7,HC}, {5,7,HC}, {4,7,HC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子		
		assertEquals(H(8, 7), "F111");
		assertEquals(HE(8, 7), -200030);
	}	
	
	@Test
	public void test_evaluation_33_point_in_center() {
		this.initChessStatus(15, new int[][]{{7,7,RC}, {6,7,RC}, {8,8,RC}, {8,9,RC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子		
		assertEquals(R(8, 7), "3131");
		assertEquals(RE(8, 7), 4040);
		
	}		

	@Test
	public void test_evaluation_43_point_in_center() {
		this.initChessStatus(15, new int[][]{{7,7,RC}, {6,7,RC}, {5,7,RC}, {8,8,RC}, {8,9,RC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子		
		assertEquals(R(8, 7), "4131");
		assertEquals(RE(8, 7), 22040);
		
	}		
	
}
