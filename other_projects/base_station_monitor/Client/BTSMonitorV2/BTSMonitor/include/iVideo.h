/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * iVideo.h - E:\Project\resource\component\DevSDK\����\iVideo\
 *
 * Copyright (C) 2010 QiYang Technologies, All Rights Reserved.
 *
 * $Id: iVideo.h 0001 2009-09-29 11:50:17 WuJunjie Exp $
 *
 * explain
 *     �豸SDK�ӿ�
 *
 * Update:
 *     2009-09-29 11:50 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef __IVIDEO_H__
#define __IVIDEO_H__

#ifdef WIN32

#ifdef IVIDEO_EXPORTS
#define CLIENT_API  __declspec(dllexport) 
#else	//#ifdef IVIDEO_EXPORTS
#define CLIENT_API  __declspec(dllimport)   //VC ��
#endif

#define CALLBACK __stdcall
#define CALL_METHOD  __stdcall  //__cdecl

#else	//linux

#define CLIENT_API	extern "C"
#define CALL_METHOD
#define CALLBACK

//#define RELEASE_HEADER	//����ͷ�ļ�
#ifdef RELEASE_HEADER

#define WORD	unsigned short
#define DWORD	unsigned long
#define LPDWORD	DWORD*
#define BOOL	unsigned long
#define TRUE	0x01
#define FALSE	0x00
#define BYTE	unsigned char
#define LONG	long
#define UINT	unsigned int
#define HDC		void*
#define HWND	void*
#define LPVOID	void*
#define NULL	0
typedef struct  tagRECT
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
}RECT;
#else	//�ڲ�����
#include "../netsdk/osIndependent.h"
#endif

#endif


#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
//	��ѯ����
typedef enum
{
	ABILITY_WATERMARK_CFG = 17,			//ˮӡ��������
	ABILITY_WIRELESS_CFG = 18,			//wireless��������
	ABILITY_DEVALL_INFO = 26,			//�豸�������б�
	ABILITY_CARD_QUERY = 0x0100,		//���Ų�ѯ����
	ABILITY_MULTIPLAY = 0x0101,			//�໭��Ԥ������
	ABILITY_INFRARED = 0X0121,			//���߱�������
} DH_SYS_ABILITY;

//////////////////////////////////////////////////////////////////////////
//	�豸֧�ֹ����б�
enum 
{
	EN_FTP = 0,							//FTP ��λ��1������¼���ļ� 2������ץͼ�ļ�
	EN_SMTP,							//SMTP ��λ��1�����������ı��ʼ� 2����������ͼƬ
	EN_NTP,								//NTP	��λ��1������ϵͳʱ��
	EN_AUTO_MAINTAIN,					//�Զ�ά�� ��λ��1������ 2���ر� 3:ɾ���ļ�
	EN_VIDEO_COVER,						//�����ڵ� ��λ��1���������ڵ�
	EN_AUTO_REGISTER,					//����ע��	��λ��1��ע���sdk������½
	EN_DHCP,							//DHCP	��λ��1��DHCP
	EN_UPNP,							//UPNP	��λ��1��UPNP
	EN_COMM_SNIFFER,					//����ץ�� ��λ��1:CommATM
	EN_NET_SNIFFER,						//����ץ�� ��λ�� 1��NetSniffer
	EN_BURN,							//��¼���� ��λ��1����ѯ��¼״̬
	EN_VIDEO_MATRIX,					//��Ƶ���� ��λ��1���Ƿ�֧����Ƶ����
	EN_AUDIO_DETECT,					//��Ƶ��� ��λ��1���Ƿ�֧����Ƶ���
	EN_STORAGE_STATION,					//�洢λ�� ��λ��1��Ftp������(Ips) 2��U�� 3��NFS 4��DISK 5��SBM
	EN_IPSSEARCH,						//IPS�洢��ѯ ��λ��1��IPS�洢��ѯ	
	EN_SNAP,							//ץͼ  ��λ��1���ֱ���2��֡��3��ץͼ��ʽ4��ץͼ�ļ���ʽ5��ͼ������
	EN_DEFAULTNIC,						//֧��Ĭ��������ѯ ��λ 1��֧��
	EN_SHOWQUALITY,						//CBRģʽ����ʾ���������� ��λ 1:֧��
};

typedef struct 
{
	DWORD IsFucEnable[512];				//�����б�������,�±��Ӧ������ö��ֵ,��λ��ʾ�ӹ���
} DH_DEV_ENABLE_INFO;

//////////////////////////////////////////////////////////////////////////
//	���Ų�ѯ�����ṹ��
typedef struct 
{
	char		IsCardQueryEnable;
	char		iRev[3];
} DH_CARD_QUERY_EN;

//////////////////////////////////////////////////////////////////////////
//	wireless�����ṹ��
typedef struct 
{
	char		IsWirelessEnable;
	char		iRev[3];
} DH_WIRELESS_EN;

//////////////////////////////////////////////////////////////////////////
//	ͼ��ˮӡ�����ṹ��
typedef struct 
{
	char		isSupportWM;	//1 ֧�֣�0 ��֧��
	char		supportWhat;	//0������ˮӡ��1��ͼƬˮӡ��2��ͬʱ֧������ˮӡ��ͼƬˮӡ
	char		reserved[2];
} DH_WATERMAKE_EN;

//////////////////////////////////////////////////////////////////////////
//	�໭��Ԥ�������ṹ��
typedef struct  
{
	int			nEnable;			//1 ֧�֣�0 ��֧��
	DWORD		dwMultiPlayMask;	//����
	char		reserved[4];
} DH_MULTIPLAY_EN;

typedef struct  
{
	BOOL bSupport;//�Ƿ�֧��
	int	 nAlarmInCount;//�������
	int  nAlarmOutCount;//�������
	int  nRemoteAddrCount;//ң��������
	BYTE reserved[32];
}DH_WIRELESS_ALARM_INFO;

/************************************************************************
 ** ��������
 ***********************************************************************/
#define DH_SERIALNO_LEN 		48		//�豸���к��ַ�����
#define DH_MAX_DISKNUM 			32		//���Ӳ�̸���
#define DH_MAX_SDCARDNUM		32		//���SD������
#define DH_MAX_BURNING_DEV_NUM	32		//����¼�豸����
#define DH_BURNING_DEV_NAMELEN	32		//��¼�豸������󳤶�
#define DH_MAX_LINK 			6		
#define DH_MAX_CHANNUM 			16		//���ͨ������
#define DH_MAX_ALARMIN 			128		//��󱨾��������
#define DH_MAX_ALARMOUT 		64		//��󱨾��������

#define DH_MAX_RIGHT_NUM		100		//�û�Ȩ�޸�������
#define DH_MAX_GROUP_NUM		20		//�û����������
#define DH_MAX_USER_NUM			200		//�û���������
#define DH_RIGHT_NAME_LENGTH	32		//Ȩ��������
#define DH_USER_NAME_LENGTH		8		//�û�������
#define DH_USER_PSW_LENGTH		8		//�û����볤��
#define DH_MEMO_LENGTH			32		//��ע����

#define DH_MAX_STRING_LEN		128

//�����豸�ص�����
#define DH_DVR_SERIAL_RETURN 1   //�豸�������кŻص�
#define DH_DVR_DISCONNECT -1      //��֤�ڼ��豸���߻ص�

//ͨ�������ַ�������س���
#define MAX_STRING_LINE_LEN	6		//�������
#define MAX_PER_STRING_LEN  20		//ÿ����󳤶�

//�ص�������Ϣ���ͣ���ӦCLIENT_StartListen
#define DH_COMM_ALARM			0x1100	//���汨����Ϣ(�����ⲿ��������Ƶ��ʧ����̬���)
#define DH_SHELTER_ALARM		0x1101	//��Ƶ�ڵ�����
#define DH_DISK_FULL_ALARM		0x1102	//Ӳ��������
#define DH_DISK_ERROR_ALARM		0x1103	//Ӳ�̹��ϱ���
#define DH_SOUND_DETECT_ALARM	0x1104	//��Ƶ��ⱨ��
#define DH_ALARM_DECODER_ALARM	0x1105	//��������������

//��չ�ص�������Ϣ���ͣ���ӦCLIENT_StartListenEx
//������������ϸ�ֿ�
#define DH_ALARM_ALARM_EX			0x2101	//�ⲿ����
#define DH_MOTION_ALARM_EX			0x2102	//��̬��ⱨ��
#define DH_VIDEOLOST_ALARM_EX		0x2103	//��Ƶ��ʧ����
#define DH_SHELTER_ALARM_EX			0x2104	//�ڵ�����
#define DH_SOUND_DETECT_ALARM_EX	0x2105	//��Ƶ��ⱨ��
#define DH_DISKFULL_ALARM_EX		0x2106	//Ӳ��������
#define DH_DISKERROR_ALARM_EX		0x2107	//��Ӳ�̱���
#define DH_ENCODER_ALARM_EX			0x210A	//����������
#define DH_URGENCY_ALARM_EX			0x210B	//��������
#define DH_WIRELESS_ALARM_EX		0x210C	//���߱���
#define DH_NEW_SOUND_DETECT_ALARM_EX 0x210D //����Ƶ��ⱨ��������Ϣ�Ľṹ���DH_NEW_SOUND_ALARM_STATE;

#define DH_CONFIG_RESULT_EVENT_EX	0X3000  //�޸����õķ����뷵�ؽṹ��DEV_SET_RESULT
#define DH_REBOOT_EVENT_EX			0x3001	//�豸�����¼�
#define DH_AUTO_TALK_START_EX		0x3002  //�豸������ʼ�����Խ�
#define DH_AUTO_TALK_STOP_EX		0x3003  //�豸����ֹͣ�����Խ�

//��ѯ�豸״̬�ӿڲ�������ѯ���� ����CLIENT_QueryDevState����
#define DH_DEVSTATE_COMM_ALARM		0x0001	//��ѯ��ͨ����(�����ⲿ��������Ƶ��ʧ����̬���)
#define DH_DEVSTATE_SHELTER_ALARM	0x0002	//��ѯ�ڵ�����
#define DH_DEVSTATE_RECORDING		0x0003	//��ѯ¼��״̬
#define DH_DEVSTATE_DISK			0x0004	//��ѯӲ��״̬
#define DH_DEVSTATE_RESOURCE		0x0005	//ϵͳ��Դ״̬
#define DH_DEVSTATE_BITRATE			0x0006	//��ȡͨ������
#define DH_DEVSTATE_CONN			0x0007	//��ȡ�豸����״̬
#define DH_DEVSTATE_PROTOCAL_VER	0x0008	//��ȡ����Э��汾��,	pBuf = int*
#define DH_DEVSTATE_TALK_ECTYPE		0x0009	//��ȡ�豸֧�ֵ������Խ���ʽ�б����ṹ��DHDEV_TALKFORMAT_LIST
#define DH_DEVSTATE_SD_CARD			0x000A	//��ȡSD����Ϣ��IPC���Ʒ��
#define DH_DEVSTATE_BURNING_DEV			0x000B	//��ѯ��¼����Ϣ
#define DH_DEVSTATE_BURNING_PROGRESS	0x000C	//��ѯ��¼����
#define DH_DEVSTATE_PLATFORM		0x000D	//��ȡ�豸֧�ֵĽ���ƽ̨
#define DH_DEVSTATE_CAMERA			0x000E	//��ȡ����ͷ������Ϣ��pBuf = DHDEV_CAMERA_INFO *�������ж���ṹ��
#define DH_DEVSTATE_SOFTWARE		0x000F	//�豸����汾��Ϣ
#define DH_DEVSTATE_LANGUAGE        0x0010  //�豸֧�ֵ���������
#define DH_DEVSTATE_DSP				0x0011	//DSP��������
#define	DH_DEVSTATE_OEM				0x0012	//OEM
#define	DH_DEVSTATE_NET				0x0013  //��������״̬��Ϣ
#define DH_DEVSTATE_TYPE			0x0014  //�豸����
#define DH_DEVSTATE_SNAP			0x0015	 //ץͼ����������ѯ
//ץͼ��Ƶ��������
#define CODETYPE_MPEG4   0
#define CODETYPE_H264    1
#define CODETYPE_JPG     2

//�������ʹ��ţ�CLIENT_GetLastError�����ķ���
#define _EC(x)						(0x80000000|x)
#define NET_NOERROR 				0				//û�д���
#define NET_ERROR					-1				//δ֪����
#define NET_SYSTEM_ERROR			_EC(1)			//Windowsϵͳ����
#define NET_NETWORK_ERROR			_EC(2)			//������󣬿�������Ϊ���糬ʱ
#define NET_DEV_VER_NOMATCH			_EC(3)			//�豸Э�鲻ƥ��
#define NET_INVALID_HANDLE			_EC(4)			//�����Ч
#define NET_OPEN_CHANNEL_ERROR		_EC(5)			//��ͨ��ʧ��
#define NET_CLOSE_CHANNEL_ERROR		_EC(6)			//�ر�ͨ��ʧ��
#define NET_ILLEGAL_PARAM			_EC(7)			//�û��������Ϸ�
#define NET_SDK_INIT_ERROR			_EC(8)			//SDK��ʼ������
#define NET_SDK_UNINIT_ERROR		_EC(9)			//SDK�������
#define NET_RENDER_OPEN_ERROR		_EC(10)			//����render��Դ����
#define NET_DEC_OPEN_ERROR			_EC(11)			//�򿪽�������
#define NET_DEC_CLOSE_ERROR			_EC(12)			//�رս�������
#define NET_MULTIPLAY_NOCHANNEL		_EC(13)			//�໭��Ԥ���м�⵽ͨ����Ϊ0
#define NET_TALK_INIT_ERROR			_EC(14)			//¼�����ʼ��ʧ��
#define NET_TALK_NOT_INIT			_EC(15)			//¼����δ����ʼ��
#define	NET_TALK_SENDDATA_ERROR		_EC(16)			//������Ƶ���ݳ���
#define NET_REAL_ALREADY_SAVING		_EC(17)			//ʵʱ�����Ѿ����ڱ���״̬
#define NET_NOT_SAVING				_EC(18)			//δ����ʵʱ����
#define NET_OPEN_FILE_ERROR			_EC(19)			//���ļ�����
#define NET_PTZ_SET_TIMER_ERROR		_EC(20)			//������̨���ƶ�ʱ��ʧ��
#define NET_RETURN_DATA_ERROR		_EC(21)			//�Է������ݵ�У�����
#define NET_INSUFFICIENT_BUFFER		_EC(22)			//û���㹻�Ļ���
#define NET_NOT_SUPPORTED			_EC(23)			//��ǰSDKδ֧�ָù���
#define NET_NO_RECORD_FOUND			_EC(24)			//��ѯ����¼��
#define NET_NOT_AUTHORIZED			_EC(25)			//�޲���Ȩ��
#define NET_NOT_NOW					_EC(26)			//��ʱ�޷�ִ��
#define NET_NO_TALK_CHANNEL			_EC(27)			//δ���ֶԽ�ͨ��
#define NET_NO_AUDIO				_EC(28)			//δ������Ƶ
#define NET_NO_INIT					_EC(29)			//CLientSDKδ����ʼ��
#define NET_DOWNLOAD_END			_EC(30)			//�����ѽ���
#define NET_EMPTY_LIST				_EC(31)			//��ѯ���Ϊ��
 
#define NET_ERROR_GETCFG_SYSATTR	_EC(32)			//��ȡ����ʧ��λ�ã�ϵͳ����
#define NET_ERROR_GETCFG_SERIAL		_EC(33)			//��ȡ����ʧ��λ�ã����к�
#define NET_ERROR_GETCFG_GENERAL	_EC(34)			//��ȡ����ʧ��λ�ã���������
#define NET_ERROR_GETCFG_DSPCAP		_EC(35)			//��ȡ����ʧ��λ�ã�DSP��������
#define NET_ERROR_GETCFG_NETCFG		_EC(36)			//��ȡ����ʧ��λ�ã���������
#define NET_ERROR_GETCFG_CHANNAME	_EC(37)			//��ȡ����ʧ��λ�ã�ͨ������
#define NET_ERROR_GETCFG_VIDEO		_EC(38)			//��ȡ����ʧ��λ�ã���Ƶ����
#define NET_ERROR_GETCFG_RECORD		_EC(39)			//��ȡ����ʧ��λ�ã�¼��ʱ����
#define NET_ERROR_GETCFG_PRONAME	_EC(40)			//��ȡ����ʧ��λ�ã�������Э������
#define NET_ERROR_GETCFG_FUNCNAME	_EC(41)			//��ȡ����ʧ��λ�ã�232���ڹ�������
#define NET_ERROR_GETCFG_485DECODER	_EC(42)			//��ȡ����ʧ��λ�ã�����������
#define NET_ERROR_GETCFG_232COM		_EC(43)			//��ȡ����ʧ��λ�ã�232��������
#define NET_ERROR_GETCFG_ALARMIN	_EC(44)			//��ȡ����ʧ��λ�ã��ⲿ������������
#define NET_ERROR_GETCFG_ALARMDET	_EC(45)			//��ȡ����ʧ��λ�ã�ͼ���ⱨ������
#define NET_ERROR_GETCFG_SYSTIME	_EC(46)			//��ȡ����ʧ��λ�ã��豸ʱ��
#define NET_ERROR_GETCFG_PREVIEW	_EC(47)			//��ȡ����ʧ��λ�ã�Ԥ������
#define NET_ERROR_GETCFG_AUTOMT		_EC(48)			//��ȡ����ʧ��λ�ã��Զ�ά������
#define NET_ERROR_GETCFG_VIDEOMTRX	_EC(49)			//��ȡ����ʧ��λ�ã���Ƶ��������
#define NET_ERROR_GETCFG_COVER		_EC(50)			//��ȡ����ʧ��λ�ã���Ƶ�����ڵ�
#define NET_ERROR_GETCFG_WATERMAKE	_EC(51)			//��ȡ����ʧ��λ�ã�ͼ��ˮӡ����
#define NET_ERROR_GETCFG_ETHERNET	_EC(300)		//��ȡ����ʧ��λ�ã���������
#define NET_ERROR_GETCFG_WLAN		_EC(301)		//��ȡ����ʧ��λ�ã�����������Ϣ
#define NET_ERROR_GETCFG_WLANDEV	_EC(302)		//��ȡ����ʧ��λ�ã��������������豸
#define NET_ERROR_GETCFG_REGISTER	_EC(303)		//��ȡ����ʧ��λ�ã�����ע���������
#define NET_ERROR_GETCFG_CAMERA		_EC(304)		//��ȡ����ʧ��λ�ã�����ͷ��������
#define NET_ERROR_GETCFG_INFRARED	_EC(305)		//��ȡ����ʧ��λ�ã����ⱨ������
#define NET_ERROR_GETCFG_SOUNDALARM	_EC(306)		//��ȡ����ʧ��λ�ã���Ƶ��������
#define NET_ERROR_GETCFG_STORAGE    _EC(307)		//��ȡ����ʧ��λ�ã��洢λ������
#define NET_ERROR_GETCFG_MAIL		_EC(308)		//��ȡ�ʼ�����ʧ��


#define NET_ERROR_SETCFG_GENERAL	_EC(55)			//��������ʧ��λ�ã���������
#define NET_ERROR_SETCFG_NETCFG		_EC(56)			//��������ʧ��λ�ã���������
#define NET_ERROR_SETCFG_CHANNAME	_EC(57)			//��������ʧ��λ�ã�ͨ������
#define NET_ERROR_SETCFG_VIDEO		_EC(58)			//��������ʧ��λ�ã���Ƶ����
#define NET_ERROR_SETCFG_RECORD		_EC(59)			//��������ʧ��λ�ã�¼��ʱ����
#define NET_ERROR_SETCFG_485DECODER	_EC(60)			//��������ʧ��λ�ã�����������
#define NET_ERROR_SETCFG_232COM		_EC(61)			//��������ʧ��λ�ã�232��������
#define NET_ERROR_SETCFG_ALARMIN	_EC(62)			//��������ʧ��λ�ã��ⲿ������������
#define NET_ERROR_SETCFG_ALARMDET	_EC(63)			//��������ʧ��λ�ã�ͼ���ⱨ������
#define NET_ERROR_SETCFG_SYSTIME	_EC(64)			//��������ʧ��λ�ã��豸ʱ��
#define NET_ERROR_SETCFG_PREVIEW	_EC(65)			//��������ʧ��λ�ã�Ԥ������
#define NET_ERROR_SETCFG_AUTOMT		_EC(66)			//��������ʧ��λ�ã��Զ�ά������
#define NET_ERROR_SETCFG_VIDEOMTRX	_EC(67)			//��������ʧ��λ�ã���Ƶ��������
#define NET_ERROR_SETCFG_COVER		_EC(69)			//��������ʧ��λ�ã���Ƶ�����ڵ�
#define NET_ERROR_SETCFG_WATERMAKE	_EC(60)			//��������ʧ��λ�ã�ͼ��ˮӡ����
#define NET_ERROR_SETCFG_WLAN		_EC(61)			//��������ʧ��λ�ã�����������Ϣ
#define NET_ERROR_SETCFG_WLANDEV	_EC(62)			//��������ʧ��λ�ã�ѡ�����������豸
#define NET_ERROR_SETCFG_REGISTER	_EC(63)			//��������ʧ��λ�ã�����ע���������
#define NET_ERROR_SETCFG_CAMERA		_EC(64)			//��������ʧ��λ�ã�����ͷ��������
#define NET_ERROR_SETCFG_INFRARED	_EC(65)			//��������ʧ��λ�ã����ⱨ������
#define NET_ERROR_SETCFG_SOUNDALARM	_EC(66)			//��������ʧ��λ�ã���Ƶ��������
#define NET_ERROR_SETCFG_STORAGE    _EC(67)			//��������ʧ��λ�ã��洢λ������

