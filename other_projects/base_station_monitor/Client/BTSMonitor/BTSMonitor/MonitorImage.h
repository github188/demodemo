#ifndef MONITOR_IMAGE_H
#define MONITOR_IMAGE_H

#include "util.h"

class MonitorImage
{
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
	void getSessionFromLine(CString line)
	{
		int pos=0;
		CString tmp;
		pos=util::split_next(line, tmp, '$', 0);
		pos=util::split_next(line, tmp, '$', pos+1);
		pos=util::split_next(line, session, '$', pos+1);
	}

	void getImageText(CString &line);
	void decodeImageData(CString &base64line);
	void savedata(CString &filename);
};

#endif