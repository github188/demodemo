package org.goku.image;

public class ImageClientEvent {
	public ASC100Client source = null;
	public ImageInfo image = null;
	public ASC100Package data = null;
	public ImageClientEvent(ASC100Client s){
		this.source = s;
	}

}