#define NET_AUDIOENCODE_NOTINIT		_EC(70)			//��Ƶ����ӿ�û�гɹ���ʼ��
#define NET_DATA_TOOLONGH			_EC(71)			//���ݹ���

#define NET_UNSUPPORTED				_EC(72)			//�豸��֧�ָò���
#define NET_DEVICE_BUSY				_EC(73)			//�豸��Դ����
#define NET_SERVER_STARTED			_EC(74)			//�������Ѿ�����
#define NET_SERVER_STOPPED			_EC(75)			//��������δ�ɹ�����

#define NET_LISTER_INCORRECT_SERIAL	_EC(80)			//�������к�����
#define NET_QUERY_DISKINFO_FAILED	_EC(81)			//��ȡӲ����Ϣʧ��

#define NET_LOGIN_ERROR_PASSWORD	_EC(100)		//���벻��ȷ
#define NET_LOGIN_ERROR_USER		_EC(101)		//�ʻ�������
#define NET_LOGIN_ERROR_TIMEOUT		_EC(102)		//�ȴ���¼���س�ʱ
#define NET_LOGIN_ERROR_RELOGGIN	_EC(103)		//�ʺ��ѵ�¼
#define NET_LOGIN_ERROR_LOCKED		_EC(104)		//�ʺ��ѱ�����
#define NET_LOGIN_ERROR_BLACKLIST	_EC(105)		//�ʺ��ѱ���Ϊ������
#define NET_LOGIN_ERROR_BUSY		_EC(106)		//��Դ���㣬ϵͳæ
#define NET_LOGIN_ERROR_CONNECT		_EC(107)		//��������ʧ��"
#define NET_LOGIN_ERROR_NETWORK		_EC(108)		//��������ʧ��"

#define NET_RENDER_SOUND_ON_ERROR	_EC(120)		//Render�����Ƶ����
#define NET_RENDER_SOUND_OFF_ERROR	_EC(121)		//Render��ر���Ƶ����
#define NET_RENDER_SET_VOLUME_ERROR	_EC(122)		//Render�������������
#define NET_RENDER_ADJUST_ERROR		_EC(123)		//Render�����û����������
#define NET_RENDER_PAUSE_ERROR		_EC(124)		//Render����ͣ���ų���
#define NET_RENDER_SNAP_ERROR		_EC(125)		//Render��ץͼ����
#define NET_RENDER_STEP_ERROR		_EC(126)		//Render�ⲽ������
#define NET_RENDER_FRAMERATE_ERROR	_EC(127)		//Render������֡�ʳ���

#define NET_GROUP_EXIST				_EC(140)		//�����Ѵ���
#define	NET_GROUP_NOEXIST			_EC(141)		//����������
#define NET_GROUP_RIGHTOVER			_EC(142)		//���Ȩ�޳���Ȩ���б�Χ
#define NET_GROUP_HAVEUSER			_EC(143)		//�������û�������ɾ��
#define NET_GROUP_RIGHTUSE			_EC(144)		//���ĳ��Ȩ�ޱ��û�ʹ�ã����ܳ���
#define NET_GROUP_SAMENAME			_EC(145)		//������ͬ���������ظ�
#define	NET_USER_EXIST				_EC(146)		//�û��Ѵ���
#define NET_USER_NOEXIST			_EC(147)		//�û�������
#define NET_USER_RIGHTOVER			_EC(148)		//�û�Ȩ�޳�����Ȩ��
#define NET_USER_PWD				_EC(149)		//�����ʺţ��������޸�����
#define NET_USER_FLASEPWD			_EC(150)		//���벻��ȷ
#define NET_USER_NOMATCHING			_EC(151)		//���벻ƥ��


#define NET_CONFIG_DEVBUSY			_EC(999)		//��ʱ�޷�����
#define NET_CONFIG_DATAILLEGAL		_EC(1000)		//�������ݲ��Ϸ�

//Զ�����ýӿ�CLIENT_GetDEVConfig,CLIENT_GetDEVConfig�����������
#define DH_DEV_DEVICECFG			1		//��ȡ�豸����
#define DH_DEV_NETCFG				2		//��ȡ�������
#define DH_DEV_CHANNELCFG			3		//��ȡͨ�����ã�ͼ��ѹ��������
#define DH_DEV_PREVIEWCFG 			4		//��ȡԤ������
#define DH_DEV_RECORDCFG			5		//��ȡ¼��ʱ�����
#define DH_DEV_COMMCFG				6		//��ȡ���ڲ���
#define DH_DEV_ALARMCFG 			7		//��ȡ��������
#define DH_DEV_TIMECFG 				8		//��ȡDVRʱ��
#define DH_DEV_TALKCFG				9		//�Խ�����
#define DH_DEV_AUTOMTCFG			10		//�Զ�ά������		
#define	DH_DEV_VEDIO_MARTIX			11		//����������Ʋ�������	
#define DH_DEV_MULTI_DDNS			12		//��ddns����������
#define DH_DEV_SNAP_CFG				13		//ץͼ�������
#define DH_DEV_WEB_URL_CFG			14		//HTTP·������
#define DH_DEV_FTP_PROTO_CFG		15		//FTP�ϴ�����
#define DH_DEV_INTERVIDEO_CFG		16		//ƽ̨�������ã���ʱchannel��������ƽ̨���ͣ�
											//channel=4  ��������������
											//channel=10	 ����������ά
											//channel=11 ����U��ͨ
#define DH_DEV_VIDEO_COVER			17		//�����ڵ�����
#define DH_DEV_TRANS_STRATEGY		18		//����������ã���������\����������
#define DH_DEV_DOWNLOAD_STRATEGY	19		//¼�����ز������ã���������\��ͨ����
#define DH_DEV_WATERMAKE_CFG		20		//ͼ��ˮӡ����
#define DH_DEV_WLAN_CFG				21		//������������
#define DH_DEV_WLAN_DEVICE_CFG		22		//���������豸����
#define DH_DEV_REGISTER_CFG			23		//����ע���������
#define DH_DEV_CAMERA_CFG			24		//����ͷ��������
#define DH_DEV_INFRARED_CFG 		25		//��ȡ���ⱨ������
#define DH_DEV_SNIFFER_CFG			26		//Snifferץ������
#define DH_DEV_MAIL_CFG				27		//�ʼ�����,�����Ժ���ô���������
#define DH_DEV_DNS_CFG				28		//DNS����������
#define DH_DEV_NTP_CFG				29		//NTP����
#define DH_DEV_AUDIO_DETECT_CFG		30		//��Ƶ�������
#define DH_DEV_STORAGE_STATION_CFG  31      //�洢λ������
#define DH_DEV_PTZ_OPT_CFG			32		//��̨��������


#define DH_DEV_USER_END_CFG			1000

/////////////////////Զ�����ýṹ����س���///////////////////////
#define DH_MAX_MAIL_ADDR_LEN		128	//�ʼ���(��)�˵�ַ��󳤶�
#define DH_MAX_MAIL_SUBJECT_LEN		64	//�ʼ�������󳤶�
#define DH_MAX_IPADDR_LEN			16	//IP��ַ�ַ�������
#define DH_MACADDR_LEN				40	//MAC��ַ�ַ�������
#define DH_MAX_URL_LEN				128	//URL�ַ�������
#define DH_MAX_DEV_ID_LEN			48	//���������󳤶�
#define	DH_MAX_HOST_NAMELEN			64  //32	//�������ַ������ȣ�
#define DH_MAX_HOST_PSWLEN			32	//����
#define DH_MAX_NAME_LEN				16	//ͨ�������ַ�������
#define DH_MAX_ETHERNET_NUM			2	//��̫����������
#define	DH_DEV_SERIALNO_LEN			48	//���к��ַ�������
#define DH_DEV_TYPE_LEN				32	//�豸�����ַ�������
#define DH_N_WEEKS					7	//һ�ܵ�����	
#define DH_N_TSECT					6	//ͨ��ʱ��θ���
#define DH_N_REC_TSECT				6	//¼��ʱ��θ���
#define DH_N_COL_TSECT				2	//��ɫʱ��θ���	
#define DH_CHAN_NAME_LEN			32	//ͨ��������,dvr DSP�������ƣ����32�ֽ�		
#define DH_N_ENCODE_AUX				3	//��չ��������	
#define DH_N_TALK					1	//���Խ�ͨ������
#define DH_N_COVERS					1	//�ڵ��������	
#define DH_N_CHANNEL				16	//���ͨ������	
#define DH_N_ALARM_TSECT			2	//������ʾʱ��θ���
#define DH_MAX_ALARMOUT_NUM			16	//��������ڸ�������
#define DH_MAX_VIDEO_IN_NUM			16	//��Ƶ����ڸ�������
#define DH_MAX_ALARM_IN_NUM			16	//��������ڸ�������
#define DH_MAX_DISK_NUM				16	//Ӳ�̸������ޣ��ݶ�Ϊ16
#define DH_MAX_DECODER_NUM			16	//��������485����������	
#define DH_MAX_232FUNCS				10	//232���ڹ��ܸ�������
#define DH_MAX_232_NUM				2	//232���ڸ�������
#define DH_MAX_DECPRO_LIST_SIZE		100	//������Э���б��������
#define DH_FTP_MAXDIRLEN			240	//FTP�ļ�Ŀ¼��󳤶�
#define DH_MATRIX_MAXOUT			16	//���������������
#define DH_TOUR_GROUP_NUM			6	//���������������
#define DH_MAX_DDNS_NUM				10	//�豸֧�ֵ�ddns������������
#define DH_MAX_SERVER_TYPE_LEN		32	//ddns���������� ����ַ�������
#define DH_MAX_DOMAIN_NAME_LEN		256	//ddns���� ����ַ�������
#define DH_MAX_DDNS_ALIAS_LEN		32	//ddns���������� ����ַ�������
#define DH_MOTION_ROW				32	//��̬������������
#define DH_MOTION_COL				32	//��̬������������
#define	DH_FTP_USERNAME_LEN			64  //FTP���ã��û�����󳤶�
#define	DH_FTP_PASSWORD_LEN			64  //FTP���ã�������󳤶�
#define	DH_TIME_SECTION				2	//FTP���ã�ÿ��ʱ��θ���
#define DH_FTP_MAX_PATH				240	//FTP���ã��ļ�·������󳤶�
#define DH_INTERVIDEO_UCOM_CHANID	32	//ƽ̨�������ã�U��ͨͨ��ID
#define DH_INTERVIDEO_UCOM_DEVID	32	//ƽ̨�������ã�U��ͨ�豸ID
#define DH_INTERVIDEO_UCOM_REGPSW	16	//ƽ̨�������ã�U��ͨע������
#define DH_INTERVIDEO_UCOM_USERNAME	32	//ƽ̨�������ã�U��ͨ�û���
#define DH_INTERVIDEO_UCOM_USERPSW	32	//ƽ̨�������ã�U��ͨ����
#define DH_INTERVIDEO_NSS_IP		32	//ƽ̨�������ã�������άIP
#define DH_INTERVIDEO_NSS_SERIAL	32	//ƽ̨�������ã�������άserial
#define DH_INTERVIDEO_NSS_USER		32	//ƽ̨�������ã�������άuser
#define DH_INTERVIDEO_NSS_PWD		50	//ƽ̨�������ã�������άpassword

#define DH_MAX_VIDEO_COVER_NUM		16	//�ڵ�����������
#define DH_MAX_WATERMAKE_DATA		4096	//ˮӡͼƬ������󳤶�
#define DH_MAX_WATERMAKE_LETTER		128	//ˮӡ������󳤶�
#define DH_MAX_WLANDEVICE_NUM		10	//����������������豸����
#define DH_MAX_ALARM_NAME			64	//��ַ����
#define DH_MAX_REGISTER_SERVER_NUM	10	//����ע�����������
#define DH_SNIFFER_FRAMEID_NUM		6   //6��FRAME ID ѡ��
#define DH_SNIFFER_CONTENT_NUM		4   //ÿ��FRAME��Ӧ��4��ץ������
#define DH_MAX_PROTOCOL_NAME_LENGTH 20
#define DH_SNIFFER_GROUP_NUM		4   //4��ץ������
#define MAX_PATH_STOR              240  //Զ��Ŀ¼�ĳ���


//�ֱ����б�������ֱ�����������롢�����
#define	DH_CAPTURE_SIZE_D1			0x00000001
#define DH_CAPTURE_SIZE_HD1			0x00000002
#define DH_CAPTURE_SIZE_BCIF		0x00000004
#define DH_CAPTURE_SIZE_CIF			0x00000008
#define DH_CAPTURE_SIZE_QCIF		0x00000010	
#define DH_CAPTURE_SIZE_VGA			0x00000020	
#define DH_CAPTURE_SIZE_QVGA		0x00000040
#define DH_CAPTURE_SIZE_SVCD		0x00000080
#define DH_CAPTURE_SIZE_QQVGA		0x00000100
#define DH_CAPTURE_SIZE_SVGA		0x00000200
#define DH_CAPTURE_SIZE_XVGA		0x00000400
#define DH_CAPTURE_SIZE_WXGA		0x00000800
#define DH_CAPTURE_SIZE_SXGA		0x00001000
#define DH_CAPTURE_SIZE_WSXGA		0x00002000   
#define DH_CAPTURE_SIZE_UXGA		0x00004000
#define DH_CAPTURE_SIZE_WUXGA       0x00008000

//��̨�����б�������ֱ�����������롢�����
//lowMask
#define PTZ_DIRECTION				0x00000001				/*!< ���� */
#define PTZ_ZOOM					0x00000002	   			/*!< �䱶 */
#define PTZ_FOCUS					0x00000004				/*!< �۽� */
#define PTZ_IRIS					0x00000008				/*!< ��Ȧ */
#define PTZ_ALARM					0x00000010				/*!< �������� */
#define PTZ_LIGHT					0x00000020				/*!< �ƹ� */
#define PTZ_SETPRESET				0x00000040				/*!< ����Ԥ�õ� */
#define PTZ_CLEARPRESET				0x00000080				/*!< ���Ԥ�õ� */
#define PTZ_GOTOPRESET				0x00000100				/*!< ת��Ԥ�õ� */
#define PTZ_AUTOPANON				0x00000200				/*!< ˮƽ��ʼ */
#define PTZ_AUTOPANOFF				0x00000400				/*!< ˮƽ���� */
#define PTZ_SETLIMIT				0x00000800				/*!< ���ñ߽� */
#define PTZ_AUTOSCANON				0x00001000				/*!< �Զ�ɨ�迪ʼ */
#define PTZ_AUTOSCANOFF				0x00002000				/*!< �Զ�ɨ�迪ֹͣ */
#define PTZ_ADDTOUR					0x00004000				/*!< ����Ѳ���� */
#define PTZ_DELETETOUR				0x00008000				/*!< ɾ��Ѳ���� */
#define PTZ_STARTTOUR				0x00010000				/*!< ��ʼѲ�� */
#define PTZ_STOPTOUR				0x00020000				/*!< ����Ѳ�� */
#define PTZ_CLEARTOUR				0x00040000				/*!< ɾ��Ѳ�� */
#define PTZ_SETPATTERN				0x00080000				/*!< ����ģʽ */
#define PTZ_STARTPATTERN			0x00100000				/*!< ��ʼģʽ */
#define PTZ_STOPPATTERN				0x00200000				/*!< ֹͣģʽ */
#define PTZ_CLEARPATTERN			0x00400000				/*!< ���ģʽ */
#define PTZ_POSITION				0x00800000				/*!< ���ٶ�λ */
#define PTZ_AUX						0x01000000				/*!< �������� */
#define PTZ_MENU					0x02000000				/*!< ����˵� */
#define PTZ_EXIT					0x04000000				/*!< �˳�����˵� */
#define PTZ_ENTER					0x08000000				/*!< ȷ�� */
#define PTZ_ESC						0x10000000				/*!< ȡ�� */
#define PTZ_MENUUPDOWN				0x20000000				/*!< �˵����²��� */
#define PTZ_MENULEFTRIGHT			0x40000000				/*!< �˵����Ҳ��� */
#define PTZ_OPT_NUM					0x80000000				//*< �����ĸ��� */

//hiMask
#define PTZ_DEV						0x00000001				/*!< ��̨���� */
#define PTZ_MATRIX					0x00000002				/*!< ������� */

//����ģʽ�б����������ģʽ��������롢�����
#define DH_CAPTURE_COMP_DIVX_MPEG4	0x00000001
#define DH_CAPTURE_COMP_MS_MPEG4 	0x00000002
#define DH_CAPTURE_COMP_MPEG2		0x00000004
#define DH_CAPTURE_COMP_MPEG1		0x00000008
#define DH_CAPTURE_COMP_H263		0x00000010
#define DH_CAPTURE_COMP_MJPG		0x00000020
#define DH_CAPTURE_COMP_FCC_MPEG4	0x00000040
#define DH_CAPTURE_COMP_H264		0x00000080

//���������б�
#define DH_CAPTURE_BRC_CBR			0
#define DH_CAPTURE_BRC_VBR			1
//#define DH_CAPTURE_BRC_MBR			2

//�������������������뱨������������������롢�����
#define DH_ALARM_UPLOAD		0x00000001
#define DH_ALARM_RECORD		0x00000002
#define DH_ALARM_PTZ		0x00000004
#define DH_ALARM_MAIL		0x00000008
#define DH_ALARM_TOUR		0x00000010
#define DH_ALARM_TIP		0x00000020
#define DH_ALARM_OUT		0x00000040
#define DH_ALARM_FTP_UL		0x00000080
#define DH_ALARM_BEEP		0x00000100
#define DH_ALARM_VOICE		0x00000200
#define DH_ALARM_SNAP		0x00000400

//"�ָ�Ĭ������"���룬������ÿ��û����
#define DH_RESTORE_COMMON			0x00000001	//��ͨ����
#define DH_RESTORE_CODING			0x00000002	//��������
#define DH_RESTORE_VIDEO			0x00000004	//¼������
#define DH_RESTORE_COMM				0x00000008	//��������
#define DH_RESTORE_NETWORK			0x00000010	//��������
#define DH_RESTORE_ALARM			0x00000020	//��������
#define DH_RESTORE_VIDEODETECT		0x00000040	//��Ƶ���
#define DH_RESTORE_PTZ				0x00000080	//��̨����
#define DH_RESTORE_OUTPUTMODE		0x00000100	//���ģʽ
#define DH_RESTORE_CHANNELNAME		0x00000200	//ͨ������
#define DH_RESTORE_ALL				0x80000000	//ȫ������


////////////////end ofԶ�����ýṹ����س���///////////////////////


/************************************************************************
 ** ö�ٶ���
 ***********************************************************************/
//�豸����
enum NET_DEVICE_TYPE 
{
	NET_PRODUCT_NONE = 0,
	NET_DVR_NONREALTIME_MACE, /*!< ��ʵʱMACE */
	NET_DVR_NONREALTIME, /*!< ��ʵʱ */
	NET_NVS_MPEG1, /*!< ������Ƶ������ */
	NET_DVR_MPEG1_2, /*!< MPEG1��·¼��� */
	NET_DVR_MPEG1_8, /*!< MPEG1��·¼��� */
	NET_DVR_MPEG4_8, /*!< MPEG4��·¼��� */
	NET_DVR_MPEG4_16, /*!< MPEG4ʮ��·¼��� */
	NET_DVR_MPEG4_SX2, /*!< LBϵ��¼��� */
	NET_DVR_MEPG4_ST2, /*!< GBϵ��¼��� */
	NET_DVR_MEPG4_SH2, /*!< HBϵ��¼��� */
	NET_DVR_MPEG4_GBE, /*!< GBEϵ��¼��� */
	NET_DVR_MPEG4_NVSII, /*!< II��������Ƶ������ */
	NET_DVR_STD_NEW, /*!< �±�׼����Э�� */
	NET_DVR_DDNS, /*DDNS������*/
	NET_DVR_ATM,  /* ATM�� */
	NET_NB_SERIAL, /* ������ʵʱNBϵ�л��� */
	NET_LN_SERIAL, /* LNϵ�в�Ʒ */
	NET_BAV_SERIAL, /* BAVϵ�в�Ʒ */
	NET_SDIP_SERIAL, /* SDIPϵ�в�Ʒ */
	NET_IPC_SERIAL, /* IPCϵ�в�Ʒ */
	NET_NVS_B,		/* NVS Bϵ�� */
	NET_NVS_C,		/* NVS Hϵ�� */
	NET_NVS_S,		/* NVS Sϵ�� */
	NET_NVS_E,		/* NVS Eϵ�� */
	NET_DVR_NEW_PROTOCOL /*��QueryDevState�в�ѯ�豸����,���ַ�����ʽ*/
};

//ͨ����̨��������
typedef enum _PTZ_ControlType
{
	DH_PTZ_UP_CONTROL = 0,				//��
	DH_PTZ_DOWN_CONTROL,			//��
	DH_PTZ_LEFT_CONTROL,			//��
	DH_PTZ_RIGHT_CONTROL,			//��
	DH_PTZ_ZOOM_ADD_CONTROL,		//�䱶+
	DH_PTZ_ZOOM_DEC_CONTROL,		//�䱶-
	DH_PTZ_FOCUS_ADD_CONTROL,		//����+
	DH_PTZ_FOCUS_DEC_CONTROL,		//����-
	DH_PTZ_APERTURE_ADD_CONTROL,	//��Ȧ+
	DH_PTZ_APERTURE_DEC_CONTROL,   //��Ȧ-
    DH_PTZ_POINT_MOVE_CONTROL,     //ת��Ԥ�õ�
    DH_PTZ_POINT_SET_CONTROL,      //����
    DH_PTZ_POINT_DEL_CONTROL,      //ɾ��
    DH_PTZ_POINT_LOOP_CONTROL,     //���Ѳ��
    DH_PTZ_LAMP_CONTROL            //�ƹ���ˢ
}DH_PTZ_ControlType;

