#pragma once

#include "util.h"

class BTSInfo{
private:
	void buildClass(const CString &info);

public:
	CString uuid;
	CString devType;
	CString route;
	CString status;
	int idevType;

public:
	BTSInfo(const CString &info)
	{
		buildClass(info);
	}
	
	void printitself()
	{
		_tprintf("uuid %s, devType %s, route %s, status %s\n",
				uuid, devType, route, status);
	}
};