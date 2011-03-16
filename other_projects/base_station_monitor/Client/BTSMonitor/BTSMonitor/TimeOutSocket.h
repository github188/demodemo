#pragma once

// CTimeOutSocket command target

class CTimeOutSocket : public CSocket
{
public:
	CTimeOutSocket();
	virtual ~CTimeOutSocket();
	virtual BOOL OnMessagePending();
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
protected:
	int m_nTimerID;
private:
	BOOL KillTimeOut(void);
	BOOL SetTimeOut(int nTimeOut);
};