//��̨������չ����
typedef enum _EXTPTZ_ControlType
{
	DH_EXTPTZ_LEFTTOP = 0X20,		//����
	DH_EXTPTZ_RIGHTTOP ,			//����
	DH_EXTPTZ_LEFTDOWN ,			//����
	DH_EXTPTZ_RIGHTDOWN,			//����
	DH_EXTPTZ_ADDTOLOOP,			//����Ԥ�õ㵽Ѳ��	Ѳ����·	Ԥ�õ�ֵ	
	DH_EXTPTZ_DELFROMLOOP,			//ɾ��Ѳ����Ԥ�õ�	Ѳ����·	Ԥ�õ�ֵ	
    DH_EXTPTZ_CLOSELOOP,			//���Ѳ��	Ѳ����·		
	DH_EXTPTZ_STARTPANCRUISE,		//��ʼˮƽ��ת			
	DH_EXTPTZ_STOPPANCRUISE,		//ֹͣˮƽ��ת			
	DH_EXTPTZ_SETLEFTBORDER,		//������߽�			
	DH_EXTPTZ_SETRIGHTBORDER,		//�����ұ߽�			
	DH_EXTPTZ_STARTLINESCAN,		//��ʼ��ɨ			
    DH_EXTPTZ_CLOSELINESCAN,		//ֹͣ��ɨ			
    DH_EXTPTZ_SETMODESTART,		//����ģʽ��ʼ	ģʽ��·		
    DH_EXTPTZ_SETMODESTOP,			//����ģʽ����	ģʽ��·		
	DH_EXTPTZ_RUNMODE,				//����ģʽ	ģʽ��·		
	DH_EXTPTZ_STOPMODE,			//ֹͣģʽ	ģʽ��·		
	DH_EXTPTZ_DELETEMODE,			//���ģʽ	ģʽ��·		
	DH_EXTPTZ_REVERSECOMM,			//��ת����			
	DH_EXTPTZ_FASTGOTO,			//���ٶ�λ	ˮƽ����(8192)	��ֱ����(8192)	�䱶(4)
	DH_EXTPTZ_AUXIOPEN,			//x34	�������ؿ�	������		
	DH_EXTPTZ_AUXICLOSE,			//0x35	�������ع�	������
	DH_EXTPTZ_OPENMENU = 0X36,		//������˵�
	DH_EXTPTZ_CLOSEMENU ,			//�رղ˵�
	DH_EXTPTZ_MENUOK,				//�˵�ȷ��
	DH_EXTPTZ_MENUCANCEL,			//�˵�ȡ��
	DH_EXTPTZ_MENUUP,				//�˵���
	DH_EXTPTZ_MENUDOWN,			//�˵���
	DH_EXTPTZ_MENULEFT,			//�˵���
	DH_EXTPTZ_MENURIGHT,			//�˵���
	DH_EXTPTZ_ALARMHANDLE = 0x40,	//����������̨ parm1:��������ͨ�� parm2:������������1-Ԥ�õ�2-��ɨ3-Ѳ�� parm3:����ֵ����Ԥ�õ��
	DH_EXTPTZ_MATRIXSWITCH = 0x41,	//�����л� parm1:��������(��Ƶ�����) parm2:(��Ƶ�����) parm3:�����
	DH_EXTPTZ_LIGHTCONTROL,			//�ƹ������
	DH_EXTPTZ_UP_TELE = 0x70,		//�� + TELE��param1=�ٶȣ�1-8������ͬ
	DH_EXTPTZ_DOWN_TELE,			//�� + TELE
	DH_EXTPTZ_LEFT_TELE,			//�� + TELE
	DH_EXTPTZ_RIGHT_TELE,			//�� + TELE
	DH_EXTPTZ_LEFTUP_TELE,			//���� + TELE
	DH_EXTPTZ_LEFTDOWN_TELE,		//���� + TELE
	DH_EXTPTZ_TIGHTUP_TELE,			//���� + TELE
	DH_EXTPTZ_RIGHTDOWN_TELE,		//���� + TELE
	DH_EXTPTZ_UP_WIDE,				//�� + WIDE��param1=�ٶȣ�1-8������ͬ
	DH_EXTPTZ_DOWN_WIDE,			//�� + WIDE
	DH_EXTPTZ_LEFT_WIDE,			//�� + WIDE
	DH_EXTPTZ_RIGHT_WIDE,			//�� + WIDE
	DH_EXTPTZ_LEFTUP_WIDE,			//���� + WIDE
	DH_EXTPTZ_LEFTDOWN_WIDE,		//���� + WIDE
	DH_EXTPTZ_TIGHTUP_WIDE,			//���� + WIDE
	DH_EXTPTZ_RIGHTDOWN_WIDE,		//���� + WIDE
	DH_EXTPTZ_TOTAL,				//�������ֵ
}DH_EXTPTZ_ControlType;

//IO��������
typedef enum _IOTYPE
{
	DH_ALARMINPUT = 1,				//��������
	DH_ALARMOUTPUT = 2,			//�������
	
	DH_DECODER_ALARMOUT = 3,	//�������������
	DH_WIRELESS_ALARMOUT =5,    //���߱������
}DH_IOTYPE;

//��ѯ��־��չ�ӿڲ������豸��־����
typedef enum _DH_LOG_QUERY_TYPE
{
	DHLOG_ALL = 0,		//������־
	DHLOG_SYSTEM,		//ϵͳ��־
	DHLOG_CONFIG,		//������־
	DHLOG_STORAGE,		//�洢���
	DHLOG_ALARM,		//������־
	DHLOG_RECORD,		//¼�����
	DHLOG_ACCOUNT,		//�ʺ����
	DHLOG_CLEAR,		//�����־
	DHLOG_PLAYBACK		//�ط����
}DH_LOG_QUERY_TYPE;

//���ص���־��Ϣ�ṹ�壭��־����
typedef enum _DH_LOG_TYPE
{
	//system logs
	DH_LOG_REBOOT = 0x0000,
	DH_LOG_SHUT,
	//config logs
	DH_LOG_CONFSAVE = 0x0100,
	DH_LOG_CONFLOAD,
	//fs logs
	DH_LOG_FSERROR = 0x0200,   //�ļ�ϵͳ����
	DH_LOG_HDD_WERR,           //Ӳ��д����
	DH_LOG_HDD_RERR,           //Ӳ�̶�����
	DH_LOG_HDD_TYPE,           //����Ӳ������
	DH_LOG_HDD_FORMAT,         //��ʽ��Ӳ��
	//alarm logs
	DH_LOG_ALM_IN = 0x0300,
	DH_LOG_ALM_END = 0x0302,
	DH_LOG_INFRAREDALM_IN = 0x03a0, //���߱�������
	DH_LOG_INFRAREDALM_END,       //���߱�������
	//record
	DH_LOG_MANUAL_RECORD = 0x0401,
	DH_LOG_CLOSED_RECORD,
	//user manager
	DH_LOG_LOGIN = 0x0500,
	DH_LOG_LOGOUT,
	DH_LOG_ADD_USER,
	DH_LOG_DELETE_USER,
	DH_LOG_MODIFY_USER,
	DH_LOG_ADD_GROUP,
	DH_LOG_DELETE_GROUP,
	DH_LOG_MODIFY_GROUP,
	//clear
	DH_LOG_CLEAR = 0x0600,
	DH_LOG_TYPE_NR = 7,
}DH_LOG_TYPE;

//�ӿ�CLIENT_QueryLogEx���ص���־��Ϣ�ṹ�壭��־����
//������int nType = 1;����reserved = &nType.
typedef enum _DH_NEWLOG_TYPE
{
	//system logs
	DH_NEWLOG_REBOOT	 = 0x0000,		//ϵͳ����
	DH_NEWLOG_SHUT,						//ϵͳ�ػ�
	
	//config logs
	DH_NEWLOG_CONFSAVE = 0x0100,
	DH_NEWLOG_CONFLOAD,
	
	//fs logs
	DH_NEWLOG_FSERROR	 = 0x0200,		//�ļ�ϵͳ����
	DH_NEWLOG_HDD_WERR,					//Ӳ��д����
	DH_NEWLOG_HDD_RERR,					//Ӳ�̶�����
	DH_NEWLOG_HDD_TYPE,					//����Ӳ������
	DH_NEWLOG_HDD_FORMAT,				//��ʽ��Ӳ��
	DH_NEWLOG_HDD_NOSPACE,				//��ǰ�����̿ռ䲻��
	
	//alarm logs
	DH_NEWLOG_ALM_IN	 = 0x0300,
	DH_NEWLOG_NETALM_IN,
	DH_NEWLOG_ALM_END,
	DH_NEWLOG_LOSS_IN,
	DH_NEWLOG_LOSS_END,
	
	//record
	DH_NEWLOG_AUTOMATIC_RECORD = 0x0400,
	DH_NEWLOG_MANUAL_RECORD,
	DH_NEWLOG_CLOSED_RECORD,
	
	//user manager
	DH_NEWLOG_LOGIN    = 0x0500,
	DH_NEWLOG_LOGOUT,
	DH_NEWLOG_ADD_USER,
	DH_NEWLOG_DELETE_USER,
	DH_NEWLOG_MODIFY_USER,
	DH_NEWLOG_ADD_GROUP,
	DH_NEWLOG_DELETE_GROUP,
	DH_NEWLOG_MODIFY_GROUP,
	
	//clear
	DH_NEWLOG_CLEAR    = 0x0600,
	DH_NEWLOG_SEARCHLOG,				//��ѯ��־
	
	//record operation
	DH_NEWLOG_SEARCH	 = 0x0700,		//¼���ѯ
	DH_NEWLOG_DOWNLOAD,					//¼������
	DH_NEWLOG_PLAYBACK,					//¼��ط�
	DH_NEWLOG_BACKUP,					/*!< ����¼���ļ� */
	DH_NEWLOG_BACKUPERROR,				/*!< ����¼���ļ�ʧ�� */
	
	DH_NEWLOG_TYPE_NR = 8,		
}DH_NEWLOG_TYPE;


//ʵʱԤ����չ�ӿ����ӵĲ�����Ԥ������
typedef enum _RealPlayType
{
	DH_RType_Realplay = 0,	//ʵʱԤ��
	DH_RType_Multiplay,	//�໭��Ԥ��
		
	DH_RType_Realplay_0,	//ʵʱ����-����������ͬ��DH_RType_Realplay
	DH_RType_Realplay_1,	//ʵʱ����-������1
	DH_RType_Realplay_2,	//ʵʱ����-������2
	DH_RType_Realplay_3,	//ʵʱ����-������3
		
	DH_RType_Multiplay_1,	//�໭��Ԥ����1����
	DH_RType_Multiplay_4,	//�໭��Ԥ����4����
	DH_RType_Multiplay_8,	//�໭��Ԥ����8����
	DH_RType_Multiplay_9,	//�໭��Ԥ����9����
	DH_RType_Multiplay_16,	//�໭��Ԥ����16����

	DH_RType_Multiplay_6,	//�໭��Ԥ����6����
	DH_RType_Multiplay_12,	//�໭��Ԥ����12����
} DH_RealPlayType;

//�豸���ƽӿڲ������������Ͷ���
typedef enum _CtrlType
{
	DH_CTRL_REBOOT = 0,	//�����豸	
	DH_CTRL_SHUTDOWN,	//�ر��豸
	DH_CTRL_DISK,		//Ӳ�̹���
	//�������
	DH_KEYBOARD_POWER = 3,
	DH_KEYBOARD_ENTER,
	DH_KEYBOARD_ESC,
	DH_KEYBOARD_UP,
	DH_KEYBOARD_DOWN,
	DH_KEYBOARD_LEFT,
	DH_KEYBOARD_RIGHT,
	DH_KEYBOARD_BTN0,
	DH_KEYBOARD_BTN1,
	DH_KEYBOARD_BTN2,
	DH_KEYBOARD_BTN3,
	DH_KEYBOARD_BTN4,
	DH_KEYBOARD_BTN5,
	DH_KEYBOARD_BTN6,
	DH_KEYBOARD_BTN7,
	DH_KEYBOARD_BTN8,
	DH_KEYBOARD_BTN9,
	DH_KEYBOARD_BTN10,
	DH_KEYBOARD_BTN11,
	DH_KEYBOARD_BTN12,
	DH_KEYBOARD_BTN13,
	DH_KEYBOARD_BTN14,
	DH_KEYBOARD_BTN15,
	DH_KEYBOARD_BTN16,
	DH_KEYBOARD_SPLIT,
	DH_KEYBOARD_ONE,
	DH_KEYBOARD_NINE,
	DH_KEYBOARD_ADDR,
	DH_KEYBOARD_INFO,
	DH_KEYBOARD_REC,
	DH_KEYBOARD_FN1,
	DH_KEYBOARD_FN2,
	DH_KEYBOARD_PLAY,
	DH_KEYBOARD_STOP,
	DH_KEYBOARD_SLOW,
	DH_KEYBOARD_FAST,
	DH_KEYBOARD_PREW,
	DH_KEYBOARD_NEXT,
	DH_KEYBOARD_JMPDOWN,
	DH_KEYBOARD_JMPUP,
	//��������
	DH_TRIGGER_ALARM_IN = 100,
	DH_TRIGGER_ALARM_OUT,
	//�������
	DH_CTRL_MATRIX,
	//SD�����ƣ�ipc��Ʒ������ͬӲ�̿���
	DH_CTRL_SDCARD,
	//��¼������
	DH_BURNING_START,		//��ʼ��¼
	DH_BURNING_STOP,		//������¼

	DH_BURNING_ADDPWD,	//��������(��0Ϊ��β���ַ���,��󳤶�8λ)
	DH_BURNING_ADDHEAD,	//����Ƭͷ(��0Ϊ��β���ַ���,��󳤶�1024�ֽ�,֧�ַ���,�зָ���'\n')��
	DH_BURNING_ADDSIGN,	//���Ӵ�㵽��¼��Ϣ(������)
	DH_BURNING_ADDCURSTOMINFO,	//�Զ������(��0Ϊ��β���ַ���,��󳤶�1024�ֽ�,֧�ַ���,�зָ���'\n')

	//�ָ��豸��Ĭ������
	DH_CTRL_RESTOREDEFAULT,

	//�豸����DSPץͼ
	DH_CTRL_CAPTURE_START,		//�����豸ץͼ

	//�����־
	DH_CTRL_CLEARLOG,
	//���߱���
	DH_TRIGGER_ALARM_WIRELESS = 200,
}CtrlType;

//�������ͣ�Ŀǰʹ�ø�ö�ٵĽӿ�����̭���벻Ҫʹ��
typedef enum _CFG_INDEX
{
    CFG_GENERAL =  0,		// ��ͨ
	CFG_COMM   		,		// ����
	CFG_NET    		,		// ����
	CFG_RECORD 		,		// ¼��
	CFG_CAPTURE		,		// ͼ������
	CFG_PTZ    		,		// ��̨
	CFG_DETECT 		,		// ��̬���
	CFG_ALARM  		,		// ����
	CFG_DISPLAY		,		// ��ʾ
	CFG_RESERVED	,		// ������ʹ��������
	CFG_TITLE  		=10,	// ͨ������
	CFG_MAIL  		=11,	// �ʼ�����
	CFG_EXCAPTURE	=12,	// Ԥ��ͼ������
	CFG_PPPOE		=13,	// pppoe����
	CFG_DDNS		=14,	// DDNS����
	CFG_SNIFFER		=15,	// ������Ӳ�������
	CFG_DSPINFO		=16,	// ����������Ϣ
	CFG_COLOR		=126,  //��ɫ������Ϣ
	CFG_ALL,				// ����
} CFG_INDEX;

typedef enum _SNAP_TYPE
{
	SNAP_TYP_TIMING = 0,
	SNAP_TYP_ALARM,
	SNAP_TYP_NUM,
} SNAP_TYPE;

//�ֱ���ö�٣���DH_DSP_ENCODECAPʹ��
typedef enum _CAPTURE_SIZE
{
	CAPTURE_SIZE_D1,		// <  720*576(PAL)  720*480(NTSC)
	CAPTURE_SIZE_HD1,		// <  352*576(PAL)  352*480(NTSC)
	CAPTURE_SIZE_BCIF,		// <  720*288(PAL)  720*240(NTSC)
	CAPTURE_SIZE_CIF,		// <  352*288(PAL)  352*240(NTSC)
	CAPTURE_SIZE_QCIF,		// <  176*144(PAL)  176*120(NTSC)
	CAPTURE_SIZE_VGA,		// <  640*480
	CAPTURE_SIZE_QVGA,		// <  320*240
	CAPTURE_SIZE_SVCD,		// <  480*480
	CAPTURE_SIZE_QQVGA,		// <  160*128
	CAPTURE_SIZE_SVGA,		// <  800*592
	CAPTURE_SIZE_XVGA,		// <  1024*768
	CAPTURE_SIZE_WXGA,		// <  1280*800
	CAPTURE_SIZE_SXGA,		// <  1280*1024  
	CAPTURE_SIZE_WSXGA,		// <  1600*1024  
	CAPTURE_SIZE_UXGA,		// <  1600*1200
	CAPTURE_SIZE_WUXGA,		// <  1920*1200
	CAPTURE_SIZE_NR  
} CAPTURE_SIZE;

typedef enum __EM_USEDEV_MODE
{
	DH_TALK_CLIENT_MODE,		//���ÿͻ��˷�ʽ���������Խ�
	DH_TALK_SERVER_MODE,		//���÷�������ʽ���������Խ�
	DH_TALK_ENCODE_TYPE,		//���������Խ������ʽ
	DH_ALARM_LISTEN_MODE        //���ñ������ķ�ʽ

} EM_USEDEV_MODE;

typedef enum __EM_UPGRADE_TYPE
{
	DH_UPGRADE_BIOS_TYPE=1,		//BIOS����
	DH_UPGRADE_WEB_TYPE,		//WEB����
	DH_UPGRADE_BOOT_YPE,		//BOOT����
	DH_UPGRADE_CHARACTER_TYPE,	//���ֿ�
	DH_UPGRADE_LOGO_TYPE,		//LOGO
	DH_UPGRADE_EXE_TYPE,		//EXE ���粥������
} EM_UPGRADE_TYPE;

//��������
typedef enum __LANGUAGE_TYPE
{
	DH_LANGUAGE_ENGLISH,				//Ӣ��	
	DH_LANGUAGE_CHINESE_SIMPLIFIED,		//��������	
	DH_LANGUAGE_CHINESE_TRADITIONAL,	//��������	
	DH_LANGUAGE_ITALIAN,				//�������	
	DH_LANGUAGE_SPANISH,				//��������
	DH_LANGUAGE_JAPANESE,				//���İ�	
	DH_LANGUAGE_RUSSIAN,				//���İ�		
	DH_LANGUAGE_FRENCH,					//���İ�		
	DH_LANGUAGE_GERMAN, 				//���İ�		
	DH_LANGUAGE_PORTUGUESE,				//��������	
	DH_LANGUAGE_TURKEY,					//��������	
	DH_LANGUAGE_POLISH,					//������	
	DH_LANGUAGE_ROMANIAN,				//��������	
	DH_LANGUAGE_HUNGARIAN,              //��������	
	DH_LANGUAGE_FINNISH,				//������	
	DH_LANGUAGE_ESTONIAN,				//��ɳ������	
	DH_LANGUAGE_KOREAN,					//����	
	DH_LANGUAGE_FARSI,					//��˹�� 	
	DH_LANGUAGE_DANSK,					//������
	DH_LANGUAGE_CZECHISH,				//�ݿ���
	DH_LANGUAGE_BULGARIA,				//����������
} DH_LANGUAGE_TYPE;

//�����Խ���ʽ
typedef enum __TALK_CODING_TYPE
{
	DH_TALK_DEFAULT = 0,
	DH_TALK_PCM = 1,					//PCM
	DH_TALK_G711a,						//G711a
	DH_TALK_AMR,						//AMR
	DH_TALK_G711u,						//G711u
	DH_TALK_G726,						//G726
}DH_TALK_CODING_TYPE;

/************************************************************************
 ** �ṹ�嶨��
 ***********************************************************************/
//ʱ��
typedef struct 
{
	DWORD dwYear;		//��
	DWORD dwMonth;		//��
	DWORD dwDay;		//��
	DWORD dwHour;		//ʱ
	DWORD dwMinute;		//��
	DWORD dwSecond;		//��
} NET_TIME,*LPNET_TIME;

//�豸��Ϣ
typedef struct {
	BYTE sSerialNumber[DH_SERIALNO_LEN];	//���к�
	BYTE byAlarmInPortNum;				//DVR�����������
	BYTE byAlarmOutPortNum;				//DVR�����������
	BYTE byDiskNum;						//DVR Ӳ�̸���
	BYTE byDVRType;						//DVR����, ��ö��DHDEV_DEVICE_TYPE
	BYTE byChanNum;						//DVR ͨ������
} NET_DEVICEINFO, *LPNET_DEVICEINFO;

//��ͨ������Ϣ
typedef struct {
	int channelcount;
	int alarminputcount;
	unsigned char alarm[16];			//�ⲿ����
	unsigned char motiondection[16];	//��̬���
	unsigned char videolost[16];		//��Ƶ��ʧ
} NET_CLIENT_STATE;

