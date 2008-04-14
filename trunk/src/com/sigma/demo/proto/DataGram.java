package com.sigma.demo.proto;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

public interface DataGram {
	
	public void writeData(DataOutput output ) throws IOException;

	public void readData(DataInput input ) throws IOException;
}
