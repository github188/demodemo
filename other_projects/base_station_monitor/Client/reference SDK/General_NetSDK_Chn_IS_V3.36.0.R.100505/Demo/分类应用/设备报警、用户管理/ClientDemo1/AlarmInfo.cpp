// AlarmInfo.cpp: implementation of the CAlarmInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClientDemo1.h"
#include "AlarmInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlarmInfo::CAlarmInfo()
{
	m_DiskError = 0;
	m_DiskFull = FALSE;
	m_InputAlarm = 0;
	m_MotionDetect = 0;
	m_nAlarmInputCount = 16;
	m_nChannelCount = 16;
	m_ShelterAlarm = 0;
	m_SoundDetect =0;
	m_VideoLost =0;
	m_AlarmCommand = DH_OTHER_ALARM;
}

CAlarmInfo::~CAlarmInfo()
{

}

CAlarmInfoEx::CAlarmInfoEx()
{
	m_AlarmCommand = DH_ALARM_NONE;
	memset(m_dwAlarm,0,sizeof(BYTE)*16);
	memset(&m_stuNewSound,0,sizeof(DH_NEW_SOUND_ALARM_STATE));
	//DVRvalarm input channel amount 
	m_nAlarmInputCount =16;
	//DVR video input channel amount 
	m_nChannelCount =16;

}

CAlarmInfoEx::~CAlarmInfoEx()
{
	
}



















