#pragma once

#include "SimpleSocket.h"

class GokuSocket: public CSimpleSocket{

public:
	GokuSocket(CString &ps, CString &ss): CSimpleSocket(ps, ss) {
		VERIFY(m_hAutoWait = CreateEvent( NULL,FALSE,FALSE,NULL));
		VERIFY(m_hStopAutoWait = CreateEvent(NULL,FALSE,FALSE,NULL));
		VERIFY(m_hExitTimerThread = CreateEvent(NULL,FALSE,FALSE,NULL));
		
		m_lWaitTime = INFINITE;
		m_hEvent[0] = m_hAutoWait;
		m_hEvent[1] = m_hStopAutoWait;
		m_hEvent[2] = m_hExitTimerThread;
	}

	~GokuSocket(){
		::SetEvent(m_hExitTimerThread);
		::Sleep(100);
		if (m_hAutoWait)	CloseHandle(m_hAutoWait);
		if (m_hStopAutoWait)	CloseHandle(m_hStopAutoWait);
		if (m_hExitTimerThread)	CloseHandle(m_hExitTimerThread);
	}
	int connect_server();
	int read_buffer(char *buffer, int size);
	void CancelSocket() {
		cs.CancelBlockingCall();
	}
	
	void StartAutoWait(int nSec){
		m_lWaitTime = nSec * 1000;
		::SetEvent(m_hAutoWait);
	}

	void StopAutoWait()	{
		m_lWaitTime = INFINITE;
		::SetEvent(m_hStopAutoWait);
	}

	long GetWaitTime() {
		return m_lWaitTime; 
	};
	void ExitAutoWait(){
		::SetEvent(m_hExitTimerThread);
	}

public:
	HANDLE m_hEvent[3];

protected:
	virtual int write_data(const char *buff, int len);

public:
	CSocket cs;
private:
	//CTimeOutSocket	cs;
	HANDLE	m_hAutoWait;
	HANDLE	m_hStopAutoWait;
	HANDLE	m_hExitTimerThread;
	long   m_lWaitTime;



};