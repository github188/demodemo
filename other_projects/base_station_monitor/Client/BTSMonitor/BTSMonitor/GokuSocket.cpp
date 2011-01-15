#include "stdafx.h"
#include "util.h"
#include "logfile.h"
#include "GokuSocket.h"

int GokuSocket::read_buffer(char *buffer, int size)
{
	CString xx;
	char *ubuffer=new char[4096];
	memset(ubuffer, 0, 4096);
	int len = cs.Receive(ubuffer, size);
	//int len = cs.Receive(buffer, size);
	//received is unicode, translate it.
	util::widechar2str((WCHAR *)ubuffer, &buffer);
	delete []ubuffer;
	util::int2str(xx, len);
	CLogFile::WriteLog("read buffer:" + xx);
	if(len > 0 && len < 1024){
		buffer[len] = 0;
		CLogFile::WriteLog((const char *)buffer);
	}
	return len;
}

int GokuSocket::write_data(const char *buff, int len)
{
	return cs.Send(buff, len);
}

int GokuSocket::connect_server()
{
	initServerAddr();
	if(cs.Create() == 0){
		MessageBox(NULL,_T("Error create"), _T("Error create"),MB_OK);
		DWORD errorcode=GetLastError();
		return -1;
	}
	CString host=ipaddr;
	if(cs.Connect(host, port)==FALSE)
	{
		MessageBox(NULL,_T("Error 3"), _T("Error connect"),MB_OK);
		return -1;
	}
	return 1;
}