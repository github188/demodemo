#ifndef MONITOR_IMAGE_H
#define MONITOR_IMAGE_H

#include "util.h"
//#include "SimpleSocket.h"
#include "GokuSocket.h"

UINT Cmd_SendAndReceiveTimer(LPVOID param);

class MonitorImage
{
public:
	CString width;
	CString height;
	CString datetime;
	CString lenth;
	CString bts;
	CString channel;
	CString session;
	CString status;
	char* data;

	int   errCode;

	CWinThread *m_pTimerImageThread;

public:
	CString errcode;
	//CSimpleSocket *mSock;
	GokuSocket *mSock;

	MonitorImage(GokuSocket *pSocket=NULL)
	{
		width="";
		height="";
		datetime="";
		lenth="";
		bts="";
		channel="";
		session="";
		status="";
		errcode="0";
		data=NULL;
		//mSock=NULL;
		mSock = pSocket;

		if (mSock)
		{
			m_pTimerImageThread = AfxBeginThread(Cmd_SendAndReceiveTimer, mSock);
		}

		errCode = 0;
	}

	~MonitorImage()
	{
		width="";
		height="";
		datetime="";
		lenth="";
		bts="";
		channel="";
		session="";
		status="";
		if(data!=NULL)
		{
			delete[] data;
		}
		if(mSock!=NULL)
		{
			delete mSock;
		}
	}

	void getSessionFromLine(CString line)
	{
		int pos=0;
		CString tmp;
		pos=util::split_next(line, errcode, '$', 0);
		pos=util::split_next(line, tmp, '$', pos+1);
		pos=util::split_next(line, session, '$', pos+1);
	}

	void getImageText(CString &line);
	void decodeImageData(CString &base64line);
	void savedata(CString &filename);
	bool getNextImage(int *err);

	//Retrive imagin session ID again, once the image is time expired.
	bool getNextImageSession(CString sBtsUUID, CString sCh, int *err);
};

#endif