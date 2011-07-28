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
	CString sUUID;		//
	CString alarmCode;  //1:��ǰʵʱ�澯, ���ڷ����ĸ澯;2:��ʷ�澯��¼
	CString BTSID;		//
	CString ChannelID;
	CString status;		//1:δ���� 2:�澯��ʱ�Զ�����  3:�ֶ�ȷ��
	CString level;		//1��2��3��4��5���澯�������Ҫ�ȿͻ�������ʱ����5��Ϊ���ظ澯��
	CString startTime;
	CString endTime;
	CString category;	//1.��Ƶ 2.ͼƬ, 3.��
	CString BTSType;	//�˾����ͣ����ڻ�û�кͿͻ�ȷ�ϣ�ֱ����ʾһ�������������


};