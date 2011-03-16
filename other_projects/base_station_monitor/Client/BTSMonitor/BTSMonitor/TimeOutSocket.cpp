// TimeOutSocket.cpp : implementation file
//

#include "stdafx.h"
#include "BTSMonitor.h"
#include "TimeOutSocket.h"


// CTimeOutSocket

CTimeOutSocket::CTimeOutSocket()
: m_nTimerID(0)
{
}

CTimeOutSocket::~CTimeOutSocket()
{
}


// CTimeOutSocket member functions

BOOL CTimeOutSocket::OnMessagePending()
{
	// TODO: Add your specialized code here and/or call the base class
	MSG msg;
	if(::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE))
	{
		if (msg.wParam == (UINT) m_nTimerID)
		{
			// Remove the message and call CancelBlockingCall.
			::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
			CancelBlockingCall();
			return FALSE;  // No need for idle time processing.
		};
	};
	return CSocket::OnMessagePending();
}

int CTimeOutSocket::Receive(void* lpBuf, int nBufLen, int nFlags)
{
	// TODO: Add your specialized code here and/or call the base class
	SetTimeOut(10000);
	int nRecv = CSocket::Receive(lpBuf, nBufLen, nFlags);
	KillTimeOut();
	return nRecv;
}

int CTimeOutSocket::Send(const void* lpBuf, int nBufLen, int nFlags)
{
	// TODO: Add your specialized code here and/or call the base class
	
	//SetTimeOut(10000);
	//int nSend = CSocket::Send(lpBuf, nBufLen, nFlags);
	//KillTimeOut();
	//return nSend; 

	return CSocket::Send(lpBuf, nBufLen, nFlags);
}

BOOL CTimeOutSocket::KillTimeOut(void)
{
	m_nTimerID = SetTimer(NULL,0,m_nTimeOut,NULL);
	return m_nTimerID;
	//return false;
}

BOOL CTimeOutSocket::SetTimeOut(int nTimeOut)
{
	return KillTimer(NULL,m_nTimerID);
	//return false;
}
