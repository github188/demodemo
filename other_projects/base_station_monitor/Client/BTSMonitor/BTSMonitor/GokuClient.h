#pragma once
#include "const.h"
#include "GokuSocket.h"
#include "BTSManager.h"
#include "AlarmManager.h"
#include "VideoPlayControl.h"
#include "MonitorImage.h"

#define IMAGE_PORT	"8003"

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
		int i=0;
		host=primary_server;
		for (; i<cnMAX_VV; i++)
		{
			m_pPlayThread[i]=NULL;
			m_pArrVideoCtrl[i]=NULL;
		}

		socket = new GokuSocket(primary_server, secondary_server);
		m_nConnectCode = socket->connect_server();
	}

	~GokuClient()
	{
		int i=0;
		for (i=0; i<cnMAX_VV; i++)
		{
			if (m_pArrVideoCtrl[i])
				m_pArrVideoCtrl[i]->status = 0;
		}

		
		for (i=0; i<cnMAX_VV; i++)
		{
			if (m_pArrVideoCtrl[i]==NULL)
				continue;

			if ( m_pArrVideoCtrl[i]->status==1)
			{
				::WaitForSingleObject(m_pPlayThread[i]->m_hThread,8000);
				if (m_pArrVideoCtrl[i]->status != -1)
					::AfxTermThread((HINSTANCE__*)(m_pPlayThread[i]->m_hThread));
			}

			m_pPlayThread[i] = NULL;
			delete m_pArrVideoCtrl[i];
			m_pArrVideoCtrl[i]=NULL;

		}


		/*/Wait the thread exit----------------------------------------------------- .
		int nTry = 3;
		for (i=0; i<nTry; i++)
		{
			BOOL bAllThreadIsExit = TRUE;
			int j=0;
			for (; j<cnMAX_VV; j++)
			{
				if (m_pArrVideoCtrl[j]==NULL )
				{
					bAllThreadIsExit = bAllThreadIsExit && TRUE;
					continue;
				}

				if (m_pArrVideoCtrl[j]->status != 1 ) //if Not 1, the thread is exited
				{
					bAllThreadIsExit = bAllThreadIsExit && TRUE;
					continue;
				}

				//status ==1, should wait more time...

				break;
			}

			if ( i >= nTry ) //if max try is reached, do not try any more...
				break;

			if (bAllThreadIsExit && j>=cnMAX_VV) //all thread is Exit,
				break;

			Sleep(500);

		}
		
		//Free all memory 
		for (i=0; i<cnMAX_VV; i++)
		{

			if (m_pArrVideoCtrl[i])
			{

				if (m_pArrVideoCtrl[i]->status==1)
					TerminateThread(m_pPlayThread[i]->m_hThread, 0);  

				::Sleep(100);

				delete m_pArrVideoCtrl[i];
				m_pArrVideoCtrl[i] = NULL;
			}
		}
		*///-----------------------------------------------------------

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
	
	//VideoPlayControl* real_play(CString &uuid, CString &channel, DataCallBack callback, int session=0);
	bool real_play(CString &uuid, CString &channel, DataCallBack callback, int session=0);
	VideoPlayControl* replay(CString &videoId, DataCallBack callback, int session=0);
	int IsConnected() { return (m_nConnectCode == -1 ? FALSE: TRUE); }
	void ReConnectServer() 	{		m_nConnectCode = socket->connect_server(); 	}

	MonitorImage* GokuClient::getRealImagebyBase64(BTSInfo *binfo);
	MonitorImage* getAlarmImagebyBase64(CString alarmID);
protected:
	int execute_command(CString &cmd);
	CString host;
	CString m_sUserName;
	CString m_sPassword;
	int		m_nSid;
	int		m_nConnectCode;
	VideoPlayControl *m_pArrVideoCtrl[cnMAX_VV];
	CWinThread		 *m_pPlayThread[cnMAX_VV];

public:
	bool Stop_Play(int nVideoID);
};

UINT alarm_getRealAlarm_thread(LPVOID param);