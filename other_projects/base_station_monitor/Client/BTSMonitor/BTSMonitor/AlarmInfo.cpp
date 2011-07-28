#include "stdafx.h"
#include "AlarmInfo.h"

void AlarmInfo::buildClass(const CString &info)
{
	const char splitch='$';
	int pos = 0;
	//pos = util::split_next(info, uuid, splitch, 0);
	pos = util::split_next(info, sUUID, splitch, 0);
	pos = util::split_next(info, alarmCode, splitch, pos + 1);
	pos = util::split_next(info, BTSID, ':', pos + 1);
	pos = util::split_next(info, ChannelID, splitch, pos + 1);
	pos = util::split_next(info, status, splitch, pos+1);
	pos = util::split_next(info, level, splitch, pos+1);
	pos = util::split_next(info, startTime, splitch, pos+1);
	pos = util::split_next(info, endTime, splitch, pos+1);
	pos = util::split_next(info, category, splitch, pos+1);
	pos = util::split_next(info, BTSType, splitch, pos+1);

}
