package org.goku.odip;

import java.io.IOException;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.nio.channels.spi.SelectorProvider;
import java.util.Iterator;

/**
 * NIO Selecting 线程。
 * @author deon
 */
public class ChannelSelector implements Runnable{
	
	private Selector selector = null;
	
	
	public ChannelSelector(){
		
	}
	
	public SelectionKey register(SocketChannel channel, int ops, Object att) 
		throws ClosedChannelException{
		return null;
	}

	@Override
	public void run() {
		try{
			selector = SelectorProvider.provider().openSelector(); 
		}catch(IOException e){		
		}
		
		try{
			this.selector.select();
			Iterator selectedKeys = this.selector.selectedKeys().iterator();
			while (selectedKeys.hasNext()) {
				SelectionKey key = (SelectionKey) selectedKeys.next();
				selectedKeys.remove();

				if (!key.isValid()) {
					continue;
				}
				
				/*
				// Check what event is available and deal with it
				if (key.isConnectable()) {
					this.finishConnection(key);
				} else if (key.isReadable()) {
					this.read(key);
				} else if (key.isWritable()) {
					this.write(key);
				}
				*/
			}	
		}catch(IOException e){		
		}
		
	}
	
	public void shutdown(){
		
	}

}
