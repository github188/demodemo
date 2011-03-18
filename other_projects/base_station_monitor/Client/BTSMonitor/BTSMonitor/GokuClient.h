#pragma once

#include "GokuSocket.h"
#include "BTSManager.h"
#include "AlarmManager.h"
#include "VideoPlayControl.h"

class GokuClient{
	CString buffer;
	CString cmd_msg; //last command status.

public:
	GokuSocket *socket;
	BTSManager btsmanager;
	AlarmManager alarmmanager;
	CWinThread *playThread;
	CWinThread *alarmThread;

public:
	GokuClient(CString &primary_server, CString &secondary_server)
	{

		socket = new GokuSocket(primary_server, secondary_server);
		m_nConnectCode = socket->connect_server();
	}

	~GokuClient()
	{
		delete socket;
	}

	/**
	 * login to master server;
	 */
	int login(const TCHAR *username, const TCHAR *password);
	int login(CString &user, CString &password);
	int logout();
	bool GetUserInfo(CString& sUserName, CString& sPassword, int& nSid);

	void listbtstree(CString &str);
	void getAlarmStr(CString &alarmStr);
	void queryAlarmInfo(CString category, CString uuid, CString startDate, CString startTime,
				CString type, CString level, CString limit, CString offset, CString &qalarmStr);
	void getRealTimeAlarmStr(CString &alarmStr);
	bool confirmAlarm(CString uuid);
	
	VideoPlayControl* real_play(CString &uuid, CString &channel, DataCallBack callback, int session=0);
	VideoPlayControl* replay(CString &videoId, DataCallBack callback, int session=0);
	int IsConnected() { return (m_nConnectCode == -1 ? FALSE: TRUE); }
	void ReConnectServer() 	{		m_nConnectCode = socket->connect_server(); 	}

protected:
	int execute_command(CString &cmd);
	CString m_sUserName;
	CString m_sPassword;
	int		m_nSid;
	int		m_nConnectCode;
};

UINT alarm_getRealAlarm_thread(LPVOID param);