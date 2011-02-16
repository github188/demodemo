#pragma once

#include "AlarmInfo.h"

class AlarmManager
{
public:
	CList<AlarmInfo* , AlarmInfo*> alarmList;
	CList<AlarmInfo*, AlarmInfo*>* getalarmList(CString &alarmStr);
};