//��ѯ�豸����״̬Ӳ����Ϣ
typedef struct {
	DWORD dwVolume;		//Ӳ�̵�����
	DWORD dwFreeSpace;	//Ӳ�̵�ʣ��ռ�
	DWORD dwStatus;		//Ӳ�̵�״̬,0-����,1-�,2-���ϵ�
}NET_DEV_DISKSTATE,*LPNET_DEV_DISKSTATE;

//¼���ļ���Ϣ
typedef struct {
    unsigned int     ch;              //ͨ����
    char             filename[128];   //�ļ���
    unsigned int     size;            //�ļ�����
    NET_TIME         starttime;       //��ʼʱ��
    NET_TIME         endtime;         //����ʱ��
    unsigned int     driveno;         //���̺�
    unsigned int     startcluster;    //��ʼ�غ�
	int				 nRecordFileType; //¼���ļ�����  0����ͨ¼��1������¼��2���ƶ���⣻3������¼��
} NET_RECORDFILE_INFO, *LPNET_RECORDFILE_INFO;

//����IO����
typedef struct 
{
	unsigned short index;	//�˿����
	unsigned short state;	//�˿�״̬
}ALARM_CONTROL;

typedef struct 
{
	int	decoderNo;	//������������,��0��ʼ
	unsigned short  alarmChn; //���������,��0��ʼ
	unsigned short  alarmState; //�������״̬,1-�򿪡�0-�ر�
}DECODER_ALARM_CONTROL;

///////////////////�û��ʺŹ�����ؽṹ��/////////////////////////////////
typedef struct _OPR_RIGHT
{
	DWORD		dwID;
	char		name[DH_RIGHT_NAME_LENGTH];
	char		memo[DH_MEMO_LENGTH];
}OPR_RIGHT;

typedef struct _USER_INFO
{
	DWORD		dwID;
	DWORD		dwGroupID;
	char		name[DH_USER_NAME_LENGTH];
	char		passWord[DH_USER_PSW_LENGTH];
	DWORD		dwRightNum;
	DWORD		rights[DH_MAX_RIGHT_NUM];
	char		memo[DH_MEMO_LENGTH];
	DWORD		dwReusable;		//���û��Ƿ������� 1-���ã�0-������
	
}USER_INFO;

typedef struct _USER_GROUP_INFO
{
	DWORD		dwID;
	char		name[DH_USER_NAME_LENGTH];
	DWORD		dwRightNum;
	DWORD		rights[DH_MAX_RIGHT_NUM];
	char		memo[DH_MEMO_LENGTH];
}USER_GROUP_INFO;

//�û���Ϣ���ýṹ
typedef struct _USER_MANAGE_INFO
{
	DWORD				dwRightNum;
	OPR_RIGHT			rightList[DH_MAX_RIGHT_NUM];
	DWORD				dwGroupNum;
	USER_GROUP_INFO		groupList[DH_MAX_GROUP_NUM];
	DWORD				dwUserNum;
	USER_INFO			userList[DH_MAX_USER_NUM];
	DWORD				dwSpecial; //������Ϣ�� 1-֧���û����ã���0-��֧���û�����
}USER_MANAGE_INFO;

//////////////////////////////////////////////////////////////////////////////
///////////////////�û��ʺŹ�����չ��ؽṹ��/////////////////////////////////
#define DH_USER_NAME_LENGTH_EX  16  //�û�������
#define DH_USER_PSW_LENGTH_EX   16  //����
typedef struct _OPR_RIGHT_EX
{
	DWORD  dwID;
	char  name[DH_RIGHT_NAME_LENGTH];
	char  memo[DH_MEMO_LENGTH];
}OPR_RIGHT_EX;

typedef struct _USER_INFO_EX
{
	DWORD  dwID;
	DWORD  dwGroupID;
	char  name[DH_USER_NAME_LENGTH_EX];
	char  passWord[DH_USER_PSW_LENGTH_EX];
	DWORD  dwRightNum;
	DWORD  rights[DH_MAX_RIGHT_NUM];
	char  memo[DH_MEMO_LENGTH];
	DWORD   dwFouctionMask;//����
	
						   /* ����,����
						    * 0x00000001 - ֧���û�����
							*/
	
	BYTE  byReserve[32];
	
}USER_INFO_EX;

typedef struct _USER_GROUP_INFO_EX
{
	DWORD  dwID;
	char  name[DH_USER_NAME_LENGTH_EX];
	DWORD  dwRightNum;
	DWORD  rights[DH_MAX_RIGHT_NUM];
	char  memo[DH_MEMO_LENGTH];
}USER_GROUP_INFO_EX;

//�û���Ϣ���ýṹ
typedef struct _USER_MANAGE_INFO_EX
{
	
	DWORD				dwRightNum;
	OPR_RIGHT_EX		rightList[DH_MAX_RIGHT_NUM];
	DWORD				dwGroupNum;
	USER_GROUP_INFO_EX  groupList[DH_MAX_GROUP_NUM];
	DWORD				dwUserNum;
	USER_INFO_EX		userList[DH_MAX_USER_NUM];
	DWORD				dwFouctionMask;//����    
							 /* ����,����
							 * 0x00000001 - ֧���û�����
							 * 0x00000002 - �����޸���ҪУ��
							 */
	BYTE  byNameMaxLength;       //֧���û�������󳤶�
	BYTE  byPSWMaxLength;       //֧�ֵ�������󳤶�
	BYTE  byReserve[254];
}USER_MANAGE_INFO_EX;
///////////////////end of �û��ʺŹ�����չ��ؽṹ��////////////////////////////
///////////////////////////////////////////////////////////////////////////////

////////////////////��־�ṹ�����///////////////////////////
//��־ʱ��ṹ
typedef struct _DHDEVTIME
{
	DWORD second		:6;					//	��	1-60		
	DWORD minute		:6;					//	��	1-60		
	DWORD hour			:5;					//	ʱ	1-24		
	DWORD day			:5;					//	��	1-31		
	DWORD month			:4;					//	��	1-12		
	DWORD year			:6;					//	��	2000-2063	
}DHDEVTIME, *LPDHDEVTIME;

typedef struct _DH_LOG_ITEM
{
    DHDEVTIME time;					//����
    unsigned short type;			//����
    unsigned char reserved;			//����
    unsigned char data;				//����
    unsigned char context[8];		//����
}DH_LOG_ITEM, *LPDH_LOG_ITEM;

//�ӿ�CLIENT_QueryLogEx���ص���־��Ϣ�ṹ��
//������int nType = 1;����reserved = &nType.
typedef struct _DH_NEWLOG_ITEM
{
	DHDEVTIME	time; 				//����
	WORD	type;					//����
	WORD	data;					//����
	char  	szOperator[8]; 			//�û���
	BYTE	context[16];		    //����	
}DH_NEWLOG_ITEM, *LPDH_NEWLOG_ITEM;

typedef struct _LOG_ITEM_RECORD
{
	DHDEVTIME	time;		//ʱ��
	BYTE	channel;		//ͨ��
	BYTE	type;		//¼������
	BYTE	reserved[2];
}LOG_ITEM_RECORD, *LPLOG_ITEM_RECORD;
////////////////////end of ��־�ṹ�����///////////////////////////

//�ص���Ƶ����֡��֡�����ṹ��
typedef struct _tagVideoFrameParam
{
	BYTE encode;		//��������
	BYTE frametype;		//I = 0, P = 1, B = 2...
	BYTE format;		//PAL - 0, NTSC - 1
	BYTE size;			//CIF - 0, HD1 - 1, 2CIF - 2, D1 - 3, VGA - 4, QCIF - 5, QVGA - 6 ,
						//SVCD - 7,QQVGA - 8, SVGA - 9, XVGA - 10,WXGA - 11,SXGA - 12,WSXGA - 13,UXGA - 14,WUXGA - 15,
	DWORD fourcc;		//�����H264��������Ϊ0��MPEG4����������дFOURCC('X','V','I','D');
	DWORD reserved;		//����
	NET_TIME struTime;	//ʱ����Ϣ
}tagVideoFrameParam;

//�ص���Ƶ����֡��֡�����ṹ��
typedef struct _tagCBPCMDataParam
{
	BYTE channels;		// ������
	BYTE samples;			// ���� 0 - 8000, 1 - 11025, 2 - 16000, 3 - 22050, 4 - 32000, 5 - 44100, 6 - 48000
	BYTE depth;			// ������� ȡֵ8����16�ȡ�ֱ�ӱ�ʾ
	BYTE param1;			// 0 - ָʾ�޷��ţ�1-ָʾ�з���
	DWORD reserved;		// ����
}tagCBPCMDataParam;

//ͨ��������Ļ���ӵ����ݽṹ
typedef struct _DH_CHANNEL_OSDSTRING
{
	BOOL bEnable;	//ʹ��
	DWORD dwPosition[MAX_STRING_LINE_LEN];	//�����ַ���λ�á���1-9������ʾ����С����λ�ö�Ӧ
											//		7����	8��		9����
											//		4��		5��		6��
											//		1����	2��		3����
	char szStrings[MAX_STRING_LINE_LEN][MAX_PER_STRING_LEN]; //��������ַ���ÿ�����20���ֽ�
}DH_CHANNEL_OSDSTRING;

////////////////////�豸���ƽӿڵĿ��Ʋ�������//////////////////////
typedef struct _DISKCTRL_PARAM
{
	DWORD dwSize;		//size of this structure, for version control
	int nIndex;			//disk index
	int ctrlType;		//control type. 
						// 0 - �������, 1 - ��Ϊ��д��, 2 - ��Ϊֻ����
						// 3 - ��Ϊ������, 4 - �ָ�����, 5 - ��Ϊ������
}DISKCTRL_PARAM;

typedef struct _ALARMCTRL_PARAM
{
	DWORD dwSize;
	int nAlarmNo;		//����ͨ���ţ���0��ʼ
	int nAction;		//0-����������1-ֹͣ����
}ALARMCTRL_PARAM;

typedef struct _MATRIXCTRL_PARAM
{
	DWORD dwSize;
	int nChannelNo;		//��Ƶ����ţ���0��ʼ
	int nMatrixNo;		//��������ţ���0��ʼ
}MATRIXCTRL_PARAM;

typedef struct _BURNING_PARM
{
	int channelMask;	//ͨ�������룬ÿλ��ʾһ��ͨ��
	int	devMask;		//�豸���룬ÿλ��ʾһ̨�豸
}BURNNG_PARM;


////////////////////end of�豸���ƽӿڵĿ��Ʋ�������//////////////////////

////////////////////��ѯ�豸״̬�Ĳ�������////////////////////////////////
typedef struct _DH_HARDDISK_STATE
{
	DWORD	dwDiskNum;		//Ӳ�̸���
	NET_DEV_DISKSTATE	stDisks[DH_MAX_DISKNUM]; //��Ӳ����Ϣ
}DH_HARDDISK_STATE,*LPDH_HARDDISK_STATE;

typedef DH_HARDDISK_STATE  DH_SDCARD_STATE;	//SD�������ݽṹͬӲ����Ϣ

typedef struct _NET_DEV_BURNING
{
	DWORD dwDriverType;	//��¼���������� 0 -	DHFS, 1 - DISK, 2- CDRW
	DWORD dwBusType;	//�������͡�0- USB, 1-1394, 2-IDE
	DWORD dwTotalSpace;	//������(KB)
	DWORD dwRemainSpace;	//ʣ������(KB)
	BYTE  dwDriverName[DH_BURNING_DEV_NAMELEN]; //��¼����������
}NET_DEV_BURNING,*LPNET_DEV_BURNING;

typedef struct _DH_BURNING_DEVINFO
{
	DWORD	dwDevNum;		//��¼�豸����
	NET_DEV_BURNING	stDevs[DH_MAX_BURNING_DEV_NUM]; //����¼�豸��Ϣ
}DH_BURNING_DEVINFO, *LPDH_BURNING_DEVINFO;

typedef struct _DH_BURNING_PROGRESS
{
	BYTE	bBurning;		//�ɿ�¼����ʾ�Ƿ����ڿ�¼
							/*
							 * �¿�¼����ֵ��
							 *0;   ���Կ�¼
							 *1;   �ǹ����豸,�豸���Ͳ���ȷ
							 *2;   û����Ч���豸,��ʣ��ռ�Ϊ0
							 *3;   �����������ڿ�¼
							 *4;   �豸Ϊ�ǿ���״̬,���ڱ��ݡ���¼��ط���
							 */
	BYTE	bRomType;		//������Ƭ����(�¿�¼���������ֵ)(0:���ļ�ϵͳ��1���ƶ�Ӳ�̻�u�̣�2������)
	BYTE	bOperateType;   //��������(�¿�¼���������ֵ)(0�����У�1�����ڱ����У�2�����ڿ�¼�У�3�����ڽ��й��̻ط�)
	BYTE	bType;			//���ݻ��¼�ù���״̬(�¿�¼���������ֵ)(0��ֹͣ����� ��1����ʼ��2������3����;4�����ڳ�ʼ��)	
	//**************************
	NET_TIME stTimeStart;	//��ʼʱ�䡡
	DWORD	dwTimeElapse;	//�ѿ�¼ʱ��(��)
	DWORD	dwTotalSpace;	//����������
	DWORD	dwRemainSpace;	//����ʣ������
	DWORD	dwBurned;		//�ѿ�¼����
	DWORD	dwStatus;		/*
							Ϊ�˼������豸�����������ģ��¿�¼��û��ʵ���ô�
							-1����ʾ�¿�¼���������Ϊ-1����ʾ�ɿ�¼��
							���������豸�Ŀ�¼״̬�����豸�õģ������Ϊ-1����ʾ�ɿ�¼����
							0-���ڳ�ʼ����
							1-���ڿ�¼�����ϸ�������Ч
							2-��¼����
							3-����ֹͣ
							*/
}DH_BURNING_PROGRESS, *LPDH_BURNING_PROGRESS;

//�豸֧�����Խṹ��
typedef struct _DH_LANGUAGE_DEVINFO
{
	DWORD	dwLanguageNum;		    //֧�����Ը���
	BYTE    byLanguageType[252];    //ö��ֵ,���DH_LANGUAGE_TYPE
}DH_DEV_LANGUAGE_INFO, *LPDH_DEV_LANGUAGE_INFO;


typedef struct  
{
	DH_TALK_CODING_TYPE		encodeType;		//��������
	int						nAudioBit;		//��ʵ�ʵ�ֵ��ʾ����8λ ����ֵΪ8
	DWORD					dwSampleRate;	//�����ʣ���16k ����ֵΪ16000
	char					reserved[64];
}DHDEV_TALKDECODE_INFO;

//�豸֧�ֵ������Խ���ʽ
typedef struct 
{
	int						nSupportNum;	//����
	DHDEV_TALKDECODE_INFO	type[64];		//�����ʽ
	char					reserved[64];
}DHDEV_TALKFORMAT_LIST;

////////////////////end of ��ѯ�豸״̬�Ĳ�������////////////////////////////////

//GPS��Ϣ�ṹ��
typedef struct _GPS_Info
{
    NET_TIME revTime;        //��λʱ��
	char DvrSerial[50];      //�豸���к�
    double longitude;        //����
    double latidude;         //γ��
    double height;           //�߶ȣ��ף�
    double angle;            //����ǣ���������Ϊԭ�㣬˳ʱ��Ϊ����
    double speed;            //�ٶȣ�����/Сʱ��
    WORD starCount;          //��λ����
    BOOL antennaState;       //����״̬(true �ã�false ��)
    BOOL orientationState;    //��λ״̬��true ��λ��false ����λ��
}GPS_Info,*LPGPS_Info;

//ץͼ�����ṹ��
typedef struct _snap_param
{
	UINT Channel;				//ץͼ��ͨ��
	UINT Quality;				//����1~6, �˲���ֻ��Գ���DVR�豸��Ч
	UINT ImageSize;				//�����С 0:QCIF,1:CIF,2:D1, �˲���ֻ��Գ���DVR�豸��Ч
	UINT mode;					//ץͼģʽ 0����ʾ����һ֡; 1����ʾ��ʱ��������; 2����ʾ��������,����DVR�豸��Ч��
	UINT InterSnap;				//ʱ�䵥λ�� ��mode=1��ʾ��ʱ��������ʱ����ʱ����Ч������ֻ�г���DVR��Ч��
	UINT CmdSerial;				//�������к�
	UINT Reserved[4];
}SNAP_PARAMS,*LPSNAP_PARAMS;

//���紥���豸ץͼ�ṹ��
typedef struct __NET_SNAP_COMMANDINFO 
{
	char			szCardInfo[16];		//������Ϣ
	char			reserved[64];		//����
} NET_SNAP_COMMANDINFO, LPNET_SNAP_COMMANDINFO;

//Э����Ϣ
typedef struct 
{
	char            protocolname[12]; //Э����
	unsigned int	baudbase;	      //������
	unsigned char	databits; 	      //����λ
	unsigned char	stopbits;	      //ֹͣλ
	unsigned char	parity;		      //У��λ
	unsigned char   reserve;
} PROTOCOL_INFO, *LPPROTOCOL_INFO;

//��Ƶ��ʽ��Ϣ
typedef struct {
	BYTE	byFormatTag;				//�������ͣ���0-PCM
	WORD	nChannels;				//������
	WORD	wBitsPerSample;			//�������			
	DWORD	nSamplesPerSec;			//������
} DH_AUDIO_FORMAT, *LPDH_AUDIO_FORMAT;

//��ѯ�豸����״̬ͨ����Ϣ:��ؽӿ�����̭���벻Ҫʹ��
typedef struct {
	BYTE byRecordStatic;		//ͨ���Ƿ���¼��,0-��¼��,1-�ֶ�¼��2-�Զ�¼��
	BYTE bySignalStatic;		//���ӵ��ź�״̬,0-����,1-�źŶ�ʧ
	BYTE byHardwareStatic;		//ͨ��Ӳ��״̬,0-����,1-�쳣,����DSP����
	char reserve;
	DWORD dwBitRate;			//ʵ������
	DWORD dwLinkNum;			//�ͻ������ӵĸ���
	DWORD dwClientIP[DH_MAX_LINK];	//�ͻ��˵�IP��ַ
}NET_DEV_CHANNELSTATE,*LPNET_DEV_CHANNELSTATE;

//��ѯ�豸����״̬:��ؽӿ�����̭���벻Ҫʹ��
typedef struct{
	DWORD dwDeviceStatic; 	//�豸��״̬,0x00 ����,0x01 CPUռ�ù���,0x02 Ӳ������
	NET_DEV_DISKSTATE  stHardDiskStatic[DH_MAX_DISKNUM]; 
	NET_DEV_CHANNELSTATE stChanStatic[DH_MAX_CHANNUM];//ͨ����״̬
	BYTE  byAlarmInStatic[DH_MAX_ALARMIN]; //�����˿ڵ�״̬,0-û�б���,1-�б���
	BYTE  byAlarmOutStatic[DH_MAX_ALARMOUT]; //��������˿ڵ�״̬,0-û�����,1-�б������
	DWORD  dwLocalDisplay;//������ʾ״̬,0-����,1-������
} NET_DEV_WORKSTATE,*LPNET_DEV_WORKSTATE;


/************************************************************************
** �����������ص�ԭ��
***********************************************************************/
typedef int (CALLBACK *fServiceCallBack)(LONG lHandle, char *pIp, WORD wPort, LONG lCommand, void *pParam, DWORD dwParamLen, DWORD dwUserData);

/************************************************************************
 ** �������Ͽ��ص�ԭ��
 ***********************************************************************/
typedef void (CALLBACK *fDisConnect)(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);

//	���������ɹ��ص�����
typedef void (CALLBACK *fHaveReConnect)(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);

/************************************************************************
 ** ʵʱԤ���ص�ԭ��
 ***********************************************************************/
typedef void(CALLBACK *fRealDataCallBack) (LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);

/************************************************************************
 ** ԭʼ���ݻص�ԭ��
 ***********************************************************************/
typedef int(CALLBACK *fDataCallBack) (LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);

/************************************************************************
 ** ��Ϣ���������ص�ԭ��
 ***********************************************************************/
typedef BOOL (CALLBACK *fMessCallBack)(LONG lCommand, LONG lLoginID, char *pBuf,
			DWORD dwBufLen, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);

/************************************************************************
 ** ��Ļ���ӻص�ԭ��
 ***********************************************************************/
typedef void (CALLBACK *fDrawCallBack)(LONG lLoginID, LONG lPlayHandle, HDC hDC, DWORD dwUser);

/************************************************************************
 ** ʵʱԤ���ص�ԭ��-��չ
 ***********************************************************************/
typedef void(CALLBACK *fRealDataCallBackEx) (LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG param, DWORD dwUser);

/************************************************************************
 ** �ط�¼����Ȼص�ԭ��
 ***********************************************************************/
typedef void(CALLBACK *fDownLoadPosCallBack) (LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, DWORD dwUser);

/************************************************************************
 ** ͨ��ʱ������¼����Ȼص�ԭ��
 ���ڰ�ʱ������û��ʹ������ص�������ʹ�õ��ǻط�¼����Ȼص�ԭ��. //by linjianyan
 ***********************************************************************/
typedef void(CALLBACK *fTimeDownLoadPosCallBack) (LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, DWORD dwUser);

/************************************************************************
 ** ��ѯ�豸��־���ݻص�ԭ��
 ***********************************************************************/
typedef void(CALLBACK *fLogDataCallBack) (LONG lLoginID, char *pBuffer, DWORD dwBufSize, DWORD nTotalSize, BOOL bEnd, DWORD dwUser);

/************************************************************************
 ** ͸�����ڻص�ԭ��
 ***********************************************************************/
