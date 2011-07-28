#pragma once

#include "AlarmInfo.h"

class AlarmManager
{
public:
	CList<AlarmInfo* , AlarmInfo*> queryAlarmList, alarmList, curNewAlarmList,curRefreshAlarmList;
	CList<AlarmInfo*, AlarmInfo*>* getalarmList(CString &alarmStr , bool bAlarmQuery=false);
	BOOL AlarmManager::IsAlarmExist(CString alarmStr);

public:
	AlarmManager();
	~AlarmManager();
};