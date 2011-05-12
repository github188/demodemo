#ifndef __LOGIN_H_
#define __LOGIN_H_

#include "userdefMsg.h"

typedef struct
{
	CString servIP;
	CString servPort;
	CString	uname;
	CString	password;
	//DWORD	hThreadId;
	HWND	UIhwnd;
}uinfo;

UINT loginProc(LPVOID param);

#endif