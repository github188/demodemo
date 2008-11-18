package org.demo.five.ai;


import static org.junit.Assert.assertEquals;

import java.util.Iterator;

import org.demo.five.model.DataModel;
import org.junit.Before;
import org.junit.Test;

public class TestGameTree_checkResult extends BaseTestGameTree{
	private static final byte RC = 1, HC = 2;
	private GameTree game = null;
	private DataModel data = null;
	private int[][] testData1 = {{7,7,RC} };
	
	@Before
	public void setUp() throws Exception {
	}

	@Test
	public void test_one_self_point_in_min_bound() {
		this.initChessStatus(15, new int[][]{{0,7,RC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子
		assertEquals(R(1, 7), "C111");
		assertEquals(R(2, 7), "C111");
		assertEquals(R(3, 7), "C111");
		assertEquals(R(4, 7), "C111");
		assertEquals(R(5, 7), "1111");
		
		assertEquals(R(0, 8), "BB2B");
	}	
	
	@Test
	public void test_one_self_point_in_max_bound() {
		this.initChessStatus(15, new int[][]{{14,7,RC}});
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子
		assertEquals(R(13, 7), "C111");
		assertEquals(R(12, 7), "C111");
		assertEquals(R(11, 7), "C111");
		assertEquals(R(10, 7), "C111");
		assertEquals(R(9, 7), "1111");
		
		assertEquals(R(14, 8), "BB2B");
	}		
	
	@Test
	public void test_one_self_point() {
		this.initChessStatus(15, testData1);
		//byte[] st = game.checkResult(5, 2, (byte)1);
		//某一点的8个方向的棋子
		assertEquals(R(8, 7), "2111");
		assertEquals(R(8, 6), "1211");
		assertEquals(R(7, 6), "1121");
		assertEquals(R(6, 6), "1112");

		assertEquals(R(6, 7), "2111");
		assertEquals(R(6, 8), "1211");
		assertEquals(R(7, 8), "1121");
		assertEquals(R(8, 8), "1112");
		
		assertEquals(R(9, 7), "2111");
		assertEquals(R(10, 7), "2111");
		assertEquals(R(11, 7), "C111");
		assertEquals(R(12, 7), "1111");
		assertEquals(R(13, 7), "1111");	
	}
	
	@Test
	public void test_two_self_point() {
		this.initChessStatus(15, new int[][]{{7,7,RC}, {8,7,RC} });
		//byte[] st = game.checkResult(5, 2, (byte)1);
	}	

	@Test
	public void test_one_opponent_point() {
		this.initChessStatus(15, testData1);
		
		assertEquals(H(8, 7), "B111");
		assertEquals(H(8, 6), "1B11");
		assertEquals(H(7, 6), "11B1");
		assertEquals(H(6, 6), "111B");

		assertEquals(H(6, 7), "B111");
		assertEquals(H(6, 8), "1B11");
		assertEquals(H(7, 8), "11B1");
		assertEquals(H(8, 8), "111B");	
	}
	
	


}
