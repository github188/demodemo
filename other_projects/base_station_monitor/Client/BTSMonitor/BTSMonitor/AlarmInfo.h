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
	CString uuid;		//
	CString alarmCode;  //1:当前实时告警, 正在发生的告警;2:历史告警记录
	CString BTSID;		//
	CString ChannelID;
	CString status;		//1:未处理 2:告警超时自动处理  3:手动确认
	CString level;		//1，2，3，4，5，告警级别，这个要等客户定。暂时定义5级为严重告警吧
	CString startTime;
	CString endTime;
	CString category;	//1.视频 2.图片, 3.无
	CString BTSType;	//端局类型，现在还没有和客户确认，直接显示一个编码就行啦。
	static const char splitch='$';

};