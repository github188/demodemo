#pragma once

#include "SimpleSocket.h"

class GokuSocket: public CSimpleSocket{

public:
	GokuSocket(CString &ps, CString &ss): CSimpleSocket(ps, ss) {}
	~GokuSocket(){}
	int connect_server();
	int read_buffer(char *buffer, int size);

protected:
	virtual int write_data(const char *buff, int len);

private:
	CSocket cs;
	//CTimeOutSocket	cs;

};