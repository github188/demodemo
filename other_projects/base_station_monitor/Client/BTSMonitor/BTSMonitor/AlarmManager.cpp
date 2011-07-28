#include "stdafx.h"
#include "AlarmManager.h"
////////////////////////////////////////////////////////////////////////////
AlarmManager::AlarmManager()
{
	alarmList.RemoveAll();
	curNewAlarmList.RemoveAll();
	curRefreshAlarmList.RemoveAll();
}
AlarmManager::~AlarmManager()
{
	BOOL bRet = FALSE;
	AlarmInfo*  pAlarmInfo = NULL;
	POSITION pos = alarmList.GetHeadPosition();
	int nAlarmCount = alarmList.GetCount();
	for (int i=0; i<nAlarmCount; i++)
	{
		pAlarmInfo = alarmList.GetNext(pos);
		if ( pAlarmInfo )
		{
			delete pAlarmInfo;
			pAlarmInfo = NULL;
		}
	}

	pos = curNewAlarmList.GetHeadPosition();
	nAlarmCount = curNewAlarmList.GetCount();
	for (int i=0; i<nAlarmCount; i++)
	{
		pAlarmInfo = curNewAlarmList.GetNext(pos);
		if ( pAlarmInfo )
		{
			delete pAlarmInfo;
			pAlarmInfo = NULL;
		}
	}


	pos = curRefreshAlarmList.GetHeadPosition();
	nAlarmCount = curRefreshAlarmList.GetCount();
	for (int i=0; i<nAlarmCount; i++)
	{
		pAlarmInfo = curRefreshAlarmList.GetNext(pos);
		if ( pAlarmInfo )
		{
			delete pAlarmInfo;
			pAlarmInfo = NULL;
		}
	}

	//queryAlarmList..
	pos = queryAlarmList.GetHeadPosition();
	nAlarmCount = queryAlarmList.GetCount();
	for (int i=0; i<nAlarmCount; i++)
	{
		pAlarmInfo = queryAlarmList.GetNext(pos);
		if ( pAlarmInfo )
		{
			delete pAlarmInfo;
			pAlarmInfo = NULL;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
CList<AlarmInfo*, AlarmInfo*>* AlarmManager::getalarmList(CString &alarmStr, bool bAlarmQuery)
{
	int ipos=0;
	int ileft=0, iright=0;
	CString alarmline;
	if (bAlarmQuery) //alarmMgr
	{
		//remove all data from the list
		BOOL bRet = FALSE;
		AlarmInfo*  pAlarmInfo = NULL;
		POSITION pos = queryAlarmList.GetHeadPosition();
		int nAlarmCount = queryAlarmList.GetCount();
		for (int i=0; i<nAlarmCount; i++)
		{
			pAlarmInfo = queryAlarmList.GetNext(pos);
			if ( pAlarmInfo )
			{
				delete pAlarmInfo;
				pAlarmInfo = NULL;
			}
		}
		queryAlarmList.RemoveAll();
		//add new query alarm to the list
		while(ipos!=1)
		{
			ipos=alarmStr.Find('\n', ileft);
			if(ipos==-1)
			{
				break;
			}
			iright=ipos;
			alarmline=alarmStr.Mid(ileft, iright-ileft+1);
			ileft=ipos+1;
			AlarmInfo* alarm=new AlarmInfo(alarmline);
			queryAlarmList.AddTail(alarm);
		}
		
		return &queryAlarmList;
	}

	//Real Alarm list
	//clear old data
	AlarmInfo* pAlarmTemp=NULL;
	POSITION pos = curNewAlarmList.GetHeadPosition();
	int nAlarmCount = curNewAlarmList.GetCount();
	for (int i=0; i<nAlarmCount; i++)
	{
		pAlarmTemp = curNewAlarmList.GetNext(pos);
		delete pAlarmTemp;
		pAlarmTemp = NULL;
	}
	curNewAlarmList.RemoveAll();

	pos = curRefreshAlarmList.GetHeadPosition();
	nAlarmCount = curRefreshAlarmList.GetCount();
	for (int i=0; i<nAlarmCount; i++)
	{
		pAlarmTemp = curRefreshAlarmList.GetNext(pos);
		delete pAlarmTemp;
		pAlarmTemp = NULL;
	}
	curRefreshAlarmList.RemoveAll();


	BOOL bHasHistoricAlarm = TRUE;
	if ( alarmList.IsEmpty() )
		bHasHistoricAlarm = FALSE;

	while(ipos!=1)
	{
		ipos=alarmStr.Find('\n', ileft);
		if(ipos==-1)
		{
			break;
		}
		iright=ipos;
		alarmline=alarmStr.Mid(ileft, iright-ileft+1);
		ileft=ipos+1;

		if (bHasHistoricAlarm)
		{
			//current alarm is new alarm , need to insert/refresh record of the listview.
			if ( IsAlarmExist(alarmline) )
			{
				//Need to refresh alarm
				AlarmInfo* alarmRefresh=new AlarmInfo(alarmline);
				curRefreshAlarmList.AddTail(alarmRefresh);				
			}
			else
			{
				//new alarm , need insert the listview
				AlarmInfo* alarm=new AlarmInfo(alarmline);
				alarmList.AddTail(alarm);

				AlarmInfo* newAlarm=new AlarmInfo(alarmline);
				curNewAlarmList.AddTail(newAlarm);
			}
		}
		else
		{
			AlarmInfo* alarm=new AlarmInfo(alarmline);
			alarmList.AddTail(alarm);

			AlarmInfo* newAlarm=new AlarmInfo(alarmline);
			curNewAlarmList.AddTail(newAlarm);
		}
	}

	return &alarmList;
}
BOOL AlarmManager::IsAlarmExist(CString alarmStr)
{
	CString sUUID;
	util::split_next(alarmStr, sUUID, '$', 0);
	if (sUUID.IsEmpty())
		return TRUE; //do not add this alarm into the listview.

	BOOL bRet = FALSE;
	AlarmInfo*  pAlarmInfo = NULL;
	POSITION pos = alarmList.GetHeadPosition();
	int nAlarmCount = alarmList.GetCount();
	for (int i=0; i<nAlarmCount; i++)
	{
		pAlarmInfo = alarmList.GetNext(pos);
		if ( pAlarmInfo->sUUID == sUUID )
		{
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}