typedef void(CALLBACK *fTransComCallBack) (LONG lLoginID, LONG lTransComChannel, char *pBuffer, DWORD dwBufSize, DWORD dwUser);

/************************************************************************
 ** �����豸����ص�ԭ��
 ***********************************************************************/
typedef void(CALLBACK *fUpgradeCallBack) (LONG lLoginID, LONG lUpgradechannel, int nTotalSize, int nSendSize, DWORD dwUser);

/************************************************************************
 ** �����Խ�����Ƶ���ݻص�ԭ��
 ***********************************************************************/
typedef void(CALLBACK *pfAudioDataCallBack) (LONG lTalkHandle, char *pDataBuf, DWORD dwBufSize, BYTE byAudioFlag, DWORD dwUser);

/***********************************************************************
** ץͼ�ص�ԭ��
***********************************************************************/
typedef void( CALLBACK *fSnapRev)(LONG lLoginID, BYTE *pBuf, UINT RevLen, UINT EncodeType, DWORD CmdSerial, DWORD dwUser);

/************************************************************************
 ** SDK��ʼ��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_Init(fDisConnect cbDisConnect, DWORD dwUser);

/************************************************************************
 ** SDK�˳�����
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_Cleanup();

/************************************************************************
 ** ���ö��������ɹ��ص����������ú�SDK�ڲ������Զ�����
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_SetAutoReconnect(fHaveReConnect cbAutoConnect, DWORD dwUser);

/************************************************************************
 ** ���غ���ִ��ʧ�ܴ���
 ***********************************************************************/
CLIENT_API DWORD CALL_METHOD CLIENT_GetLastError(void);

/************************************************************************
 ** �������ӷ�������ʱʱ��ͳ��Դ���
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_SetConnectTime(int nWaitTime, int nTryTimes);


/************************************************************************
 ** ��ȡSDK�İ汾��Ϣ
 ***********************************************************************/
CLIENT_API DWORD CALL_METHOD CLIENT_GetSDKVersion();


/************************************************************************
 ** ���豸ע��
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_Login(char *pchDVRIP, WORD wDVRPort, char *pchUserName, char *pchPassword, LPNET_DEVICEINFO lpDeviceInfo, int *error = 0);

//	nSpecCap = 0ΪTCP��ʽ�µĵ��룻nSpecCap = 2Ϊ����ע��ĵ��룻nSpecCap = 3Ϊ�鲥��ʽ�µĵ��룻nSpecCap = 4ΪUDP��ʽ�µĵ���
CLIENT_API LONG CALL_METHOD CLIENT_LoginEx(char *pchDVRIP, WORD wDVRPort, char *pchUserName, char *pchPassword, int nSpecCap, void* pCapParam, LPNET_DEVICEINFO lpDeviceInfo, int *error = 0);

/************************************************************************
 ** ���豸ע��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_Logout(LONG lLoginID);

/************************************************************************
 ** 
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_SetDVRMessCallBack(fMessCallBack cbMessage,DWORD dwUser);


/************************************************************************
 ** ���������Խ�ģʽ
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetDeviceMode(LONG lLoginID, EM_USEDEV_MODE emType, void* pValue);


/************************************************************************
 ** ��ʼ֡���豸
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StartListen(LONG lLoginID);

//��ӦLB�豸����汾
CLIENT_API BOOL CALL_METHOD CLIENT_StartListenEx(LONG lLoginID);

/************************************************************************
 ** ֹͣ֡���豸
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopListen(LONG lLoginID);

/************************************************************************
 ** ��ʼ�����������������ƶ�DVR����
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_ListenServer(char* ip, WORD port, int nTimeout, fServiceCallBack cbListen, DWORD dwUserData);

/************************************************************************
 ** ֹͣ������������ֹͣ�����ƶ�DVR����
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopListenServer(LONG lServerHandle);

/************************************************************************
 ** ��Ӧ�豸��ע������
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_ResponseDevReg(char *devSerial, char* ip, WORD port, BOOL bAccept);

/************************************************************************
 ** ��������˿�
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_StartService(WORD wPort, char *pIp = NULL, fServiceCallBack pfscb = NULL, 
												DWORD dwTimeOut = 0xffffffff, DWORD dwUserData = 0);

/************************************************************************
 ** ֹͣ����˿�
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopService(LONG lHandle);

/************************************************************************
 ** ������Ļ���ӻص�
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_RigisterDrawFun(fDrawCallBack cbDraw, DWORD dwUser);


/************************************************************************
 ** ��ʼʵʱԤ��
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_RealPlay(LONG lLoginID, int nChannelID, HWND hWnd);

CLIENT_API LONG CALL_METHOD CLIENT_RealPlayEx(LONG lLoginID, int nChannelID, HWND hWnd, DH_RealPlayType rType = DH_RType_Realplay);

CLIENT_API BOOL	CALL_METHOD CLIENT_AdjustFluency(LONG lRealHandle, int nLevel);

/************************************************************************
 ** ֹͣʵʱԤ��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopRealPlay(LONG lRealHandle);

/************************************************************************
 ** ֹͣʵʱԤ��_��չ
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopRealPlayEx(LONG lRealHandle);

/************************************************************************
 ** ��������Ϊ�ļ�
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SaveRealData(LONG lRealHandle, const char *pchFileName);


/************************************************************************
 ** ������������Ϊ�ļ�
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopSaveRealData(LONG lRealHandle);


/************************************************************************
 ** ����ͼ������
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_ClientSetVideoEffect(LONG lPlayHandle, unsigned char nBrightness, unsigned char nContrast, unsigned char nHue, unsigned char nSaturation);


/************************************************************************
 ** ��ȡͼ������
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_ClientGetVideoEffect(LONG lPlayHandle, unsigned char *nBrightness, unsigned char *nContrast, unsigned char *nHue, unsigned char *nSaturation);


/************************************************************************
 ** ������
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_OpenSound(LONG hPlayHandle);


/************************************************************************
 ** �ر�����
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_CloseSound();



/************************************************************************
 ** �������Խ�
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_StartTalkEx(LONG lLoginID, pfAudioDataCallBack pfcb, DWORD dwUser);

/************************************************************************
 ** ���������Խ�������
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetAudioClientVolume(LONG lTalkHandle, WORD wVolume);

/************************************************************************
 ** �����������ݵ��豸
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_TalkSendData(LONG lTalkHandle, char *pSendBuf, DWORD dwBufSize);

/************************************************************************
 ** ��ʼPC��¼��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_RecordStart();

/************************************************************************
 ** ����PC��¼��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_RecordStop();

/************************************************************************
 ** ������Ƶ����
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_AudioDec(char *pAudioDataBuf, DWORD dwBufSize);

/************************************************************************
 ** ֹͣ�����Խ�
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopTalkEx(LONG lTalkHandle);

/************************************************************************
 ** ����豸���㲥��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_AudioBroadcastAddDev(LONG lLoginID);

/************************************************************************
 ** �ӹ㲥����ɾ���豸
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_AudioBroadcastDelDev(LONG lLoginID);

/************************************************************************
 ** ��������
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetVolume(LONG lPlayHandle, int nVolume);

/************************************************************************
 ** ��Ƶ����--��ʼ��(�ض���׼��ʽ->�Զ����ʽ)
 ***********************************************************************/
CLIENT_API int  CALL_METHOD CLIENT_InitAudioEncode(DH_AUDIO_FORMAT aft);

/************************************************************************
 ** ��Ƶ����--���ݱ���
 ***********************************************************************/
CLIENT_API int	CALL_METHOD	CLIENT_AudioEncode(LONG lTalkHandle, BYTE *lpInBuf, DWORD *lpInLen, BYTE *lpOutBuf, DWORD *lpOutLen);

/************************************************************************
 ** ��Ƶ����--����˳�
 ***********************************************************************/
CLIENT_API int	CALL_METHOD	CLIENT_ReleaseAudioEncode();

/************************************************************************
 ** ����ʵʱԤ���ص�
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetRealDataCallBack(LONG lRealHandle, fRealDataCallBack cbRealData, DWORD dwUser);

/************************************************************************
 ** ����ʵʱԤ���ص�-��չ
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetRealDataCallBackEx(LONG lRealHandle, fRealDataCallBackEx cbRealData, DWORD dwUser, DWORD dwFlag);

/************************************************************************
 ** ץͼ
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_CapturePicture(LONG hPlayHandle, const char *pchPicFileName);

/************************************************************************
 ** ��ѯĳ��ʱ�����Ƿ���¼��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryRecordTime(LONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char* pchCardid, BOOL *bResult, int waittime=1000);

/************************************************************************
 ** ͨ��ʱ���ѯ¼���ļ�
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryRecordFile(LONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char* pchCardid, LPNET_RECORDFILE_INFO nriFileinfo, int maxlen, int *filecount, int waittime=1000, BOOL bTime = FALSE);

/************************************************************************
 ** �ط�¼���ļ�
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByRecordFile(LONG lLoginID, LPNET_RECORDFILE_INFO lpRecordFile, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwUserData);

/************************************************************************
 ** �ط�¼���ļ���չ_���������ݻص�����
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByRecordFileEx(LONG lLoginID, LPNET_RECORDFILE_INFO lpRecordFile, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwPosUser, fDataCallBack fDownLoadDataCallBack, DWORD dwDataUser);

/************************************************************************
 ** ��ͣ¼��ط�
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_PausePlayBack(LONG lPlayHandle, BOOL bPause);

/************************************************************************
 ** ��λ¼��ط���ʼ��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SeekPlayBack(LONG lPlayHandle, unsigned int offsettime, unsigned int offsetbyte);

/************************************************************************
 ** ֹͣ¼��ط�
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopPlayBack(LONG lPlayHandle);

/************************************************************************
 ** ����¼��ط�
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StepPlayBack(LONG lPlayHandle, BOOL bStop);

/************************************************************************
 ** ���¼��ط�
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_FastPlayBack(LONG lPlayHandle);

/************************************************************************
 ** ����¼��ط�
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SlowPlayBack(LONG lPlayHandle);

/************************************************************************
 ** �ָ������ط��ٶ�
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_NormalPlayBack(LONG lPlayHandle);

/************************************************************************
 ** ���ûط�֡��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetFramePlayBack(LONG lPlayHandle, int framerate);

/************************************************************************
 ** ��ȡ�ط�֡��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_GetFramePlayBack(LONG lPlayHandle, int *fileframerate, int *playframerate);

/************************************************************************
 ** ��ȡ�ط�OSDʱ��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_GetPlayBackOsdTime(LONG lPlayHandle, LPNET_TIME lpOsdTime, LPNET_TIME lpStartTime, LPNET_TIME lpEndTime);

/************************************************************************
 ** ����¼���ļ�
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_DownloadByRecordFile(LONG lLoginID,LPNET_RECORDFILE_INFO lpRecordFile, char *sSavedFileName, fDownLoadPosCallBack cbDownLoadPos, DWORD dwUserData);

/************************************************************************
 ** ͨ��ʱ������¼��
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_DownloadByTime(LONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char *sSavedFileName, fTimeDownLoadPosCallBack cbTimeDownLoadPos, DWORD dwUserData);

/************************************************************************
 ** ֹͣ¼������
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopDownload(LONG lFileHandle);

/************************************************************************
 ** ��ѯ¼�����ؽ���
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_GetDownloadPos(LONG lFileHandle, int *nTotalSize, int *nDownLoadSize);

/************************************************************************
 ** �໭��ʵʱԤ��
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_MultiPlay(LONG lLoginID, HWND hWnd);

/************************************************************************
 ** ֹͣ�໭��ʵʱԤ��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopMultiPlay(LONG lMultiHandle);

/************************************************************************
 ** ��ѯϵͳ���������� ���ýӿ�����̭����ʹ��CLIENT_GetDevConfig
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryConfig(LONG lLoginID, int nConfigType, char *pConfigbuf, int maxlen, int *nConfigbuflen, int waittime=1000);

/************************************************************************
 ** ����ϵͳ���������� ���ýӿ�����̭����ʹ��CLIENT_SetDevConfig
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetupConfig(LONG lLoginID, int nConfigType, char *pConfigbuf, int nConfigbuflen, int waittime=1000);

/************************************************************************
 ** ��ѯ�豸ͨ������
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryChannelName(LONG lLoginID, char *pChannelName, int maxlen, int *nChannelCount, int waittime=1000);

/************************************************************************
 ** �����豸ͨ������
 ***********************************************************************/
CLIENT_API BOOL  CALL_METHOD CLIENT_SetupChannelName(LONG lLoginID,char *pbuf, int nbuflen);

/************************************************************************
 ** �����豸ͨ���ַ�����
 ***********************************************************************/
CLIENT_API BOOL  CALL_METHOD CLIENT_SetupChannelOsdString(LONG lLoginID, int nChannelNo, DH_CHANNEL_OSDSTRING* struOsdString, int nbuflen);

/************************************************************************
 ** ͨ����̨����
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_PTZControl(LONG lLoginID, int nChannelID, DWORD dwPTZCommand, DWORD dwStep, BOOL dwStop);

/************************************************************************
 ** ����̨����
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_DHPTZControl(LONG lLoginID, int nChannelID, DWORD dwPTZCommand, 
                                    unsigned char param1, unsigned char param2, unsigned char param3, BOOL dwStop);

/************************************************************************
 ** ����̨������չ�ӿڣ�֧����ά���ٶ�λ
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_DHPTZControlEx(LONG lLoginID, int nChannelID, DWORD dwPTZCommand, 
                                    LONG lParam1, LONG lParam2, LONG lParam3, BOOL dwStop);

/************************************************************************
 ** �첽��ѯ�豸��־
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryLogCallback(LONG lLoginID, fLogDataCallBack cbLogData, DWORD dwUser);

/************************************************************************
 ** ͬ����ѯ�豸��־
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryLog(LONG lLoginID, char *pLogBuffer, int maxlen, int *nLogBufferlen, int waittime=3000);

CLIENT_API BOOL CALL_METHOD CLIENT_QueryLogEx(LONG lLoginID, DH_LOG_QUERY_TYPE logType, char *pLogBuffer, int maxlen, int *nLogBufferlen, void* reserved, int waittime=3000);

/************************************************************************
 ** ����͸������ͨ��
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_CreateTransComChannel(LONG lLoginID, int TransComType, 
                                      unsigned int baudrate, unsigned int databits,
                                      unsigned int stopbits, unsigned int parity,
                                      fTransComCallBack cbTransCom, DWORD dwUser);

/************************************************************************
 ** ͸�����ڷ�������
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SendTransComData(LONG lTransComChannel, char *pBuffer, DWORD dwBufSize);

/************************************************************************
 ** �ͷ�ͨ������ͨ��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_DestroyTransComChannel(LONG lTransComChannel);

/************************************************************************
 ** �ýӿ�����̭���벻Ҫʹ��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_Reset(LONG lLoginID, BOOL bReset);

/************************************************************************
 ** ��ʼ�����豸����
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_StartUpgrade(LONG lLoginID, char *pchFileName, fUpgradeCallBack cbUpgrade, DWORD dwUser);

CLIENT_API LONG CALL_METHOD CLIENT_StartUpgradeEx(LONG lLoginID, EM_UPGRADE_TYPE emType, char *pchFileName, fUpgradeCallBack cbUpgrade, DWORD dwUser);


/************************************************************************
 ** ��������
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SendUpgrade(LONG lUpgradeID);

/************************************************************************
 ** ���������豸����
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopUpgrade(LONG lUpgradeID);

/************************************************************************
 ** ���ͨ��¼��״̬
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryRecordState(LONG lLoginID, char *pRSBuffer, int maxlen, int *nRSBufferlen, int waittime=1000);

/************************************************************************
 ** ����ͨ��¼��״̬
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetupRecordState(LONG lLoginID, char *pRSBuffer, int nRSBufferlen);

/************************************************************************
 ** ��ѯ�豸��ǰʱ��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryDeviceTime(LONG lLoginID, LPNET_TIME pDeviceTime, int waittime=1000);

/************************************************************************
 ** �����豸��ǰʱ��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetupDeviceTime(LONG lLoginID, LPNET_TIME pDeviceTime);

/************************************************************************
 ** ��ѯ����Э�� �� �ýӿ�����̭����ʹ��CLIENT_GetDevConfig
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryComProtocol(LONG lLoginID, int nProtocolType, char *pProtocolBuffer, int maxlen, int *nProtocollen, int waittime=1000);

/************************************************************************
 ** ��ѯϵͳ��Ϣ �� �ýӿ�����̭����ʹ��CLIENT_GetDevConfig
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QuerySystemInfo(LONG lLoginID, int nSystemType, char *pSysInfoBuffer, int maxlen, int *nSysInfolen, int waittime=1000);


/************************************************************************
�����Ժ�����չ�ӿ�(��Щ�豸�û�����󳤶���16���ֽڵ�)
*************************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryUserInfoEx(LONG lLoginID, USER_MANAGE_INFO_EX *info, int waittime=1000);

/************************************************************************
 ** ��ѯ�豸�û�(�Ͻӿڡ�֧��8λ�û������ȡ�)
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryUserInfo(LONG lLoginID, USER_MANAGE_INFO *info, int waittime=1000);

/************************************************************************
 ** �����豸�û�
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_OperateUserInfo(LONG lLoginID, int nOperateType, void *opParam, void *subParam, int waittime=1000);
/************************************************************************
 ** �����豸�û����½ӿڣ�֧��8λ��16λ���û������볤�ȣ���
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_OperateUserInfoEx(LONG lLoginID, int nOperateType, void *opParam, void *subParam, int waittime=1000);

/************************************************************************
 ** ��ʼ�Խ� �� �ýӿ�����̭����ʹ��CLIENT_StartTalkEx
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StartTalk(LONG lRealHandle, BOOL bCustomSend=false);

/************************************************************************
 ** �����Խ� �� �ýӿ�����̭����ʹ��CLIENT_StopTalkEx
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopTalk(LONG lRealHandle);

/************************************************************************
 ** �����Զ���Խ����� �� �ýӿ�����̭����ʹ��CLIENT_TalkSendData
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SendTalkData_Custom(LONG lRealHandle, char *pBuffer, DWORD dwBufSize);

/************************************************************************
 ** ��ȡͨ������
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_GetStatiscFlux(LONG lLoginID, LONG lPlayHandle);

/************************************************************************
 ** ��ѯIO״̬
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryIOControlState(LONG lLoginID, DH_IOTYPE emType, 
                                           void *pState, int maxlen, int *nIOCount, int waittime=1000);

/************************************************************************
 ** IO����
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_IOControl(LONG lLoginID, DH_IOTYPE emType, void *pState, int maxlen);

/************************************************************************
 ** ��ѯ�豸����״̬ �ýӿ�����̭����ʹ��CLIENT_QueryDevState
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_GetDEVWorkState(LONG lLoginID, LPNET_DEV_WORKSTATE lpWorkState, int waittime=1000);

/************************************************************************
 ** ǿ��I֡
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_MakeKeyFrame(LONG lLoginID, int nChannelID, int nSubChannel=0);

/************************************************************************
 ** �����豸��������
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetMaxFlux(LONG lLoginID, WORD wFlux);

/************************************************************************
 ** ͨ���豸���ֻ����豸���к���DDNS��������ѯ�豸IP
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_GetDVRIPByResolveSvr(char *pchDVRIP, WORD wDVRPort, 
                                            BYTE *sDVRName, WORD wDVRNameLen, 
                                            BYTE *sDVRSerialNumber, WORD wDVRSerialLen, 
                                            char* sGetIP);

/************************************************************************
 ** ͨ��ʱ��ط�¼��
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByTime(LONG lLoginID, int nChannelID, 
                                      LPNET_TIME lpStartTime, LPNET_TIME lpStopTime, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwPosUser);

/************************************************************************
 ** ͨ��ʱ��ط�¼����չ_���������ݻص�����
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByTimeEx(LONG lLoginID, int nChannelID, 
                                      LPNET_TIME lpStartTime, LPNET_TIME lpStopTime, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwPosUser,
									  fDataCallBack fDownLoadDataCallBack, DWORD dwDataUser);

/************************************************************************
 ** ��ʼ����¼���ļ�
 ***********************************************************************/
CLIENT_API LONG	CALL_METHOD CLIENT_FindFile(LONG lLoginID,
								int nChannelId,
								int nRecordFileType, 
								char* cardid, 
								LPNET_TIME time_start,
								LPNET_TIME time_end,
								BOOL bTime, 
								int waittime);

/************************************************************************
 ** ����¼���ļ�
 ***********************************************************************/
CLIENT_API int	CALL_METHOD CLIENT_FindNextFile(LONG lFindHandle,LPNET_RECORDFILE_INFO lpFindData);

/************************************************************************
 ** ����¼���ļ�����
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_FindClose(LONG lFindHandle);

/************************************************************************
 ** �����豸������ֵ TRUE �ɹ��� FALSE ʧ��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_RebootDev(LONG lLoginID);

/************************************************************************
 ** �ر��豸������ֵ TRUE �ɹ��� FALSE ʧ��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_ShutDownDev(LONG lLoginID);

/************************************************************************
 ** �豸���ƣ�����ֵ TRUE �ɹ��� FALSE ʧ��
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_ControlDevice(LONG lLoginID, CtrlType type, void *param, int waittime = 1000);

/************************************************************************
 ** ������ѯ�豸״̬������������Ӳ�̡�¼��״̬�ȡ�
 ***********************************************************************/
//	�� nType  = DH_DEVSTATE_BURNING_PROGRESS ʱ,*pRetLen��ʾ��ѯ�豸��id��.��������ԭ����û����������ʹ�õ���ڲ�����
CLIENT_API BOOL CALL_METHOD CLIENT_QueryDevState(LONG lLoginID, int nType, char *pBuf, int nBufLen, int *pRetLen, int waittime=1000);

