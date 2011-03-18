#include "stdafx.h"
#include "util.h"
#include "logfile.h"
#include "GokuSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int GokuSocket::read_buffer(char *buffer, int size)
{

	CString xx;

	//cs.SetTimeOut(10000);
	CWaitCursor wait;	

	::EnterCriticalSection( &m_Lock );
	int len = cs.Receive(buffer, size);
    ::LeaveCriticalSection( &m_Lock );

	//cs.KillTimeOut();
	if (len<0)		return 0; //if socket failed to logon, here will be returned -1, add this to viod crash.

	WCHAR *ubuffer=new WCHAR[len];
	util::nettolocal(ubuffer, buffer, len);
	len=util::widechar2str(ubuffer, &buffer);
	util::int2str(xx, len);
	CLogFile::WriteLog("read buffer:" + xx);
	if(len > 0 && len < 1024){
		buffer[len] = 0;
		CLogFile::WriteLog((const char *)buffer);
	}

	wait.Restore();         

	delete []ubuffer;
	return len;
}

int GokuSocket::write_data(const char *buff, int len)
{
	return cs.Send(buff, len);
}

int GokuSocket::connect_server()
{
	initServerAddr();
	
	if (cs.m_hSocket == INVALID_SOCKET) //如果是从新连接，不需要再Create()
	{
		if(cs.Create() == 0){
			MessageBox(NULL,_T("Error create"), _T("Error create"),MB_OK);
			DWORD errorcode=GetLastError();
			return -1;
		}
	}

	CString host=ipaddr;
	if(cs.Connect(host, port)==FALSE)
	{
		MessageBox(NULL,_T("Error connect"), _T("Error connect"),MB_OK);
		return -1;
	}
	return 1;
}