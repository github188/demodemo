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
	//CString size;
	//pos=util::split_next(line, size, '$', 0);
	pos=util::split_next(line, width, '$', 0);
	pos=util::split_next(line, height, '$', 0);
	pos=util::split_next(line, datetime, '$', pos+1);
	pos=util::split_next(line, lenth, '$', pos+1);
	pos=util::split_next(line, bts, '$', pos+1);
	pos=util::split_next(line, channel, '$', pos+1);
	pos=util::split_next(line, status, '$', pos+1);
}

void MonitorImage::decodeImageData(CString &base64line)
{
	int len=util::str2int(lenth);
	if(data!=NULL)
		delete[] data;
	data=new char[len];
	DecodeBase64(base64line.GetBuffer(), data);
}

void MonitorImage::savedata(CString &filename)
{
	ofstream ofs(filename, ios::binary|ios::out);
	int len=util::str2int(lenth);
	//int len=3017;
	ofs.write(data, len);
	ofs.close();
}

bool MonitorImage::getNextImage()
{
	CString sCmd, sMsg;
	sCmd.Append("img>next_image?session=");
	sCmd.Append(session);
	if ( !mSock->SendCmdAndRecvMsg(sCmd,sMsg) )
			return false;
	CString line;
	int ileft=0, iright=0, ipos=0;
	ipos=sMsg.Find('\n', ileft);
	line=sMsg.Mid(ileft, ipos-ileft+1);
	ileft=ipos;
	ipos=sMsg.Find('\n', ileft+1);
	line=sMsg.Mid(ileft+1, ipos-ileft);
	getImageText(line);
	line=sMsg.Mid(ipos+1, sMsg.GetLength()-ipos);
	decodeImageData(line);
	return true;
}