///////////////�����ǳ���dvr��صĹ��ܽӿ� /////////////////////////
/************************************************************************
 ** ����ץͼ�ص�����
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_SetSnapRevCallBack(fSnapRev OnSnapRevMessage, DWORD dwUser);

/************************************************************************
 ** ץͼ����
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SnapPicture(LONG lLoginID, SNAP_PARAMS par);

/************************************************************************
 ** GPS��Ϣ���Ļص�
 ***********************************************************************/
typedef void( CALLBACK *fGPSRev)(LONG lLoginID,           //�豸���
                                 GPS_Info  GpsInfo,       //GPS��Ϣ
                                 DWORD dwUserData);       //�û�����

/************************************************************************
 ** ����GPS���Ļص�����
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_SetSubcribeGPSCallBack(fGPSRev OnGPSMessage, DWORD dwUser);

/************************************************************************
 ** GPS��Ϣ����
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SubcribeGPS (
									LONG lLoginID,        //�豸���
									BOOL bStart,		  //�Ƿ�����
                                    LONG KeepTime,        //���ĳ���ʱ�䣨��λ�룩
                                    LONG InterTime);      //����ʱ����GPS����Ƶ�ʣ���λ�룩  

/////////////////////////end of ����dvr��صĹ��� ///////////////////////////////


/////////////////////////δʵ��//////////////////////////
/************************************************************************
 ** ����ʵʱԤ�������С
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetPlayerBufNumber(LONG lRealHandle, DWORD dwBufNum);

/************************************************************************
 ** ͨ��ʱ�������ļ���CLIENT_DownloadByTime��ʵ�ָù���
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_GetFileByTime(LONG lLoginID, int nChannelID, 
                                     LPNET_TIME lpStartTime, LPNET_TIME lpStopTime, char *sSavedFileName);

/************************************************************************
 ** ����طſ��ƣ���ϸ��Ϊ�����ƽӿ���
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_PlayBackControl(LONG lPlayHandle, 
                                       DWORD dwControlCode, DWORD dwInValue, DWORD *lpOutValue);

//////////////////////////end of δʵ��//////////////////////


/************************************************************************
** Զ�������豸�ӿ� �������ͼ�DH_DEV_DEVICECFG ����
***********************************************************************/
CLIENT_API BOOL  CALL_METHOD CLIENT_GetDevConfig(LONG lLoginID, DWORD dwCommand, LONG lChannel, LPVOID lpOutBuffer, DWORD dwOutBufferSize, LPDWORD lpBytesReturned,int waittime=500);
CLIENT_API BOOL  CALL_METHOD CLIENT_SetDevConfig(LONG lLoginID, DWORD dwCommand, LONG lChannel, LPVOID lpInBuffer, DWORD dwInBufferSize, int waittime=500);


/************************************************************************
 ** ƽ̨����ӿ�
 ***********************************************************************/
CLIENT_API BOOL  CALL_METHOD CLIENT_GetPlatFormInfo(LONG lLoginID, DWORD dwCommand, int nSubCommand, int nParam, LPVOID lpOutBuffer, DWORD dwOutBufferSize, LPDWORD lpBytesReturned,int waittime=500);
CLIENT_API BOOL  CALL_METHOD CLIENT_SetPlatFormInfo(LONG lLoginID, DWORD dwCommand, int nSubCommand, int nParam, LPVOID lpInBuffer, DWORD dwInBufferSize, int waittime=500);

/************************************************************************
**  ��ȡ��̨��Ϣ	
***********************************************************************/
CLIENT_API BOOL  CALL_METHOD CLIENT_GetPtzOptAttr(LONG lLoginID,DWORD dwProtocolIndex,LPVOID lpOutBuffer,DWORD dwBufLen,DWORD *lpBytesReturned,int waittime=500);

/******************************************************************************
** �����Ǹ��������ýṹ��
 *****************************************************************************/

/************************************************************************
 *	ϵͳ��Ϣ
************************************************************************/

/*�汾��Ϣ:��16λ��ʾ���汾�ţ���16λ��ʾ�ΰ汾��
 *����ʱ���������֯�ǣ�yyyymmdd */
typedef struct 
{
	DWORD	dwSoftwareVersion;
	DWORD	dwSoftwareBuildDate;
	DWORD	dwDspSoftwareVersion;
	DWORD	dwDspSoftwareBuildDate;
	DWORD	dwPanelVersion;
	DWORD	dwPanelSoftwareBuildDate;
	DWORD	dwHardwareVersion;
	DWORD	dwHardwareDate;
	DWORD	dwWebVersion;
	DWORD	dwWebBuildDate;
} DH_VERSION_INFO, *LPDH_VERSION_INFO;

/* DSP��������(CLIENT_GetDevConfig���)���Ժ�����CLIENT_QueryDevState�ӿ� */
typedef struct 
{
	DWORD	dwVideoStandardMask;	//��Ƶ��ʽ���룬��λ��ʾ�豸�ܹ�֧�ֵ���Ƶ��ʽ
	DWORD	dwImageSizeMask;		//�ֱ������룬��λ��ʾ�豸�ܹ�֧�ֵķֱ�������
	DWORD	dwEncodeModeMask;		//����ģʽ���룬��λ��ʾ�豸�ܹ�֧�ֵı���ģʽ����	
	
	DWORD	dwStreamCap;			//��λ��ʾ�豸֧�ֵĶ�ý�幦�ܣ�
									//��һλ��ʾ֧��������
									//�ڶ�λ��ʾ֧�ָ�����1
									//����λ��ʾ֧�ָ�����2
									//����λ��ʾ֧��jpgץͼ
	DWORD	dwImageSizeMask_Assi[8];//��ʾ������Ϊ���ֱ���ʱ��֧�ֵĸ������ֱ������롣

	DWORD	dwMaxEncodePower;		//- DSP ֧�ֵ���߱������� 
	WORD	wMaxSupportChannel;		//- ÿ�� DSP ֧�����������Ƶͨ���� 
	WORD	wChannelMaxSetSync;		//- DSP ÿͨ���������������Ƿ�ͬ�� 0-��ͬ��, 1-ͬ��
} DH_DSP_ENCODECAP, *LPDH_DSP_ENCODECAP;

/* �豸��Ϣ���ýṹ */
typedef struct 
{
	DWORD dwSize;
	/*�������豸��ֻ������*/
	DH_VERSION_INFO			stVersion;
	DH_DSP_ENCODECAP		stDspEncodeCap;		//DSP��������
	BYTE	szDevSerialNo[DH_DEV_SERIALNO_LEN];	//���к�
	BYTE	byDevType;							//�豸���ͣ���ö��DHDEV_DEVICE_TYPE
	BYTE	szDevType[DH_DEV_TYPE_LEN];			//�豸��ϸ�ͺţ��ַ�����ʽ������Ϊ��
	BYTE	byVideoCaptureNum;					//��Ƶ������
	BYTE	byAudioCaptureNum;					//��Ƶ������
	BYTE	byTalkInChanNum;					//NSP
	BYTE	byTalkOutChanNum;					//NSP
	BYTE	byDecodeChanNum;					//NSP
	BYTE	byAlarmInNum;						//�����������
	BYTE	byAlarmOutNum;						//�����������
	BYTE	byNetIONum;							//�������
	BYTE	byUsbIONum;							//USB������
	BYTE	byIdeIONum;							//IDE����
	BYTE	byComIONum;							//��������
	BYTE	byLPTIONum;							//��������
	BYTE	byVgaIONum;							//NSP
	BYTE	byIdeControlNum;					//NSP
	BYTE	byIdeControlType;					//NSP
	BYTE	byCapability;						//NSP����չ����
	BYTE	byMatrixOutNum;						//��Ƶ�����������

	/*�������豸�Ŀ�д����*/
	BYTE	byOverWrite;						//Ӳ��������ʽ�����ǡ�ֹͣ��
	BYTE	byRecordLen;						//¼��������
	BYTE	byStartChanNo;						//NSP
	WORD	wDevNo;								//�豸��ţ�����ң��
	BYTE	byVideoStandard;					//��Ƶ��ʽ
	BYTE	byDateFormat;						//���ڸ�ʽ
	BYTE	byDateSprtr;						//���ڷָ��(0-"."�� 1-"-"�� 2-"/")
	BYTE	byTimeFmt;							//ʱ���ʽ (0-24Сʱ��1��12Сʱ)
	BYTE	byLanguage;						    //ö��ֵ���DH_LANGUAGE_TYPE
} DHDEV_SYSTEM_ATTR_CFG, *LPDHDEV_SYSTEM_ATTR_CFG;

/************************************************************************
 *	ͼ��ͨ��
************************************************************************/

/* ¼�����ͣ���ʱ����̬��⡢���� */
typedef enum _REC_TYPE
{
	DH_REC_TYPE_TIM = 0,
	DH_REC_TYPE_MTD,
	DH_REC_TYPE_ALM,
	DH_REC_TYPE_NUM,
}REC_TYPE;

/* ʱ��νṹ */															    
typedef struct 
{
	BOOL	bEnable;	//����ʾ¼��ʱ���ʱ����λ��ʾ����ʹ�ܣ��ӵ�λ����λ�ֱ��ʾ����¼�󡢱���¼����ͨ¼��
	int		iBeginHour;
	int		iBeginMin;
	int		iBeginSec;
	int		iEndHour;
	int		iEndMin;
	int		iEndSec;
} DH_TSECT, *LPDH_TSECT;

/* ����:���߾ఴ����8192�ı��� */
typedef struct {
   long		left;
   long		top;
   long		right;
   long		bottom;
} DH_RECT,*LPDH_RECT;

/* OSD���Խṹ */
typedef struct  tagENCODE_WIDGET
{
	DWORD		rgbaFrontground;		/*!< �����ǰ��RGB����͸���� */
	DWORD		rgbaBackground;			/*!< ����ĺ�RGB����͸����*/
	DH_RECT		rcRect;					/*!< λ�� */
	BYTE		bShow;					/*!< �����ʾ */
	BYTE		byReserved[3];
} DH_ENCODE_WIDGET, *LPDH_ENCODE_WIDGET;

/* ͨ������Ƶ���� */
typedef struct 
{
	//��Ƶ����
	BYTE	byVideoEnable;				/* ��Ƶʹ��:1���򿪣�0���ر� */
	BYTE	byBitRateControl;			/* ��������,���ճ������� */
	BYTE	byFramesPerSec;				/* ֡�� */
	BYTE	byEncodeMode;				/* ����ģʽ,���ճ������� */
	BYTE	byImageSize;				/* �ֱ��ʲ�,���ճ������� */
	BYTE	byImageQlty;				/* ����1-6 */
	WORD	wLimitStream;				/* ����������, ��Χ��50~4*1024 (k) */

	//��Ƶ����
	BYTE	byAudioEnable;				/* ��Ƶʹ��:1���򿪣�0���ر� */
	BYTE	wFormatTag;					/* �������ͣ���PCM */
	WORD	nChannels;					/* ������ */
	WORD	wBitsPerSample;				/* ������� */		
	DWORD	nSamplesPerSec;				/* ������ */
	
	BYTE	bIFrameInterval;			/* I֡���֡��������������I֮֡���P֡������0-149*/
	char	reserved;
} DH_VIDEOENC_OPT, *LPDH_VIDEOENC_OPT;

/* ������ɫ���� */
typedef struct 
{
	DH_TSECT	stSect;
	BYTE		byBrightness;			/*! ����	0-100*/
	BYTE		byContrast;				/*! �Աȶ�	0-100*/
	BYTE		bySaturation;			/*!	���Ͷ�	0-100*/
	BYTE		byHue;					/*!	ɫ��	0-100*/
	BYTE		byGainEn;				/*! ����ʹ��	 */ 
	BYTE		byGain;					/*! ����	0-100*/
	BYTE		byReserved[2];
} DH_COLOR_CFG, *LPDH_COLOR_CFG;

/* ͼ��ͨ�����Խṹ�� */
typedef struct 
{
	DWORD				dwSize;
	char				szChannelName[DH_CHAN_NAME_LEN];
	DH_VIDEOENC_OPT		stMainVideoEncOpt[DH_REC_TYPE_NUM];
	DH_VIDEOENC_OPT		stAssiVideoEncOpt[DH_N_ENCODE_AUX];		
	DH_COLOR_CFG		stColorCfg[DH_N_COL_TSECT];
	DH_ENCODE_WIDGET	stTimeOSD;
	DH_ENCODE_WIDGET	stChannelOSD;
	DH_ENCODE_WIDGET	stBlindCover[DH_N_COVERS];
	BYTE				byBlindEnable;			/*! �����ڸǿ��ء�0x00��ʹ���ڸǣ�0x01���ڸ��豸����Ԥ����0x10���ڸ�¼�񣨼�����Ԥ������0x11���ڸ�	*/
	BYTE				byBlindMask;			/*! �����ڸ�����  ��һλ���豸����Ԥ�����ڶ�λ��¼��(������Ԥ��) */
	BYTE				byReserved[2];
} DHDEV_CHANNEL_CFG, *LPDHDEV_CHANNEL_CFG;


/************************************************************************
 *	�����ڵ�����
************************************************************************/
typedef struct __VIDEO_COVER_ATTR
{
	DH_RECT				rcBlock;			// ���ǵ���������
	int					nColor;				// ���ǵ���ɫ
	BYTE				bBlockType;			// ���Ƿ�ʽ��0-�ڿ飬1-������
	BYTE				bEncode;			// ���뼶�ڵ�, 1����Ч, 0��������
	BYTE				bPriview;           // Ԥ���ڵ�, 1����Ч, 0������Ч
	char				reserved[29];		// ����
}VIDEO_COVER_ATTR;

typedef struct __DHDEV_VIDEOCOVER_CFG 
{
	DWORD				dwSize;
	char				szChannelName[DH_CHAN_NAME_LEN];	// ֻ��
	BYTE				bTotalBlocks;		// ֧�ֵ��ڵ�����
	BYTE				bCoverCount;		// �����õ��ڵ�����
	VIDEO_COVER_ATTR	CoverBlock[DH_MAX_VIDEO_COVER_NUM];		// ���ǵ�����	
	char				reserved[30];		// ����
}DHDEV_VIDEOCOVER_CFG;


/************************************************************************
 *	Ԥ��ͼ�����
************************************************************************/

/* Ԥ��ͼ����� */
typedef struct 
{
	DWORD dwSize;
	DH_VIDEOENC_OPT	stPreView;
	DH_COLOR_CFG  stColorCfg[DH_N_COL_TSECT];
}DHDEV_PREVIEW_CFG;

/************************************************************************
* �����Խ���Ƶ���Խṹ�壭δʵ�֣�����չ *
************************************************************************/

typedef struct 
{
	//��Ƶ�������
	BYTE	byInFormatTag;					//�������ͣ���PCM
	BYTE	byInChannels;					//������
	WORD	wInBitsPerSample;				//�������					
	DWORD	dwInSamplesPerSec;				//������

	//��Ƶ�������
	BYTE	byOutFormatTag;					//�������ͣ���PCM
	BYTE	byOutChannels;					//������
	WORD	wOutBitsPerSample;				//�������				
	DWORD	dwOutSamplesPerSec;				//������
} DHDEV_TALK_CFG, *LPDHDEV_TALK_CFG;

/************************************************************************
 *	��ʱ¼��
************************************************************************/

typedef struct 
{
	DWORD		dwSize;
	DH_TSECT	stSect[DH_N_WEEKS][DH_N_REC_TSECT];
	BYTE		byPreRecordLen;					/*! Ԥ¼ʱ��,��λ��s,0��ʾ��Ԥ¼*/
	BYTE		byRedundancyEn;					/*! ¼�����࿪��*/
	BYTE		byReserved[2];
} DHDEV_RECORD_CFG, *LPDH_RECORD_CFG;

/************************************************************************
 *	��������
************************************************************************/

typedef struct 
{
	int iType;
	int iValue;
} DH_PTZ_LINK, LPDH_PTZ_LINK;

typedef struct 
{
	/******************************************************************************
	** ��Ϣ��������
	**
	** ���豸������Ϣ,Ʃ�����һ�����뱨��,������Ƶ��ʧʱ,��Ҫ����Ӧ�Ĵ���.
	** �˴�������δ���һ����Ϣ
	*****************************************************************************/

	/* ��Ϣ����ʽ,����ͬʱ���ִ���ʽ,����
	 * 0x00000001 - ����:�ϴ����������
	 * 0x00000002 - ¼��:����
	 * 0x00000004 - ��̨����
	 * 0x00000008 - �����ʼ�
	 * 0x00000010 - �豸���ر�����Ѳ
	 * 0x00000020 - �豸��ʾʹ��
	 * 0x00000040 - �豸�������ʹ��
	 * 0x00000080 - Ftp�ϴ�ʹ��
	 * 0x00000100 - ����
	 * 0x00000200 - ������ʾ
	 * 0x00000400 - ץͼʹ��*/

	/*��ǰ������֧�ֵĴ���ʽ����λ�����ʾ*/
	DWORD		dwActionMask;

	/* ������������λ�����ʾ�����嶯������Ҫ�Ĳ����ڸ��Ե�����������*/
	DWORD		dwActionFlag;
	
	/* �������������ͨ��,�������������,Ϊ 1 ��ʾ���������  */ 
	BYTE		byRelAlarmOut[DH_MAX_ALARMOUT_NUM];
	DWORD		dwDuration;		/* ��������ʱ�� */

	/*����¼��*/
	BYTE		byRecordChannel[DH_MAX_VIDEO_IN_NUM];		/* ����������¼��ͨ��,Ϊ1��ʾ������ͨ�� */
	DWORD		dwRecLatch;		/* ¼�����ʱ�� */

	/*ץͼͨ��*/
	BYTE		bySnap[DH_MAX_VIDEO_IN_NUM];
	/*��Ѳͨ��*/
	BYTE		byTour[DH_MAX_VIDEO_IN_NUM];

	/*  ��̨����: */
	DH_PTZ_LINK	struPtzLink[DH_MAX_VIDEO_IN_NUM];
	DWORD		dwEventLatch;   /*!< ������ʼ��ʱʱ�䣬sΪ��λ ��Χ��0����15 Ĭ��ֵ��0*/
	/* �����������������ͨ��,�������������,Ϊ 1 ��ʾ���������  */ 
	BYTE		byRelWIAlarmOut[DH_MAX_ALARMOUT_NUM];
	BYTE		bMessageToNet;	//��Ϣ�ϴ�������ʹ��
	BYTE		byReserved[111];       
} DH_MSG_HANDLE;

/* �ⲿ���� */
typedef struct
{
	BYTE		byAlarmType;						/* ����������,0������,1������  */
	BYTE		byAlarmEn;							/*����ʹ��*/
	BYTE		byReserved[2];						
	DH_TSECT	stSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
	DH_MSG_HANDLE	struHandle;	/* ����ʽ  */
} DH_ALARMIN_CFG,*LPDHDEV_ALARMIN_CFG; 

/* ��̬��ⱨ�� */
typedef struct 
{
	BYTE			byMotionEn;							/* ��̬��ⱨ��ʹ��*/
	BYTE			byReserved;
	WORD			wSenseLevel;						/*������*/
	WORD			wMotionRow;							/* ��̬������������ */
	WORD			wMotionCol;							/* ��̬������������ */
	BYTE			byDetected[DH_MOTION_ROW][DH_MOTION_COL];	/*����������32*32������*/
	DH_TSECT		stSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
	DH_MSG_HANDLE	struHandle;	/*����ʽ*/
}DH_MOTION_DETECT_CFG;

/* ��Ƶ��ʧ���� */
typedef struct
{
	BYTE			byAlarmEn;							/*��Ƶ��ʧ����ʹ��*/
	BYTE			byReserved[3];
	DH_TSECT		stSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
	DH_MSG_HANDLE	struHandle;	/* ����ʽ  */
} DH_VIDEO_LOST_CFG;

/* ͼ���ڵ����� */
typedef struct
{
	BYTE			byBlindEnable;						/* ʹ�� */
	BYTE			byBlindLevel;						/* ������1-6 */
	BYTE			byReserved[2];
	DH_TSECT	stSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
	DH_MSG_HANDLE	struHandle;	/* ����ʽ  */
} DH_BLIND_CFG;

/* Ӳ����Ϣ(�ڲ�����) */
typedef struct 
{
	BYTE				byNoDiskEn;				/* ��Ӳ��ʱ���� */
	BYTE				byReserved_1[3];
	DH_TSECT			stNDSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
	DH_MSG_HANDLE		struNDHandle;				/* ����ʽ  */

	BYTE				byLowCapEn;				/* Ӳ�̵�����ʱ����*/
	BYTE				byLowerLimit;			/* ������ֵ 0-99*/
	BYTE				byReserved_2[2];
	DH_TSECT			stLCSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
	DH_MSG_HANDLE		struLCHandle;			/* ����ʽ  */

	BYTE				byDiskErrEn;			/* Ӳ�̹��ϱ���*/
	BYTE				byReserved_3[3];
	DH_TSECT			stEDSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
	DH_MSG_HANDLE		struEDHandle;	/* ����ʽ  */

} DH_DISK_ALARM_CFG;

typedef struct
{
	BYTE				byEnable;
	BYTE				byReserved[3];
	DH_MSG_HANDLE		struHandle;
} DH_NETBROKEN_ALARM_CFG;
///*��Ƶ����*/
//typedef struct 
//{
//	BYTE		bySoundAlarmEn;			/*��Ƶ����ʹ��*/
//	BYTE		byVol;					/*����������ֵ*/
//	BYTE		byReserved[2];			/*�����ֽ�*/
//	DH_TSECT	stSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
//	DH_MSG_HANDLE	struHandle;	/* ����ʽ  */
//}DH_SOUND_ALARM_CFG;


