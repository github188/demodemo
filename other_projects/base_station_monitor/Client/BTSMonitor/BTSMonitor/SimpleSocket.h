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
	bool m_bIsConnect;
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
		
		m_bIsConnect = false;
		
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
	void SetConnectStatus(bool bConnect) {m_bIsConnect =bConnect; }; 
	bool IsConnectStatus() {return m_bIsConnect; }; 

	virtual BOOL SocketDetach() {return FALSE;};
	virtual BOOL SocketAttach() {return FALSE;};
};

//Add this for 
class CSimpleSocketImpl: public CSimpleSocket{

public:
	CSimpleSocketImpl(CString& ps, CString& ss): CSimpleSocket(ps, ss) {

		memset(&m_hSoc,0,sizeof(SOCKET));
	};

	int connect_server(){
		//const char *host = ipaddr.c_str();
		//const char *Lhost = ipaddr.c_str();
		//host.l
		initServerAddr();
		//CString host(ipaddr.c_str());
	
		if (cs.m_hSocket == INVALID_SOCKET) //如果是从新连接，不需要再Create()
		{
			if(cs.Create() == FALSE){
				//MessageBox(NULL,_T("创建网络连接失败!"), _T("错误"),MB_ICONERROR|MB_OK);
				CString strError;
				strError.Format("Create Socket error, Error Code: %d",GetLastError()); 
				CLogFile::WriteLog(strError);

				SetConnectStatus(false);
				return -1;
			}
		}

		CString host = ipaddr;
		if(cs.Connect(host, port)==FALSE)
		{
			//MessageBox(NULL,_T("无法连接网络!"), _T("错误"),MB_ICONERROR|MB_OK);
			CString strError;
			strError.Format("Connect Socket error, Error Code: %d",GetLastError()); 
			CLogFile::WriteLog(strError);

			SetConnectStatus(false);
			return -1;
		}

		SetConnectStatus(true);

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

	BOOL SocketDetach() {
		 BOOL bOk = FALSE; 
		 m_hSoc = cs.Detach();
		 if (m_hSoc)
			bOk = TRUE;
		return bOk;
	}

	BOOL SocketAttach() {
		BOOL bOk = FALSE;
		if (m_hSoc)
			bOk = cs.Attach(m_hSoc);
		return bOk;
	}

protected:
		virtual int write_data(const char *buff, int len){
		//char sendcmd_startch[200];
		//wcstombs(sendcmd_startch, buff, len);
		return cs.Send(buff, len);
	}

private:
	CSocket cs; //=new CSocket();
	SOCKET	m_hSoc;

};