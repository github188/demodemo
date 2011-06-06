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
	pos=util::split_next(line, width,  '$', pos+1);
	pos=util::split_next(line, datetime, '$', pos+1);
	pos=util::split_next(line, lenth,  '$', pos+1);
	pos=util::split_next(line, bts,    '$', pos+1);
	pos=util::split_next(line, channel, '$', pos+1);
	pos=util::split_next(line, status,  '$', pos+1);
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


bool MonitorImage::getNextImage(int *err)
{
	CString sCmd, sMsg;
	sCmd.Append("img>next_image?session=");
	if (session.IsEmpty()) 
	{
		CLogFile::WriteLog("Session is Empty");
		*err = 0xE1;
		return false;
	}
	sCmd.Append(session);
	sCmd.Append("\n");
	if ( !mSock->SendCmdAndRecvMsg(sCmd,sMsg) )
	{
		*err = 0xE2;
		return false;
	}

	
	if ( sMsg.IsEmpty() ) 
	{
		*err = 0xE3;
		return false;
	}

	CString line;
	int ileft=0, iright=0, ipos=0;

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
			*err = 0xE4;
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
}

UINT Cmd_SendAndReceiveTimer(LPVOID param)
{
	GokuSocket *socket = (GokuSocket *)param;

	if (!socket)		
	{
		return 0;
	}

	bool bAutoWaiting = false;

	bool bRuning = true;
	while(bRuning)
	{
		DWORD dwRet = ::WaitForMultipleObjects(2, socket->m_hEvent, false, socket->GetWaitTime());
		switch(dwRet)
		{
		case WAIT_OBJECT_0: //SetAutoWait....that mean's begin to counting...
			{
				bAutoWaiting = true;
			}
			break;
		case WAIT_OBJECT_0+1:
			{
				bAutoWaiting = false;
			}
			break;
		case WAIT_OBJECT_0+2:
			bRuning = false;
			break;
		case WAIT_ABANDONED_0:
			break;
		case WAIT_ABANDONED_0+1:
			break;
		case WAIT_ABANDONED_0+2:
			break;
		case WAIT_TIMEOUT:
			{
				socket->CancelSocket();
			}
			break;
		case WAIT_FAILED:
			{
				DWORD dwError = GetLastError();
				bRuning = false;

			}
			break;
		}
	}//...while...

	return 0x13;
}