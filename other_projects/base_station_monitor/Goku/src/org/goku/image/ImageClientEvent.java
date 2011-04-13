package org.goku.image;

import java.nio.ByteBuffer;

public class ImageClientEvent {
	public ASC100Client source = null;
	public ImageInfo image = null;
	public ASC100Package data = null;
	public ByteBuffer raw = null;
	public ImageClientEvent(ASC100Client s){
		this.source = s;
	}

}
