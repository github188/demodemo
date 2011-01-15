#pragma once

#include "logfile.h"

#define BUFSIZE		4 * 1024

class CSimpleSocket{
protected:
	CString primary_server;
	CString secondary_server;
	int primary;

	char buffer[BUFSIZE];
	int bufferPos;
	int bufferLimit;

	virtual int write_data(const char *buffer, int size){
		CLogFile::WriteLog("write_data in SimpleSocket...");
		return 0;
	}

	void initServerAddr();

public:
	CString ipaddr;
	int port;
	CSimpleSocket(const CString &pserver, const CString &sserver){
		primary = 0;
		primary_server = pserver;
		secondary_server = sserver;
		bufferLimit=BUFSIZE;
		bufferPos=0;
		memset(buffer, 0, sizeof(buffer));
		//strcpy(buffer, "this is only a test...\n do you know it?\n");
	}
	~CSimpleSocket(){}

	virtual int connect_server() {return 0;}
	virtual void close() {}
	virtual int read_buffer(char *buffer, int size);

	//int write_wstring(wstring &data){
	//	write_log(L"write_wstring:" + data);
	//	int writeLen = wcstombs(buffer, data.c_str(), data.length());
	//	int len = write_data(buffer, writeLen);
	//    bufferLimit = bufferPos = 0;
	//    return len;
	//};

	int readline(CString &des, long timeout=3);
	int write_wstring(CString &data);
};