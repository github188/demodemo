#pragma once
#include "const.h"
#include "GokuSocket.h"
#include "BTSManager.h"
#include "AlarmManager.h"
#include "VideoPlayControl.h"
#include "MonitorImage.h"
#include "ImageSocket.h"

UINT alarm_getRealAlarm_thread(LPVOID param);
UINT Command_SendAndReceiveTimer(LPVOID param);
UINT video_thread_control(LPVOID param);
class GokuClient{
	CString buffer;
	CString cmd_msg; //last command status.

public:
	MonitorImage *m_pMoImage;
	VideoPlayControl *m_pRealPlayControl;
	VideoPlayControl *m_pReplayControl;

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
		for (; i<cnTOTAL_VV_CNT/*cnMAX_VV*/; i++)
		{
			m_pPlayThread[i]=NULL;
			m_pArrVideoCtrl[i]=NULL;
		}
		m_pAlarmVideoCtrl = NULL;
		m_pAlarmVideoThread = NULL;

		socket = new GokuSocket(primary_server, secondary_server);
		m_nConnectCode = socket->connect_server();
		
		m_pVedioCtrlThread = NULL;
		m_pTimerThread = NULL;

		if (m_nConnectCode>0)
		{
			m_pTimerThread = AfxBeginThread(Command_SendAndReceiveTimer, socket);
		}
		
		m_hEventVideoCtrl[0] = ::CreateEvent(NULL,FALSE,FALSE,NULL); //Exit Video Ctrl
		m_hEventVideoCtrl[1] = ::CreateEvent(NULL,FALSE,FALSE,NULL); //Stop Time out...wait for other function over...

		m_pVedioCtrlThread = AfxBeginThread(video_thread_control, this);

		m_nSid = 0;

		m_pMoImage = NULL;
		m_lWaitTime = cnWAIT_TIME;

