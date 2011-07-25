package org.notebook.cache;

import java.util.Date;

public class UpgradeModel {
	public String source;
	public long sourceSize;
	
	public String dst;
	public long dstSize;
	
	public boolean isUpdate;
	public Date updateDate;
}
