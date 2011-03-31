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
	::EnterCriticalSection( &m_Lock );

	des.Empty();
	char ch = 0;
	if(m_bufferPos >= m_bufferLimit){
		ZeroMemory(m_sBuffer, sizeof(m_sBuffer));
		//bufferLimit = read_buffer(m_sBuffer, sizeof(buffer));
		m_bufferLimit = read_buffer(m_sBuffer, BUFSIZE);
		m_bufferPos = 0;
	}
	for(;m_bufferPos < m_bufferLimit && timeout > 0;){
		ch = m_sBuffer[m_bufferPos++];
		if(ch == '\n'){
			break;
		}else{
			char charray[2];
			charray[0]=ch;
			charray[1]='\0';
			des.Append(charray);
		}
		if(m_bufferPos >= m_bufferLimit){
			//sleep(1);
			
			//ZeroMemory(m_sBuffer, sizeof(m_sBuffer));
			//bufferLimit = read_buffer(buffer, sizeof(buffer));
			//bufferPos = 0;

			timeout--;
		}
	}
	CLogFile::WriteLog("readline:" + des);

	::LeaveCriticalSection( &m_Lock );

	return des.GetLength();
}

int CSimpleSocket::write_wstring(CString &data)
{	
	::EnterCriticalSection( &m_Lock );

	CString sLog;
	//CLogFile::WriteLog("write_wstring:" + data);
	int writeLen = data.GetLength();
	int len = write_data(data, writeLen);
	m_bufferLimit = m_bufferPos = 0;

	::LeaveCriticalSection( &m_Lock );

	return len;
}

void CSimpleSocket::SendCmdAndRecvMsg(CString& sCmd, CString& sMsg)
{
	::EnterCriticalSection( &m_Lock );
	
	//Send Cmd
	int writeLen = sCmd.GetLength( );
	int dwSend = write_data(sCmd,writeLen);
	
	if (dwSend == SOCKET_ERROR || dwSend == 0)
	{
		CString sError;
		sError.Format("数据发送失败！网络可能出现问题,错误代码:%d",  GetLastError());
		AfxMessageBox(sError);
		return;
		//Need to close socket, and ReOpen(),  Close();??
	}
	m_bufferLimit = m_bufferPos = 0;

	//Receive Data
	ZeroMemory(m_sBuffer, sizeof(m_sBuffer));
	m_bufferLimit = read_buffer(m_sBuffer, BUFSIZE);

	sMsg = m_sBuffer;

	::LeaveCriticalSection( &m_Lock );
}
