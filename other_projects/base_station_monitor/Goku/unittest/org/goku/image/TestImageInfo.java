package org.goku.image;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;

import java.nio.ByteBuffer;

import org.junit.Test;

public class TestImageInfo {
	
	/**
	 * 向ImageInfo添一些数据，确定得到的ReTry Frames是正确的。
	 */
	@Test
	public void test_getReTryFrames(){
		ImageInfo info = new ImageInfo();
		ByteBuffer buf = ByteBuffer.allocate(2);
		info.setFameCount(10);
		info.setDataSize(20);
		assertArrayEquals(info.getReTryFrames(), new int[]{1,2,3,4,5,6,7,8,9,10});
		
		info.recieveData(1, buf);
		info.recieveData(2, buf);
		
		info.recieveData(10, buf);		
		assertArrayEquals(info.getReTryFrames(), new int[]{3,4,5,6,7,8,9});
		
		info.recieveData(5, buf);
		assertArrayEquals(info.getReTryFrames(), new int[]{3,4,6,7,8,9});
	}
	
	
	/**
	 * 测试数据帧拼装正确。
	 */
	@Test
	public void test_FillFrameData(){
		ImageInfo info = new ImageInfo();
		ByteBuffer buf = ByteBuffer.allocate(2);
		info.setFameCount(5);
		info.setDataSize(10);		
		assertArrayEquals(info.getReTryFrames(), new int[]{1,2,3,4,5});		
		assertEquals(info.haveMoreData(), true);
		
		buf.clear();buf.put(new byte[]{1,1});buf.flip();	
		info.recieveData(1, buf);
		
		buf.clear();buf.put(new byte[]{2,2});buf.flip();	
		info.recieveData(2, buf);
		
		buf.clear();buf.put(new byte[]{5,5});buf.flip();		
		info.recieveData(5, buf);

		buf.clear();buf.put(new byte[]{3,3});buf.flip();		
		info.recieveData(3, buf);

		buf.clear();buf.put(new byte[]{4,4});buf.flip();		
		info.recieveData(4, buf);
		
				
		assertEquals(info.haveMoreData(), false);
		info.buffer.position(0);
		
		byte[] data = new byte[info.buffer.remaining()];
		info.buffer.get(data);		
		assertArrayEquals(data, new byte[]{1,1,2,2,3,3,4,4,5,5});				
	}	

}
