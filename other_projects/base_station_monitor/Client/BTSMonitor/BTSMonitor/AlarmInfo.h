#pragma once

#include "util.h"

class AlarmInfo
{
public:
	AlarmInfo()
	{
	}

	AlarmInfo(const CString &info)
	{
		buildClass(info);
	}

	void buildClass(const CString &info);

public:
	CString uuid;
	CString alarmCode;
	CString BTSID;
	CString ChannelID;
	CString status;
	CString level;
	CString startTime;
	CString endTime;
	CString category;
	CString BTSType;
	static const char splitch='$';
};