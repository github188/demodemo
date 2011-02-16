#include "stdafx.h"
#include "AlarmManager.h"

CList<AlarmInfo*, AlarmInfo*>* AlarmManager::getalarmList(CString &alarmStr)
{
	int ipos=0;
	int ileft=0, iright=0;
	CString alarmline;
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
		alarmList.AddTail(alarm);
	}

	return &alarmList;
}
