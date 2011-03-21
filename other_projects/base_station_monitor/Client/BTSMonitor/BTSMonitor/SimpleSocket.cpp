#include "stdafx.h"
#include "SimpleSocket.h"
#include "logfile.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CSimpleSocket::read_buffer(char *buffer, int size)
{
	CLogFile::WriteLog("read_buffer in SimpleSocket");
	return -1;
};

void CSimpleSocket::initServerAddr()
{
	CString cur;
	if(primary == 0)
	{
		cur = primary_server;
		primary = 1;
	}else 
	{
		cur = secondary_server;
		primary = 0;
	}
	int pos = util::split_next(cur, ipaddr, ':', 0);

	CString tempPort;

	util::split_next(cur, tempPort, ':', pos + 1);
	port=util::str2int(tempPort);
}

int CSimpleSocket::readline(CString &des, long timeout)
{
	des.Empty();
	char ch = 0;
	if(bufferPos >= bufferLimit){
		ZeroMemory(m_sBuffer, sizeof(m_sBuffer));
		//bufferLimit = read_buffer(m_sBuffer, sizeof(buffer));
		bufferLimit = read_buffer(m_sBuffer, BUFSIZE);
		bufferPos = 0;
	}
	for(;bufferPos < bufferLimit && timeout > 0;){
		ch = m_sBuffer[bufferPos++];
		if(ch == '\n'){
			break;
		}else{
			char charray[2];
			charray[0]=ch;
			charray[1]='\0';
			des.Append(charray);
		}
		if(bufferPos >= bufferLimit){
			//sleep(1);
			
			//ZeroMemory(m_sBuffer, sizeof(m_sBuffer));
			//bufferLimit = read_buffer(buffer, sizeof(buffer));
			//bufferPos = 0;

			timeout--;
		}
	}
	CLogFile::WriteLog("readline:" + des);
	return des.GetLength();
}

int CSimpleSocket::write_wstring(CString &data)
{	

	CString sLog;
	//CLogFile::WriteLog("write_wstring:" + data);
	int writeLen = data.GetLength();
	int len = write_data(data, writeLen);
	bufferLimit = bufferPos = 0;
	return len;
}