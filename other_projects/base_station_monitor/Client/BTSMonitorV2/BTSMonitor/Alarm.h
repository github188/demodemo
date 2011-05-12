#pragma once

#include "util.h"

class Alarm{
private:
	void buildClass(CString &info);

public:
	CString uuid;
	CString baseStation;
	CString alarmType;
	CString alarmStatus;
	CString user;
	CString startTime;
	CString endTime;
	CString level;
	Alarm(CString &info){
		buildClass(info);
	}
};
