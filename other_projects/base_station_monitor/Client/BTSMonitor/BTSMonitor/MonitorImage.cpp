#include "stdafx.h"
#include "MonitorImage.h"
#include "util.h"
#include "base64.h"
#include <iostream>
#include <fstream>
using namespace std;

void MonitorImage::getImageText(CString &line)
{
	int pos=0;
	//size:352*288$time:2011-04-09 18-27-55$length:3017$base:1004$ch:1$status:$2
	CString size;
	pos=util::split_next(line, size, '$', 0);
	pos=util::split_next(line, datetime, '$', pos+1);
	pos=util::split_next(line, lenth, '$', pos+1);
	pos=util::split_next(line, bts, '$', pos+1);
	pos=util::split_next(line, channel, '$', pos+1);
	pos=util::split_next(line, status, '$', pos+1);
}

void MonitorImage::decodeImageData(CString &base64line)
{
	int len=util::str2int(lenth);
	data=new char[3017];
	DecodeBase64(base64line.GetBuffer(), data);
}

void MonitorImage::savedata(CString &filename)
{
	ofstream ofs(filename, ios::binary|ios::out);
	ofs.write(data, 3017);
	ofs.close();
}