package org.demo.five.ai;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

/**
 * 在双方都有棋子的情况下计算棋力。
 * @author dalwu
 *
 */
public class TestGameTree_evaluation_complex extends BaseTestGameTree{
	
	@Test
	public void test_evaluation_one_point_in_center() {
		this.initChessStatus(15, new int[][]{{7,7,RC}});	
		assertEquals(H(6, 7), "B111");
		assertEquals(HE(6, 7), -70);
	}

	@Test
	public void test_evaluation_two_point_in_center() {
		this.initChessStatus(15, new int[][]{{7,7,RC}, {6,7,RC}});
		assertEquals(H(8, 7), "B111");
		assertEquals(HE(8, 7), -70);
		
		assertEquals(H(7, 6), "1BB1");
		assertEquals(HE(7, 6), -60);
		
		this.initChessStatus(15, new int[][]{{6,7,RC}, {8,7,RC}});
		assertEquals(H(7, 7), "0111");
		assertEquals(HE(7, 7), -62);
		
		this.initChessStatus(15, new int[][]{{5,7,RC}, {9,7,RC}});
		assertEquals(H(7, 7), "0111");
		assertEquals(HE(7, 7), -62);		
		
		this.initChessStatus(15, new int[][]{{4,7,RC}, {9,7,RC}});
		assertEquals(H(7, 7), "0111");
		assertEquals(HE(7, 7), -62);	
		assertEquals(H(5, 7), "0111");
		assertEquals(HE(5, 7), -62);			
		
		this.initChessStatus(15, new int[][]{{4,7,RC}, {10,7,RC}});
		assertEquals(H(7, 7), "1111");
		assertEquals(HE(7, 7), -80);	
		
		assertEquals(H(5, 7), "B111");
		assertEquals(HE(5, 7), -70);
	
		assertEquals(H(6, 7), "1111");
		assertEquals(HE(6, 7), -80);
	}
	
}
