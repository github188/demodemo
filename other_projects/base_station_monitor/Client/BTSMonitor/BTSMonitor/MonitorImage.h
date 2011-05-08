#ifndef MONITOR_IMAGE_H
#define MONITOR_IMAGE_H

#include "util.h"
#include "SimpleSocket.h"

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

public:
	CString errcode;
	CSimpleSocket *mSock;

	MonitorImage()
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
		mSock=NULL;
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
<<<<<<< .mine
	bool getNextImage(int *err);

	//Retrive imagin session ID again, once the image is time expired.
	bool getNextImageSession(CString sBtsUUID, CString sCh, int *err);
=======
	int getNextImage();
>>>>>>> .r813
};

#endif