/* �������� */
typedef struct
{
	DWORD dwSize;
	
	DH_ALARMIN_CFG 		struLocalAlmIn[DH_MAX_ALARM_IN_NUM];
	DH_ALARMIN_CFG		struNetAlmIn[DH_MAX_ALARM_IN_NUM];
	DH_MOTION_DETECT_CFG struMotion[DH_MAX_VIDEO_IN_NUM];
	DH_VIDEO_LOST_CFG	struVideoLost[DH_MAX_VIDEO_IN_NUM];
	DH_BLIND_CFG		struBlind[DH_MAX_VIDEO_IN_NUM];
	DH_DISK_ALARM_CFG	struDiskAlarm;
	DH_NETBROKEN_ALARM_CFG	struNetBrokenAlarm;
//	DH_SOUND_ALARM_CFG	struSoundAlarm[DH_MAX_VIDEO_IN_NUM];
	//���ܻ����豸�쳣
} DHDEV_ALARM_SCHEDULE;

/************************************************************************
 *	��������
************************************************************************/
/*                                                                      */
/*��̫������*/
typedef struct 
{
	char	sDevIPAddr[DH_MAX_IPADDR_LEN];			/* DVR IP ��ַ  */ 
	char	sDevIPMask[DH_MAX_IPADDR_LEN];			/* DVR IP ��ַ����  */ 
	char	sGatewayIP[DH_MAX_IPADDR_LEN];			/* ���ص�ַ  */ 

	/* 10M/100M  ����Ӧ,���� 
	 * 1-10MBase - T
	 * 2-10MBase-T ȫ˫�� 
	 * 3-100MBase - TX
	 * 4-100M ȫ˫��
	 * 5-10M/100M  ����Ӧ 
	 */
	//Ϊ����չ��DWORD����ĸ�
	BYTE	dwNetInterface;							/* NSP */
	BYTE	bTranMedia;								/* 0������ 1������ */
	BYTE	bValid;									/* ��λ��ʾ����һλ��1����Ч 0����Ч���ڶ�λ��0��DHCP�ر� 1��DHCPʹ�ܣ�����λ��0����֧��DHCP 1��֧��DHCP */
	BYTE	bDefaultEth;							/* �Ƿ���ΪĬ�ϵ����� 1��Ĭ�� 0����Ĭ�� */
	char	byMACAddr[DH_MACADDR_LEN];				/* MAC��ַ��ֻ�� */
} DH_ETHERNET; 

/* Զ���������� */
typedef struct 
{
	BYTE	byEnable;								/* ����ʹ��  */
	BYTE	byReserved;
	WORD	wHostPort;								/* Զ������ �˿�  */
	char	sHostIPAddr[DH_MAX_IPADDR_LEN];			/* Զ������ IP ��ַ  */ 		
	char	sHostUser[DH_MAX_HOST_NAMELEN];			/* Զ������ �û��� */ 
	char	sHostPassword[DH_MAX_HOST_PSWLEN];		/* Զ������ ���� */ 
}DH_REMOTE_HOST;

/* �ʼ����� */
typedef struct 
{
	char	sMailIPAddr[DH_MAX_IPADDR_LEN];			/*�ʼ�������IP��ַ*/
	WORD	wMailPort;								/*�ʼ��������˿�*/
	WORD	wReserved;								/* ���� */
	char	sSenderAddr[DH_MAX_MAIL_ADDR_LEN];		/*���͵�ַ*/
	char	sUserName[DH_MAX_NAME_LEN];			/* �û��� */
	char	sUserPsw[DH_MAX_NAME_LEN];				/* �û����� */
	char	sDestAddr[DH_MAX_MAIL_ADDR_LEN];			/*Ŀ�ĵ�ַ*/
	char	sCcAddr[DH_MAX_MAIL_ADDR_LEN];			/*���͵�ַ*/
	char	sBccAddr[DH_MAX_MAIL_ADDR_LEN];			/*������ַ*/
	char	sSubject[DH_MAX_MAIL_SUBJECT_LEN];		/*����*/
}DH_MAIL_CFG;

/*�������ýṹ��*/
typedef struct
{ 
	DWORD				dwSize; 

	char				sDevName[DH_MAX_NAME_LEN];				/* �豸������ */

	WORD				wTcpMaxConnectNum;						/* TCP���������(һ��ָ��Ƶ����������) */
	WORD				wTcpPort;								/* TCP֡���˿� */
	WORD				wUdpPort;								/* UDP�����˿� */
	WORD				wHttpPort;								/* HTTP�˿ں� */
	WORD				wHttpsPort;							/* HTTPS�˿ں� */
	WORD				wSslPort;								/* SSL�˿ں�*/
	DH_ETHERNET			stEtherNet[DH_MAX_ETHERNET_NUM];/* ��̫����  */ 

	DH_REMOTE_HOST		struAlarmHost;		/* ���������� */
	DH_REMOTE_HOST		struLogHost;		/* ��־������ */
	DH_REMOTE_HOST		struSmtpHost;		/* SMTP������ */
	DH_REMOTE_HOST		struMultiCast;		/* �ಥ�� */
	DH_REMOTE_HOST		struNfs;			/* NFS������ */
	DH_REMOTE_HOST		struPppoe;			/* PPPoE������ */
	char				sPppoeIP[DH_MAX_IPADDR_LEN]; /* PPPoEע�᷵�ص�IP */
	DH_REMOTE_HOST		struDdns;			/* DDNS������ */
	char				sDdnsHostName[DH_MAX_HOST_NAMELEN];		/* DDNS������*/
	DH_REMOTE_HOST		struDns;			/* DNS������*/
	DH_MAIL_CFG			struMail;			/* �ʼ����� */
} DHDEV_NET_CFG;

/************************************************************************
 *	��������
************************************************************************/

/* ���ڻ������� */
typedef struct
{
	BYTE	byDataBit;		//����λ 0-5,1-6,2-7,3-8
	BYTE	byStopBit;		//ֹͣλ 0-1λ, 1-1.5λ, 2-2λ 
	BYTE	byParity;		//У��λ 0-no, 1-odd, 2-even 
	BYTE	byBaudRate;	//{0-300,1-600,2-1200,3-2400,4-4800
	// 5-9600,6-19200,7-38400,8-57600,9-115200} 
} DH_COMM_PROP;

/* 485���������� */
typedef struct
{ 
	DH_COMM_PROP	struComm;

	BYTE			wProtocol;			/*!< Э������ ����Э����±꣬��̬�仯 */
	BYTE			byReserved;			/*!����  */	
	BYTE			wDecoderAddress;	/*��������ַ:0 - 255*/ 
	BYTE 			byMartixID;			/*�����*/
} DH_485_CFG; 

/* 232�������� */
typedef struct
{
	DH_COMM_PROP	struComm;
	BYTE			byFunction;/*!< ���ڹ��ܣ���Ӧ��������ȡ���Ĺ������б� */
	BYTE			byReserved[3];
} DH_RS232_CFG;

/* �������ýṹ�� */
typedef struct
{
	DWORD			dwSize;
	//������Э��
	DWORD			dwDecProListNum;				//Э�����
	char			DecProName[DH_MAX_DECPRO_LIST_SIZE][DH_MAX_NAME_LEN]; //Э�����б�
	DH_485_CFG		stDecoder[DH_MAX_DECODER_NUM];		//����������ǰ����

	DWORD			dw232FuncNameNum;				//232���ܸ���
	char			s232FuncName[DH_MAX_232FUNCS][DH_MAX_NAME_LEN];	//�������б�
	DH_RS232_CFG	st232[DH_MAX_232_NUM];		//��232���ڵ�ǰ����
} DHDEV_COMM_CFG;


/************************************************************************
 *	�Զ�ά������
 ***********************************************************************/
/* �Զ�ά������ */
typedef struct
{
	DWORD dwSize;

	//�Զ�����
	BYTE byAutoRebootDay; // 0-�Ӳ�, 1-ÿ��, 2-ÿ������, 3-ÿ����һ,.....
	BYTE byAutoRebootTime;  // 0-0:00 1-1:00,........23-:23:00 */
	//�Զ�ɾ���ļ�
	BYTE byAutoDeleteFilesTime; /*! 0-�Ӳ�, 1-24H, 2-48H, 3-72H, 4-96H, 5-ONE WEEK, 6-ONE MONTH */

	BYTE reserved[13]; // ����λ
} DHDEV_AUTOMT_CFG;

/************************************************************************
 * ����������Ʋ�������
 ************************************************************************/

typedef struct
{
	BOOL		bTourEnable;			//	��Ѳʹ��
	int			nTourPeriod;			//	��Ѳ�������λ��, 5-300 
	DWORD		dwChannelMask;			//	��Ѳ��ͨ����������ʽ��ʾ
	char		reserved[64];
}DH_VIDEOGROUP_CFG;

typedef struct
{
	DWORD	dwSize;
	int		nMatrixNum;			//	�������(ע���������޸�)
	DH_VIDEOGROUP_CFG struVideoGroup[DH_MATRIX_MAXOUT];
	char	reserved[32];
}DHDEV_VIDEO_MATRIX_CFG;   

/************************************************************************
 *	��ddns���ýṹ��
************************************************************************/

typedef struct
{
	DWORD	dwId;				//ddns������id��
	BOOL	bEnable;			//ʹ�ܣ�ͬһʱ��ֻ����һ��ddns����������ʹ��״̬
	char	szServerType[DH_MAX_SERVER_TYPE_LEN];	//���������ͣ�ϣ��..
	char	szServerIp[DH_MAX_DOMAIN_NAME_LEN];		//������ip��������
	DWORD	dwServerPort;						//�������˿�
	char	szDomainName[DH_MAX_DOMAIN_NAME_LEN];	//dvr��������jeckean.3322.org
	char	szUserName[DH_MAX_HOST_NAMELEN];	//�û���
	char	szUserPsw[DH_MAX_HOST_PSWLEN];		//����
	char	szAlias[DH_MAX_DDNS_ALIAS_LEN];		//��������������"dahua inter ddns"
	DWORD	dwAlivePeriod;						//DDNS ����ʱ��
	char	reserved[256];
}DH_DDNS_SERVER_CFG;

typedef struct
{
	DWORD	dwSize;
	DWORD	dwDdnsServerNum;	
	DH_DDNS_SERVER_CFG struDdnsServer[DH_MAX_DDNS_NUM];	
}DHDEV_MULTI_DDNS_CFG;


/************************************************************************
 *   �ʼ����ýṹ��
 ************************************************************************/
typedef struct 
{
	char	sMailIPAddr[DH_MAX_DOMAIN_NAME_LEN];	/*�ʼ���������ַ(IP��������)*/
	char	sSubMailIPAddr[DH_MAX_DOMAIN_NAME_LEN];
	WORD	wMailPort;								/*�ʼ��������˿�*/
	WORD	wSubMailPort;
	WORD	wReserved;								/*����*/
	char	sSenderAddr[DH_MAX_MAIL_ADDR_LEN];		/*���͵�ַ*/
	char	sUserName[DH_MAX_NAME_LEN];				/*�û���*/
	char	sUserPsw[DH_MAX_NAME_LEN];				/*�û�����*/
	char	sDestAddr[DH_MAX_MAIL_ADDR_LEN];		/*Ŀ�ĵ�ַ*/
	char	sCcAddr[DH_MAX_MAIL_ADDR_LEN];			/*���͵�ַ*/
	char	sBccAddr[DH_MAX_MAIL_ADDR_LEN];			/*������ַ*/
	char	sSubject[DH_MAX_MAIL_SUBJECT_LEN];		/*����*/
	char	reserved[256];
} DHDEV_MAIL_CFG;


/************************************************************************
 *   �豸����汾��Ϣ
 ************************************************************************/
typedef struct  
{
	char	szDevSerialNo[DH_DEV_SERIALNO_LEN];		//���к�
	char	byDevType;								//�豸���ͣ���ö��DHDEV_DEVICE_TYPE
	char	szDevType[DH_DEV_TYPE_LEN];				//�豸��ϸ�ͺţ��ַ�����ʽ������Ϊ��
	int		nProtocalVer;							//Э��汾��
	char	szSoftWareVersion[DH_MAX_URL_LEN];
	DWORD	dwSoftwareBuildDate;
	char	szDspSoftwareVersion[DH_MAX_URL_LEN];
	DWORD	dwDspSoftwareBuildDate;
	char	szPanelVersion[DH_MAX_URL_LEN];
	DWORD	dwPanelSoftwareBuildDate;
	char	szHardwareVersion[DH_MAX_URL_LEN];
	DWORD	dwHardwareDate;
	char	szWebVersion[DH_MAX_URL_LEN];
	DWORD	dwWebBuildDate;
	char	reserved[256];
} DHDEV_VERSION_INFO;


/************************************************************************
 *   DSP��������
 ************************************************************************/
typedef struct 
{
	DWORD	dwVideoStandardMask;	//��Ƶ��ʽ���룬��λ��ʾ�豸�ܹ�֧�ֵ���Ƶ��ʽ
	DWORD	dwImageSizeMask;		//�ֱ������룬��λ��ʾ�豸�ܹ�֧�ֵķֱ�������
	DWORD	dwEncodeModeMask;		//����ģʽ���룬��λ��ʾ�豸�ܹ�֧�ֵı���ģʽ����	
	
	DWORD	dwStreamCap;			//��λ��ʾ�豸֧�ֵĶ�ý�幦�ܣ�
									//��һλ��ʾ֧��������
									//�ڶ�λ��ʾ֧�ָ�����1
									//����λ��ʾ֧�ָ�����2
									//����λ��ʾ֧��jpgץͼ
	DWORD	dwImageSizeMask_Assi[32];//��ʾ������Ϊ���ֱ���ʱ��֧�ֵĸ������ֱ������롣

	DWORD	dwMaxEncodePower;		//- DSP ֧�ֵ���߱������� 
	WORD	wMaxSupportChannel;		//- ÿ�� DSP ֧�����������Ƶͨ���� 
	WORD	wChannelMaxSetSync;		//- DSP ÿͨ���������������Ƿ�ͬ�� 0-��ͬ��, 1-ͬ��

	char	reserved[128];
} DHDEV_DSP_ENCODECAP, *LPDHDEV_DSP_ENCODECAP;


/************************************************************************
 *   ץͼ��������
 ************************************************************************/
typedef struct 
{
	DWORD	dwSize;
	BOOL	bTimingEnable;		//��ʱץͼ���أ�����ץͼ�����ڸ������������������֣�
	DH_VIDEOENC_OPT struSnapEnc[SNAP_TYP_NUM];	//ץͼ�������ã���֧�����еķֱ��ʡ����ʡ�֡�����ã�֡���������Ǹ�������ʾһ��ץͼ�Ĵ�����
}DHDEV_SNAP_CFG;

/************************************************************************
 *   web·������
 ************************************************************************/
typedef struct 
{
	DWORD	dwSize;
	BOOL	bSnapEnable;	//�Ƿ�ץͼ
	int		iSnapInterval;	//ץͼ����
	char	szHostIp[DH_MAX_IPADDR_LEN]; /* HTTP����IP */
	WORD	wHostPort;
	int		iMsgInterval;	//״̬��Ϣ���ͼ��
	char	szUrlState[DH_MAX_URL_LEN];		//״̬��Ϣ�ϴ�URL
	char	szUrlImage[DH_MAX_URL_LEN];		//ͼƬ�ϴ�Url
	char	szDevId[DH_MAX_DEV_ID_LEN];		//������web���
	BYTE	byReserved[2];
}DHDEV_URL_CFG;

/************************************************************************
 *   ͼ��ˮӡ����
 ************************************************************************/
typedef struct __DHDEV_WATERMAKE_CFG 
{
	DWORD				dwSize;
	int					nEnable;			//	ʹ��
	int					nStream;			//	������1��n��0-��������
	int					nKey;				//	��������(1-���֣�2-ͼƬ)
	char				szLetterData[DH_MAX_WATERMAKE_LETTER];	//	����
	char				szData[DH_MAX_WATERMAKE_DATA];		//	ͼƬ����
	BYTE				bReserved[512];		//	����
} DHDEV_WATERMAKE_CFG;

/************************************************************************
 *   FTP�ϴ�����
 ************************************************************************/
typedef struct
{
	struct
	{
		DH_TSECT	struSect;	//��ʱ����ڵġ�ʹ�ܡ���Ч���ɺ���
		BOOL		bMdEn;		//�ϴ���̬���¼��
		BOOL		bAlarmEn;	//�ϴ��ⲿ����¼��
		BOOL		bTimerEn;	//�ϴ���ͨ��ʱ¼��
		DWORD		dwRev[4];
	} struPeriod[DH_TIME_SECTION];
}DH_FTP_UPLOAD_CFG;

typedef struct
{
	DWORD				dwSize;
	BOOL				bEnable;		//�Ƿ�����
	char				szHostIp[DH_MAX_IPADDR_LEN];		//����IP
	WORD				wHostPort;		//�����˿�
	char				szDirName[DH_FTP_MAX_PATH];			//FTPĿ¼·��
	char				szUserName[DH_FTP_USERNAME_LEN];	//�û���
	char				szPassword[DH_FTP_PASSWORD_LEN];	//����
	int					iFileLen;					//�ļ�����
	int					iInterval;					//�����ļ�ʱ����
	DH_FTP_UPLOAD_CFG	struUploadCfg[DH_MAX_CHANNUM][DH_N_WEEKS];
	BYTE 				reserved[130];
}DHDEV_FTP_PROTO_CFG;

// DNS����������
typedef struct  
{
	char				szPrimaryIp[DH_MAX_IPADDR_LEN];
	char				szSecondaryIp[DH_MAX_IPADDR_LEN];
	char				reserved[256];
}DHDEV_DNS_CFG;


/************************************************************************
 *   ¼�����ز�������
 ************************************************************************/
typedef struct
{
	DWORD dwSize;
	
	BOOL bEnable;	//TRUE���������أ�FALSE-��ͨ����
}DHDEV_DOWNLOAD_STRATEGY_CFG;

/************************************************************************
 *   ���紫���������
 ************************************************************************/
typedef struct
{
	DWORD dwSize;
	
	BOOL bEnable;
	int  iStrategy;	//0-�������ȣ� 1-���������ȣ� 2-�Զ���
}DHDEV_TRANSFER_STRATEGY_CFG;


/************************************************************************
 *	IPC��һЩ����
************************************************************************/
//	��������������Ϣ
typedef struct 
{
	int					nEnable;			// ����ʹ��
	char				szSSID[36];			// SSID
	int					nLinkMode;			// ����ģʽ 0:auto 1:adhoc 2:Infrastructure
	int					nEncryption;		// ���� 0: off 2:WEP64bit 3: WEP128bit
	int					nKeyType;			// 0:Hex 1:ASCII
    int					nKeyID;				// ���
	char				szKeys[4][32];		// ��������
	int					nKeyFlag;
	char				reserved[12];
} DHDEV_WLAN_INFO;

//	ѡ��ʹ��ĳ�������豸
typedef struct  
{
	char				szSSID[36];
	int					nLinkMode;			// ����ģʽ 0:adhoc 1:Infrastructure
	int 				nEncryption;		// ���� 0: off  2:WEP64bit 3: WEP128bit
	char				reserved[48];
} DHDEV_WLAN_DEVICE;

//	�������������豸�б�
typedef struct  
{
	DWORD				dwSize;
	BYTE				bWlanDevCount;		// �������������豸����
	DHDEV_WLAN_DEVICE	lstWlanDev[DH_MAX_WLANDEVICE_NUM];
	char				reserved[255];
} DHDEV_WLAN_DEVICE_LIST;

//	����ע���������
typedef struct  
{
	char				szServerIp[32];		//ע�������IP
	int					nServerPort;		//�˿ں�
	char				reserved[64];
} DHDEV_SERVER_INFO;

typedef struct  
{
	DWORD				dwSize;
	BYTE				bServerNum;         //֧�ֵ����ip��
	DHDEV_SERVER_INFO	lstServer[DH_MAX_REGISTER_SERVER_NUM];
	BYTE				bEnable;			//ʹ��
	char				szDeviceID[32];		//�豸id��
	char				reserved[94];
} DHDEV_REGISTER_SERVER;

//	����ͷ����
typedef struct __DHDEV_CAMERA_INFO
{
	BYTE				bBrightnessEn;		// ���ȿɵ� 1���ɣ�0������
	BYTE				bContrastEn;		// �Աȶȿɵ�
	BYTE				bColorEn;			// ɫ�ȿɵ�
	BYTE				bGainEn;			// ����ɵ�
	BYTE				bSaturationEn;		// ���Ͷȿɵ�
	BYTE				bBacklightEn;		// ���ⲹ���ɵ�
	BYTE				bExposureEn;		// �ع�ѡ��ɵ�
	BYTE				bColorConvEn;		// �Զ��ʺ�ת���ɵ�
	BYTE				bAttrEn;			// ����ѡ�� 1���ɣ�0������
	BYTE				bMirrorEn;			// ���� 1��֧�֣�0����֧��
    BYTE				bFlipEn;			// ��ת 1��֧�֣�0����֧��
	BYTE				bRev[125];			// ����
} DHDEV_CAMERA_INFO;

