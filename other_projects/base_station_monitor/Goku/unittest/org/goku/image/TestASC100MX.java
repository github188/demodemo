package org.goku.image;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.jmock.Expectations;
import org.jmock.Mockery;
import org.jmock.lib.legacy.ClassImposteriser;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class TestASC100MX {
	private ASC100MX testObj = null;
	private ThreadPoolExecutor threadPool = null;
	Mockery context = null;
	
	/**
	 * JMock使用的一个Demo. 
	 */
	@Test
	public void testDemoJmock2(){
		final ASC100Client client = context.mock(ASC100Client.class);
		context.checking(new Expectations() {{			 
			 oneOf(client).getSrId(); will(returnValue("ea.12.1"));
			 oneOf(client).getClientId(); will(returnValue("ea.12"));
		}});
		
		client.getSrId();
		client.getClientId();
				 
		context.assertIsSatisfied();
	}	
	
	/**
	 * 注册一个Client,收到一个简单的串口数据后。Client可以收到需要处理的数据。
	 * @throws IOException 
	 * @throws InterruptedException 
	 * 
	 */
	@Test
	public void testRoutingToASC100Client() throws Exception{
		final ASC100Client client = context.mock(ASC100Client.class);
		context.checking(new Expectations() {{			 
			 allowing(client).getSrId(); will(returnValue("ea.12"));
			 allowing(client).getClientId(); will(returnValue("ea.12.1"));
			 allowing(client).defaultMx(); will(returnValue("127.0.0.1"));
			 allowing(client).setASC100MX(with(any(ASC100MX.class)));
			 oneOf(client).process(with(any(ByteBuffer.class)));
		}});
		testObj.register(client);
		threadPool.execute(testObj);
		//等待服务端启动。
		Thread.sleep(100);
		
		this.sendUPDPackage(6001, new byte[]{0, 1, 10, 0, 0x12, (byte)0xea, 1, 0, 2, 0, (byte)0xff, 1});
		Thread.sleep(100);
				 
		context.assertIsSatisfied();
	}
	
	
	private void sendUPDPackage(int port, byte[] data) throws IOException{
		DatagramChannel channel = DatagramChannel.open();
		ByteBuffer buffer = ByteBuffer.allocate(65535);	  
		buffer.clear();
		buffer.put(data);
		buffer.flip();
		
		InetSocketAddress addr = new InetSocketAddress("127.0.0.1", port);
		
		for(int i = 10; i > 0 && buffer.hasRemaining(); i--){
			channel.send(buffer, addr);
		}
		if(buffer.hasRemaining()){
			throw new IOException("Time out to send UPD package.");
		}
		channel.close();
	}
	
	@Before
	public void initTestObject(){
		threadPool = new ThreadPoolExecutor(2, 10, 60, TimeUnit.SECONDS, 
				new LinkedBlockingDeque<Runnable>(5));
		testObj = new ASC100MX(5001, 6001, threadPool);
		context = new Mockery();
		context.setImposteriser(ClassImposteriser.INSTANCE);		
	}
	
	@After
	public void cleanTestObject(){
		if(testObj != null){
			testObj.close();
		}
	}	
}
