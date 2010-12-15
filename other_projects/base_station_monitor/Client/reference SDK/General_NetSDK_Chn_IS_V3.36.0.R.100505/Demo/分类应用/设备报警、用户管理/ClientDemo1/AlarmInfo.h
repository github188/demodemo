// AlarmInfo.h: interface for the CAlarmInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALARMINFO_H__720FA033_A07C_4017_9730_C5CDD93D14D5__INCLUDED_)
#define AFX_ALARMINFO_H__720FA033_A07C_4017_9730_C5CDD93D14D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include"dhnetsdk.h"
//Input channel alarm 
typedef WORD                 DH_ALARM_INPUTALARM;
/*
 * DH_ALARM_INPUTALARM Commands
 */
#define DH_AI_CHANNEL0		1
#define DH_AI_CHANNEL1		2
#define DH_AI_CHANNEL2		4
#define DH_AI_CHANNEL3		8
#define DH_AI_CHANNEL4		16
#define DH_AI_CHANNEL5		32
#define DH_AI_CHANNEL6		64
#define DH_AI_CHANNEL7		128
#define DH_AI_CHANNEL8		256
#define DH_AI_CHANNEL9		512
#define DH_AI_CHANNEL10		1024
#define DH_AI_CHANNEL11		2048
#define DH_AI_CHANNEL12		4096
#define DH_AI_CHANNEL13		8192
#define DH_AI_CHANNEL14		16384
#define DH_AI_CHANNEL15		32768

//Video loss alarm 
typedef WORD                 DH_ALARM_VIDEOLOST;
/*
 * DH_ALARM_VIDEOLOST Commands
 */
#define DH_AV_CHANNEL0		1
#define DH_AV_CHANNEL1		2
#define DH_AV_CHANNEL2		4
#define DH_AV_CHANNEL3		8
#define DH_AV_CHANNEL4		16
#define DH_AV_CHANNEL5		32
#define DH_AV_CHANNEL6		64
#define DH_AV_CHANNEL7		128
#define DH_AV_CHANNEL8		256
#define DH_AV_CHANNEL9		512
#define DH_AV_CHANNEL10		1024
#define DH_AV_CHANNEL11		2048
#define DH_AV_CHANNEL12		4096
#define DH_AV_CHANNEL13		8192
#define DH_AV_CHANNEL14		16384
#define DH_AV_CHANNEL15		32768

//Motion detection alarm 
typedef WORD                 DH_ALARM_MOTION;
/*
 * DH_ALARM_MOTION Commands
 */
#define DH_AM_CHANNEL0		1
#define DH_AM_CHANNEL1		2
#define DH_AM_CHANNEL2		4
#define DH_AM_CHANNEL3		8
#define DH_AM_CHANNEL4		16
#define DH_AM_CHANNEL5		32
#define DH_AM_CHANNEL6		64
#define DH_AM_CHANNEL7		128
#define DH_AM_CHANNEL8		256
#define DH_AM_CHANNEL9		512
#define DH_AM_CHANNEL10		1024
#define DH_AM_CHANNEL11		2048
#define DH_AM_CHANNEL12		4096
#define DH_AM_CHANNEL13		8192
#define DH_AM_CHANNEL14		16384
#define DH_AM_CHANNEL15		32768

//Camera masking alarm 
typedef WORD                 DH_ALARM_SHELTER;
/*
 * DH_ALARM_SHELTER Commands
 */
#define DH_AS_CHANNEL0		1
#define DH_AS_CHANNEL1		2
#define DH_AS_CHANNEL2		4
#define DH_AS_CHANNEL3		8
#define DH_AS_CHANNEL4		16
#define DH_AS_CHANNEL5		32
#define DH_AS_CHANNEL6		64
#define DH_AS_CHANNEL7		128
#define DH_AS_CHANNEL8		256
#define DH_AS_CHANNEL9		512
#define DH_AS_CHANNEL10		1024
#define DH_AS_CHANNEL11		2048
#define DH_AS_CHANNEL12		4096
#define DH_AS_CHANNEL13		8192
#define DH_AS_CHANNEL14		16384
#define DH_AS_CHANNEL15		32768

//Audio alarm 
typedef WORD                 DH_ALARM_SOUNDDETECT;
/*
 * DH_ALARM_SOUNDDETECT Commands
 */
#define DH_SD_CHANNEL0		1
#define DH_SD_CHANNEL1		2
#define DH_SD_CHANNEL2		4
#define DH_SD_CHANNEL3		8
#define DH_SD_CHANNEL4		16
#define DH_SD_CHANNEL5		32
#define DH_SD_CHANNEL6		64
#define DH_SD_CHANNEL7		128
#define DH_SD_CHANNEL8		256
#define DH_SD_CHANNEL9		512
#define DH_SD_CHANNEL10		1024
#define DH_SD_CHANNEL11		2048
#define DH_SD_CHANNEL12		4096
#define DH_SD_CHANNEL13		8192
#define DH_SD_CHANNEL14		16384
#define DH_SD_CHANNEL15		32768