//	����ͷ��������
typedef struct __DHDEV_CAMERA_CFG 
{
	DWORD				dwSize;
	BYTE				bExposure;			//�ع�ģʽ 1-9:�ֶ��ع�ȼ�; 0:�Զ��ع�
	BYTE				bBacklight;			//���ⲹ�� 3:ǿ��2���У�1������ 0:��
	BYTE				bAutoColor2BW;		//��/ҹģʽ 2:����1���Զ�; 0:��
	BYTE				bMirror;			//����  1 ���� 0��
	BYTE				bFlip;				//��ת  1 ���� 0��
	char				bRev[123];			//����
} DHDEV_CAMERA_CFG;

#define ALARM_MAX_NAME 64
//(����)���ⱨ������
typedef struct
{
	BOOL				bEnable;							//��������ʹ��
	char				szAlarmName[DH_MAX_ALARM_NAME];		//������������
	int					nAlarmInPattern;					//���������벨��
	int					nAlarmOutPattern;					//�����������
	char				szAlarmInAddress[DH_MAX_ALARM_NAME];//���������ַ
	int					nSensorType;						//�ⲿ�豸���������ͳ��� or ����
	int					nDefendEfectTime;					//��������ʱʱ�䣬�ڴ�ʱ���ñ���������Ч
	int					nDefendAreaType;					//�������� 
	int					nAlarmSmoothTime;					//����ƽ��ʱ�䣬���ڴ�ʱ�������ֻ��һ��������������������������Ե�����һ��
	char				reserved[128];
	
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];
	DH_MSG_HANDLE		struHandle;	/* ����ʽ  */
} DH_INFRARED_INFO;

//����ң��������
typedef struct 
{
	BYTE  address[ALARM_MAX_NAME];							// ң������ַ
	BYTE  name[ALARM_MAX_NAME];								// ң��������
	BYTE  reserved[32];										// �����ֶ�
}DH_WI_CONFIG_ROBOT;

//���߱����������
typedef struct 
{
	BYTE  address[ALARM_MAX_NAME];							//���������ַ
	BYTE  name[ALARM_MAX_NAME];								//�����������
	BYTE  reserved[32];										//�����ֶ�
}DH_WI_CONFIG_ALARM_OUT;

typedef struct  
{
	DWORD				dwSize;
	BYTE				bAlarmInNum;						//���߱���������
	BYTE				bAlarmOutNum;						//���߱��������
	DH_WI_CONFIG_ALARM_OUT AlarmOutAddr[16];				//���������ַ
	BYTE				bRobotNum;							//ң��������
	DH_WI_CONFIG_ROBOT RobotAddr[16];						//ң������ַ
	DH_INFRARED_INFO	InfraredAlarm[16];
	char				reserved[256];
} DH_INFRARED_CFG;

//����Ƶ��ⱨ����Ϣ
typedef struct {
	int channel;						//����ͨ����
	int alarmType;						//��������0--��Ƶֵ���� 1---��Ƶֵ����
	unsigned int volume;				//����ֵ
	char 	reserved[256];
} NET_NEW_SOUND_ALARM_STATE;

typedef struct  
{
	int channelcount;                   //������ͨ������
	NET_NEW_SOUND_ALARM_STATE  SoundAlarmInfo[DH_MAX_ALARM_IN_NUM];
}DH_NEW_SOUND_ALARM_STATE;

//����Ƶ��ⱨ������
typedef struct
{
	BOOL				bEnable;							//��������ʹ��
	int					Volume_min;							//��������Сֵ
	int					Volume_max;							//���������ֵ
	char				reserved[128];	
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];	
	DH_MSG_HANDLE		struHandle;	/* ����ʽ  */
} DH_AUDIO_DETECT_INFO;

typedef struct  
{
	DWORD					dwSize;
	int						AlarmNum;
	DH_AUDIO_DETECT_INFO	AudioDetectAlarm[DH_MAX_ALARM_IN_NUM];
	char					reserved[256];
} DH_AUDIO_DETECT_CFG;

//�洢λ�����ýṹ�壬ÿͨ����������,ÿͨ������ѡ����ִ洢����, Ŀǰ��������, ���ƶ�, Զ�̴洢.
typedef struct 
{
	DWORD 	dwSize;
	DWORD 	dwLocalMask;      //���ش洢����  ��λ��ʾ��
								//��һλ	ϵͳԤ¼
								//�ڶ�λ	��ʱ¼��
								//����λ	����¼��
								//����λ	����¼��
								//����λ	����¼��
								//����λ	�ֶ�¼��

	DWORD 	dwMobileMask;				//���ƶ��洢���� �洢�����籾�ش洢����
	int  	RemoteType;					//Զ�̴洢���� 0: Ftp  1: Smb 
	DWORD 	dwRemoteMask;				//Զ�̴洢���� �洢�����籾�ش洢����
	DWORD 	dwRemoteSecondSelLocal;		//Զ���쳣ʱ���ش洢����
	DWORD 	dwRemoteSecondSelMobile;	//Զ���쳣ʱ���ƶ��洢����
	char 	SubRemotePath[MAX_PATH_STOR];  //Զ��Ŀ¼, ���г���Ϊ260
	char	reserved[128];
}DH_STORAGE_STATION_CFG;

/************************************************************************
 *   ����ץ������
 ************************************************************************/
typedef struct
{
	int					Offset;			//��־λ��λƫ��
	int					Length;			//��־λ�ĳ���
	char				Key[16];		//��־λ��ֵ
} DH_SNIFFER_FRAMEID;

typedef struct 
{
	int					Offset;			//��־λ��λƫ��
	int					Offset2;		//Ŀǰû��Ӧ��
	int					Length;			//��־λ�ĳ���
	int					Length2;		//Ŀǰû��Ӧ��
	char				KeyTitle[24];	//�����ֵ
} DH_SNIFFER_CONTENT;

typedef struct 
{
	DH_SNIFFER_FRAMEID	snifferFrameId;								//ÿ��FRAME ID ѡ��
	DH_SNIFFER_CONTENT	snifferContent[DH_SNIFFER_CONTENT_NUM];		//ÿ��FRAME��Ӧ��4��ץ������
} DH_SNIFFER_FRAME;

//ÿ��ץ����Ӧ�����ýṹ
typedef struct
{
	char				SnifferSrcIP[DH_MAX_IPADDR_LEN];			//ץ��Դ��ַ		
	int					SnifferSrcPort;								//ץ��Դ�˿�
	char				SnifferDestIP[DH_MAX_IPADDR_LEN]; 			//ץ��Ŀ���ַ
	int					SnifferDestPort;							//ץ��Ŀ��˿�
	char				reserved[28];								//�����ֶ�
	DH_SNIFFER_FRAME	snifferFrame[DH_SNIFFER_FRAMEID_NUM];		//6��FRAME ѡ��
	int					displayPosition;							//��ʾλ��
	int					recdChannelMask;							//ͨ������
} DH_ATM_SNIFFER_CFG;

typedef struct  
{
	DWORD dwSize;
	DH_ATM_SNIFFER_CFG SnifferConfig[4];
	char	reserved[256];								//�����ֶ�
}DHDEV_SNIFFER_CFG;


/************************************************************************
 *   OEM��ѯ
 ************************************************************************/
typedef struct  
{
	char				szVendor[DH_MAX_STRING_LEN];
	char				szType[DH_MAX_STRING_LEN];
	char				reserved[128];
}DHDEV_OEM_INFO;


/************************************************************************
 *   NTP����
 ************************************************************************/

typedef enum __DH_TIME_ZONE_TYPE
{
	DH_TIME_ZONE_0,				// {0, 0*3600,"GMT+00:00"}
	DH_TIME_ZONE_1,				// {1, 1*3600,"GMT+01:00"}
	DH_TIME_ZONE_2,				// {2, 2*3600,"GMT+02:00"}
	DH_TIME_ZONE_3,				// {3, 3*3600,"GMT+03:00"}
	DH_TIME_ZONE_4,				// {4, 3*3600+1800,"GMT+03:30"}
	DH_TIME_ZONE_5,				// {5, 4*3600,"GMT+04:00"}
	DH_TIME_ZONE_6,				// {6, 4*3600+1800,"GMT+04:30"}
	DH_TIME_ZONE_7,				// {7, 5*3600,"GMT+05:00"}
	DH_TIME_ZONE_8,				// {8, 5*3600+1800,"GMT+05:30"}
	DH_TIME_ZONE_9,				// {9, 5*3600+1800+900,"GMT+05:45"}
	DH_TIME_ZONE_10,			// {10, 6*3600,"GMT+06:00"}
	DH_TIME_ZONE_11,			// {11, 6*3600+1800,"GMT+06:30"}
	DH_TIME_ZONE_12,			// {12, 7*3600,"GMT+07:00"}
	DH_TIME_ZONE_13,			// {13, 8*3600,"GMT+08:00"}
	DH_TIME_ZONE_14,			// {14, 9*3600,"GMT+09:00"}
	DH_TIME_ZONE_15,			// {15, 9*3600+1800,"GMT+09:30"}
	DH_TIME_ZONE_16,			// {16, 10*3600,"GMT+10:00"}
	DH_TIME_ZONE_17,			// {17, 11*3600,"GMT+11:00"}
	DH_TIME_ZONE_18,			// {18, 12*3600,"GMT+12:00"}
	DH_TIME_ZONE_19,			// {19, 13*3600,"GMT+13:00"}
	DH_TIME_ZONE_20,			// {20, -1*3600,"GMT-01:00"}
	DH_TIME_ZONE_21,			// {21, -2*3600,"GMT-02:00"}
	DH_TIME_ZONE_22,			// {22, -3*3600,"GMT-03:00"}
	DH_TIME_ZONE_23,			// {23, -3*3600-1800,"GMT-03:30"}
	DH_TIME_ZONE_24,			// {24, -4*3600,"GMT-04:00"}
	DH_TIME_ZONE_25,			// {25, -5*3600,"GMT-05:00"}
	DH_TIME_ZONE_26,			// {26, -6*3600,"GMT-06:00"}
	DH_TIME_ZONE_27,			// {27, -7*3600,"GMT-07:00"}
	DH_TIME_ZONE_28,			// {28, -8*3600,"GMT-08:00"}
	DH_TIME_ZONE_29,			// {29, -9*3600,"GMT-09:00"}
	DH_TIME_ZONE_30,			// {30, -10*3600,"GMT-10:00"}
	DH_TIME_ZONE_31,			// {31, -11*3600,"GMT-11:00"}
	DH_TIME_ZONE_32,			// {32, -12*3600,"GMT-12:00"}
}DH_TIME_ZONE_TYPE;

typedef struct  
{
	BOOL				bEnable;			//	�Ƿ�����
	int					nHostPort;			//	NTP������Ĭ�϶˿�Ϊ123
	char				szHostIp[32];		//	����IP
	char				szDomainName[128];	//	����
	int					nType;				//	�������� 0����ʾIP��1����ʾ������2����ʾIP������
	int					nUpdateInterval;	//	����ʱ��(����)
	int					nTimeZone;			//	��DH_TIME_ZONE_TYPE
	char				reserved[128];
}DHDEV_NTP_CFG;


/************************************************************************
 *   ƽ̨�������� �� U��ͨ
 ************************************************************************/
typedef struct
{
    BOOL bChnEn;
    char szChnId[DH_INTERVIDEO_UCOM_CHANID];
}DH_INTERVIDEO_UCOM_CHN_CFG;

typedef struct
{
	DWORD dwSize;
	BOOL bFuncEnable;		// ���빦��ʹ����� 0 - ʹ��
	BOOL bAliveEnable;		// ����ʹ�����
	DWORD dwAlivePeriod;	// �������ڣ���λ�룬0-3600
	char szServerIp[DH_MAX_IPADDR_LEN];		// CMS��IP
	WORD wServerPort;						// CMS��Port
    char szRegPwd[DH_INTERVIDEO_UCOM_REGPSW]; //ע������
	char szDeviceId[DH_INTERVIDEO_UCOM_DEVID];	//�豸id
	char szUserName[DH_INTERVIDEO_UCOM_USERNAME];
	char szPassWord[DH_INTERVIDEO_UCOM_USERPSW];
    DH_INTERVIDEO_UCOM_CHN_CFG  struChnInfo[DH_MAX_CHANNUM];//ͨ��id,en
}DHDEV_INTERVIDEO_UCOM_CFG;

/************************************************************************
 *   ƽ̨�������� �� ��������
 ************************************************************************/
typedef struct
{
	DWORD dwSize;
    unsigned short usCompanyID[2];  /* ��˾ID,��ֵ,��ͬ�ĵ���������˾,����4�ֽڶ���,Ŀǰֻ�������һ�� */
    char szDeviceNO[32];  /* ǰ���豸���кţ��ַ���, ����'\0'��������32byte */
    char szVSName[32];  /* ǰ���豸���ƣ��ַ���, ����'\0'��������16byte */
    char szVapPath[32];  /* VAP·�� */
    unsigned short usTcpPort;  /* TCP �˿�,��ֵ: ��ֵ 1~65535 */    
    unsigned short usUdpPort;  /* UDP �˿�,��ֵ: ��ֵ 1~65535 */    
    bool bCsEnable[4];  /* ���ķ�����ʹ�ܱ�־, ��ֵ: trueʹ��, false��ʹ��,����4�ֽڶ���,Ŀǰֻ�������һ�� */
    char szCsIP[16];  /* ���ķ�����IP��ַ, �ַ���, ����'\0'��������16byte */
    unsigned short usCsPort[2];  /* ���ķ������˿�,��ֵ: ��ֵ 1~65535,����4�ֽڶ���,Ŀǰֻ�������һ�� */    
    bool bHsEnable[4];  /* ����������ʹ�ܱ�־, ��ֵ: trueʹ��, false��ʹ��,����4�ֽڶ���,Ŀǰֻ�������һ�� */
    char szHsIP[16];  /* ����������IP��ַ, �ַ���, ����'\0'��������16byte */
    unsigned short usHsPort[2];  /* �����������˿�,��ֵ: ��ֵ 1~65535,����4�ֽڶ���,Ŀǰֻ�������һ�� */ 
    int iHsIntervalTime;  /* �����������������,��ֵ(��λ:��) */ 
    bool bRsEnable[4];  /* ע�������ʹ�ܱ�־, ��ֵ: trueʹ��, false��ʹ��,����4�ֽڶ���,Ŀǰֻ�������һ�� */
    char szRsIP[16];  /* ע�������IP��ַ, �ַ���, ����'\0'��������16byte */
    unsigned short usRsPort[2];  /* ע��������˿�,��ֵ: ��ֵ 1~65535,����4�ֽڶ���,Ŀǰֻ�������һ�� */
    int iRsAgedTime;  /* ע���������Чʱ��,��ֵ(��λ:Сʱ) */
    char szAuthorizeServerIp[16];  /* ��Ȩ������IP */
    unsigned short usAuthorizePort[2];  /* ��Ȩ�������˿�,����4�ֽڶ���,Ŀǰֻ�������һ�� */
    char szAuthorizeUsername[32];  /* ��Ȩ�������ʺ� */
    char szAuthorizePassword[36];  /* ��Ȩ���������� */
    
    char szIpACS[16];  /* ACS(�Զ�ע�������) IP */
    unsigned short usPortACS[2];  /* ACS Port,����4�ֽڶ���,Ŀǰֻ�������һ�� */
    char szUsernameACS[32];  /* ACS�û��� */
    char szPasswordACS[36];  /* ACS���� */
    bool bVideoMonitorEnabled[4];  /* DVS�Ƿ����ϱ�ǰ����Ƶ�źż����Ϣ, ��ֵ: trueʹ��, false��ʹ�� */
    int iVideoMonitorInterval;  /* �ϱ����ڣ����ӣ� */
    
    char szCoordinateGPS[64];  /* GPS���� */
    char szPosition[32];  /* �豸λ�� */
    char szConnPass[36];  /* �豸������ */
}DHDEV_INTERVIDEO_BELL_CFG;

/************************************************************************
 *   ƽ̨�������� �� ������ά
 ************************************************************************/
typedef struct  
{
	DWORD dwSize;
	unsigned short		nSevPort;							//�������˿�, ��ֵ, ��ֵ1~65535
    char				szSevIp[DH_INTERVIDEO_NSS_IP];		//������IP��ַ, �ַ���, ����'\0'��������32byte
    char				szDevSerial[DH_INTERVIDEO_NSS_SERIAL];	//ǰ���豸���кţ��ַ���,����'\0'��������32byte
    char				szUserName[DH_INTERVIDEO_NSS_USER];
    char				szPwd[DH_INTERVIDEO_NSS_PWD];
}DHDEV_INTERVIDEO_NSS_CFG;

/************************************************************************
 *   ��������״̬��Ϣ
 ************************************************************************/

typedef struct
{
	int					nChannelNum;//ͨ����
	char				szUseType[32];//ͨ����;
	DWORD				dwStreamSize;//������С(��λ:kb/s)
	char				reserved[32];//����
}DHDEV_USE_CHANNEL_STATE;

typedef struct 
{
	char						szUserName[32];//�û���
	char						szUserGroup[32];//�û���
	NET_TIME					time;//����ʱ��
	int							nOpenedChannelNum;//������ͨ������
	DHDEV_USE_CHANNEL_STATE		channelInfo[DH_MAX_CHANNUM];
	char						reserved[64];
}DHDEV_USER_NET_INFO;

typedef	struct 
{
	int							nUserCount;//�û�����
	DHDEV_USER_NET_INFO			stuUserInfo[32];
	char						reserved[256];
}DHDEV_TOTAL_NET_STATE;
/************************************************************************
 *   ��̨������Ϣ
 ************************************************************************/
#define  NAME_MAX_LEN 16
typedef struct 
{
	DWORD		dwHighMask; /*!< �����������λ */
	DWORD		dwLowMask; /*!< �����������λ */
	char		szName[NAME_MAX_LEN]; /*!< ������Э����NAME_MAX_LEN=16 */
	WORD		wCamAddrMin; /*!< ͨ����ַ����Сֵ */
	WORD		wCamAddrMax; /*!< ͨ����ַ�����ֵ */
	WORD		wMonAddrMin; /*!< ���ӵ�ַ����Сֵ */
	WORD		wMonAddrMax; /*!< ���ӵ�ַ�����ֵ */
	BYTE		bPresetMin; /*!< Ԥ�õ����Сֵ */
	BYTE		bPresetMax; /*!< Ԥ�õ�����ֵ */
	BYTE		bTourMin; /*!< �Զ�Ѳ����·����Сֵ */
	BYTE		bTourMax; /*!< �Զ�Ѳ����·�����ֵ */
	BYTE		bPatternMin; /*!< �켣��·����Сֵ */
	BYTE		bPatternMax; /*!< �켣��·�����ֵ */
	BYTE		bTileSpeedMin; /*!< ��ֱ�ٶȵ���Сֵ */
	BYTE		bTileSpeedMax; /*!< ��ֱ�ٶȵ����ֵ */
	BYTE		bPanSpeedMin; /*!< ˮƽ�ٶȵ���Сֵ */
	BYTE		bPanSpeedMax; /*!< ˮƽ�ٶȵ����ֵ */
	BYTE		bAuxMin; /*!< �������ܵ���Сֵ */
	BYTE		bAuxMax; /*!< �������ܵ����ֵ */
	int			nInternal; /*!< ���������ʱ���� */
	char		cType; /*!< Э������� */
	char		Reserved[7];
}PTZ_OPT_ATTR;
/************************************************************************
 *   �����豸����Ϣ
 ************************************************************************/
typedef struct
{
	DWORD		dwType;	 //����(��GetDevConfig SetDevConfig������)
	WORD		wResultCode;//�����룺
					 /*		0:�ɹ� 
					  *		1:ʧ��
					  *		2:���ݲ��Ϸ�
					  *		3:��ʱ�޷�����	
					  *		4:û��Ȩ��
					  */
	
	WORD   		wRebootSign;//������־��
					/*
					 *	0:����Ҫ���� 1:��Ҫ��������Ч
					 */
	DWORD		dwReserved[2];//����	
}DEV_SET_RESULT;

/******************************************************************************
* ץͼ�������Խṹ��
*****************************************************************************/

typedef struct 
{
	int			nChannelNum;//ͨ����

	DWORD		dwVideoStandardMask	;//�ֱ���(��λ) ����鿴ö�� CAPTURE_SIZE						
	int			nFramesCount;//Frequence[128]�������Ч����
	char 		Frames[128];	//֡�ʣ�����ֵ��
								//-25:25��1֡
								//-24:24��1֡
								//-23:23��1֡
								//-22:22��1֡
								//����
								//0:��Ч
								//1��1��1֡
								//2��1��2֡
								//3��1��3֡
								//4��1��4֡
								//5��1��5֡
								//17��1��17֡
								//18��1��18֡
								//19��1��19֡
								//20��1��20֡
								//����
								//25: 1��25֡
	int			nSnapModeCount;//SnapMode[16]�������Ч����
	char		SnapMode[16];//������ֵ�� 0����ʱ����ץͼ 1���ֶ�����ץͼ
	int			nPicFormatCount;//Format[16]�������Ч����
	char 		PictureFormat[16];//������ֵ��0:BMP��ʽ 1:JPG��ʽ
	int			nPicQualityCount;//Quality[32]�������Ч����
	char 		PictureQuality[32];//������ֵ��	
							//100:ͼ������100%
							//80:ͼ������80%
							//60:ͼ������60%
							//50:ͼ������50%
							//30:ͼ������30%
							//10:ͼ������10%
	char 		nReserved[128];//����
} DH_QUERY_SNAP_INFO;

typedef struct 
{
	int			nChannelCount; //ͨ������
	DH_QUERY_SNAP_INFO  stuSnap[DH_MAX_CHANNUM];
}DH_SNAP_ATTR_EN;
/******************************************************************************
* end of ���ýṹ��
*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // __IVIDEO_H__

