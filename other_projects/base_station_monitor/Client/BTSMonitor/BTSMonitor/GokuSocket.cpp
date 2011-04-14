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


	//memset(buffer,0,size);
	//int len = cs.Receive(buffer, size);
	//int len = cs.Receive(szBuffer,BUFSIZE);

	///Receive all data ---------------------------------
	int nTotalRead=0;
	const int cnLF=0xA;
	const int cnLD=0XD;
	int  nRead=0;
	BOOL bRead=TRUE;
	int nSpace=0;
	while(bRead)
	{
		nSpace = BUFSIZE-nTotalRead;
		nRead = cs.Receive(buffer+nTotalRead, nSpace);
		if (nRead==0 || SOCKET_ERROR == nRead)
		{
			CString sError;
			sError.Format("数据接收失败！发送的数据，或者网络可能出现问题,错误代码:%d",  GetLastError());
			AfxMessageBox(sError);
			bRead = FALSE;
			return 0;
		}

		nTotalRead+=nRead;
		if (nRead>0 && nTotalRead>4)
		{
			if ( ( *(buffer+nTotalRead-1) == cnLF) && ( *(buffer+nTotalRead-3) == cnLD) )
			{
				bRead = FALSE;
				break;
			}

			continue;
		}
		else
		{
			//AfxMessageBox("Socket数据读取有问题,放弃读取操作!");
			continue;
			//bRead = FALSE;
			//break;
		}
	}

	if (nTotalRead >= BUFSIZE)
		AfxMessageBox("数据缓冲区已满!");
	///--------------------------------------------------------

	int len = nTotalRead;
	//if (len<0)		return 0; //if socket failed to logon, here will be returned -1, add this to viod crash.
	if (nTotalRead<0)	return 0;
	
	WCHAR *ubuffer=new WCHAR[len];
	ZeroMemory(ubuffer,sizeof(ubuffer));
	util::nettolocal(ubuffer, buffer, len);
	ZeroMemory(buffer, BUFSIZE);
	int nNum = util::widechar2str(ubuffer, &buffer);


	/*/--------------------------------------------------
	if (len%2)
	{
		CString str;
		str.Format("Odd %d", len);
		AfxMessageBox(str);
	}

	int nBytes = len/2;
	u_short *ps=NULL;
	ps=(u_short *)&szBuffer;
	if ( (*ps) == 0xFFFE )
	{
		ps=(u_short *)(szBuffer+2);
		nBytes = len/2-1;
	}

	//u_short pconvs;
	for(int i=0;i<nBytes;i++,ps++)
	{
		*ps=ntohs(*ps);
		//ps++;
		//memcpy(&dstlocal[i], &pconvs, sizeof(pconvs));
	}
	//UNICODE转换为ANSI   
	int length = WideCharToMultiByte(CP_ACP, 0, (WCHAR *)(szBuffer+2), nBytes, NULL,	0,	 NULL,NULL); 
	//int length = WideCharToMultiByte(CP_ACP, 0, (WCHAR *)(szBuffer+2), -1, NULL,		0,	 NULL,NULL); 
	int nNum   = WideCharToMultiByte(CP_ACP, 0, (WCHAR *)(szBuffer+2), nBytes, buffer, length,  NULL,NULL);  
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

	delete []ubuffer;
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

			//MessageBox(NULL,_T("Error create"), _T("Error create"),MB_OK);
			//DWORD errorcode=GetLastError();

			CString strError;
			strError.Format("Create Socket error, Error Code: %d",GetLastError()); 
			CLogFile::WriteLog(strError);

			return -1;
		}
	}

	CString host=ipaddr;
	if(cs.Connect(host, port)==FALSE)
	{
		//MessageBox(NULL,_T("Error connect"), _T("Error connect"),MB_OK);

		CString strError;
		strError.Format("Connect Socket error, Error Code: %d",GetLastError()); 
		CLogFile::WriteLog(strError);

		return -1;
	}
	return 1;
}