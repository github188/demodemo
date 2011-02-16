#pragma once

#include "GokuSocket.h"
#include "BTSManager.h"
#include "AlarmManager.h"
#include "VideoPlayControl.h"

class GokuClient{
	GokuSocket *socket;
	CString buffer;
	CString cmd_msg; //last command status.

public:
	BTSManager btsmanager;
	AlarmManager alarmmanager;

public:
	GokuClient(CString &primary_server, CString &secondary_server)
	{
		socket = new GokuSocket(primary_server, secondary_server);
		socket->connect_server();
	}

	~GokuClient(){delete socket;}

	/**
	 * login to master server;
	 */
	int login(const TCHAR *username, const TCHAR *password);
	int login(CString &user, CString &password);

	int logout();

	void listbtstree(CString &str);
	void getAlarmStr(CString &alarmStr);
	void getRealTimeAlarmStr(CString &alarmStr);
	bool confirmAlarm(CString uuid);
	
	VideoPlayControl* real_play(CString &uuid, DataCallBack callback, int session=0);
	VideoPlayControl* replay(CString &videoId, DataCallBack callback, int session=0);

protected:
	int execute_command(CString &cmd);
};