		m_pRealPlayControl=NULL;
		m_pReplayControl=NULL;

	}

	~GokuClient()
	{
		//Exit Video Thread Control....
		SetExitVedioThread();
		DWORD dwRet = ::WaitForSingleObject(m_pVedioCtrlThread->m_hThread,1000);
		if ( dwRet = WAIT_TIMEOUT)
		{
			::AfxTermThread((HINSTANCE__*)(m_pVedioCtrlThread->m_hThread));
			m_pVedioCtrlThread = NULL;
		}
		CloseHandle(m_hEventVideoCtrl[0]); m_hEventVideoCtrl[0] = NULL;
		CloseHandle(m_hEventVideoCtrl[1]); m_hEventVideoCtrl[1] = NULL;


		//Exit Video play thread Control...
		int i=0;
		for (i=0; i<cnTOTAL_VV_CNT/*cnMAX_VV*/; i++)
		{
			if (m_pArrVideoCtrl[i])
			{
				m_pArrVideoCtrl[i]->bIsBlocking = true; //
				m_pArrVideoCtrl[i]->status = 0; //Thread is runing...
				m_pArrVideoCtrl[i]->socket->CancelSocket();
			}
		}

		if (m_pAlarmVideoCtrl) //Alarm Query Thread
		{
			m_pAlarmVideoCtrl->bIsBlocking = true;
			m_pAlarmVideoCtrl->status = 0;
			m_pAlarmVideoCtrl->socket->CancelSocket();
		}

		::Sleep(100);
		dwRet = 0;
		for (i=0; i<cnTOTAL_VV_CNT/*cnMAX_VV*/; i++)
		{
			if (m_pArrVideoCtrl[i]==NULL)
				continue;

			if ( m_pArrVideoCtrl[i]->status == -1) //Exit
			{
				m_pPlayThread[i] = NULL;
				delete m_pArrVideoCtrl[i];
				m_pArrVideoCtrl[i]=NULL;

				continue;
			}

			dwRet = ::WaitForSingleObject(m_pPlayThread[i]->m_hThread,1000);
			switch(dwRet)
			{
			case WAIT_OBJECT_0:
			case WAIT_ABANDONED_0:
			case WAIT_FAILED:
				{
					m_pPlayThread[i] = NULL;
					delete m_pArrVideoCtrl[i];
					m_pArrVideoCtrl[i]=NULL;
				}
				break;
			case WAIT_TIMEOUT:
				{
					::AfxTermThread((HINSTANCE__*)(m_pPlayThread[i]->m_hThread));
					m_pPlayThread[i] = NULL;
					delete m_pArrVideoCtrl[i];
					m_pArrVideoCtrl[i]=NULL;

				}
				break;
			}

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
		socket->ExitAutoWait();

		delete socket;

		if ( m_pMoImage) 
		{
			delete m_pMoImage;
			m_pMoImage = NULL;
		}

		if (m_pAlarmVideoCtrl)
			delete m_pAlarmVideoCtrl;
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
	void queryAlarmInfo(CString category, CString uuid, CString sCh,CString startDate, CString startTime,
				CString type, CString level, CString limit, CString offset, CString &qalarmStr);
	void getRealTimeAlarmStr(CString &alarmStr);
	bool confirmAlarm(CString uuid);
	
	//Get Task List Information.
	bool getTaskList(CString& sTaskList);
	bool saveTaskInfo(CString sTaskID, 
		CString sName, 
		CString sUUID, 
		CString sCh, 
		CString sWindowID, 
		CString sWeek, 
		CString sStartDate,
		CString sEndDate, 
		CString sStartTime,
		CString sEndTime,
		CString sMinShowTime,
		CString sShowOrder);

	//VideoPlayControl* real_play(CString &uuid, CString &channel, DataCallBack callback, int session=0);
	bool real_play(CString &uuid, CString &channel, DataCallBack callback, int session, HWND hWnd=NULL);

	//VideoPlayControl* replay(CString &videoId, DataCallBack callback, int session=0);
	bool replay(CString &videoId, DataCallBack callback, int session=0, CString sAlarmVideoName="", bool bSave=false);
	void ReplayjumpToPos(CString pos);

	int IsConnected() { return (m_nConnectCode == -1 ? FALSE: TRUE); }
	void ReConnectServer() 	{		m_nConnectCode = socket->connect_server(); 	}

	//Modify errno is the windows reserve type
	//MonitorImage* getRealImagebyBase64(BTSInfo *binfo, int *errno);
	//MonitorImage* getAlarmImagebyBase64(CString alarmID, int *errno);
	MonitorImage* getRealImagebyBase64(BTSInfo *binfo, int *err);
	MonitorImage* getRealImagebyBase64(CString sBtsUUID,CString sCh, CString sRoute , int *err);
	//MonitorImage* getAlarmImagebyBase64(CString alarmID, int *err);
	MonitorImage* getAlarmImagebyBase64(CString sBtsUUID,CString sCh, CString sRoute,CString alarmID, int *err);

	int stop_Alarm(CString btsid, CString timeout);
	bool StopAlamVideoPlay();
protected:
	int execute_command(CString &cmd);
	CString host;
	CString m_sUserName;
	CString m_sPassword;
	int		m_nSid;
	int		m_nConnectCode;
	CWinThread		 *m_pVedioCtrlThread; //Control GoKuSocket
	CWinThread		 *m_pTimerThread; //Control GoKuSocket

public:
	VideoPlayControl *m_pArrVideoCtrl[cnTOTAL_VV_CNT]; //[cnMAX_VV]; //Real Play
	VideoPlayControl *m_pAlarmVideoCtrl;         //Replay Alarm Vedio

	CWinThread		 *m_pPlayThread[cnTOTAL_VV_CNT]; //[cnMAX_VV];   //Real Play thread handle
	CWinThread		 *m_pAlarmVideoThread;       //Alarm Video thread


public:
	bool Stop_Play(int nVideoID, int nStopType=0); //nStopType same to status : 0 normal exit, 2: Timeout & Reconnect
	void SetExitVedioThread() {::SetEvent(m_hEventVideoCtrl[0]);}
	void PauseVedioThread() {
		m_lWaitTime = INFINITE;
		::SetEvent(m_hEventVideoCtrl[1]);
	}
	//When Stop real Video play, the VideoCtrl Object maybe null some time, Thread maybe cause crash...
	//So, during stop video , we pause the thread for a while...
	void ContinueVedioThread() {
		m_lWaitTime = cnWAIT_TIME;
		::SetEvent(m_hEventVideoCtrl[1]);
	}
	long GetWaitTime() {return m_lWaitTime;}; 

	//Alarm Saving count...
	void SetAlarmVideoSavingCount(int nCount) {m_nAlarmVideoSaveCount = nCount;}
	int  GetAlarmVideoSavingCount( ) {return m_nAlarmVideoSaveCount;}
	
	HANDLE m_hEventVideoCtrl[2];

private:
	long	m_lWaitTime;
	int	   m_nAlarmVideoSaveCount;
};
