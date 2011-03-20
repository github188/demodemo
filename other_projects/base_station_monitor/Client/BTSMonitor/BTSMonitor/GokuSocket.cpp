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

	//char szBuffer[BUFSIZE];   
	//ZeroMemory(szBuffer,sizeof(szBuffer)); 
	
	CString xx;

	CWaitCursor wait;	


	//::EnterCriticalSection( &m_Lock );
	//memset(buffer,0,size);
	int len = cs.Receive(buffer, size);
	//int len = cs.Receive(szBuffer,BUFSIZE);
    //::LeaveCriticalSection( &m_Lock );

	if (len<0)		return 0; //if socket failed to logon, here will be returned -1, add this to viod crash.

	WCHAR *ubuffer=new WCHAR[len];
	//ZeroMemory(ubuffer,sizeof(ubuffer));
	util::nettolocal(ubuffer, buffer, len);
	ZeroMemory(buffer, BUFSIZE);
	int nNum = util::widechar2str(ubuffer, &buffer);


	/*/--------------------------------------------------
	u_short *ps=(u_short *)(szBuffer+2);
	//u_short pconvs;
	for(int i=0;i<len/2-1;i++,ps++)
	{
		*ps=ntohs(*ps);
		//ps++;
		//memcpy(&dstlocal[i], &pconvs, sizeof(pconvs));
	}
	//UNICODE转换为ANSI   
	int length = WideCharToMultiByte(CP_ACP, 0, (WCHAR *)(szBuffer+2), len/2-1, NULL,		0,	 NULL,NULL); 
	//int length = WideCharToMultiByte(CP_ACP, 0, (WCHAR *)(szBuffer+2), -1, NULL,		0,	 NULL,NULL); 
	int nNum   = WideCharToMultiByte(CP_ACP, 0, (WCHAR *)(szBuffer+2), len/2-1, buffer, length,  NULL,NULL);  
	*///------------------------------------------------------
	
	//write to Log File.
	//util::int2str(xx, len);
	util::int2str(xx,nNum);
	CLogFile::WriteLog("read buffer:" + xx);
	if(len > 0 && len < 1024){
		buffer[len] = 0;
		CLogFile::WriteLog((const char *)buffer);
	}

	wait.Restore();         

	//delete []ubuffer;
	//return len;
	return nNum;
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