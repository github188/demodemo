#pragma once

#include "logfile.h"
#include "util.h"
#include "TimeOutSocket.h"

#define BUFSIZE		400 * 1024

class CSimpleSocket{
protected:
	CString primary_server;
	CString secondary_server;
	int primary;

	char m_sBuffer[BUFSIZE];
	int  m_bufferPos;
	int  m_bufferLimit;
	
	//
    CRITICAL_SECTION m_Lock;

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
		m_bufferLimit=BUFSIZE;
		m_bufferPos=0;
		memset(m_sBuffer, 0, sizeof(m_sBuffer));
		//strcpy(buffer, "this is only a test...\n do you know it?\n");

		::InitializeCriticalSection( &m_Lock );

	}
	~CSimpleSocket()
	{
        ::DeleteCriticalSection( &m_Lock );
	}

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

	void InitSocketParameter(CString spServer, CString ssServer,int primary=0)
	{
		primary_server = spServer;
		secondary_server = ssServer;
		primary = primary;	
	}

	//Send & Receive Data
	void SendCmdAndRecvMsg(CString& sCmd, CString& sMsg);
};

//Add this for 
class CSimpleSocketImpl: public CSimpleSocket{

public:
	CSimpleSocketImpl(CString& ps, CString& ss): CSimpleSocket(ps, ss) {
	};

	int connect_server(){
		//const char *host = ipaddr.c_str();
		//const char *Lhost = ipaddr.c_str();
		//host.l
		initServerAddr();
		//CString host(ipaddr.c_str());
		if(cs.Create() == FALSE){
			MessageBox(NULL,_T("Error create"), _T("Error create"),MB_OK);
			return -1;
		}
		CString host = ipaddr;
		//MessageBox(NULL,_T("Error 2"), host, MB_OK);
		if(cs.Connect(host, port)==FALSE)
		{
			MessageBox(NULL,_T("Error connect"), _T("Error connect"),MB_OK);
			return -1;
		}
		return 1;
	}
	int read_buffer(char *buffer, int size){
		CString xx;
		int len = cs.Receive(buffer, size);
		//buffer[len] = 0;
		util::int2str(xx, len);
		CLogFile::WriteLog(CString("read buffer:") + xx);
		if(len > 0 && len < 1024){
			buffer[len] = 0;
			CLogFile::WriteLog((const char *)buffer);
		}

		return len;
	}

protected:
		virtual int write_data(const char *buff, int len){
		//char sendcmd_startch[200];
		//wcstombs(sendcmd_startch, buff, len);
		return cs.Send(buff, len);
	}

private:
	CSocket cs; //=new CSocket();

};