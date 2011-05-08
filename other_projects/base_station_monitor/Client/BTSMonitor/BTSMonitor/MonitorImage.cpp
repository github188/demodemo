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
	pos=util::split_next(line, height, '$', 0);
	pos=util::split_next(line, width, '$', pos+1);
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
	data=new char[len+1];
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

<<<<<<< .mine
bool MonitorImage::getNextImage(int *err)
=======
int MonitorImage::getNextImage()
>>>>>>> .r813
{
	CString sCmd, sMsg;
	sCmd.Append("img>next_image?session=");
	sCmd.Append(session);
	sCmd.Append("\n");
	if ( !mSock->SendCmdAndRecvMsg(sCmd,sMsg) )
	{
		*err = 0xFF;
		return false;
	}

	
	if ( sMsg.IsEmpty() ) 
	{
		*err = 0xFF;
		return false;
	}

	CString line;
	int ileft=0, iright=0, ipos=0;
<<<<<<< .mine
	//Get First Line, retrieve the Image Stauts
	ipos=sMsg.Find('\n', ileft);
	*err = atoi(sMsg.Left(1));
	if (*err == 0)
	{
		line=sMsg.Mid(ileft, ipos-ileft+1);
		ileft=ipos;
		ipos=sMsg.Find('\n', ileft+1);
		line=sMsg.Mid(ileft+1, ipos-ileft);
		if (line.IsEmpty())
		{
			*err = 0xFF;
			return false;
		}

		getImageText(line);
		line=sMsg.Mid(ipos+1, sMsg.GetLength()-ipos);
		decodeImageData(line);

		return true;
	}

	return false;
}

bool MonitorImage::getNextImageSession(CString sBtsUUID, CString sCh, int *err)
{
	CString sCmd;
	sCmd.Append("img>real_image?");
	sCmd.Append("baseStation=");
	sCmd.Append(sBtsUUID);
	sCmd.Append("&");
	sCmd.Append("channel=");
	sCmd.Append(sCh);
	sCmd.Append("\n");
	
	CString sMsg;
	if (!mSock->SendCmdAndRecvMsg(sCmd,sMsg))
		return false;

	CString line;
	int ileft=0, iright=0, ipos=0;
	ipos=sMsg.Find('\n', ileft);
	line=sMsg.Mid(ileft, ipos-ileft+1);

	//get the session.
	getSessionFromLine(line);
	*err=util::str2int(errcode);
	if(*err!=0)
		return false;

	return true;
=======
	ipos=sMsg.Find('$',ileft);
	CString strnum=sMsg.Mid(ileft, ipos-ileft);
	int num=util::str2int(strnum);
	if(num==0)
	{
		ipos=sMsg.Find('\n', ileft);
		line=sMsg.Mid(ileft, ipos-ileft+1);
		ileft=ipos;
		ipos=sMsg.Find('\n', ileft+1);
		line=sMsg.Mid(ileft+1, ipos-ileft);
		getImageText(line);
		line=sMsg.Mid(ipos+1, sMsg.GetLength()-ipos);
		decodeImageData(line);
	}
	
	return num;
>>>>>>> .r813

}