//HDD malfunction alarm 
typedef DWORD                 DH_ALARM_DISKERROR;
/*
 * DH_ALARM_DISKERROR Commands
 */
#define DH_DR_DISK0			1
#define DH_DR_DISK1			2
#define DH_DR_DISK2			4
#define DH_DR_DISK3			8
#define DH_DR_DISK4			16
#define DH_DR_DISK5			32
#define DH_DR_DISK6			64
#define DH_DR_DISK7			128
#define DH_DR_DISK8			256
#define DH_DR_DISK9			512
#define DH_DR_DISK10		1024
#define DH_DR_DISK11		2048
#define DH_DR_DISK12		4096
#define DH_DR_DISK13		8192
#define DH_DR_DISK14		16384
#define DH_DR_DISK15		32768
#define DH_DR_DISK16		65536
#define DH_DR_DISK17		131072
#define DH_DR_DISK18		262144
#define DH_DR_DISK19		524288
#define DH_DR_DISK20		1048576
#define DH_DR_DISK21		2097152
#define DH_DR_DISK22		4194304
#define DH_DR_DISK23		8388608
#define DH_DR_DISK24		16777216
#define DH_DR_DISK25		33554432
#define DH_DR_DISK26		67108864
#define DH_DR_DISK27		134217728
#define DH_DR_DISK28		268435456
#define DH_DR_DISK29		536870912
#define DH_DR_DISK30		1073741824
#define DH_DR_DISK31		2147483648

//HDD insufficient capacity alarm 
typedef BOOL                 DH_ALARM_DISKFULL;

typedef enum	{
	//DH_INPUT_ALARM,
	//DH_VIDEOLOST_ALARM,
	DH_COMMON_ALARM,
	DH_VIDEOSHELTER_ALARM,
	//DH_MOTIONDETECT_ALARM,
	DH_SOUNDDETECT_ALARM,
	DH_DISKFULL_ALARM,
	DH_DISKERROR_ALARM,
	DH_OTHER_ALARM,
}EN_DVR_ALARMTYPE;
//New alarm protocol type 
typedef enum 
{
	DH_ALARM_ALARMEX,//External alarm 
	DH_MOTIONDETECT_ALARMEX,//Motion detection alarm 
	DH_VEDIOSHELTER_ALARMEX,//Video loss alarm 
	DH_SHELTER_ALARMEX,//Camera masking alarm 
	DH_SOUNDDETECT_ALARMEX,//Audio detectino alarm
	DH_DISKFULL_ALARMEX,//HDD full alarm 
	DH_DISKERROR_ALARMEX,//HDD malfunction alarm 
	DH_ENCODER_ALARMEX,//Encoder alarm 
	DH_URGENCY_ALARMEX,//Emergency alarm 
	DH_WIRELESS_ALARMEX,//Wireless alarm 
	DH_ALARM_DECODER_ALARMEX,//Alarm decoder alarm 
	DH_ALARM_NONE//No alarm. To initialize.
}EN_DVR_ALARMTYPE_EX;


class CAlarmInfo  
{
// Construction
public:
	CAlarmInfo();
	virtual ~CAlarmInfo();

public:
	//Alarm type 
	EN_DVR_ALARMTYPE m_AlarmCommand;
	//HDD insufficient capacity alarm 
	DH_ALARM_DISKFULL m_DiskFull;
	//HDD malfunction alarm 
	DH_ALARM_DISKERROR m_DiskError;
	//Audio alarm 
	DH_ALARM_SOUNDDETECT m_SoundDetect;
	//Camera masking alarm 
	DH_ALARM_SHELTER m_ShelterAlarm;
	//Motion detection alarm 
	DH_ALARM_MOTION m_MotionDetect;
	//Video loass alarm 
	DH_ALARM_VIDEOLOST m_VideoLost;
	//Input channel alarm 
	DH_ALARM_INPUTALARM m_InputAlarm;
	//DVR alarm input channel amount 
	int m_nAlarmInputCount;
	//DVR video input channel amount 
	int m_nChannelCount;

};
class CAlarmInfoEx
{
public:
	CAlarmInfoEx();
	virtual ~CAlarmInfoEx();
public:
	//Alarm type 
	EN_DVR_ALARMTYPE_EX		m_AlarmCommand;
	BYTE					m_dwAlarm[16];
	DH_NEW_SOUND_ALARM_STATE	m_stuNewSound;
	ALARM_DECODER_ALARM		m_stuAlarmDecoderAlarm;
	//DVR alarm input channel amount 
	int m_nAlarmInputCount;
	//DVR video input channel amount 
	int m_nChannelCount;
};
#endif // !defined(AFX_ALARMINFO_H__720FA033_A07C_4017_9730_C5CDD93D14D5__INCLUDED_)





















