#pragma once

#include "AlarmInfo.h"

class AlarmManager
{
public:
	CList<AlarmInfo* , AlarmInfo*> alarmList, curNewAlarmList,curRefreshAlarmList;
	CList<AlarmInfo*, AlarmInfo*>* getalarmList(CString &alarmStr);
	BOOL AlarmManager::IsAlarmExist(CString alarmStr);
public:
	AlarmManager();
	~AlarmManager();
};