#pragma once

#include "SimpleSocket.h"

class GokuSocket: public CSimpleSocket{

public:
	GokuSocket(CString &ps, CString &ss): CSimpleSocket(ps, ss) 
	{
        ::InitializeCriticalSection( &m_Lock );
	}
	~GokuSocket()
	{
        ::DeleteCriticalSection( &m_Lock );
	}
	int connect_server();
	int read_buffer(char *buffer, int size);

protected:
	virtual int write_data(const char *buff, int len);

private:
	CSocket cs;
	//CTimeOutSocket	cs;
	//
    CRITICAL_SECTION m_Lock;

};