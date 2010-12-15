
#ifndef DHNETSDK_H
#define DHNETSDK_H

#include "dhassistant.h"

#ifdef WIN32

#ifdef DHNETSDK_EXPORTS
#define CLIENT_API  __declspec(dllexport) 
#else
#define CLIENT_API  __declspec(dllimport)
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
#define BOOL	int
#define TRUE	1
#define FALSE	0
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
} RECT;

#else	//�ڲ�����
#include "../netsdk/osIndependent.h"
#endif

#endif


#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 ** ��������
 ***********************************************************************/
#define DH_SERIALNO_LEN 			48			// �豸���к��ַ�����
#define DH_MAX_DISKNUM 				256			// ���Ӳ�̸���
#define DH_MAX_SDCARDNUM			32			// ���SD������
#define DH_MAX_BURNING_DEV_NUM		32			// ����¼�豸����
#define DH_BURNING_DEV_NAMELEN		32			// ��¼�豸������󳤶�
#define DH_MAX_LINK 				6
#define DH_MAX_CHANNUM 				16			// ���ͨ������
#define DH_MAX_ALARMIN 				128			// ��󱨾��������
#define DH_MAX_ALARMOUT 			64			// ��󱨾��������
#define DH_MAX_RIGHT_NUM			100			// �û�Ȩ�޸�������
#define DH_MAX_GROUP_NUM			20			// �û����������
#define DH_MAX_USER_NUM				200			// �û���������
#define DH_RIGHT_NAME_LENGTH		32			// Ȩ��������
#define DH_USER_NAME_LENGTH			8			// �û�������
#define DH_USER_PSW_LENGTH			8			// �û����볤��
#define DH_MEMO_LENGTH				32			// ��ע����
#define DH_MAX_STRING_LEN			128
#define DH_DVR_SERIAL_RETURN		1			// �豸�������кŻص�
#define DH_DVR_DISCONNECT			-1			// ��֤�ڼ��豸���߻ص�
#define MAX_STRING_LINE_LEN			6			// �������
#define MAX_PER_STRING_LEN			20			// ÿ����󳤶�
#define DH_MAX_MAIL_NAME_LEN		64			// �µ��ʼ��ṹ��֧�ֵ��û�������
#define DH_MAX_MAIL_PSW_LEN			64			// �µ��ʼ��ṹ��֧�ֵ����볤��

// Զ�����ýṹ����س���
#define DH_MAX_MAIL_ADDR_LEN		128			// �ʼ���(��)��ַ��󳤶�
#define DH_MAX_MAIL_SUBJECT_LEN		64			// �ʼ�������󳤶�
#define DH_MAX_IPADDR_LEN			16			// IP��ַ�ַ�������
#define DH_MACADDR_LEN				40			// MAC��ַ�ַ�������
#define DH_MAX_URL_LEN				128			// URL�ַ�������
#define DH_MAX_DEV_ID_LEN			48			// ���������󳤶�
#define	DH_MAX_HOST_NAMELEN			64			// ���������ȣ�
#define DH_MAX_HOST_PSWLEN			32			// ���볤��
#define DH_MAX_NAME_LEN				16			// ͨ�������ַ�������
#define DH_MAX_ETHERNET_NUM			2			// ��̫����������
#define	DH_DEV_SERIALNO_LEN			48			// ���к��ַ�������
#define DH_DEV_TYPE_LEN				32			// �豸�����ַ�������
#define DH_N_WEEKS					7			// һ�ܵ�����	
#define DH_N_TSECT					6			// ͨ��ʱ��θ���
#define DH_N_REC_TSECT				6			// ¼��ʱ��θ���
#define DH_N_COL_TSECT				2			// ��ɫʱ��θ���	
#define DH_CHAN_NAME_LEN			32			// ͨ�������ȣ�DVR DSP�������ƣ����32�ֽ�		
#define DH_N_ENCODE_AUX				3			// ��չ��������	
#define DH_N_TALK					1			// ���Խ�ͨ������
#define DH_N_COVERS					1			// �ڵ��������	
#define DH_N_CHANNEL				16			// ���ͨ������	
#define DH_N_ALARM_TSECT			2			// ������ʾʱ��θ���
#define DH_MAX_ALARMOUT_NUM			16			// ��������ڸ�������
#define DH_MAX_AUDIO_IN_NUM			16			// ��Ƶ����ڸ�������
#define DH_MAX_VIDEO_IN_NUM			16			// ��Ƶ����ڸ�������
#define DH_MAX_ALARM_IN_NUM			16			// ��������ڸ�������
#define DH_MAX_DISK_NUM				16			// Ӳ�̸������ޣ��ݶ�Ϊ16
#define DH_MAX_DECODER_NUM			16			// ������(485)��������	
#define DH_MAX_232FUNCS				10			// 232���ڹ��ܸ�������
#define DH_MAX_232_NUM				2			// 232���ڸ�������
#define DH_MAX_DECPRO_LIST_SIZE		100			// ������Э���б��������
#define DH_FTP_MAXDIRLEN			240			// FTP�ļ�Ŀ¼��󳤶�
#define DH_MATRIX_MAXOUT			16			// ���������������
#define DH_TOUR_GROUP_NUM			6			// ���������������
#define DH_MAX_DDNS_NUM				10			// �豸֧�ֵ�ddns������������
#define DH_MAX_SERVER_TYPE_LEN		32			// ddns���������ͣ�����ַ�������
#define DH_MAX_DOMAIN_NAME_LEN		256			// ddns����������ַ�������
#define DH_MAX_DDNS_ALIAS_LEN		32			// ddns����������������ַ�������
#define DH_MOTION_ROW				32			// ��̬������������
#define DH_MOTION_COL				32			// ��̬������������
#define	DH_FTP_USERNAME_LEN			64			// FTP���ã��û�����󳤶�
#define	DH_FTP_PASSWORD_LEN			64			// FTP���ã�������󳤶�
#define	DH_TIME_SECTION				2			// FTP���ã�ÿ��ʱ��θ���
#define DH_FTP_MAX_PATH				240			// FTP���ã��ļ�·������󳤶�
#define DH_INTERVIDEO_UCOM_CHANID	32			// ƽ̨�������ã�U��ͨͨ��ID
#define DH_INTERVIDEO_UCOM_DEVID	32			// ƽ̨�������ã�U��ͨ�豸ID
#define DH_INTERVIDEO_UCOM_REGPSW	16			// ƽ̨�������ã�U��ͨע������
#define DH_INTERVIDEO_UCOM_USERNAME	32			// ƽ̨�������ã�U��ͨ�û���
#define DH_INTERVIDEO_UCOM_USERPSW	32			// ƽ̨�������ã�U��ͨ����
#define DH_INTERVIDEO_NSS_IP		32			// ƽ̨�������ã�������άIP
#define DH_INTERVIDEO_NSS_SERIAL	32			// ƽ̨�������ã�������άserial
#define DH_INTERVIDEO_NSS_USER		32			// ƽ̨�������ã�������άuser
#define DH_INTERVIDEO_NSS_PWD		50			// ƽ̨�������ã�������άpassword
#define DH_MAX_VIDEO_COVER_NUM		16			// �ڵ�����������
#define DH_MAX_WATERMAKE_DATA		4096		// ˮӡͼƬ������󳤶�
#define DH_MAX_WATERMAKE_LETTER		128			// ˮӡ������󳤶�
#define DH_MAX_WLANDEVICE_NUM		10			// ����������������豸����
#define DH_MAX_ALARM_NAME			64			// ��ַ����
#define DH_MAX_REGISTER_SERVER_NUM	10			// ����ע�����������
#define DH_SNIFFER_FRAMEID_NUM		6			// 6��FRAME ID ѡ��
#define DH_SNIFFER_CONTENT_NUM		4			// ÿ��FRAME��Ӧ��4��ץ������
#define DH_SNIFFER_CONTENT_NUM_EX	8			// ÿ��FRAME��Ӧ��8��ץ������
#define DH_SNIFFER_PROTOCOL_SIZE	20			// Э�����ֳ���
#define DH_MAX_PROTOCOL_NAME_LENGTH 20
#define DH_SNIFFER_GROUP_NUM		4			// 4��ץ������
#define MAX_PATH_STOR				240			// Զ��Ŀ¼�ĳ���
#define DH_ALARM_OCCUR_TIME_LEN		40			// �µı����ϴ�ʱ��ĳ���
#define DH_VIDEO_OSD_NAME_NUM		64			// ���ӵ����Ƴ��ȣ�Ŀǰ֧��32��Ӣ�ģ�16������
#define DH_VIDEO_CUSTOM_OSD_NUM		8			// ֧�ֵ��Զ�����ӵ���Ŀ��������ʱ���ͨ��
#define DH_CONTROL_AUTO_REGISTER_NUM 100        // ֧�ֶ�������ע��������ĸ���
#define DH_MMS_RECEIVER_NUM          100        // ֧�ֶ��Ž����ߵĸ���
#define DH_MMS_SMSACTIVATION_NUM     100        // ֧�ֶ��ŷ����ߵĸ���
#define DH_MMS_DIALINACTIVATION_NUM  100        // ֧�ֲ��ŷ����ߵĸ���
#define DH_MAX_ALARMOUT_NUM_EX		 32			// ��������ڸ���������չ
#define DH_MAX_VIDEO_IN_NUM_EX		 32			// ��Ƶ����ڸ���������չ
#define DH_MAX_ALARM_IN_NUM_EX		 32			// ��������ڸ�������
#define DH_MAX_IPADDR_OR_DOMAIN_LEN	 64			// IP��ַ�ַ�������
	
// ��ѯ���ͣ���ӦCLIENT_QueryDevState�ӿ�
#define DH_DEVSTATE_COMM_ALARM		0x0001		// ��ѯ��ͨ����״̬(�����ⲿ��������Ƶ��ʧ����̬���)
#define DH_DEVSTATE_SHELTER_ALARM	0x0002		// ��ѯ�ڵ�����״̬
#define DH_DEVSTATE_RECORDING		0x0003		// ��ѯ¼��״̬
#define DH_DEVSTATE_DISK			0x0004		// ��ѯӲ����Ϣ
#define DH_DEVSTATE_RESOURCE		0x0005		// ��ѯϵͳ��Դ״̬
#define DH_DEVSTATE_BITRATE			0x0006		// ��ѯͨ������
#define DH_DEVSTATE_CONN			0x0007		// ��ѯ�豸����״̬
#define DH_DEVSTATE_PROTOCAL_VER	0x0008		// ��ѯ����Э��汾�ţ�pBuf = int*
#define DH_DEVSTATE_TALK_ECTYPE		0x0009		// ��ѯ�豸֧�ֵ������Խ���ʽ�б����ṹ��DHDEV_TALKFORMAT_LIST
#define DH_DEVSTATE_SD_CARD			0x000A		// ��ѯSD����Ϣ(IPC���Ʒ)
#define DH_DEVSTATE_BURNING_DEV		0x000B		// ��ѯ��¼����Ϣ
#define DH_DEVSTATE_BURNING_PROGRESS 0x000C		// ��ѯ��¼����
#define DH_DEVSTATE_PLATFORM		0x000D		// ��ѯ�豸֧�ֵĽ���ƽ̨
#define DH_DEVSTATE_CAMERA			0x000E		// ��ѯ����ͷ������Ϣ(IPC���Ʒ)��pBuf = DHDEV_CAMERA_INFO *�������ж���ṹ��
#define DH_DEVSTATE_SOFTWARE		0x000F		// ��ѯ�豸����汾��Ϣ
#define DH_DEVSTATE_LANGUAGE        0x0010		// ��ѯ�豸֧�ֵ���������
#define DH_DEVSTATE_DSP				0x0011		// ��ѯDSP��������
#define	DH_DEVSTATE_OEM				0x0012		// ��ѯOEM��Ϣ
#define	DH_DEVSTATE_NET				0x0013		// ��ѯ��������״̬��Ϣ
#define DH_DEVSTATE_TYPE			0x0014		// ��ѯ�豸����
#define DH_DEVSTATE_SNAP			0x0015		// ��ѯץͼ��������(IPC���Ʒ)
#define DH_DEVSTATE_RECORD_TIME		0x0016		// ��ѯ����¼��ʱ������¼��ʱ��
#define DH_DEVSTATE_NET_RSSI        0x0017      // ��ѯ���������ź�ǿ�ȣ����ṹ��DHDEV_WIRELESS_RSS_INFO
#define DH_DEVSTATE_BURNING_ATTACH	0x0018		// ��ѯ������¼ѡ��
#define DH_DEVSTATE_BACKUP_DEV		0x0019		// ��ѯ�����豸�б�
#define DH_DEVSTATE_BACKUP_DEV_INFO	0x001a		// ��ѯ�����豸��ϸ��Ϣ
#define DH_DEVSTATE_BACKUP_FEEDBACK	0x001b		// ���ݽ��ȷ���
#define DH_DEVSTATE_ATM_QUERY_TRADE  0x001c		// ��ѯATM��������
#define DH_DEVSTATE_SIP				0x001d		// ��ѯsip״̬

// �������ͣ���ӦCLIENT_GetDevConfig��CLIENT_SetDevConfig�ӿ�
#define DH_DEV_DEVICECFG			0x0001		// �豸��������
#define DH_DEV_NETCFG				0x0002		// ��������
#define DH_DEV_CHANNELCFG			0x0003		// ͼ��ͨ������
#define DH_DEV_PREVIEWCFG 			0x0004		// Ԥ����������
#define DH_DEV_RECORDCFG			0x0005		// ¼������
#define DH_DEV_COMMCFG				0x0006		// ������������
#define DH_DEV_ALARMCFG 			0x0007		// ������������
#define DH_DEV_TIMECFG 				0x0008		// DVRʱ������
#define DH_DEV_TALKCFG				0x0009		// �Խ���������
#define DH_DEV_AUTOMTCFG			0x000A		// �Զ�ά������		
#define	DH_DEV_VEDIO_MARTIX			0x000B		// ����������Ʋ�������	
#define DH_DEV_MULTI_DDNS			0x000C		// ��ddns����������
#define DH_DEV_SNAP_CFG				0x000D		// ץͼ�������
#define DH_DEV_WEB_URL_CFG			0x000E		// HTTP·������
#define DH_DEV_FTP_PROTO_CFG		0x000F		// FTP�ϴ�����
#define DH_DEV_INTERVIDEO_CFG		0x0010		// ƽ̨�������ã���ʱchannel��������ƽ̨���ͣ�
												// channel=4�� �������������أ�channel=10������������ά��channel=11������U��ͨ
#define DH_DEV_VIDEO_COVER			0x0011		// �����ڵ�����
#define DH_DEV_TRANS_STRATEGY		0x0012		// ����������ã���������\����������
#define DH_DEV_DOWNLOAD_STRATEGY	0x0013		// ¼�����ز������ã���������\��ͨ����
#define DH_DEV_WATERMAKE_CFG		0x0014		// ͼ��ˮӡ����
#define DH_DEV_WLAN_CFG				0x0015		// ������������
#define DH_DEV_WLAN_DEVICE_CFG		0x0016		// ���������豸����
#define DH_DEV_REGISTER_CFG			0x0017		// ����ע���������
#define DH_DEV_CAMERA_CFG			0x0018		// ����ͷ��������
#define DH_DEV_INFRARED_CFG 		0x0019		// ���ⱨ������
#define DH_DEV_SNIFFER_CFG			0x001A		// Snifferץ������
#define DH_DEV_MAIL_CFG				0x001B		// �ʼ�����
#define DH_DEV_DNS_CFG				0x001C		// DNS����������
#define DH_DEV_NTP_CFG				0x001D		// NTP����
#define DH_DEV_AUDIO_DETECT_CFG		0x001E		// ��Ƶ�������
#define DH_DEV_STORAGE_STATION_CFG  0x001F      // �洢λ������
#define DH_DEV_PTZ_OPT_CFG			0x0020		// ��̨��������(�Ѿ��ϳ�����ʹ��CLIENT_GetPtzOptAttr��ȡ��̨��������)
#define DH_DEV_DST_CFG				0x0021      // ����ʱ����
#define DH_DEV_ALARM_CENTER_CFG		0x0022		// ������������
#define DH_DEV_VIDEO_OSD_CFG        0x0023		// ��ƵOSD��������
#define DH_DEV_CDMAGPRS_CFG         0x0024		// CDMA\GPRS��������
#define DH_DEV_IPFILTER_CFG         0x0025		// IP��������
#define DH_DEV_TALK_ENCODE_CFG      0x0026      // �����Խ���������
#define DH_DEV_RECORD_PACKET_CFG    0X0027      // ¼������������
#define DH_DEV_MMS_CFG              0X0028		// ����MMS���� 
#define DH_DEV_SMSACTIVATION_CFG	0X0029		// ���ż���������������
#define DH_DEV_DIALINACTIVATION_CFG	0X002A		// ���ż���������������
#define DH_DEV_FILETRANS_STOP		0x002B		// ֹͣ�ļ��ϴ�
#define DH_DEV_FILETRANS_BURN		0x002C		// ��¼�ļ��ϴ�
#define DH_DEV_SNIFFER_CFG_EX		0x0030		// ����ץ������
#define DH_DEV_DOWNLOAD_RATE_CFG	0x0031		// �����ٶ�����
#define DH_DEV_PANORAMA_SWITCH_CFG	0x0032		// ȫ���л���������
#define DH_DEV_LOST_FOCUS_CFG		0x0033		// ʧȥ���㱨������
#define DH_DEV_ALARM_DECODE_CFG		0x0034		// ��������������
#define DH_DEV_VIDEOOUT_CFG         0x0035      // ��Ƶ�����������
#define DH_DEV_POINT_CFG			0x0036		// Ԥ�Ƶ�ʹ������
#define DH_DEV_IP_COLLISION_CFG     0x0037      // Ip��ͻ��ⱨ������
#define DH_DEV_OSD_ENABLE_CFG		0x0038		// OSD����ʹ������
#define DH_DEV_LOCALALARM_CFG 		0x0039		// ���ر�������(�ṹ��DH_ALARMIN_CFG_EX)
#define DH_DEV_NETALARM_CFG 		0x003A		// ���籨������(�ṹ��DH_ALARMIN_CFG_EX)
#define DH_DEV_MOTIONALARM_CFG 		0x003B		// ���챨������(�ṹ��DH_MOTION_DETECT_CFG_EX)
#define DH_DEV_VIDEOLOSTALARM_CFG 	0x003C		// ��Ƶ��ʧ��������(�ṹ��DH_VIDEO_LOST_CFG_EX)
#define DH_DEV_BLINDALARM_CFG 		0x003D		// ��Ƶ�ڵ���������(�ṹ��DH_BLIND_CFG_EX)
#define DH_DEV_DISKALARM_CFG 		0x003E		// Ӳ�̱�������(�ṹ��DH_DISK_ALARM_CFG_EX)
#define DH_DEV_NETBROKENALARM_CFG 	0x003F		// �����жϱ�������(�ṹ��DH_NETBROKEN_ALARM_CFG_EX)
#define DH_DEV_ENCODER_CFG          0x0040      // ����ͨ����ǰ�˱�������Ϣ�����DVRʹ�ã��ṹ��DEV_ENCODER_CFG��
#define DH_DEV_TV_ADJUST_CFG        0x0041      // TV�������ã�channel����TV��(0��ʼ)�����ͽṹ�壩
#define DH_DEV_ABOUT_VEHICLE_CFG	0x0042		// ����������ã���������ʹ��
#define DH_DEV_ATM_OVERLAY_ABILITY	0x0043		// ��ȡatm����֧��������Ϣ
#define DH_DEV_ATM_OVERLAY_CFG		0x0044		// atm�������ã���atm����
#define DH_DEV_DECODER_TOUR_CFG		0x0045		// ������������Ѳ����
#define DH_DEV_SIP_CFG				0x0046		// SIP����

#define DH_DEV_USER_END_CFG			1000

// �������ͣ���ӦCLIENT_StartListen�ӿ�
#define DH_COMM_ALARM				0x1100		// ���汨��(�����ⲿ��������Ƶ��ʧ����̬���)
#define DH_SHELTER_ALARM			0x1101		// ��Ƶ�ڵ�����
#define DH_DISK_FULL_ALARM			0x1102		// Ӳ��������
#define DH_DISK_ERROR_ALARM			0x1103		// Ӳ�̹��ϱ���
#define DH_SOUND_DETECT_ALARM		0x1104		// ��Ƶ��ⱨ��
#define DH_ALARM_DECODER_ALARM		0x1105		// ��������������

// ��չ�������ͣ���ӦCLIENT_StartListenEx�ӿ�
#define DH_ALARM_ALARM_EX			0x2101		// �ⲿ����
#define DH_MOTION_ALARM_EX			0x2102		// ��̬��ⱨ��
#define DH_VIDEOLOST_ALARM_EX		0x2103		// ��Ƶ��ʧ����
#define DH_SHELTER_ALARM_EX			0x2104		// ��Ƶ�ڵ�����
#define DH_SOUND_DETECT_ALARM_EX	0x2105		// ��Ƶ��ⱨ��
#define DH_DISKFULL_ALARM_EX		0x2106		// Ӳ��������
#define DH_DISKERROR_ALARM_EX		0x2107		// ��Ӳ�̱���
#define DH_ENCODER_ALARM_EX			0x210A		// ����������
#define DH_URGENCY_ALARM_EX			0x210B		// ��������
#define DH_WIRELESS_ALARM_EX		0x210C		// ���߱���
#define DH_NEW_SOUND_DETECT_ALARM_EX 0x210D		// ����Ƶ��ⱨ����������Ϣ�Ľṹ���DH_NEW_SOUND_ALARM_STATE��
#define DH_ALARM_DECODER_ALARM_EX	0x210E		// ��������������
#define DH_DECODER_DECODE_ABILITY	0x210F		// ��������������������
#define DH_FDDI_DECODER_ABILITY		0x2110		// ���˱�����״̬����
#define DH_PANORAMA_SWITCH_ALARM_EX	0x2111		// �л���������
#define DH_LOSTFOCUS_ALARM_EX		0x2112		// ʧȥ���㱨��
#define DH_OEMSTATE_EX				0x2113		// oem��ͣ״̬
#define DH_DSP_ALARM_EX				0x2114		// DSP����
#define DH_ATMPOS_BROKEN_EX			0x2115		// atm��pos���Ͽ������� 0�����ӶϿ� 1����������
#define DH_RECORD_CHANGED_EX		0x2116		// ¼��״̬�仯����
#define DH_DEVICE_REBOOT_EX			0x2118		// �豸��������
#define DH_CONFIG_CHANGED_EX		0x2117		// ���÷����仯����

// �¼�����
#define DH_CONFIG_RESULT_EVENT_EX	0x3000		// �޸����õķ����룻���ؽṹ��DEV_SET_RESULT
#define DH_REBOOT_EVENT_EX			0x3001		// �豸�����¼������δ�������������ǰ�޸ĵ����ò���������Ч
#define DH_AUTO_TALK_START_EX		0x3002		// �豸�������뿪ʼ�����Խ�
#define DH_AUTO_TALK_STOP_EX		0x3003		// �豸����ֹͣ�����Խ�
#define DH_CONFIG_CHANGE_EX			0x3004		// �豸���÷����ı�

// �����ϴ����ܵı������ͣ���ӦCLIENT_StartService�ӿڡ�NEW_ALARM_UPLOAD�ṹ��.
#define DH_UPLOAD_ALARM				0x4000		// �ⲿ����		
#define DH_UPLOAD_MOTION_ALARM		0x4001		// ��̬��ⱨ��
#define DH_UPLOAD_VIDEOLOST_ALARM	0x4002		// ��Ƶ��ʧ����
#define DH_UPLOAD_SHELTER_ALARM		0x4003		// ��Ƶ�ڵ�����
#define DH_UPLOAD_SOUND_DETECT_ALARM 0x4004		// ��Ƶ��ⱨ��
#define DH_UPLOAD_DISKFULL_ALARM	0x4005		// Ӳ��������
#define DH_UPLOAD_DISKERROR_ALARM	0x4006		// ��Ӳ�̱���
#define DH_UPLOAD_ENCODER_ALARM		0x4007		// ����������
#define DH_UPLOAD_DECODER_ALARM		0x400B		// ��������������
#define DH_UPLOAD_EVENT				0x400C		// ��ʱ�ϴ���

// �첽�ӿڻص�����
#define RESPONSE_DECODER_CTRL_TV	0x00000001	// ��ӦCLIENT_CtrlDecTVScreen�ӿ�
#define RESPONSE_DECODER_SWITCH_TV	0x00000002	// ��ӦCLIENT_SwitchDecTVEncoder�ӿ�
#define RESPONSE_DECODER_PLAYBACK	0x00000003	// ��ӦCLIENT_DecTVPlayback�ӿ�

// �ֱ����б�������ֱ�����������롢�����
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
#define DH_CAPTURE_SIZE_LFT         0x00010000
#define DH_CAPTURE_SIZE_720		    0x00020000
#define DH_CAPTURE_SIZE_1080		0x00040000
#define DH_CAPTURE_SIZE_1_3M		0x00080000

// ����ģʽ�б����������ģʽ��������롢�����
#define DH_CAPTURE_COMP_DIVX_MPEG4	0x00000001
#define DH_CAPTURE_COMP_MS_MPEG4 	0x00000002
#define DH_CAPTURE_COMP_MPEG2		0x00000004
#define DH_CAPTURE_COMP_MPEG1		0x00000008
#define DH_CAPTURE_COMP_H263		0x00000010
#define DH_CAPTURE_COMP_MJPG		0x00000020
#define DH_CAPTURE_COMP_FCC_MPEG4	0x00000040
#define DH_CAPTURE_COMP_H264		0x00000080

// �������������������뱨������������������롢�����
#define DH_ALARM_UPLOAD				0x00000001
#define DH_ALARM_RECORD				0x00000002
#define DH_ALARM_PTZ				0x00000004
#define DH_ALARM_MAIL				0x00000008
#define DH_ALARM_TOUR				0x00000010
#define DH_ALARM_TIP				0x00000020
#define DH_ALARM_OUT				0x00000040
#define DH_ALARM_FTP_UL				0x00000080
#define DH_ALARM_BEEP				0x00000100
#define DH_ALARM_VOICE				0x00000200
#define DH_ALARM_SNAP				0x00000400

// "�ָ�Ĭ������"���룬�ɽ����롢�����
#define DH_RESTORE_COMMON			0x00000001	// ��ͨ����
#define DH_RESTORE_CODING			0x00000002	// ��������
#define DH_RESTORE_VIDEO			0x00000004	// ¼������
#define DH_RESTORE_COMM				0x00000008	// ��������
#define DH_RESTORE_NETWORK			0x00000010	// ��������
#define DH_RESTORE_ALARM			0x00000020	// ��������
#define DH_RESTORE_VIDEODETECT		0x00000040	// ��Ƶ���
#define DH_RESTORE_PTZ				0x00000080	// ��̨����
#define DH_RESTORE_OUTPUTMODE		0x00000100	// ���ģʽ
#define DH_RESTORE_CHANNELNAME		0x00000200	// ͨ������
#define DH_RESTORE_ALL				0x80000000	// ȫ������

// ��̨�����б�
// ���ĸ��ֽ�����
#define PTZ_DIRECTION				0x00000001	// ����
#define PTZ_ZOOM					0x00000002	// �䱶
#define PTZ_FOCUS					0x00000004	// �۽�
#define PTZ_IRIS					0x00000008	// ��Ȧ
#define PTZ_ALARM					0x00000010	// ��������
#define PTZ_LIGHT					0x00000020	// �ƹ�
#define PTZ_SETPRESET				0x00000040	// ����Ԥ�õ�
#define PTZ_CLEARPRESET				0x00000080	// ���Ԥ�õ�
#define PTZ_GOTOPRESET				0x00000100	// ת��Ԥ�õ�
#define PTZ_AUTOPANON				0x00000200	// ˮƽ��ʼ
#define PTZ_AUTOPANOFF				0x00000400	// ˮƽ����
#define PTZ_SETLIMIT				0x00000800	// ���ñ߽�
#define PTZ_AUTOSCANON				0x00001000	// �Զ�ɨ�迪ʼ
#define PTZ_AUTOSCANOFF				0x00002000	// �Զ�ɨ�迪ֹͣ
#define PTZ_ADDTOUR					0x00004000	// ����Ѳ����
#define PTZ_DELETETOUR				0x00008000	// ɾ��Ѳ����
#define PTZ_STARTTOUR				0x00010000	// ��ʼѲ��
#define PTZ_STOPTOUR				0x00020000	// ����Ѳ��
#define PTZ_CLEARTOUR				0x00040000	// ɾ��Ѳ��
#define PTZ_SETPATTERN				0x00080000	// ����ģʽ
#define PTZ_STARTPATTERN			0x00100000	// ��ʼģʽ
#define PTZ_STOPPATTERN				0x00200000	// ֹͣģʽ
#define PTZ_CLEARPATTERN			0x00400000	// ���ģʽ
#define PTZ_POSITION				0x00800000	// ���ٶ�λ
#define PTZ_AUX						0x01000000	// ��������
#define PTZ_MENU					0x02000000	// ����˵�
#define PTZ_EXIT					0x04000000	// �˳�����˵�
#define PTZ_ENTER					0x08000000	// ȷ��
#define PTZ_ESC						0x10000000	// ȡ��
#define PTZ_MENUUPDOWN				0x20000000	// �˵����²���
#define PTZ_MENULEFTRIGHT			0x40000000	// �˵����Ҳ���
#define PTZ_OPT_NUM					0x80000000	// �����ĸ���
// ���ĸ��ֽ�����
#define PTZ_DEV						0x00000001	// ��̨����
#define PTZ_MATRIX					0x00000002	// �������

// ץͼ��Ƶ��������
#define CODETYPE_MPEG4				0
#define CODETYPE_H264				1
#define CODETYPE_JPG				2

// ���������б�
#define DH_CAPTURE_BRC_CBR			0
#define DH_CAPTURE_BRC_VBR			1
//#define DH_CAPTURE_BRC_MBR		2

// �������ʹ��ţ���ӦCLIENT_GetLastError�ӿڵķ���ֵ
#define _EC(x)						(0x80000000|x)
#define NET_NOERROR 				0			// û�д���
#define NET_ERROR					-1			// δ֪����
#define NET_SYSTEM_ERROR			_EC(1)		// Windowsϵͳ����
#define NET_NETWORK_ERROR			_EC(2)		// ������󣬿�������Ϊ���糬ʱ
#define NET_DEV_VER_NOMATCH			_EC(3)		// �豸Э�鲻ƥ��
#define NET_INVALID_HANDLE			_EC(4)		// �����Ч
#define NET_OPEN_CHANNEL_ERROR		_EC(5)		// ��ͨ��ʧ��
#define NET_CLOSE_CHANNEL_ERROR		_EC(6)		// �ر�ͨ��ʧ��
#define NET_ILLEGAL_PARAM			_EC(7)		// �û��������Ϸ�
#define NET_SDK_INIT_ERROR			_EC(8)		// SDK��ʼ������
#define NET_SDK_UNINIT_ERROR		_EC(9)		// SDK�������
#define NET_RENDER_OPEN_ERROR		_EC(10)		// ����render��Դ����
#define NET_DEC_OPEN_ERROR			_EC(11)		// �򿪽�������
#define NET_DEC_CLOSE_ERROR			_EC(12)		// �رս�������
#define NET_MULTIPLAY_NOCHANNEL		_EC(13)		// �໭��Ԥ���м�⵽ͨ����Ϊ0
#define NET_TALK_INIT_ERROR			_EC(14)		// ¼�����ʼ��ʧ��
#define NET_TALK_NOT_INIT			_EC(15)		// ¼����δ����ʼ��
#define	NET_TALK_SENDDATA_ERROR		_EC(16)		// ������Ƶ���ݳ���
#define NET_REAL_ALREADY_SAVING		_EC(17)		// ʵʱ�����Ѿ����ڱ���״̬
#define NET_NOT_SAVING				_EC(18)		// δ����ʵʱ����
#define NET_OPEN_FILE_ERROR			_EC(19)		// ���ļ�����
#define NET_PTZ_SET_TIMER_ERROR		_EC(20)		// ������̨���ƶ�ʱ��ʧ��
#define NET_RETURN_DATA_ERROR		_EC(21)		// �Է������ݵ�У�����
#define NET_INSUFFICIENT_BUFFER		_EC(22)		// û���㹻�Ļ���
#define NET_NOT_SUPPORTED			_EC(23)		// ��ǰSDKδ֧�ָù���
#define NET_NO_RECORD_FOUND			_EC(24)		// ��ѯ����¼��
#define NET_NOT_AUTHORIZED			_EC(25)		// �޲���Ȩ��
#define NET_NOT_NOW					_EC(26)		// ��ʱ�޷�ִ��
#define NET_NO_TALK_CHANNEL			_EC(27)		// δ���ֶԽ�ͨ��
#define NET_NO_AUDIO				_EC(28)		// δ������Ƶ
#define NET_NO_INIT					_EC(29)		// ����SDKδ����ʼ��
#define NET_DOWNLOAD_END			_EC(30)		// �����ѽ���
#define NET_EMPTY_LIST				_EC(31)		// ��ѯ���Ϊ��
#define NET_ERROR_GETCFG_SYSATTR	_EC(32)		// ��ȡϵͳ��������ʧ��
#define NET_ERROR_GETCFG_SERIAL		_EC(33)		// ��ȡ���к�ʧ��
#define NET_ERROR_GETCFG_GENERAL	_EC(34)		// ��ȡ��������ʧ��
#define NET_ERROR_GETCFG_DSPCAP		_EC(35)		// ��ȡDSP��������ʧ��
#define NET_ERROR_GETCFG_NETCFG		_EC(36)		// ��ȡ��������ʧ��
#define NET_ERROR_GETCFG_CHANNAME	_EC(37)		// ��ȡͨ������ʧ��
#define NET_ERROR_GETCFG_VIDEO		_EC(38)		// ��ȡ��Ƶ����ʧ��
#define NET_ERROR_GETCFG_RECORD		_EC(39)		// ��ȡ¼������ʧ��
#define NET_ERROR_GETCFG_PRONAME	_EC(40)		// ��ȡ������Э������ʧ��
#define NET_ERROR_GETCFG_FUNCNAME	_EC(41)		// ��ȡ232���ڹ�������ʧ��
#define NET_ERROR_GETCFG_485DECODER	_EC(42)		// ��ȡ����������ʧ��
#define NET_ERROR_GETCFG_232COM		_EC(43)		// ��ȡ232��������ʧ��
#define NET_ERROR_GETCFG_ALARMIN	_EC(44)		// ��ȡ�ⲿ������������ʧ��
#define NET_ERROR_GETCFG_ALARMDET	_EC(45)		// ��ȡ��̬��ⱨ��ʧ��
#define NET_ERROR_GETCFG_SYSTIME	_EC(46)		// ��ȡ�豸ʱ��ʧ��
#define NET_ERROR_GETCFG_PREVIEW	_EC(47)		// ��ȡԤ������ʧ��
#define NET_ERROR_GETCFG_AUTOMT		_EC(48)		// ��ȡ�Զ�ά������ʧ��
#define NET_ERROR_GETCFG_VIDEOMTRX	_EC(49)		// ��ȡ��Ƶ��������ʧ��
#define NET_ERROR_GETCFG_COVER		_EC(50)		// ��ȡ�����ڵ�����ʧ��
#define NET_ERROR_GETCFG_WATERMAKE	_EC(51)		// ��ȡͼ��ˮӡ����ʧ��
#define NET_ERROR_SETCFG_GENERAL	_EC(55)		// �޸ĳ�������ʧ��
#define NET_ERROR_SETCFG_NETCFG		_EC(56)		// �޸���������ʧ��
#define NET_ERROR_SETCFG_CHANNAME	_EC(57)		// �޸�ͨ������ʧ��
#define NET_ERROR_SETCFG_VIDEO		_EC(58)		// �޸���Ƶ����ʧ��
#define NET_ERROR_SETCFG_RECORD		_EC(59)		// �޸�¼������ʧ��
#define NET_ERROR_SETCFG_485DECODER	_EC(60)		// �޸Ľ���������ʧ��
#define NET_ERROR_SETCFG_232COM		_EC(61)		// �޸�232��������ʧ��
#define NET_ERROR_SETCFG_ALARMIN	_EC(62)		// �޸��ⲿ���뱨������ʧ��
#define NET_ERROR_SETCFG_ALARMDET	_EC(63)		// �޸Ķ�̬��ⱨ������ʧ��
#define NET_ERROR_SETCFG_SYSTIME	_EC(64)		// �޸��豸ʱ��ʧ��
#define NET_ERROR_SETCFG_PREVIEW	_EC(65)		// �޸�Ԥ������ʧ��
#define NET_ERROR_SETCFG_AUTOMT		_EC(66)		// �޸��Զ�ά������ʧ��
#define NET_ERROR_SETCFG_VIDEOMTRX	_EC(67)		// �޸���Ƶ��������ʧ��
#define NET_ERROR_SETCFG_COVER		_EC(68)		// �޸������ڵ�����ʧ��
#define NET_ERROR_SETCFG_WATERMAKE	_EC(69)		// �޸�ͼ��ˮӡ����ʧ��
#define NET_ERROR_SETCFG_WLAN		_EC(70)		// �޸�����������Ϣʧ��
#define NET_ERROR_SETCFG_WLANDEV	_EC(71)		// ѡ�����������豸ʧ��
#define NET_ERROR_SETCFG_REGISTER	_EC(72)		// �޸�����ע���������ʧ��
#define NET_ERROR_SETCFG_CAMERA		_EC(73)		// �޸�����ͷ��������ʧ��
#define NET_ERROR_SETCFG_INFRARED	_EC(74)		// �޸ĺ��ⱨ������ʧ��
#define NET_ERROR_SETCFG_SOUNDALARM	_EC(75)		// �޸���Ƶ��������ʧ��
#define NET_ERROR_SETCFG_STORAGE    _EC(76)		// �޸Ĵ洢λ������ʧ��
#define NET_AUDIOENCODE_NOTINIT		_EC(77)		// ��Ƶ����ӿ�û�гɹ���ʼ��
#define NET_DATA_TOOLONGH			_EC(78)		// ���ݹ���
#define NET_UNSUPPORTED				_EC(79)		// �豸��֧�ָò���
#define NET_DEVICE_BUSY				_EC(80)		// �豸��Դ����
#define NET_SERVER_STARTED			_EC(81)		// �������Ѿ�����
#define NET_SERVER_STOPPED			_EC(82)		// ��������δ�ɹ�����
#define NET_LISTER_INCORRECT_SERIAL	_EC(83)		// �������к�����
#define NET_QUERY_DISKINFO_FAILED	_EC(84)		// ��ȡӲ����Ϣʧ��
#define NET_ERROR_GETCFG_SESSION	_EC(85)		// ��ȡ����Session��Ϣ
#define NET_USER_FLASEPWD_TRYTIME	_EC(86)		// ����������󳬹����ƴ���
#define NET_LOGIN_ERROR_PASSWORD	_EC(100)	// ���벻��ȷ
#define NET_LOGIN_ERROR_USER		_EC(101)	// �ʻ�������
#define NET_LOGIN_ERROR_TIMEOUT		_EC(102)	// �ȴ���¼���س�ʱ
#define NET_LOGIN_ERROR_RELOGGIN	_EC(103)	// �ʺ��ѵ�¼
#define NET_LOGIN_ERROR_LOCKED		_EC(104)	// �ʺ��ѱ�����
#define NET_LOGIN_ERROR_BLACKLIST	_EC(105)	// �ʺ��ѱ���Ϊ������
#define NET_LOGIN_ERROR_BUSY		_EC(106)	// ��Դ���㣬ϵͳæ
#define NET_LOGIN_ERROR_CONNECT		_EC(107)	// ��¼�豸��ʱ���������粢����
#define NET_LOGIN_ERROR_NETWORK		_EC(108)	// ��������ʧ��
#define NET_LOGIN_ERROR_SUBCONNECT	_EC(109)	// ��¼�豸�ɹ������޷�������Ƶͨ������������״��
#define NET_RENDER_SOUND_ON_ERROR	_EC(120)	// Render�����Ƶ����
#define NET_RENDER_SOUND_OFF_ERROR	_EC(121)	// Render��ر���Ƶ����
#define NET_RENDER_SET_VOLUME_ERROR	_EC(122)	// Render�������������
#define NET_RENDER_ADJUST_ERROR		_EC(123)	// Render�����û����������
#define NET_RENDER_PAUSE_ERROR		_EC(124)	// Render����ͣ���ų���
#define NET_RENDER_SNAP_ERROR		_EC(125)	// Render��ץͼ����
#define NET_RENDER_STEP_ERROR		_EC(126)	// Render�ⲽ������
#define NET_RENDER_FRAMERATE_ERROR	_EC(127)	// Render������֡�ʳ���
#define NET_GROUP_EXIST				_EC(140)	// �����Ѵ���
#define	NET_GROUP_NOEXIST			_EC(141)	// ����������
#define NET_GROUP_RIGHTOVER			_EC(142)	// ���Ȩ�޳���Ȩ���б�Χ
#define NET_GROUP_HAVEUSER			_EC(143)	// �������û�������ɾ��
#define NET_GROUP_RIGHTUSE			_EC(144)	// ���ĳ��Ȩ�ޱ��û�ʹ�ã����ܳ���
#define NET_GROUP_SAMENAME			_EC(145)	// ������ͬ���������ظ�
#define	NET_USER_EXIST				_EC(146)	// �û��Ѵ���
#define NET_USER_NOEXIST			_EC(147)	// �û�������
#define NET_USER_RIGHTOVER			_EC(148)	// �û�Ȩ�޳�����Ȩ��
#define NET_USER_PWD				_EC(149)	// �����ʺţ��������޸�����
#define NET_USER_FLASEPWD			_EC(150)	// ���벻��ȷ
#define NET_USER_NOMATCHING			_EC(151)	// ���벻ƥ��
#define NET_ERROR_GETCFG_ETHERNET	_EC(300)	// ��ȡ��������ʧ��
#define NET_ERROR_GETCFG_WLAN		_EC(301)	// ��ȡ����������Ϣʧ��
#define NET_ERROR_GETCFG_WLANDEV	_EC(302)	// ��ȡ���������豸ʧ��
#define NET_ERROR_GETCFG_REGISTER	_EC(303)	// ��ȡ����ע�����ʧ��
#define NET_ERROR_GETCFG_CAMERA		_EC(304)	// ��ȡ����ͷ����ʧ��
#define NET_ERROR_GETCFG_INFRARED	_EC(305)	// ��ȡ���ⱨ������ʧ��
#define NET_ERROR_GETCFG_SOUNDALARM	_EC(306)	// ��ȡ��Ƶ��������ʧ��
#define NET_ERROR_GETCFG_STORAGE    _EC(307)	// ��ȡ�洢λ������ʧ��
#define NET_ERROR_GETCFG_MAIL		_EC(308)	// ��ȡ�ʼ�����ʧ��
#define NET_CONFIG_DEVBUSY			_EC(309)	// ��ʱ�޷�����
#define NET_CONFIG_DATAILLEGAL		_EC(310)	// �������ݲ��Ϸ�
#define NET_ERROR_GETCFG_DST        _EC(311)    // ��ȡ����ʱ����ʧ��
#define NET_ERROR_SETCFG_DST        _EC(312)    // ��������ʱ����ʧ��
#define NET_ERROR_GETCFG_VIDEO_OSD  _EC(313)    // ��ȡ��ƵOSD��������ʧ��
#define NET_ERROR_SETCFG_VIDEO_OSD  _EC(314)    // ������ƵOSD��������ʧ��
#define NET_ERROR_GETCFG_GPRSCDMA   _EC(315)    // ��ȡCDMA\GPRS��������ʧ��
#define NET_ERROR_SETCFG_GPRSCDMA   _EC(316)    // ����CDMA\GPRS��������ʧ��
#define NET_ERROR_GETCFG_IPFILTER   _EC(317)    // ��ȡIP��������ʧ��
#define NET_ERROR_SETCFG_IPFILTER   _EC(318)    // ����IP��������ʧ��
#define NET_ERROR_GETCFG_TALKENCODE _EC(319)    // ��ȡ�����Խ���������ʧ��
#define NET_ERROR_SETCFG_TALKENCODE _EC(320)    // ���������Խ���������ʧ��
#define NET_ERROR_GETCFG_RECORDLEN  _EC(321)    // ��ȡ¼������������ʧ��
#define NET_ERROR_SETCFG_RECORDLEN  _EC(322)    // ����¼������������ʧ��
#define	NET_DONT_SUPPORT_SUBAREA	_EC(323)	// ��֧������Ӳ�̷���
#define	NET_ERROR_GET_AUTOREGSERVER	_EC(324)	// ��ȡ�豸������ע���������Ϣʧ��
#define	NET_ERROR_CONTROL_AUTOREGISTER		_EC(325)	// ����ע���ض���ע�����
#define	NET_ERROR_DISCONNECT_AUTOREGISTER	_EC(326)	// �Ͽ�����ע�����������
#define NET_ERROR_GETCFG_MMS				_EC(327)	// ��ȡmms����ʧ��
#define NET_ERROR_SETCFG_MMS				_EC(328)	// ����mms����ʧ��
#define NET_ERROR_GETCFG_SMSACTIVATION      _EC(329)	// ��ȡ���ż���������������ʧ��
#define NET_ERROR_SETCFG_SMSACTIVATION      _EC(330)	// ���ö��ż���������������ʧ��
#define NET_ERROR_GETCFG_DIALINACTIVATION	_EC(331)	// ��ȡ���ż���������������ʧ��
#define NET_ERROR_SETCFG_DIALINACTIVATION	_EC(332)	// ���ò��ż���������������ʧ��
#define NET_ERROR_GETCFG_VIDEOOUT           _EC(333)    // ��ѯ��Ƶ�����������ʧ��
#define NET_ERROR_SETCFG_VIDEOOUT			_EC(334)	// ������Ƶ�����������ʧ��
#define NET_ERROR_GETCFG_OSDENABLE			_EC(335)	// ��ȡosd����ʹ������ʧ��
#define NET_ERROR_SETCFG_OSDENABLE			_EC(336)	// ����osd����ʹ������ʧ��
#define NET_ERROR_SETCFG_ENCODERINFO        _EC(337)    // ��������ͨ��ǰ�˱����������ʧ��
#define NET_ERROR_GETCFG_TVADJUST		    _EC(338)	// ��ȡTV��������ʧ��
#define NET_ERROR_SETCFG_TVADJUST			_EC(339)	// ����TV��������ʧ��

#define NET_ERROR_CONNECT_FAILED			_EC(340)	// ����������ʧ��
#define NET_ERROR_SETCFG_BURNFILE			_EC(341)	// �����¼�ļ��ϴ�ʧ��
#define NET_ERROR_SNIFFER_GETCFG			_EC(342)	// ��ȡץ��������Ϣʧ��
#define NET_ERROR_SNIFFER_SETCFG			_EC(343)	// ����ץ��������Ϣʧ��
#define NET_ERROR_DOWNLOADRATE_GETCFG		_EC(344)	// ��ѯ����������Ϣʧ��
#define NET_ERROR_DOWNLOADRATE_SETCFG		_EC(345)	// ��������������Ϣʧ��
#define NET_ERROR_SEARCH_TRANSCOM			_EC(346)	// ��ѯ���ڲ���ʧ��
#define NET_ERROR_GETCFG_POINT				_EC(347)	// ��ȡԤ�Ƶ���Ϣ����
#define NET_ERROR_SETCFG_POINT				_EC(348)	// ����Ԥ�Ƶ���Ϣ����
#define NET_SDK_LOGOUT_ERROR				_EC(349)    // SDKû�������ǳ��豸
#define NET_ERROR_GET_VEHICLE_CFG			_EC(350)	// ��ȡ��������ʧ��
#define NET_ERROR_SET_VEHICLE_CFG			_EC(351)	// ���ó�������ʧ��
#define NET_ERROR_GET_ATM_OVERLAY_CFG		_EC(352)	// ��ȡatm��������ʧ��
#define NET_ERROR_SET_ATM_OVERLAY_CFG		_EC(353)	// ����atm��������ʧ��
#define NET_ERROR_GET_ATM_OVERLAY_ABILITY	_EC(354)	// ��ȡatm��������ʧ��
#define NET_ERROR_GET_DECODER_TOUR_CFG		_EC(355)	// ��ȡ������������Ѳ����ʧ��
#define NET_ERROR_SET_DECODER_TOUR_CFG		_EC(356)	// ���ý�����������Ѳ����ʧ��
#define NET_ERROR_CTRL_DECODER_TOUR			_EC(357)	// ���ƽ�����������Ѳʧ��
#define NET_GROUP_OVERSUPPORTNUM			_EC(358)	// �����豸֧������û�����Ŀ
#define NET_USER_OVERSUPPORTNUM				_EC(359)	// �����豸֧������û���Ŀ
#define NET_ERROR_GET_SIP_CFG				_EC(368)	// ��ȡSIP����ʧ��
#define NET_ERROR_SET_SIP_CFG				_EC(369)	// ����SIP����ʧ��
#define NET_ERROR_GET_SIP_ABILITY			_EC(370)	// ��ȡSIP����ʧ��


/************************************************************************
 ** ö�ٶ���
 ***********************************************************************/
// �豸����
enum NET_DEVICE_TYPE 
{
	NET_PRODUCT_NONE = 0,
	NET_DVR_NONREALTIME_MACE,					// ��ʵʱMACE
	NET_DVR_NONREALTIME,						// ��ʵʱ
	NET_NVS_MPEG1,								// ������Ƶ������
	NET_DVR_MPEG1_2,							// MPEG1��·¼���
	NET_DVR_MPEG1_8,							// MPEG1��·¼���
	NET_DVR_MPEG4_8,							// MPEG4��·¼���
	NET_DVR_MPEG4_16,							// MPEG4ʮ��·¼���
	NET_DVR_MPEG4_SX2,							// LBϵ��¼���
	NET_DVR_MEPG4_ST2,							// GBϵ��¼���
	NET_DVR_MEPG4_SH2,							// HBϵ��¼���
	NET_DVR_MPEG4_GBE,							// GBEϵ��¼���
	NET_DVR_MPEG4_NVSII,						// II��������Ƶ������
	NET_DVR_STD_NEW,							// �±�׼����Э��
	NET_DVR_DDNS,								// DDNS������
	NET_DVR_ATM,								// ATM��
	NET_NB_SERIAL,								// ������ʵʱNBϵ�л���
	NET_LN_SERIAL,								// LNϵ�в�Ʒ
	NET_BAV_SERIAL,								// BAVϵ�в�Ʒ
	NET_SDIP_SERIAL,							// SDIPϵ�в�Ʒ
	NET_IPC_SERIAL,								// IPCϵ�в�Ʒ
	NET_NVS_B,									// NVS Bϵ��
	NET_NVS_C,									// NVS Hϵ��
	NET_NVS_S,									// NVS Sϵ��
	NET_NVS_E,									// NVS Eϵ��
	NET_DVR_NEW_PROTOCOL,						// ��QueryDevState�в�ѯ�豸����,���ַ�����ʽ
	NET_NVD_SERIAL,								// ������
	NET_DVR_N5,									// N5
	NET_DVR_MIX_DVR,							// ���DVR
	NET_SVR_SERIAL,								// SVRϵ��
	NET_SVR_BS,									// SVR-BS
	NET_NVR_SERIAL,								// NVRϵ��
};

// ��������
typedef enum __LANGUAGE_TYPE
{
	DH_LANGUAGE_ENGLISH,						// Ӣ��	
	DH_LANGUAGE_CHINESE_SIMPLIFIED,				// ��������	
	DH_LANGUAGE_CHINESE_TRADITIONAL,			// ��������	
	DH_LANGUAGE_ITALIAN,						// �������	
	DH_LANGUAGE_SPANISH,						// ��������
	DH_LANGUAGE_JAPANESE,						// ���İ�	
	DH_LANGUAGE_RUSSIAN,						// ���İ�		
	DH_LANGUAGE_FRENCH,							// ���İ�		
	DH_LANGUAGE_GERMAN, 						// ���İ�		
	DH_LANGUAGE_PORTUGUESE,						// ��������	
	DH_LANGUAGE_TURKEY,							// ��������	
	DH_LANGUAGE_POLISH,							// ������	
	DH_LANGUAGE_ROMANIAN,						// ��������	
	DH_LANGUAGE_HUNGARIAN,						// ��������	
	DH_LANGUAGE_FINNISH,						// ������	
	DH_LANGUAGE_ESTONIAN,						// ��ɳ������	
	DH_LANGUAGE_KOREAN,							// ����	
	DH_LANGUAGE_FARSI,							// ��˹�� 	
	DH_LANGUAGE_DANSK,							// ������
	DH_LANGUAGE_CZECHISH,						// �ݿ���
	DH_LANGUAGE_BULGARIA,						// ����������
	DH_LANGUAGE_SLOVAKIAN,						// ˹�工����
	DH_LANGUAGE_SLOVENIA,						// ˹����������
	DH_LANGUAGE_CROATIAN,						// ���޵�����
	DH_LANGUAGE_DUTCH,							// ������
	DH_LANGUAGE_GREEK,							// ϣ����
	DH_LANGUAGE_UKRAINIAN,						// �ڿ�����
	DH_LANGUAGE_SWEDISH,						// �����
	DH_LANGUAGE_SERBIAN,						// ����ά����
	DH_LANGUAGE_VIETNAMESE,						// Խ����
	DH_LANGUAGE_LITHUANIAN,						// ��������
	DH_LANGUAGE_FILIPINO,						// ���ɱ���
	DH_LANGUAGE_ARABIC,							// ��������
	DH_LANGUAGE_CATALAN,						// ��̩��������
	DH_LANGUAGE_LATVIAN,						// ����ά����
	DH_LANGUAGE_THAI,							// ̩��
	DH_LANGUAGE_HEBREW,							// ϣ������
	DH_LANGUAGE_Bosnian,						// ��˹������
} DH_LANGUAGE_TYPE;

// ��������
typedef enum __EM_UPGRADE_TYPE
{
	DH_UPGRADE_BIOS_TYPE = 1,					// BIOS����
	DH_UPGRADE_WEB_TYPE,						// WEB����
	DH_UPGRADE_BOOT_YPE,						// BOOT����
	DH_UPGRADE_CHARACTER_TYPE,					// ���ֿ�
	DH_UPGRADE_LOGO_TYPE,						// LOGO
	DH_UPGRADE_EXE_TYPE,						// EXE�����粥������
} EM_UPGRADE_TYPE;

// ¼������(��ʱ����̬��⡢����)
typedef enum __REC_TYPE
{
	DH_REC_TYPE_TIM = 0,
	DH_REC_TYPE_MTD,
	DH_REC_TYPE_ALM,
	DH_REC_TYPE_NUM,
} REC_TYPE;

// �������� 
typedef enum __GPRSCDMA_NETWORK_TYPE
{
	DH_TYPE_AUTOSEL = 0,						// �Զ�ѡ��
	DH_TYPE_TD_SCDMA,							// TD-SCDMA���� 
	DH_TYPE_WCDMA,								// WCDMA����
	DH_TYPE_CDMA_1x,							// CDMA 1.x����
	DH_TYPE_EDGE,								// GPRS����
	DH_TYPE_EVDO,								// EVDO����
	DH_TYPE_WIFI,								// ����
} EM_GPRSCDMA_NETWORK_TYPE;

// �ӿ����ͣ���ӦCLIENT_SetSubconnCallBack�ӿ�
typedef enum __EM_INTERFACE_TYPE
{
	DH_INTERFACE_OTHER = 0x00000000,			// δ֪�ӿ�
	DH_INTERFACE_REALPLAY,						// ʵʱ���ӽӿ�
	DH_INTERFACE_PREVIEW,						// �໭��Ԥ���ӿ�
	DH_INTERFACE_PLAYBACK,						// �طŽӿ�
	DH_INTERFACE_DOWNLOAD,						// ���ؽӿ�
} EM_INTERFACE_TYPE;

/////////////////////////////////�������/////////////////////////////////

// Ԥ�����ͣ���ӦCLIENT_RealPlayEx�ӿ�
typedef enum _RealPlayType
{
	DH_RType_Realplay = 0,						// ʵʱԤ��
	DH_RType_Multiplay,							// �໭��Ԥ��
		
	DH_RType_Realplay_0,						// ʵʱ����-����������ͬ��DH_RType_Realplay
	DH_RType_Realplay_1,						// ʵʱ����-������1
	DH_RType_Realplay_2,						// ʵʱ����-������2
	DH_RType_Realplay_3,						// ʵʱ����-������3
		
	DH_RType_Multiplay_1,						// �໭��Ԥ����1����
	DH_RType_Multiplay_4,						// �໭��Ԥ����4����
	DH_RType_Multiplay_8,						// �໭��Ԥ����8����
	DH_RType_Multiplay_9,						// �໭��Ԥ����9����
	DH_RType_Multiplay_16,						// �໭��Ԥ����16����
	DH_RType_Multiplay_6,						// �໭��Ԥ����6����
	DH_RType_Multiplay_12,						// �໭��Ԥ����12����
} DH_RealPlayType;

/////////////////////////////////��̨���/////////////////////////////////

// ͨ����̨��������
typedef enum _PTZ_ControlType
{
	DH_PTZ_UP_CONTROL = 0,						// ��
	DH_PTZ_DOWN_CONTROL,						// ��
	DH_PTZ_LEFT_CONTROL,						// ��
	DH_PTZ_RIGHT_CONTROL,						// ��
	DH_PTZ_ZOOM_ADD_CONTROL,					// �䱶+
	DH_PTZ_ZOOM_DEC_CONTROL,					// �䱶-
	DH_PTZ_FOCUS_ADD_CONTROL,					// ����+
	DH_PTZ_FOCUS_DEC_CONTROL,					// ����-
	DH_PTZ_APERTURE_ADD_CONTROL,				// ��Ȧ+
	DH_PTZ_APERTURE_DEC_CONTROL,				// ��Ȧ-
    DH_PTZ_POINT_MOVE_CONTROL,					// ת��Ԥ�õ�
    DH_PTZ_POINT_SET_CONTROL,					// ����
    DH_PTZ_POINT_DEL_CONTROL,					// ɾ��
    DH_PTZ_POINT_LOOP_CONTROL,					// ���Ѳ��
    DH_PTZ_LAMP_CONTROL							// �ƹ���ˢ
} DH_PTZ_ControlType;

// ��̨������չ����
typedef enum _EXTPTZ_ControlType
{
	DH_EXTPTZ_LEFTTOP = 0x20,					// ����
	DH_EXTPTZ_RIGHTTOP,							// ����
	DH_EXTPTZ_LEFTDOWN,							// ����
	DH_EXTPTZ_RIGHTDOWN,						// ����
	DH_EXTPTZ_ADDTOLOOP,						// ����Ԥ�õ㵽Ѳ�� Ѳ����· Ԥ�õ�ֵ
	DH_EXTPTZ_DELFROMLOOP,						// ɾ��Ѳ����Ԥ�õ� Ѳ����· Ԥ�õ�ֵ
    DH_EXTPTZ_CLOSELOOP,						// ���Ѳ�� Ѳ����·
	DH_EXTPTZ_STARTPANCRUISE,					// ��ʼˮƽ��ת
	DH_EXTPTZ_STOPPANCRUISE,					// ֹͣˮƽ��ת
	DH_EXTPTZ_SETLEFTBORDER,					// ������߽�
	DH_EXTPTZ_SETRIGHTBORDER,					// �����ұ߽�
	DH_EXTPTZ_STARTLINESCAN,					// ��ʼ��ɨ
    DH_EXTPTZ_CLOSELINESCAN,					// ֹͣ��ɨ
    DH_EXTPTZ_SETMODESTART,						// ����ģʽ��ʼ	ģʽ��·
    DH_EXTPTZ_SETMODESTOP,						// ����ģʽ����	ģʽ��·
	DH_EXTPTZ_RUNMODE,							// ����ģʽ	ģʽ��·
	DH_EXTPTZ_STOPMODE,							// ֹͣģʽ	ģʽ��·
	DH_EXTPTZ_DELETEMODE,						// ���ģʽ	ģʽ��·
	DH_EXTPTZ_REVERSECOMM,						// ��ת����
	DH_EXTPTZ_FASTGOTO,							// ���ٶ�λ ˮƽ����(8192) ��ֱ����(8192) �䱶(4)
	DH_EXTPTZ_AUXIOPEN,							// �������ؿ� ������
	DH_EXTPTZ_AUXICLOSE,						// �������ع� ������
	DH_EXTPTZ_OPENMENU = 0x36,					// ������˵�
	DH_EXTPTZ_CLOSEMENU,						// �رղ˵�
	DH_EXTPTZ_MENUOK,							// �˵�ȷ��
	DH_EXTPTZ_MENUCANCEL,						// �˵�ȡ��
	DH_EXTPTZ_MENUUP,							// �˵���
	DH_EXTPTZ_MENUDOWN,							// �˵���
	DH_EXTPTZ_MENULEFT,							// �˵���
	DH_EXTPTZ_MENURIGHT,						// �˵���
	DH_EXTPTZ_ALARMHANDLE = 0x40,				// ����������̨ parm1����������ͨ����parm2��������������1-Ԥ�õ�2-��ɨ3-Ѳ����parm3������ֵ����Ԥ�õ��
	DH_EXTPTZ_MATRIXSWITCH = 0x41,				// �����л� parm1����������(��Ƶ�����)��parm2����Ƶ����ţ�parm3�������
	DH_EXTPTZ_LIGHTCONTROL,						// �ƹ������
	DH_EXTPTZ_UP_TELE = 0x70,					// �� + TELE param1=�ٶ�(1-8)����ͬ
	DH_EXTPTZ_DOWN_TELE,						// �� + TELE
	DH_EXTPTZ_LEFT_TELE,						// �� + TELE
	DH_EXTPTZ_RIGHT_TELE,						// �� + TELE
	DH_EXTPTZ_LEFTUP_TELE,						// ���� + TELE
	DH_EXTPTZ_LEFTDOWN_TELE,					// ���� + TELE
	DH_EXTPTZ_TIGHTUP_TELE,						// ���� + TELE
	DH_EXTPTZ_RIGHTDOWN_TELE,					// ���� + TELE
	DH_EXTPTZ_UP_WIDE,							// �� + WIDE param1=�ٶ�(1-8)����ͬ
	DH_EXTPTZ_DOWN_WIDE,						// �� + WIDE
	DH_EXTPTZ_LEFT_WIDE,						// �� + WIDE
	DH_EXTPTZ_RIGHT_WIDE,						// �� + WIDE
	DH_EXTPTZ_LEFTUP_WIDE,						// ���� + WIDE
	DH_EXTPTZ_LEFTDOWN_WIDE,					// ���� + WIDE
	DH_EXTPTZ_TIGHTUP_WIDE,						// ���� + WIDE
	DH_EXTPTZ_RIGHTDOWN_WIDE,					// ���� + WIDE
	DH_EXTPTZ_TOTAL,							// �������ֵ
} DH_EXTPTZ_ControlType;

/////////////////////////////////��־���/////////////////////////////////

// ��־��ѯ����
typedef enum _DH_LOG_QUERY_TYPE
{
	DHLOG_ALL = 0,								// ������־
	DHLOG_SYSTEM,								// ϵͳ��־
	DHLOG_CONFIG,								// ������־
	DHLOG_STORAGE,								// �洢���
	DHLOG_ALARM,								// ������־
	DHLOG_RECORD,								// ¼�����
	DHLOG_ACCOUNT,								// �ʺ����
	DHLOG_CLEAR,								// �����־
	DHLOG_PLAYBACK								// �ط����
} DH_LOG_QUERY_TYPE;

// ��־����
typedef enum _DH_LOG_TYPE
{
	DH_LOG_REBOOT = 0x0000,						// �豸����
	DH_LOG_SHUT,								// �豸�ػ�
	DH_LOG_UPGRADE = 0x0004,					// �豸����
	DH_LOG_CONFSAVE = 0x0100,					// ��������
	DH_LOG_CONFLOAD,							// ��ȡ����
	DH_LOG_FSERROR = 0x0200,					// �ļ�ϵͳ����
	DH_LOG_HDD_WERR,							// Ӳ��д����
	DH_LOG_HDD_RERR,							// Ӳ�̶�����
	DH_LOG_HDD_TYPE,							// ����Ӳ������
	DH_LOG_HDD_FORMAT,							// ��ʽ��Ӳ��
	DH_LOG_HDD_NOSPACE,							// ��ǰ�����̿ռ䲻��
	DH_LOG_HDD_TYPE_RW,							// ����Ӳ������Ϊ��д��
	DH_LOG_HDD_TYPE_RO,							// ����Ӳ������Ϊֻ����	
	DH_LOG_HDD_TYPE_RE,							// ����Ӳ������Ϊ������
	DH_LOG_HDD_TYPE_SS,							// ����Ӳ������Ϊ������
	DH_LOG_HDD_NONE,							// ��Ӳ�̼�¼
	DH_LOG_HDD_NOWORKHDD,						// �޹�����(û�ж�д��)
	DH_LOG_HDD_TYPE_BK,							// ����Ӳ������Ϊ������
	DH_LOG_HDD_TYPE_REVERSE,					// ����Ӳ������Ϊ��������
	DH_LOG_ALM_IN = 0x0300,						// �ⲿ���뱨����ʼ
	DH_LOG_NETALM_IN,							// ���籨������
	DH_LOG_ALM_END = 0x0302,					// �ⲿ���뱨��ֹͣ
	DH_LOG_LOSS_IN,								// ��Ƶ��ʧ������ʼ
	DH_LOG_LOSS_END,							// ��Ƶ��ʧ��������
	DH_LOG_MOTION_IN,							// ��̬��ⱨ����ʼ
	DH_LOG_MOTION_END,							// ��̬��ⱨ������
	DH_LOG_ALM_BOSHI,							// ��������������
	DH_LOG_NET_ABORT = 0x0308,					// ����Ͽ�
	DH_LOG_NET_ABORT_RESUME,					// ����ָ�
	DH_LOG_CODER_BREAKDOWN,						// ����������
	DH_LOG_CODER_BREAKDOWN_RESUME,				// ���������ϻָ�
	DH_LOG_BLIND_IN,							// ��Ƶ�ڵ�
	DH_LOG_BLIND_END,							// ��Ƶ�ڵ��ָ�
	DH_LOG_ALM_TEMP_HIGH,						// �¶ȹ���
	DH_LOG_ALM_VOLTAGE_LOW,						// ��ѹ����
	DH_LOG_ALM_BATTERY_LOW,						// �����������
	DH_LOG_ALM_ACC_BREAK,						// ACC�ϵ�
	DH_LOG_INFRAREDALM_IN = 0x03a0,				// ���߱�����ʼ
	DH_LOG_INFRAREDALM_END,						// ���߱�������
	DH_LOG_IPCONFLICT,							// IP��ͻ
	DH_LOG_IPCONFLICT_RESUME,					// IP�ָ�
	DH_LOG_SDPLUG_IN,							// SD������
	DH_LOG_SDPLUG_OUT,							// SD���γ�
	DH_LOG_NET_PORT_BIND_FAILED,				// ����˿ڰ�ʧ��
	DH_LOG_AUTOMATIC_RECORD = 0x0400,			// �Զ�¼��
	DH_LOG_MANUAL_RECORD = 0x0401,				// �ֶ�¼��
	DH_LOG_CLOSED_RECORD,						// ֹͣ¼��
	DH_LOG_LOGIN = 0x0500,						// ��¼
	DH_LOG_LOGOUT,								// ע��
	DH_LOG_ADD_USER,							// ����û�
	DH_LOG_DELETE_USER,							// ɾ���û�
	DH_LOG_MODIFY_USER,							// �޸��û�
	DH_LOG_ADD_GROUP,							// ����û���
	DH_LOG_DELETE_GROUP,						// ɾ���û���
	DH_LOG_MODIFY_GROUP,						// �޸��û���
	DH_LOG_NET_LOGIN = 0x0508,					// �����û���¼
	DH_LOG_CLEAR = 0x0600,						// �����־
	DH_LOG_SEARCHLOG,							// ��ѯ��־
	DH_LOG_SEARCH = 0x0700,						// ¼���ѯ
	DH_LOG_DOWNLOAD,							// ¼������
	DH_LOG_PLAYBACK,							// ¼��ط�
	DH_LOG_BACKUP,								// ����¼���ļ�
	DH_LOG_BACKUPERROR,							// ����¼���ļ�ʧ��

	DH_LOG_TYPE_NR = 7,
} DH_LOG_TYPE;

// ��չ��־���ͣ���ӦCLIENT_QueryLogEx�ӿڣ�����(int nType = 1������reserved = &nType)
typedef enum _DH_NEWLOG_TYPE
{
	DH_NEWLOG_REBOOT = 0x0000,					// �豸����
	DH_NEWLOG_SHUT,								// �豸�ػ�
	DH_NEWLOG_UPGRADE = 0x0004,					// �豸����
	DH_NEWLOG_CONFSAVE = 0x0100,				// ��������
	DH_NEWLOG_CONFLOAD,							// ��ȡ����
	DH_NEWLOG_FSERROR = 0x0200,					// �ļ�ϵͳ����
	DH_NEWLOG_HDD_WERR,							// Ӳ��д����
	DH_NEWLOG_HDD_RERR,							// Ӳ�̶�����
	DH_NEWLOG_HDD_TYPE,							// ����Ӳ������
	DH_NEWLOG_HDD_FORMAT,						// ��ʽ��Ӳ��
	DH_NEWLOG_HDD_NOSPACE,						// ��ǰ�����̿ռ䲻��
	DH_NEWLOG_HDD_TYPE_RW,						// ����Ӳ������Ϊ��д��
	DH_NEWLOG_HDD_TYPE_RO,						// ����Ӳ������Ϊֻ����	
	DH_NEWLOG_HDD_TYPE_RE,						// ����Ӳ������Ϊ������
	DH_NEWLOG_HDD_TYPE_SS,						// ����Ӳ������Ϊ������
	DH_NEWLOG_HDD_NONE,							// ��Ӳ�̼�¼��־
	DH_NEWLOG_HDD_NOWORKHDD,					// �޹�����(û�ж�д��)
	DH_NEWLOG_HDD_TYPE_BK,						// ����Ӳ������Ϊ������
	DH_NEWLOG_HDD_TYPE_REVERSE,					// ����Ӳ������Ϊ��������
	DH_NEWLOG_ALM_IN = 0x0300,					// �ⲿ���뱨����ʼ
	DH_NEWLOG_NETALM_IN,						// ���籨��
	DH_NEWLOG_ALM_END = 0x0302,					// �ⲿ���뱨��ֹͣ
	DH_NEWLOG_LOSS_IN,							// ��Ƶ��ʧ������ʼ
	DH_NEWLOG_LOSS_END,							// ��Ƶ��ʧ��������
	DH_NEWLOG_MOTION_IN,						// ��̬��ⱨ����ʼ
	DH_NEWLOG_MOTION_END,						// ��̬��ⱨ������
	DH_NEWLOG_ALM_BOSHI,						// ��������������
	DH_NEWLOG_NET_ABORT = 0x0308,				// ����Ͽ�
	DH_NEWLOG_NET_ABORT_RESUME,					// ����ָ�
	DH_NEWLOG_CODER_BREAKDOWN,					// ����������
	DH_NEWLOG_CODER_BREAKDOWN_RESUME,			// ���������ϻָ�
	DH_NEWLOG_BLIND_IN,							// ��Ƶ�ڵ�
	DH_NEWLOG_BLIND_END,						// ��Ƶ�ڵ��ָ�
	DH_NEWLOG_ALM_TEMP_HIGH,					// �¶ȹ���
	DH_NEWLOG_ALM_VOLTAGE_LOW,					// ��ѹ����
	DH_NEWLOG_ALM_BATTERY_LOW,					// �����������
	DH_NEWLOG_ALM_ACC_BREAK,					// ACC�ϵ�
	DH_NEWLOG_INFRAREDALM_IN = 0x03a0,			// ���߱�����ʼ
	DH_NEWLOG_INFRAREDALM_END,					// ���߱�������
	DH_NEWLOG_IPCONFLICT,						// IP��ͻ
	DH_NEWLOG_IPCONFLICT_RESUME,				// IP�ָ�
	DH_NEWLOG_SDPLUG_IN,						// SD������
	DH_NEWLOG_SDPLUG_OUT,						// SD���γ�
	DH_NEWLOG_NET_PORT_BIND_FAILED,				// ����˿ڰ�ʧ��
	DH_NEWLOG_AUTOMATIC_RECORD = 0x0400,		// �Զ�¼��
	DH_NEWLOG_MANUAL_RECORD,					// �ֶ�¼��
	DH_NEWLOG_CLOSED_RECORD,					// ֹͣ¼��
	DH_NEWLOG_LOGIN = 0x0500,					// ��¼
	DH_NEWLOG_LOGOUT,							// ע��
	DH_NEWLOG_ADD_USER,							// ����û�
	DH_NEWLOG_DELETE_USER,						// ɾ���û�
	DH_NEWLOG_MODIFY_USER,						// �޸��û�
	DH_NEWLOG_ADD_GROUP,						// ����û���
	DH_NEWLOG_DELETE_GROUP,						// ɾ���û���
	DH_NEWLOG_MODIFY_GROUP,						// �޸��û���
	DH_NEWLOG_NET_LOGIN = 0x0508,				// �����û���¼
	DH_NEWLOG_CLEAR = 0x0600,					// �����־
	DH_NEWLOG_SEARCHLOG,						// ��ѯ��־
	DH_NEWLOG_SEARCH = 0x0700,					// ¼���ѯ
	DH_NEWLOG_DOWNLOAD,							// ¼������
	DH_NEWLOG_PLAYBACK,							// ¼��ط�
	DH_NEWLOG_BACKUP,							// ����¼���ļ�
	DH_NEWLOG_BACKUPERROR,						// ����¼���ļ�ʧ��
	
	DH_NEWLOG_TYPE_NR = 8,		
} DH_NEWLOG_TYPE;

///////////////////////////////�����Խ����///////////////////////////////

// ������������
typedef enum __TALK_CODING_TYPE
{
	DH_TALK_DEFAULT = 0,						// ��ͷPCM
	DH_TALK_PCM = 1,							// ��ͷPCM
	DH_TALK_G711a,								// G711a
	DH_TALK_AMR,								// AMR
	DH_TALK_G711u,								// G711u
	DH_TALK_G726,								// G726
} DH_TALK_CODING_TYPE;

// �Խ���ʽ
typedef enum __EM_USEDEV_MODE
{
	DH_TALK_CLIENT_MODE,						// ���ÿͻ��˷�ʽ���������Խ�
	DH_TALK_SERVER_MODE,						// ���÷�������ʽ���������Խ�
	DH_TALK_ENCODE_TYPE,						// ���������Խ������ʽ
	DH_ALARM_LISTEN_MODE,						// ���ñ������ķ�ʽ
	DH_CONFIG_AUTHORITY_MODE,					// ����ͨ��Ȩ�޽������ù���
} EM_USEDEV_MODE;

// AMR��������
typedef enum __EM_ARM_ENCODE_MODE
{
	DH_TALK_AMR_AMR475 = 1,						// AMR475����
	DH_TALK_AMR_AMR515,							// AMR515����
	DH_TALK_AMR_AMR59,							// AMR59����
	DH_TALK_AMR_AMR67,							// AMR67����
	DH_TALK_AMR_AMR74,							// AMR74����
	DH_TALK_AMR_AMR795,							// AMR795����
	DH_TALK_AMR_AMR102,							// AMR102����
	DH_TALK_AMR_AMR122,							// AMR122����
} EM_ARM_ENCODE_MODE;

/////////////////////////////////�������/////////////////////////////////

// �������ͣ���ӦCLIENT_ControlDevice�ӿ�
typedef enum _CtrlType
{
	DH_CTRL_REBOOT = 0,							// �����豸	
	DH_CTRL_SHUTDOWN,							// �ر��豸
	DH_CTRL_DISK,								// Ӳ�̹���
	DH_KEYBOARD_POWER = 3,						// �������
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
	DH_TRIGGER_ALARM_IN = 100,					// ������������
	DH_TRIGGER_ALARM_OUT,						// �����������
	DH_CTRL_MATRIX,								// �������
	DH_CTRL_SDCARD,								// SD������(IPC��Ʒ)����ͬӲ�̿���
	DH_BURNING_START,							// ��¼�����ƣ���ʼ��¼
	DH_BURNING_STOP,							// ��¼�����ƣ�������¼
	DH_BURNING_ADDPWD,							// ��¼�����ƣ���������(��'\0'Ϊ��β���ַ�������󳤶�8λ)
	DH_BURNING_ADDHEAD,							// ��¼�����ƣ�����Ƭͷ(��'\0'Ϊ��β���ַ�������󳤶�1024�ֽڣ�֧�ַ��У��зָ���'\n')
	DH_BURNING_ADDSIGN,							// ��¼�����ƣ����Ӵ�㵽��¼��Ϣ(������)
	DH_BURNING_ADDCURSTOMINFO,					// ��¼�����ƣ��Զ������(��'\0'Ϊ��β���ַ�������󳤶�1024�ֽڣ�֧�ַ��У��зָ���'\n')
	DH_CTRL_RESTOREDEFAULT,						// �ָ��豸��Ĭ������
	DH_CTRL_CAPTURE_START,						// �����豸ץͼ
	DH_CTRL_CLEARLOG,							// �����־
	DH_TRIGGER_ALARM_WIRELESS = 200,			// �������߱���(IPC��Ʒ)
	DH_MARK_IMPORTANT_RECORD,					// ��ʶ��Ҫ¼���ļ�
	DH_CTRL_DISK_SUBAREA, 						// ����Ӳ�̷���	
	DH_BURNING_ATTACH,							// ��¼�����ƣ�������¼.
	DH_BURNING_PAUSE,							// ��¼��ͣ
	DH_BURNING_CONTINUE,						// ��¼����
	DH_BURNING_POSTPONE,						// ��¼˳��
	DH_CTRL_OEMCTRL,							// ��ͣ����
	DH_BACKUP_START,							// �豸���ݿ�ʼ
	DH_BACKUP_STOP,								// �豸����ֹͣ
} CtrlType;

// IO���������ӦCLIENT_QueryIOControlState�ӿ�
typedef enum _IOTYPE
{
	DH_ALARMINPUT = 1,							// ���Ʊ�������
	DH_ALARMOUTPUT = 2,							// ���Ʊ������
	DH_DECODER_ALARMOUT = 3,					// ���Ʊ������������
	DH_WIRELESS_ALARMOUT = 5,					// �������߱������
	DH_ALARM_TRIGGER_MODE = 7,					// ����������ʽ���ֶ�,�Զ�,�رգ���ʹ��TRIGGER_MODE_CONTROL�ṹ��
} DH_IOTYPE;

/////////////////////////////////�������/////////////////////////////////

// �ֱ���ö�٣���DH_DSP_ENCODECAPʹ��
typedef enum _CAPTURE_SIZE
{
	CAPTURE_SIZE_D1,							// 704*576(PAL)  704*480(NTSC)
	CAPTURE_SIZE_HD1,							// 352*576(PAL)  352*480(NTSC)
	CAPTURE_SIZE_BCIF,							// 704*288(PAL)  704*240(NTSC)
	CAPTURE_SIZE_CIF,							// 352*288(PAL)  352*240(NTSC)
	CAPTURE_SIZE_QCIF,							// 176*144(PAL)  176*120(NTSC)
	CAPTURE_SIZE_VGA,							// 640*480
	CAPTURE_SIZE_QVGA,							// 320*240
	CAPTURE_SIZE_SVCD,							// 480*480
	CAPTURE_SIZE_QQVGA,							// 160*128
	CAPTURE_SIZE_SVGA,							// 800*592
	CAPTURE_SIZE_XVGA,							// 1024*768
	CAPTURE_SIZE_WXGA,							// 1280*800
	CAPTURE_SIZE_SXGA,							// 1280*1024  
	CAPTURE_SIZE_WSXGA,							// 1600*1024  
	CAPTURE_SIZE_UXGA,							// 1600*1200
	CAPTURE_SIZE_WUXGA,							// 1920*1200
	CAPTURE_SIZE_LTF,							// 240*192
	CAPTURE_SIZE_720,							// 1280*720
	CAPTURE_SIZE_1080,							// 1920*1080
	CAPTURE_SIZE_1_3M,							// 1280*960
	CAPTURE_SIZE_NR  
} CAPTURE_SIZE;

// �����ļ����ͣ���CLIENT_ExportConfigFile�ӿ�ʹ��
typedef enum __DH_CONFIG_FILE_TYPE
{
	DH_CONFIGFILE_ALL = 0,						// ȫ�������ļ�
	DH_CONFIGFILE_LOCAL,						// ���������ļ�
	DH_CONFIGFILE_NETWORK,						// ���������ļ�
	DH_CONFIGFILE_USER,							// �û������ļ�
} DH_CONFIG_FILE_TYPE;

// NTP
typedef enum __DH_TIME_ZONE_TYPE
{
	DH_TIME_ZONE_0,								// {0, 0*3600,"GMT+00:00"}
	DH_TIME_ZONE_1,								// {1, 1*3600,"GMT+01:00"}
	DH_TIME_ZONE_2,								// {2, 2*3600,"GMT+02:00"}
	DH_TIME_ZONE_3,								// {3, 3*3600,"GMT+03:00"}
	DH_TIME_ZONE_4,								// {4, 3*3600+1800,"GMT+03:30"}
	DH_TIME_ZONE_5,								// {5, 4*3600,"GMT+04:00"}
	DH_TIME_ZONE_6,								// {6, 4*3600+1800,"GMT+04:30"}
	DH_TIME_ZONE_7,								// {7, 5*3600,"GMT+05:00"}
	DH_TIME_ZONE_8,								// {8, 5*3600+1800,"GMT+05:30"}
	DH_TIME_ZONE_9,								// {9, 5*3600+1800+900,"GMT+05:45"}
	DH_TIME_ZONE_10,							// {10, 6*3600,"GMT+06:00"}
	DH_TIME_ZONE_11,							// {11, 6*3600+1800,"GMT+06:30"}
	DH_TIME_ZONE_12,							// {12, 7*3600,"GMT+07:00"}
	DH_TIME_ZONE_13,							// {13, 8*3600,"GMT+08:00"}
	DH_TIME_ZONE_14,							// {14, 9*3600,"GMT+09:00"}
	DH_TIME_ZONE_15,							// {15, 9*3600+1800,"GMT+09:30"}
	DH_TIME_ZONE_16,							// {16, 10*3600,"GMT+10:00"}
	DH_TIME_ZONE_17,							// {17, 11*3600,"GMT+11:00"}
	DH_TIME_ZONE_18,							// {18, 12*3600,"GMT+12:00"}
	DH_TIME_ZONE_19,							// {19, 13*3600,"GMT+13:00"}
	DH_TIME_ZONE_20,							// {20, -1*3600,"GMT-01:00"}
	DH_TIME_ZONE_21,							// {21, -2*3600,"GMT-02:00"}
	DH_TIME_ZONE_22,							// {22, -3*3600,"GMT-03:00"}
	DH_TIME_ZONE_23,							// {23, -3*3600-1800,"GMT-03:30"}
	DH_TIME_ZONE_24,							// {24, -4*3600,"GMT-04:00"}
	DH_TIME_ZONE_25,							// {25, -5*3600,"GMT-05:00"}
	DH_TIME_ZONE_26,							// {26, -6*3600,"GMT-06:00"}
	DH_TIME_ZONE_27,							// {27, -7*3600,"GMT-07:00"}
	DH_TIME_ZONE_28,							// {28, -8*3600,"GMT-08:00"}
	DH_TIME_ZONE_29,							// {29, -9*3600,"GMT-09:00"}
	DH_TIME_ZONE_30,							// {30, -10*3600,"GMT-10:00"}
	DH_TIME_ZONE_31,							// {31, -11*3600,"GMT-11:00"}
	DH_TIME_ZONE_32,							// {32, -12*3600,"GMT-12:00"}
} DH_TIME_ZONE_TYPE;

typedef enum _SNAP_TYPE
{
	SNAP_TYP_TIMING = 0,
	SNAP_TYP_ALARM,
	SNAP_TYP_NUM,
} SNAP_TYPE;

/////////////////////////////////��̭����/////////////////////////////////

// �������ͣ�Ŀǰʹ�ø�ö�ٵĽӿ�����̭���벻Ҫʹ��
typedef enum _CFG_INDEX
{
    CFG_GENERAL = 0,							// ��ͨ
	CFG_COMM,									// ����
	CFG_NET,									// ����
	CFG_RECORD,									// ¼��
	CFG_CAPTURE,								// ͼ������
	CFG_PTZ,									// ��̨
	CFG_DETECT,									// ��̬���
	CFG_ALARM,									// ����
	CFG_DISPLAY,								// ��ʾ
	CFG_RESERVED,								// ������ʹ��������
	CFG_TITLE = 10,								// ͨ������
	CFG_MAIL = 11,								// �ʼ�����
	CFG_EXCAPTURE = 12,							// Ԥ��ͼ������
	CFG_PPPOE = 13,								// pppoe����
	CFG_DDNS = 14,								// DDNS����
	CFG_SNIFFER	= 15,							// ������Ӳ�������
	CFG_DSPINFO	= 16,							// ����������Ϣ
	CFG_COLOR = 126,							// ��ɫ������Ϣ
	CFG_ALL,									// ����
} CFG_INDEX;


/************************************************************************
 ** �ṹ�嶨��
 ***********************************************************************/
// ʱ��
typedef struct 
{
	DWORD				dwYear;					// ��
	DWORD				dwMonth;				// ��
	DWORD				dwDay;					// ��
	DWORD				dwHour;					// ʱ
	DWORD				dwMinute;				// ��
	DWORD				dwSecond;				// ��
} NET_TIME,*LPNET_TIME;

// ��־��Ϣ���ʱ�䶨��
typedef struct _DHDEVTIME
{
	DWORD				second		:6;			// ��	1-60		
	DWORD				minute		:6;			// ��	1-60		
	DWORD				hour		:5;			// ʱ	1-24		
	DWORD				day			:5;			// ��	1-31		
	DWORD				month		:4;			// ��	1-12		
	DWORD				year		:6;			// ��	2000-2063	
} DHDEVTIME, *LPDHDEVTIME;

// �ص�����(�첽�ӿ�)
typedef struct __NET_CALLBACK_DATA 
{
	int					nResultCode;			// �����룻0���ɹ�
	char				*pBuf;					// �������ݣ����������û����ٵģ��ӽӿ��βδ���
	int					nRetLen;				// ���ճ���
	LONG				lOperateHandle;			// �������
	void*				userdata;				// ������Ӧ�û�����
	char				reserved[16];
} NET_CALLBACK_DATA, *LPNET_CALLBACK_DATA;

///////////////////////////////������ض���///////////////////////////////

// �ص���Ƶ����֡��֡�����ṹ��
typedef struct _tagVideoFrameParam
{
	BYTE				encode;					// ��������
	BYTE				frametype;				// I = 0, P = 1, B = 2...
	BYTE				format;					// PAL - 0, NTSC - 1
	BYTE				size;					// CIF - 0, HD1 - 1, 2CIF - 2, D1 - 3, VGA - 4, QCIF - 5, QVGA - 6 ,
												// SVCD - 7,QQVGA - 8, SVGA - 9, XVGA - 10,WXGA - 11,SXGA - 12,WSXGA - 13,UXGA - 14,WUXGA - 15, LFT - 16, 720 - 17, 1080 - 18
	DWORD				fourcc;					// �����H264��������Ϊ0��MPEG4����������дFOURCC('X','V','I','D');
	DWORD				reserved;				// ����
	NET_TIME			struTime;				// ʱ����Ϣ
} tagVideoFrameParam;

// �ص���Ƶ����֡��֡�����ṹ��
typedef struct _tagCBPCMDataParam
{
	BYTE				channels;				// ������
	BYTE				samples;				// ���� 0 - 8000, 1 - 11025, 2 - 16000, 3 - 22050, 4 - 32000, 5 - 44100, 6 - 48000
	BYTE				depth;					// ������� ȡֵ8����16�ȡ�ֱ�ӱ�ʾ
	BYTE				param1;					// 0 - ָʾ�޷��ţ�1-ָʾ�з���
	DWORD				reserved;				// ����
} tagCBPCMDataParam;

// ͨ��������Ļ���ӵ����ݽṹ
typedef struct _DH_CHANNEL_OSDSTRING
{
	BOOL				bEnable;				// ʹ��
	DWORD				dwPosition[MAX_STRING_LINE_LEN];	//�����ַ���λ�á���1-9������ʾ����С����λ�ö�Ӧ
												//		7����	8��		9����
												//		4��		5��		6��
												//		1����	2��		3����
	char				szStrings[MAX_STRING_LINE_LEN][MAX_PER_STRING_LEN];	// ��������ַ���ÿ�����20���ֽ�
} DH_CHANNEL_OSDSTRING;

///////////////////////////////�ط���ض���///////////////////////////////

// ¼���ļ���Ϣ
typedef struct
{
    unsigned int		ch;						// ͨ����
    char				filename[128];			// �ļ���
    unsigned int		size;					// �ļ�����
    NET_TIME			starttime;				// ��ʼʱ��
    NET_TIME			endtime;				// ����ʱ��
    unsigned int		driveno;				// ���̺�(��������¼��ͱ���¼������ͣ�0��127��ʾ����¼��128��ʾ����¼��)
    unsigned int		startcluster;			// ��ʼ�غ�
	BYTE				nRecordFileType;		// ¼���ļ�����  0����ͨ¼��1������¼��2���ƶ���⣻3������¼��4��ͼƬ
	BYTE                bImportantRecID;		// 0:��ͨ¼�� 1:��Ҫ¼��
	BYTE                bHint;					// �ļ���λ����
	BYTE                bReserved;				// ����
} NET_RECORDFILE_INFO, *LPNET_RECORDFILE_INFO;

// ����¼��ʱ��
typedef struct  
{
	int					nChnCount;				// ͨ����Ŀ
	NET_TIME			stuFurthestTime[16];	// ����¼��ʱ��,��ЧֵΪǰ��0 �� (nChnCount-1)��.���ĳͨ��û��¼��Ļ�������¼��ʱ��ȫΪ0
	BYTE				bReserved[384];			// �����ֶ�
} NET_FURTHEST_RECORD_TIME;

///////////////////////////////������ض���///////////////////////////////

// ��ͨ������Ϣ
typedef struct
{
	int					channelcount;
	int					alarminputcount;
	unsigned char		alarm[16];				// �ⲿ����
	unsigned char		motiondection[16];		// ��̬���
	unsigned char		videolost[16];			// ��Ƶ��ʧ
} NET_CLIENT_STATE;

// ����IO����
typedef struct 
{
	unsigned short		index;					// �˿����
	unsigned short		state;					// �˿�״̬
} ALARM_CONTROL;

// ������ʽ
typedef struct
{
	unsigned short		index;					// �˿����
	unsigned short		mode;					// ������ʽ(0�ر�1�ֶ�2�Զ�);�����õ�ͨ����sdkĬ�Ͻ�����ԭ�������á�
	BYTE				bReserved[28];			
} TRIGGER_MODE_CONTROL;

// ��������������
typedef struct 
{
	int					decoderNo;				// �����������ţ���0��ʼ
	unsigned short		alarmChn;				// ��������ڣ���0��ʼ
	unsigned short		alarmState;				// �������״̬��1���򿪣�0���ر�
} DECODER_ALARM_CONTROL;

// �����ϴ����ܵı�����Ϣ
typedef struct  
{
	DWORD				dwAlarmType;			// �������ͣ�dwAlarmType = DH_UPLOAD_EVENTʱ��dwAlarmMask��bAlarmDecoderIndex����Ч��
	DWORD				dwAlarmMask;			// ������Ϣ���룬��λ��ʾ������ͨ��״̬
	char				szGlobalIP[DH_MAX_IPADDR_LEN];	// �ͻ���IP��ַ
	char				szDomainName[DH_MAX_DOMAIN_NAME_LEN];	// �ͻ�������
	int					nPort;					// �����ϴ�ʱ�ͻ������ӵĶ˿�
	char				szAlarmOccurTime[DH_ALARM_OCCUR_TIME_LEN];	// ����������ʱ��
	BYTE				bAlarmDecoderIndex;		// ��ʾ�ڼ���������������dwAlarmType = DH_UPLOAD_DECODER_ALARM ����Ч.
	BYTE				bReservedSpace[15];
} NEW_ALARM_UPLOAD;

// ¼��״̬�仯������Ϣ
typedef struct
{
	int					nChannel;				// ¼��ͨ����
	char				reserved[12];
} ALARM_RECORDING_CHANGED;

/////////////////////////////�����Խ���ض���/////////////////////////////

// ��Ƶ��ʽ��Ϣ
typedef struct
{
	BYTE				byFormatTag;			// �������ͣ���0��PCM
	WORD				nChannels;				// ������
	WORD				wBitsPerSample;			// �������			
	DWORD				nSamplesPerSec;			// ������
} DH_AUDIO_FORMAT, *LPDH_AUDIO_FORMAT;

/////////////////////////////�û�������ض���/////////////////////////////

// ֧���û�����󳤶�Ϊ8λ���豸����ӦCLIENT_QueryUserInfo��CLIENT_OperateUserInfo�ӿ�
// Ȩ����Ϣ
typedef struct _OPR_RIGHT
{
	DWORD				dwID;
	char				name[DH_RIGHT_NAME_LENGTH];
	char				memo[DH_MEMO_LENGTH];
} OPR_RIGHT;

// �û���Ϣ
typedef struct _USER_INFO
{
	DWORD				dwID;
	DWORD				dwGroupID;
	char				name[DH_USER_NAME_LENGTH];
	char				passWord[DH_USER_PSW_LENGTH];
	DWORD				dwRightNum;
	DWORD				rights[DH_MAX_RIGHT_NUM];
	char				memo[DH_MEMO_LENGTH];
	DWORD				dwReusable;				// �Ƿ��ã�1�����ã�0��������
} USER_INFO;

// �û�����Ϣ
typedef struct _USER_GROUP_INFO
{
	DWORD				dwID;
	char				name[DH_USER_NAME_LENGTH];
	DWORD				dwRightNum;
	DWORD				rights[DH_MAX_RIGHT_NUM];
	char				memo[DH_MEMO_LENGTH];
} USER_GROUP_INFO;

// �û���Ϣ��
typedef struct _USER_MANAGE_INFO
{
	DWORD				dwRightNum;				// Ȩ����Ϣ
	OPR_RIGHT			rightList[DH_MAX_RIGHT_NUM];
	DWORD				dwGroupNum;				// �û�����Ϣ
	USER_GROUP_INFO		groupList[DH_MAX_GROUP_NUM];
	DWORD				dwUserNum;				// �û���Ϣ
	USER_INFO			userList[DH_MAX_USER_NUM];
	DWORD				dwSpecial;				// �û�����������1��֧���û����ã�0����֧���û�����
} USER_MANAGE_INFO;

// ֧���û�����󳤶�Ϊ8λ��16λ���豸����Ӧ��չ�ӿ�CLIENT_QueryUserInfoEx��CLIENT_OperateUserInfoEx
#define DH_USER_NAME_LENGTH_EX		16			// �û�������
#define DH_USER_PSW_LENGTH_EX		16			// ����

// Ȩ����Ϣ
typedef struct _OPR_RIGHT_EX
{
	DWORD				dwID;
	char				name[DH_RIGHT_NAME_LENGTH];
	char				memo[DH_MEMO_LENGTH];
} OPR_RIGHT_EX;

// �û���Ϣ
typedef struct _USER_INFO_EX
{
	DWORD				dwID;
	DWORD				dwGroupID;
	char				name[DH_USER_NAME_LENGTH_EX];
	char				passWord[DH_USER_PSW_LENGTH_EX];
	DWORD				dwRightNum;
	DWORD				rights[DH_MAX_RIGHT_NUM];
	char				memo[DH_MEMO_LENGTH];
	DWORD				dwFouctionMask;			// ���룬0x00000001 - ֧���û�����
	BYTE				byReserve[32];
} USER_INFO_EX;

// �û�����Ϣ
typedef struct _USER_GROUP_INFO_EX
{
	DWORD				dwID;
	char				name[DH_USER_NAME_LENGTH_EX];
	DWORD				dwRightNum;
	DWORD				rights[DH_MAX_RIGHT_NUM];
	char				memo[DH_MEMO_LENGTH];
} USER_GROUP_INFO_EX;

// �û���Ϣ��
typedef struct _USER_MANAGE_INFO_EX
{
	DWORD				dwRightNum;				// Ȩ����Ϣ
	OPR_RIGHT_EX		rightList[DH_MAX_RIGHT_NUM];
	DWORD				dwGroupNum;				// �û�����Ϣ
	USER_GROUP_INFO_EX  groupList[DH_MAX_GROUP_NUM];
	DWORD				dwUserNum;				// �û���Ϣ
	USER_INFO_EX		userList[DH_MAX_USER_NUM];
	DWORD				dwFouctionMask;			// ���룻0x00000001 - ֧���û����ã�0x00000002 - �����޸���ҪУ��
	BYTE				byNameMaxLength;		// ֧�ֵ��û�����󳤶�
	BYTE				byPSWMaxLength;			// ֧�ֵ�������󳤶�
	BYTE				byReserve[254];
} USER_MANAGE_INFO_EX;

///////////////////////////////��ѯ��ض���///////////////////////////////

// �豸֧����������
typedef struct _DH_LANGUAGE_DEVINFO
{
	DWORD				dwLanguageNum;			// ֧�����Ը���
	BYTE				byLanguageType[252];	// ö��ֵ�����DH_LANGUAGE_TYPE
} DH_DEV_LANGUAGE_INFO, *LPDH_DEV_LANGUAGE_INFO;

// Ӳ����Ϣ
typedef struct
{
	DWORD				dwVolume;				// Ӳ�̵�����
	DWORD				dwFreeSpace;			// Ӳ�̵�ʣ��ռ�
	BYTE				dwStatus;				// Ӳ�̵�״̬,0-����,1-�,2-���ϵȣ���DWORD����ĸ�BYTE
	BYTE				bDiskNum;				// Ӳ�̺�
	BYTE				bSubareaNum;			// ������
	BYTE				bSignal;				// ��ʶ��0Ϊ���� 1ΪԶ��
} NET_DEV_DISKSTATE,*LPNET_DEV_DISKSTATE;

// �豸Ӳ����Ϣ
typedef struct _DH_HARDDISK_STATE
{
	DWORD				dwDiskNum;				// ����
	NET_DEV_DISKSTATE	stDisks[DH_MAX_DISKNUM];// Ӳ�̻������Ϣ
} DH_HARDDISK_STATE, *LPDH_HARDDISK_STATE;

typedef DH_HARDDISK_STATE	DH_SDCARD_STATE;	// SD�������ݽṹͬӲ����Ϣ

// ����������Ϣ
typedef struct  
{
	DH_TALK_CODING_TYPE	encodeType;				// ��������
	int					nAudioBit;				// λ������8��16
	DWORD				dwSampleRate;			// �����ʣ���8000��16000
	char				reserved[64];
} DHDEV_TALKDECODE_INFO;

// �豸֧�ֵ������Խ�����
typedef struct 
{
	int					nSupportNum;			// ����
	DHDEV_TALKDECODE_INFO type[64];				// ��������
	char				reserved[64];
} DHDEV_TALKFORMAT_LIST;

// ��̨������Ϣ
#define  NAME_MAX_LEN 16
typedef struct 
{
	DWORD				dwHighMask;				// �����������λ
	DWORD				dwLowMask;				// �����������λ
	char				szName[NAME_MAX_LEN];	// ������Э����
	WORD				wCamAddrMin;			// ͨ����ַ����Сֵ
	WORD				wCamAddrMax;			// ͨ����ַ�����ֵ
	WORD				wMonAddrMin;			// ���ӵ�ַ����Сֵ
	WORD				wMonAddrMax;			// ���ӵ�ַ�����ֵ
	BYTE				bPresetMin;				// Ԥ�õ����Сֵ
	BYTE				bPresetMax;				// Ԥ�õ�����ֵ
	BYTE				bTourMin;				// �Զ�Ѳ����·����Сֵ
	BYTE				bTourMax;				// �Զ�Ѳ����·�����ֵ
	BYTE				bPatternMin;			// �켣��·����Сֵ
	BYTE				bPatternMax;			// �켣��·�����ֵ
	BYTE				bTileSpeedMin;			// ��ֱ�ٶȵ���Сֵ
	BYTE				bTileSpeedMax;			// ��ֱ�ٶȵ����ֵ
	BYTE				bPanSpeedMin;			// ˮƽ�ٶȵ���Сֵ
	BYTE				bPanSpeedMax;			// ˮƽ�ٶȵ����ֵ
	BYTE				bAuxMin;				// �������ܵ���Сֵ
	BYTE				bAuxMax;				// �������ܵ����ֵ
	int					nInternal;				// ���������ʱ����
	char				cType;					// Э�������
	BYTE				bReserved_1;			// ����
	BYTE				bFuncMask;				// Э�鹦������,��λ��ʾ,�ã���ʾ֧��,��һλ��ʾ������̨���ܣ�������λ��ʱ����
	BYTE				bReserved_2;
	char				Reserved[4];
} PTZ_OPT_ATTR;

// ��¼����Ϣ
typedef struct _NET_DEV_BURNING
{
	DWORD				dwDriverType;			// ��¼���������ͣ�0��DHFS��1��DISK��2��CDRW
	DWORD				dwBusType;				// �������ͣ�0��USB��1��1394��2��IDE
	DWORD				dwTotalSpace;			// ������(KB)
	DWORD				dwRemainSpace;			// ʣ������(KB)
	BYTE				dwDriverName[DH_BURNING_DEV_NAMELEN];	// ��¼����������
} NET_DEV_BURNING, *LPNET_DEV_BURNING;

// �豸��¼����Ϣ
typedef struct _DH_BURNING_DEVINFO
{
	DWORD				dwDevNum;				// ��¼�豸����
	NET_DEV_BURNING		stDevs[DH_MAX_BURNING_DEV_NUM];	// ����¼�豸��Ϣ
} DH_BURNING_DEVINFO, *LPDH_BURNING_DEVINFO;

// ��¼����
typedef struct _DH_BURNING_PROGRESS
{
	BYTE				bBurning;				// ��¼��״̬��0�����Կ�¼��1����¼�����Ͳ��ԣ���һ���ǹ����豸��
												// 2��δ�ҵ���¼����3�������������ڿ�¼��4����¼�����ڷǿ���״̬�����ڱ��ݡ���¼��ط���
	BYTE				bRomType;				// ��Ƭ���ͣ�0�����ļ�ϵͳ��1���ƶ�Ӳ�̻�U�̣�2������
	BYTE				bOperateType;			// �������ͣ�0�����У�1�����ڱ����У�2�����ڿ�¼�У�3�����ڽ��й��̻ط�
	BYTE				bType;					// ���ݻ��¼����״̬��0��ֹͣ�������1����ʼ��2������3������4�����ڳ�ʼ��
	NET_TIME			stTimeStart;			// ��ʼʱ�䡡
	DWORD				dwTimeElapse;			// �ѿ�¼ʱ��(��)
	DWORD				dwTotalSpace;			// ����������
	DWORD				dwRemainSpace;			// ����ʣ������
	DWORD				dwBurned;				// �ѿ�¼����
	WORD				dwStatus;				// ����
	WORD				wChannelMask;			// ���ڿ�¼��ͨ������
} DH_BURNING_PROGRESS, *LPDH_BURNING_PROGRESS;

// ��־��Ϣ����Ӧ�ӿ�CLIENT_QueryLog�ӿ�
typedef struct _DH_LOG_ITEM
{
    DHDEVTIME			time;					// ����
    unsigned short		type;					// ����
    unsigned char		reserved;				// ����
    unsigned char		data;					// ����
    unsigned char		context[8];				// ����
} DH_LOG_ITEM, *LPDH_LOG_ITEM;

// ��־��Ϣ����Ӧ��չ�ӿ�CLIENT_QueryLogEx������reserved(int nType=1;reserved=&nType;)
typedef struct _DH_NEWLOG_ITEM
{
	DHDEVTIME			time;					// ����
	WORD				type;					// ����
	WORD				data;					// ����
	char				szOperator[8]; 			// �û���
	BYTE				context[16];		    // ����	
} DH_NEWLOG_ITEM, *LPDH_NEWLOG_ITEM;

// ��־��Ϣ����Ӧ�ӿ�CLIENT_QueryDeviceLog�ӿ�
typedef struct _DH_DEVICE_LOG_ITEM
{
	int					nLogType;				// ��־����
	DHDEVTIME			stuOperateTime;			// ����
	char				szOperator[16]; 		// ������
	BYTE				bReserved[3];
	BYTE				bUnionType;				//  union�ṹ����,0:szLogContext��1:stuOldLog��
	union
	{
		char			szLogContext[64];		// ��־����
		struct 
		{
			DH_LOG_ITEM		stuLog;				// �ɵ���־�ṹ��
			BYTE			bReserved[48];		// ����
		}stuOldLog;
	};
	char				reserved[16];
} DH_DEVICE_LOG_ITEM, *LPDH_DEVICE_LOG_ITEM;

// ¼����־��Ϣ����Ӧ��־�ṹ�����context
typedef struct _LOG_ITEM_RECORD
{
	DHDEVTIME			time;					// ʱ��
	BYTE				channel;				// ͨ��
	BYTE				type;					// ¼������
	BYTE				reserved[2];
} LOG_ITEM_RECORD, *LPLOG_ITEM_RECORD;

typedef struct _QUERY_DEVICE_LOG_PARAM
{
	DH_LOG_QUERY_TYPE	emLogType;				// ��ѯ��־����
	NET_TIME			stuStartTime;			// ��ѯ��־�Ŀ�ʼʱ��
	NET_TIME			stuEndTime;				// ��ѯ��־�Ľ���ʱ��
	int					nStartNum;				// ��ʱ����дӵڼ�����־��ʼ��ѯ����ʼ��һ�β�ѯ����Ϊ0
	int					nEndNum;				// һ�β�ѯ�е��ڼ�����־������
	BYTE				bReserved[48];
} QUERY_DEVICE_LOG_PARAM;

// �豸Ӳ�����¼����Ϣ
typedef struct __DEV_DISK_RECORD_INFO 
{
	NET_TIME			stuBeginTime;			// ����¼��ʱ��
	NET_TIME			stuEndTime;				// ���¼��ʱ��
	char				reserved[128];
} DEV_DISK_RECORD_INFO;

///////////////////////////////������ض���///////////////////////////////

// Ӳ�̲���
typedef struct _DISKCTRL_PARAM
{
	DWORD				dwSize;					// �ṹ���С���汾������
	int					nIndex;					// ΪӲ����Ϣ�ṹ��DH_HARDDISK_STATE�������stDisks�±꣬��0��ʼ
	int					ctrlType;				// �������ͣ�
												// 0 - �������, 1 - ��Ϊ��д��, 2 - ��Ϊֻ����
												// 3 - ��Ϊ������, 4 - �ָ�����, 5 - ��Ϊ������
} DISKCTRL_PARAM;

typedef struct  
{
	BYTE				bSubareaNum;			// Ԥ�����ĸ���
	BYTE				bIndex;					// ΪӲ����Ϣ�ṹ��DH_HARDDISK_STATE�������stDisks�±꣬��0��ʼ
	BYTE				bSubareaSize[32];		// ������С���ٷֱȣ�
	BYTE				bReserved[30];			// ����
} DISKCTRL_SUBAREA;

// ����״̬
typedef struct _ALARMCTRL_PARAM
{
	DWORD				dwSize;
	int					nAlarmNo;				// ����ͨ���ţ���0��ʼ
	int					nAction;				// 1������������0��ֹͣ����
} ALARMCTRL_PARAM;

// �������
typedef struct _MATRIXCTRL_PARAM
{
	DWORD				dwSize;
	int					nChannelNo;				// ��Ƶ����ţ���0��ʼ
	int					nMatrixNo;				// ��������ţ���0��ʼ
} MATRIXCTRL_PARAM;

// ��¼����
typedef struct _BURNING_PARM
{
	int					channelMask;			// ͨ�����룬��λ��ʾҪ��¼��ͨ��
	int					devMask;				// ��¼�����룬���ݲ�ѯ���Ŀ�¼���б���λ��ʾ
} BURNNG_PARM;

// ������¼
typedef struct _BURNING_PARM_ATTACH
{
	BOOL				bAttachBurn;			// �Ƿ�Ϊ������¼��0:����; 1:��
	BYTE				bReserved[12];			// �����ֶ�
} BURNING_PARM_ATTACH;

///////////////////////////////������ض���///////////////////////////////

//-------------------------------�豸����---------------------------------
// �豸��Ϣ
typedef struct
{
	BYTE				sSerialNumber[DH_SERIALNO_LEN];	// ���к�
	BYTE				byAlarmInPortNum;		// DVR�����������
	BYTE				byAlarmOutPortNum;		// DVR�����������
	BYTE				byDiskNum;				// DVRӲ�̸���
	BYTE				byDVRType;				// DVR����, ��ö��DHDEV_DEVICE_TYPE
	BYTE				byChanNum;				// DVRͨ������
} NET_DEVICEINFO, *LPNET_DEVICEINFO;

// �豸����汾��Ϣ����16λ��ʾ���汾�ţ���16λ��ʾ�ΰ汾��
typedef struct 
{
	DWORD				dwSoftwareVersion;
	DWORD				dwSoftwareBuildDate;
	DWORD				dwDspSoftwareVersion;
	DWORD				dwDspSoftwareBuildDate;
	DWORD				dwPanelVersion;
	DWORD				dwPanelSoftwareBuildDate;
	DWORD				dwHardwareVersion;
	DWORD				dwHardwareDate;
	DWORD				dwWebVersion;
	DWORD				dwWebBuildDate;
} DH_VERSION_INFO, *LPDH_VERSION_INFO;

// �豸����汾��Ϣ����ӦCLIENT_QueryDevState�ӿ�
typedef struct  
{
	char				szDevSerialNo[DH_DEV_SERIALNO_LEN];	// ���к�
	char				byDevType;				// �豸���ͣ���ö��DHDEV_DEVICE_TYPE
	char				szDevType[DH_DEV_TYPE_LEN];	// �豸��ϸ�ͺţ��ַ�����ʽ������Ϊ��
	int					nProtocalVer;			// Э��汾��
	char				szSoftWareVersion[DH_MAX_URL_LEN];
	DWORD				dwSoftwareBuildDate;
	char				szDspSoftwareVersion[DH_MAX_URL_LEN];
	DWORD				dwDspSoftwareBuildDate;
	char				szPanelVersion[DH_MAX_URL_LEN];
	DWORD				dwPanelSoftwareBuildDate;
	char				szHardwareVersion[DH_MAX_URL_LEN];
	DWORD				dwHardwareDate;
	char				szWebVersion[DH_MAX_URL_LEN];
	DWORD				dwWebBuildDate;
	char				reserved[256];
} DHDEV_VERSION_INFO;

// DSP������������ӦCLIENT_GetDevConfig�ӿ�
typedef struct 
{
	DWORD				dwVideoStandardMask;	// ��Ƶ��ʽ���룬��λ��ʾ�豸�ܹ�֧�ֵ���Ƶ��ʽ
	DWORD				dwImageSizeMask;		// �ֱ������룬��λ��ʾ�豸�ܹ�֧�ֵķֱ�������
	DWORD				dwEncodeModeMask;		// ����ģʽ���룬��λ��ʾ�豸�ܹ�֧�ֵı���ģʽ����	
	DWORD				dwStreamCap;			// ��λ��ʾ�豸֧�ֵĶ�ý�幦�ܣ�
												// ��һλ��ʾ֧��������
												// �ڶ�λ��ʾ֧�ָ�����1
												// ����λ��ʾ֧�ָ�����2
												// ����λ��ʾ֧��jpgץͼ
	DWORD				dwImageSizeMask_Assi[8];// ��ʾ������Ϊ���ֱ���ʱ��֧�ֵĸ������ֱ������롣
	DWORD				dwMaxEncodePower;		// DSP֧�ֵ���߱������� 
	WORD				wMaxSupportChannel;		// ÿ��DSP֧�����������Ƶͨ���� 
	WORD				wChannelMaxSetSync;		// DSPÿͨ���������������Ƿ�ͬ����0����ͬ����1��ͬ��
} DH_DSP_ENCODECAP, *LPDH_DSP_ENCODECAP;

// DSP������������չ���ͣ���ӦCLIENT_QueryDevState�ӿ�
typedef struct 
{
	DWORD				dwVideoStandardMask;	// ��Ƶ��ʽ���룬��λ��ʾ�豸�ܹ�֧�ֵ���Ƶ��ʽ
	DWORD				dwImageSizeMask;		// �ֱ������룬��λ��ʾ�豸�ܹ�֧�ֵķֱ���
	DWORD				dwEncodeModeMask;		// ����ģʽ���룬��λ��ʾ�豸�ܹ�֧�ֵı���ģʽ
	DWORD				dwStreamCap;			// ��λ��ʾ�豸֧�ֵĶ�ý�幦�ܣ�
												// ��һλ��ʾ֧��������
												// �ڶ�λ��ʾ֧�ָ�����1
												// ����λ��ʾ֧�ָ�����2
												// ����λ��ʾ֧��jpgץͼ
	DWORD				dwImageSizeMask_Assi[32];// ��ʾ������Ϊ���ֱ���ʱ��֧�ֵĸ������ֱ������롣
	DWORD				dwMaxEncodePower;		// DSP֧�ֵ���߱������� 
	WORD				wMaxSupportChannel;		// ÿ��DSP֧�����������Ƶͨ���� 
	WORD				wChannelMaxSetSync;		// DSPÿͨ���������������Ƿ�ͬ����0����ͬ����1��ͬ��
	BYTE				bMaxFrameOfImageSize[32];// ��ͬ�ֱ����µ����ɼ�֡�ʣ���dwVideoStandardMask��λ��Ӧ
	BYTE				bEncodeCap;				// ��־������ʱҪ����������������������ò�����Ч��
												// 0���������ı�������+�������ı������� <= �豸�ı���������
												// 1���������ı�������+�������ı������� <= �豸�ı���������
												// �������ı������� <= �������ı���������
												// �������ķֱ��� <= �������ķֱ��ʣ�
												// �������͸�������֡�� <= ǰ����Ƶ�ɼ�֡��
												// 2��N5�ļ��㷽��
												// �������ķֱ��� <= �������ķֱ���
												// ��ѯ֧�ֵķֱ��ʺ���Ӧ���֡��
	char				reserved[95];
} DHDEV_DSP_ENCODECAP, *LPDHDEV_DSP_ENCODECAP;

// ϵͳ��Ϣ
typedef struct 
{
	DWORD				dwSize;
	/* �������豸��ֻ������ */
	DH_VERSION_INFO		stVersion;
	DH_DSP_ENCODECAP	stDspEncodeCap;			// DSP��������
	BYTE				szDevSerialNo[DH_DEV_SERIALNO_LEN];	// ���к�
	BYTE				byDevType;				// �豸���ͣ���ö��DHDEV_DEVICE_TYPE
	BYTE				szDevType[DH_DEV_TYPE_LEN];	// �豸��ϸ�ͺţ��ַ�����ʽ������Ϊ��
	BYTE				byVideoCaptureNum;		// ��Ƶ������
	BYTE				byAudioCaptureNum;		// ��Ƶ������
	BYTE				byTalkInChanNum;		// NSP
	BYTE				byTalkOutChanNum;		// NSP
	BYTE				byDecodeChanNum;		// NSP
	BYTE				byAlarmInNum;			// �����������
	BYTE				byAlarmOutNum;			// �����������
	BYTE				byNetIONum;				// �������
	BYTE				byUsbIONum;				// USB������
	BYTE				byIdeIONum;				// IDE����
	BYTE				byComIONum;				// ��������
	BYTE				byLPTIONum;				// ��������
	BYTE				byVgaIONum;				// NSP
	BYTE				byIdeControlNum;		// NSP
	BYTE				byIdeControlType;		// NSP
	BYTE				byCapability;			// NSP����չ����
	BYTE				byMatrixOutNum;			// ��Ƶ�����������
	/* �������豸�Ŀ�д���� */
	BYTE				byOverWrite;			// Ӳ��������ʽ(���ǡ�ֹͣ)
	BYTE				byRecordLen;			// ¼��������
	BYTE				byDSTEnable;			// �Ƿ�ʵ������ʱ 1-ʵ�� 0-��ʵ��
	WORD				wDevNo;					// �豸��ţ�����ң��
	BYTE				byVideoStandard;		// ��Ƶ��ʽ:0-PAL,1-NTSC
	BYTE				byDateFormat;			// ���ڸ�ʽ
	BYTE				byDateSprtr;			// ���ڷָ��(0��"."��1��"-"��2��"/")
	BYTE				byTimeFmt;				// ʱ���ʽ (0-24Сʱ��1��12Сʱ)
	BYTE				byLanguage;				// ö��ֵ���DH_LANGUAGE_TYPE
} DHDEV_SYSTEM_ATTR_CFG, *LPDHDEV_SYSTEM_ATTR_CFG;

// �޸��豸���÷�����Ϣ
typedef struct
{
	DWORD				dwType;					// ����(��GetDevConfig��SetDevConfig������)
	WORD				wResultCode;			// �����룻0���ɹ���1��ʧ�ܣ�2�����ݲ��Ϸ���3����ʱ�޷����ã�4��û��Ȩ��
	WORD   				wRebootSign;			// ������־��0������Ҫ������1����Ҫ��������Ч
	DWORD				dwReserved[2];			// ����	
} DEV_SET_RESULT;

//DST(����ʱ)����
typedef struct  
{
	int					nYear;					// ��[200 - 2037]
	int					nMonth;					// ��[1 - 12]
	int					nHour;					// Сʱ [0 - 23]
	int					nMinute;				// ���� [0 - 59]
	int					nWeekOrDay;				// [-1 - 4]0:��ʾʹ�ð����ڼ���ķ���
												// 1: ���ܼ���: ��һ��,2: �ڶ���,3: ������,4: ������,-1: ���һ��
	union
	{
		int				iWeekDay;				// ��[0 - 6](nWeekOrDay�����ܼ���ʱ)0:������, 1:����һ, 2:���ڶ�,3:������,4:������,5:������,6:������
		int				iDay;					// ����[1 - 31] (nWeekOrDay�������ڼ���)
	};
	
	DWORD				dwReserved[8];			// ����	
}DH_DST_POINT;

typedef struct  
{
	DWORD				dwSize;
	int					nDSTType;				// ����ʱ��λ��ʽ 0:�����ڶ�λ��ʽ, 1:���ܶ�λ��ʽ
	DH_DST_POINT        stDSTStart;             // ��ʼ����ʱ
	DH_DST_POINT        stDSTEnd;				// ��������ʱ
	DWORD				dwReserved[16];			// ����	
}DHDEV_DST_CFG;


// �Զ�ά������
typedef struct
{
	DWORD				dwSize;
	BYTE				byAutoRebootDay;		// �Զ�������0���Ӳ�, 1��ÿ�죬2��ÿ�����գ�3��ÿ����һ��......
	BYTE				byAutoRebootTime;		// 0��0:00��1��1:00��......23��23:00
	BYTE				byAutoDeleteFilesTime;	// �Զ�ɾ���ļ���0���Ӳ���1��24H��2��48H��3��72H��4��96H��5��ONE WEEK��6��ONE MONTH
	BYTE				reserved[13];			// ����λ
} DHDEV_AUTOMT_CFG;

//-----------------------------ͼ��ͨ������-------------------------------

// ʱ��νṹ															    
typedef struct 
{
	BOOL				bEnable;				// ����ʾ¼��ʱ���ʱ����λ��ʾ����ʹ�ܣ��ӵ�λ����λ�ֱ��ʾ����¼�󡢱���¼����ͨ¼��
	int					iBeginHour;
	int					iBeginMin;
	int					iBeginSec;
	int					iEndHour;
	int					iEndMin;
	int					iEndSec;
} DH_TSECT, *LPDH_TSECT;

// ���򣻸��߾ఴ����8192�ı���
typedef struct {
   long					left;
   long					top;
   long					right;
   long					bottom;
} DH_RECT, *LPDH_RECT;

// OSD���Խṹ
typedef struct  tagENCODE_WIDGET
{
	DWORD				rgbaFrontground;		// �����ǰ�������ֽڱ�ʾ���ֱ�Ϊ�졢�̡�����͸����
	DWORD				rgbaBackground;			// ����ı��������ֽڱ�ʾ���ֱ�Ϊ�졢�̡�����͸����
	DH_RECT				rcRect;					// λ��
	BYTE				bShow;					// ��ʾʹ��
	BYTE				byReserved[3];
} DH_ENCODE_WIDGET, *LPDH_ENCODE_WIDGET;

// ͨ������Ƶ����
typedef struct 
{
	// ��Ƶ����
	BYTE				byVideoEnable;			// ��Ƶʹ�ܣ�1���򿪣�0���ر�
	BYTE				byBitRateControl;		// �������ƣ����ճ���"��������"����
	BYTE				byFramesPerSec;			// ֡��
	BYTE				byEncodeMode;			// ����ģʽ�����ճ���"����ģʽ"����
	BYTE				byImageSize;			// �ֱ��ʣ����ճ���"�ֱ���"����
	BYTE				byImageQlty;			// ����1-6
	WORD				wLimitStream;			// ����������
	// ��Ƶ����
	BYTE				byAudioEnable;			// ��Ƶʹ�ܣ�1���򿪣�0���ر�
	BYTE				wFormatTag;				// ��Ƶ��������
	WORD				nChannels;				// ������
	WORD				wBitsPerSample;			// �������	
	BYTE				bAudioOverlay;			// ��Ƶ����ʹ��
	BYTE				bReserved_2;
	DWORD				nSamplesPerSec;			// ������
	BYTE				bIFrameInterval;		// I֡���֡��������������I֮֡���P֡������0-149
	BYTE				bScanMode;				// NSP
	BYTE				bReserved_3;
	BYTE				bReserved_4;
} DH_VIDEOENC_OPT, *LPDH_VIDEOENC_OPT;

// ������ɫ����
typedef struct 
{
	DH_TSECT			stSect;
	BYTE				byBrightness;			// ���ȣ�0-100
	BYTE				byContrast;				// �Աȶȣ�0-100
	BYTE				bySaturation;			// ���Ͷȣ�0-100
	BYTE				byHue;					// ɫ�ȣ�0-100
	BYTE				byGainEn;				// ����ʹ��
	BYTE				byGain;					// ���棻0-100
	BYTE				byReserved[2];
} DH_COLOR_CFG, *LPDH_COLOR_CFG;

// ͼ��ͨ�����Խṹ��
typedef struct 
{
	//DWORD				dwSize;
	WORD				dwSize;
	BYTE				bNoise;
	BYTE				bReserved;
	char				szChannelName[DH_CHAN_NAME_LEN];
	DH_VIDEOENC_OPT		stMainVideoEncOpt[DH_REC_TYPE_NUM];
	DH_VIDEOENC_OPT		stAssiVideoEncOpt[DH_N_ENCODE_AUX];		
	DH_COLOR_CFG		stColorCfg[DH_N_COL_TSECT];
	DH_ENCODE_WIDGET	stTimeOSD;
	DH_ENCODE_WIDGET	stChannelOSD;
	DH_ENCODE_WIDGET	stBlindCover[DH_N_COVERS];	// �������ڵ�
	BYTE				byBlindEnable;			// �����ڸǿ��أ�0x00����ʹ���ڸǣ�0x01�����ڸ��豸����Ԥ����0x10�����ڸ�¼������Ԥ����0x11�����ڸ�
	BYTE				byBlindMask;			// �����ڸ����룻��һλ���豸����Ԥ�����ڶ�λ��¼��(������Ԥ��) */
	BYTE				bVolume;				// ������ֵ(0~100�ɵ�)
	BYTE				bVolumeEnable;			// ������ֵʹ��
} DHDEV_CHANNEL_CFG, *LPDHDEV_CHANNEL_CFG;

// Ԥ��ͼ�����
typedef struct 
{
	DWORD				dwSize;
	DH_VIDEOENC_OPT		stPreView;
	DH_COLOR_CFG		stColorCfg[DH_N_COL_TSECT];
}DHDEV_PREVIEW_CFG;

//-------------------------------��������---------------------------------

// ���ڻ�������
typedef struct
{
	BYTE				byDataBit;				// ����λ��0��5��1��6��2��7��3��8
	BYTE				byStopBit;				// ֹͣλ��0��1λ��1��1.5λ��2��2λ
	BYTE				byParity;				// У��λ��0����У�飬1����У�飻2��żУ��
	BYTE				byBaudRate;				// �����ʣ�0��300��1��600��2��1200��3��2400��4��4800��
												// 5��9600��6��19200��7��38400��8��57600��9��115200
} DH_COMM_PROP;

// 485����������
typedef struct
{ 
	DH_COMM_PROP		struComm;
	BYTE				wProtocol;				// Э�����ͣ���Ӧ"Э�����б�"�±�
	BYTE				bPTZType;				// 0-���ݣ�������̨ 1-Զ��������̨,ͨ��������ѯ(��DEV_ENCODER_CFG)��
	BYTE				wDecoderAddress;		// ��������ַ��0 - 255
	BYTE 				byMartixID;				// �����
} DH_485_CFG;

// 232��������
typedef struct
{
	DH_COMM_PROP		struComm;
	BYTE				byFunction;				// ���ڹ��ܣ���Ӧ"�������б�"�±�
	BYTE				byReserved[3];
} DH_RS232_CFG;

// �������ýṹ��
typedef struct
{
	DWORD				dwSize;

	DWORD				dwDecProListNum;				// ������Э�����
	char				DecProName[DH_MAX_DECPRO_LIST_SIZE][DH_MAX_NAME_LEN]; // Э�����б�
	DH_485_CFG			stDecoder[DH_MAX_DECODER_NUM];	// ����������ǰ����

	DWORD				dw232FuncNameNum;		// 232���ܸ���
	char				s232FuncName[DH_MAX_232FUNCS][DH_MAX_NAME_LEN];	// �������б�
	DH_RS232_CFG		st232[DH_MAX_232_NUM];	// ��232���ڵ�ǰ����
} DHDEV_COMM_CFG;

// ����״̬
typedef struct
{
	unsigned int		uBeOpened;
	unsigned int		uBaudRate;
	unsigned int		uDataBites;
	unsigned int		uStopBits;
	unsigned int		uParity;
	BYTE				bReserved[32];
} DH_COMM_STATE;

//-------------------------------¼������---------------------------------

// ��ʱ¼��
typedef struct 
{
	DWORD				dwSize;
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];
	BYTE				byPreRecordLen;			// Ԥ¼ʱ�䣬��λ��s��0��ʾ��Ԥ¼
	BYTE				byRedundancyEn;			// ¼�����࿪��
	BYTE                byRecordType;           // ¼���������ͣ�0-������ 1-������1 2-������2 3-������3
	BYTE				byReserved;
} DHDEV_RECORD_CFG, *LPDH_RECORD_CFG;

// NTP����
typedef struct  
{
	BOOL				bEnable;				//	�Ƿ�����
	int					nHostPort;				//	NTP������Ĭ�϶˿�Ϊ123
	char				szHostIp[32];			//	����IP
	char				szDomainName[128];		//	����
	int					nType;					//	�������ã�0����ʾIP��1����ʾ������2����ʾIP������
	int					nUpdateInterval;		//	����ʱ��(����)
	int					nTimeZone;				//	��DH_TIME_ZONE_TYPE
	char				reserved[128];
} DHDEV_NTP_CFG;

// FTP�ϴ�����
typedef struct
{
	struct
	{
		DH_TSECT		struSect;				// ��ʱ����ڵġ�ʹ�ܡ���Ч���ɺ���
		BOOL			bMdEn;					// �ϴ���̬���¼��
		BOOL			bAlarmEn;				// �ϴ��ⲿ����¼��
		BOOL			bTimerEn;				// �ϴ���ͨ��ʱ¼��
		DWORD			dwRev[4];
	} struPeriod[DH_TIME_SECTION];
} DH_FTP_UPLOAD_CFG;

typedef struct
{
	DWORD				dwSize;
	BOOL				bEnable;							// �Ƿ�����
	char				szHostIp[DH_MAX_IPADDR_LEN];		// ����IP
	WORD				wHostPort;							// �����˿�
	char				szDirName[DH_FTP_MAX_PATH];			// FTPĿ¼·��
	char				szUserName[DH_FTP_USERNAME_LEN];	// �û���
	char				szPassword[DH_FTP_PASSWORD_LEN];	// ����
	int					iFileLen;							// �ļ�����
	int					iInterval;							// �����ļ�ʱ����
	DH_FTP_UPLOAD_CFG	struUploadCfg[DH_MAX_CHANNUM][DH_N_WEEKS];
	char 				protocol;							// 0-FTP 1-SMB
	char				NASVer;								// ����洢�������汾0=�ϵ�FTP(��������ʾʱ���)��1=NAS�洢(����������ʱ���)
	DWORD				dwFunctionMask;						// �����������룬��λ��ʾ����16λ(����洢)���α�ʾFTP��SMB��NFS����16λ(���ش洢)����ΪDISK��U
	BYTE 				reserved[124];
} DHDEV_FTP_PROTO_CFG;

//-------------------------------��������---------------------------------

// ��̫������
typedef struct 
{
	char				sDevIPAddr[DH_MAX_IPADDR_LEN];	// DVR IP ��ַ
	char				sDevIPMask[DH_MAX_IPADDR_LEN];	// DVR IP ��ַ����
	char				sGatewayIP[DH_MAX_IPADDR_LEN];	// ���ص�ַ

	/*
	 * 1��10Mbps ȫ˫��
	 * 2��10Mbps ����Ӧ
	 * 3��10Mbps ��˫��
	 * 4��100Mbps ȫ˫��
	 * 5��100Mbps ����Ӧ
	 * 6��100Mbps ��˫��
	 * 7������Ӧ
	 */
	// Ϊ����չ��DWORD����ĸ�
	BYTE				dwNetInterface;			// NSP
	BYTE				bTranMedia;				// 0�����ߣ�1������
	BYTE				bValid;					// ��λ��ʾ����һλ��1����Ч 0����Ч���ڶ�λ��0��DHCP�ر� 1��DHCPʹ�ܣ�����λ��0����֧��DHCP 1��֧��DHCP
	BYTE				bDefaultEth;			// �Ƿ���ΪĬ�ϵ����� 1��Ĭ�� 0����Ĭ��
	char				byMACAddr[DH_MACADDR_LEN];	// MAC��ַ��ֻ��
} DH_ETHERNET; 

// Զ����������
typedef struct 
{
	BYTE				byEnable;				// ����ʹ��
	BYTE				byAssistant;            // Ŀǰֻ����PPPoE���������ã�0���������������ţ�1�������������ϲ���
	WORD				wHostPort;				// Զ������ �˿�
	char				sHostIPAddr[DH_MAX_IPADDR_LEN];		// Զ������ IP ��ַ		
	char				sHostUser[DH_MAX_HOST_NAMELEN];		// Զ������ �û���
	char				sHostPassword[DH_MAX_HOST_PSWLEN];	// Զ������ ����
} DH_REMOTE_HOST;

// �ʼ�����
typedef struct 
{
	char				sMailIPAddr[DH_MAX_IPADDR_LEN];	// �ʼ�������IP��ַ
	WORD				wMailPort;				// �ʼ��������˿�
	WORD				wReserved;				// ����
	char				sSenderAddr[DH_MAX_MAIL_ADDR_LEN];	// ���͵�ַ
	char				sUserName[DH_MAX_NAME_LEN];			// �û���
	char				sUserPsw[DH_MAX_NAME_LEN];			// �û�����
	char				sDestAddr[DH_MAX_MAIL_ADDR_LEN];	// Ŀ�ĵ�ַ
	char				sCcAddr[DH_MAX_MAIL_ADDR_LEN];		// ���͵�ַ
	char				sBccAddr[DH_MAX_MAIL_ADDR_LEN];		// ������ַ
	char				sSubject[DH_MAX_MAIL_SUBJECT_LEN];	// ����
} DH_MAIL_CFG;

// �������ýṹ��
typedef struct
{ 
	DWORD				dwSize; 

	char				sDevName[DH_MAX_NAME_LEN];	// �豸������

	WORD				wTcpMaxConnectNum;		// TCP���������
	WORD				wTcpPort;				// TCP֡���˿�
	WORD				wUdpPort;				// UDP�����˿�
	WORD				wHttpPort;				// HTTP�˿ں�
	WORD				wHttpsPort;				// HTTPS�˿ں�
	WORD				wSslPort;				// SSL�˿ں�
	DH_ETHERNET			stEtherNet[DH_MAX_ETHERNET_NUM];	// ��̫����

	DH_REMOTE_HOST		struAlarmHost;			// ����������
	DH_REMOTE_HOST		struLogHost;			// ��־������
	DH_REMOTE_HOST		struSmtpHost;			// SMTP������
	DH_REMOTE_HOST		struMultiCast;			// �ಥ��
	DH_REMOTE_HOST		struNfs;				// NFS������
	DH_REMOTE_HOST		struPppoe;				// PPPoE������
	char				sPppoeIP[DH_MAX_IPADDR_LEN]; // PPPoEע�᷵�ص�IP
	DH_REMOTE_HOST		struDdns;				// DDNS������
	char				sDdnsHostName[DH_MAX_HOST_NAMELEN];	// DDNS������
	DH_REMOTE_HOST		struDns;				// DNS������
	DH_MAIL_CFG			struMail;				// �ʼ�����
} DHDEV_NET_CFG;

// ��ddns���ýṹ��
typedef struct
{
	DWORD				dwId;					// ddns������id��
	BOOL				bEnable;				// ʹ�ܣ�ͬһʱ��ֻ����һ��ddns����������ʹ��״̬
	char				szServerType[DH_MAX_SERVER_TYPE_LEN];	// ���������ͣ�ϣ��..
	char				szServerIp[DH_MAX_DOMAIN_NAME_LEN];		// ������ip��������
	DWORD				dwServerPort;			// �������˿�
	char				szDomainName[DH_MAX_DOMAIN_NAME_LEN];	// dvr��������jeckean.3322.org
	char				szUserName[DH_MAX_HOST_NAMELEN];		// �û���
	char				szUserPsw[DH_MAX_HOST_PSWLEN];			// ����
	char				szAlias[DH_MAX_DDNS_ALIAS_LEN];			// ��������������"dahua inter ddns"
	DWORD				dwAlivePeriod;							// DDNS ����ʱ��
	char				reserved[256];
} DH_DDNS_SERVER_CFG;

typedef struct
{
	DWORD				dwSize;
	DWORD				dwDdnsServerNum;	
	DH_DDNS_SERVER_CFG	struDdnsServer[DH_MAX_DDNS_NUM];	
} DHDEV_MULTI_DDNS_CFG;

// �ʼ����ýṹ��
typedef struct 
{
	char				sMailIPAddr[DH_MAX_DOMAIN_NAME_LEN];	// �ʼ���������ַ(IP��������)
	char				sSubMailIPAddr[DH_MAX_DOMAIN_NAME_LEN];
	WORD				wMailPort;								// �ʼ��������˿�
	WORD				wSubMailPort;
	WORD				wReserved;								// ����
	char				sSenderAddr[DH_MAX_MAIL_ADDR_LEN];		// ���͵�ַ
	char				sUserName[DH_MAX_MAIL_NAME_LEN];		// �û���
	char				sUserPsw[DH_MAX_MAIL_NAME_LEN];			// �û�����
	char				sDestAddr[DH_MAX_MAIL_ADDR_LEN];		// Ŀ�ĵ�ַ
	char				sCcAddr[DH_MAX_MAIL_ADDR_LEN];			// ���͵�ַ
	char				sBccAddr[DH_MAX_MAIL_ADDR_LEN];			// ������ַ
	char				sSubject[DH_MAX_MAIL_SUBJECT_LEN];		// ����
	BYTE				bEnable;								// ʹ��0:false,	1:true
	BYTE				bSSLEnable;								// SSLʹ��
	WORD				wSendInterval;							// ����ʱ����,[0,3600]��
	BYTE				bAnonymous;								// ����ѡ��[0,1], 0��ʾFALSE,1��ʾTRUE.
	BYTE				bAttachEnable;							// ����ʹ��[0,1], 0��ʾFALSE,1��ʾTRUE.
	char				reserved[154];
} DHDEV_MAIL_CFG;

// DNS����������
typedef struct  
{
	char				szPrimaryIp[DH_MAX_IPADDR_LEN];
	char				szSecondaryIp[DH_MAX_IPADDR_LEN];
	char				reserved[256];
} DHDEV_DNS_CFG;

// ¼�����ز�������
typedef struct
{
	DWORD				dwSize;
	BOOL				bEnable;				// TRUE���������أ�FALSE����ͨ����
}DHDEV_DOWNLOAD_STRATEGY_CFG;

// ���紫���������
typedef struct
{
	DWORD				dwSize;
	BOOL				bEnable;
	int					iStrategy;				// 0���������ȣ�1�����������ȣ�2���Զ�
}DHDEV_TRANSFER_STRATEGY_CFG;

// ���õ���ʱ����ز���
typedef struct  
{
	int					nWaittime;				// �ȴ���ʱʱ��(����Ϊ��λ)��Ϊ0Ĭ��5000ms
	int					nConnectTime;			// ���ӳ�ʱʱ��(����Ϊ��λ)��Ϊ0Ĭ��1500ms
	int					nConnectTryNum;			// ���ӳ��Դ�����Ϊ0Ĭ��1��
	int					nSubConnectSpaceTime;	// ������֮��ĵȴ�ʱ��(����Ϊ��λ)��Ϊ0Ĭ��10ms
	int					nGetDevInfoTime;		// ��ȡ�豸��Ϣ��ʱʱ�䣬Ϊ0Ĭ��1000ms
	int					nConnectBufSize;		// ÿ�����ӽ������ݻ����С(�ֽ�Ϊ��λ)��Ϊ0Ĭ��250*1024
	int					nGetConnInfoTime;		// ��ȡ��������Ϣ��ʱʱ��(����Ϊ��λ)��Ϊ0Ĭ��1000ms
	BYTE				bReserved[20];			// �����ֶ�
} NET_PARAM;

// ��ӦCLIENT_SearchDevices�ӿ�
typedef struct 
{
	char				szIP[DH_MAX_IPADDR_LEN];		// IP
	int					nPort;							// �˿�
	char				szSubmask[DH_MAX_IPADDR_LEN];	// ��������
	char				szGateway[DH_MAX_IPADDR_LEN];	// ����
	char				szMac[DH_MACADDR_LEN];			// MAC��ַ
	char				szDeviceType[DH_DEV_TYPE_LEN];	// �豸����
	BYTE				bReserved[32];					// �����ֽ�
} DEVICE_NET_INFO;

//-------------------------------��������---------------------------------

// ��̨����
typedef struct 
{
	int					iType;
	int					iValue;
} DH_PTZ_LINK, *LPDH_PTZ_LINK;

// ���������ṹ��
typedef struct 
{
	/* ��Ϣ����ʽ������ͬʱ���ִ���ʽ������
	 * 0x00000001 - �����ϴ�
	 * 0x00000002 - ����¼��
	 * 0x00000004 - ��̨����
	 * 0x00000008 - �����ʼ�
	 * 0x00000010 - ������Ѳ
	 * 0x00000020 - ������ʾ
	 * 0x00000040 - �������
	 * 0x00000080 - Ftp�ϴ�
	 * 0x00000100 - ����
	 * 0x00000200 - ������ʾ
	 * 0x00000400 - ץͼ
	*/

	/* ��ǰ������֧�ֵĴ���ʽ����λ�����ʾ */
	DWORD				dwActionMask;

	/* ������������λ�����ʾ�����嶯������Ҫ�Ĳ����ڸ��Ե����������� */
	DWORD				dwActionFlag;
	
	/* �������������ͨ�������������������Ϊ1��ʾ��������� */ 
	BYTE				byRelAlarmOut[DH_MAX_ALARMOUT_NUM];
	DWORD				dwDuration;				/* ��������ʱ�� */

	/* ����¼�� */
	BYTE				byRecordChannel[DH_MAX_VIDEO_IN_NUM]; /* ����������¼��ͨ����Ϊ1��ʾ������ͨ�� */
	DWORD				dwRecLatch;				/* ¼�����ʱ�� */

	/* ץͼͨ�� */
	BYTE				bySnap[DH_MAX_VIDEO_IN_NUM];
	/* ��Ѳͨ�� */
	BYTE				byTour[DH_MAX_VIDEO_IN_NUM];

	/* ��̨���� */
	DH_PTZ_LINK			struPtzLink[DH_MAX_VIDEO_IN_NUM];
	DWORD				dwEventLatch;			/* ������ʼ��ʱʱ�䣬sΪ��λ����Χ��0~15��Ĭ��ֵ��0 */
	/* �����������������ͨ�������������������Ϊ1��ʾ��������� */ 
	BYTE				byRelWIAlarmOut[DH_MAX_ALARMOUT_NUM];
	BYTE				bMessageToNet;
	BYTE                bMMSEn;                /*���ű���ʹ��*/
	BYTE                bySnapshotTimes;       /*���ŷ���ץͼ���� */
	BYTE				bMatrixEn;				/*!< ����ʹ�� */
	DWORD				dwMatrix;				/*!< �������� */			
	BYTE				bLog;					/*!< ��־ʹ�ܣ�Ŀǰֻ����WTN��̬�����ʹ�� */
	BYTE				byReserved[103];   
} DH_MSG_HANDLE;

// �ⲿ����
typedef struct
{
	BYTE				byAlarmType;			// ���������ͣ�0�����գ�1������
	BYTE				byAlarmEn;				// ����ʹ��
	BYTE				byReserved[2];						
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT]; //NSP
	DH_MSG_HANDLE		struHandle;				// ����ʽ
} DH_ALARMIN_CFG, *LPDHDEV_ALARMIN_CFG; 

// ��̬��ⱨ��
typedef struct 
{
	BYTE				byMotionEn;				// ��̬��ⱨ��ʹ��
	BYTE				byReserved;
	WORD				wSenseLevel;			// ������
	WORD				wMotionRow;				// ��̬������������
	WORD				wMotionCol;				// ��̬������������
	BYTE				byDetected[DH_MOTION_ROW][DH_MOTION_COL]; // ����������32*32������
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];	//NSP
	DH_MSG_HANDLE		struHandle;				//����ʽ
} DH_MOTION_DETECT_CFG;

// ��Ƶ��ʧ����
typedef struct
{
	BYTE				byAlarmEn;				// ��Ƶ��ʧ����ʹ��
	BYTE				byReserved[3];
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];	//NSP
	DH_MSG_HANDLE		struHandle;				// ����ʽ
} DH_VIDEO_LOST_CFG;

// ͼ���ڵ�����
typedef struct
{
	BYTE				byBlindEnable;			// ʹ��
	BYTE				byBlindLevel;			// ������1-6
	BYTE				byReserved[2];
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];	//NSP
	DH_MSG_HANDLE		struHandle;				// ����ʽ
} DH_BLIND_CFG;

// Ӳ����Ϣ(�ڲ�����)
typedef struct 
{
	BYTE				byNoDiskEn;				// ��Ӳ��ʱ����
	BYTE				byReserved_1[3];
	DH_TSECT			stNDSect[DH_N_WEEKS][DH_N_REC_TSECT]; //NSP
	DH_MSG_HANDLE		struNDHandle;			// ����ʽ

	BYTE				byLowCapEn;				// Ӳ�̵�����ʱ����
	BYTE				byLowerLimit;			// ������ֵ��0-99
	BYTE				byReserved_2[2];
	DH_TSECT			stLCSect[DH_N_WEEKS][DH_N_REC_TSECT]; //NSP
	DH_MSG_HANDLE		struLCHandle;			// ����ʽ

	BYTE				byDiskErrEn;			// Ӳ�̹��ϱ���
	BYTE				bDiskNum;
	BYTE				byReserved_3[2];
	DH_TSECT			stEDSect[DH_N_WEEKS][DH_N_REC_TSECT]; //NSP
	DH_MSG_HANDLE		struEDHandle;			// ����ʽ
} DH_DISK_ALARM_CFG;

typedef struct
{
	BYTE				byEnable;
	BYTE				byReserved[3];
	DH_MSG_HANDLE		struHandle;
} DH_NETBROKEN_ALARM_CFG;

// ��������
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
} DHDEV_ALARM_SCHEDULE;

#define DECODER_OUT_SLOTS_MAX_NUM 		16
#define DECODER_IN_SLOTS_MAX_NUM 		16

// ��������������
typedef struct  
{
	DWORD				dwAddr;									// ������������ַ
	BOOL				bEnable;								// ����������ʹ��
	DWORD				dwOutSlots[DECODER_OUT_SLOTS_MAX_NUM];	// ����ֻ֧��8��.
	int					nOutSlotNum;							// dwOutSlots������ЧԪ�ظ���.
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];
	DH_MSG_HANDLE		struHandle[DECODER_IN_SLOTS_MAX_NUM];	// ����ֻ֧��8��.
	int					nMsgHandleNum;							// stuHandle������ЧԪ�ظ���.
	BYTE				bReserved[120];
} DH_ALARMDEC_CFG;

// �����ϴ�������
typedef struct  
{
	BYTE				byEnable;				// �ϴ�ʹ��
	BYTE				bReserverd;				// ����
	WORD				wHostPort;				// �������������˿�
	char				sHostIPAddr[DH_MAX_IPADDR_LEN];		// ��������IP

	int					nByTimeEn;				// ��ʱ�ϴ�ʹ�ܣ����������������ϴ�IP��������
	int					nUploadDay;				/* �����ϴ�����	
													"Never = 0", "Everyday = 1", "Sunday = 2", 
													"Monday = 3", Tuesday = 4", "Wednesday = 5",
													"Thursday = 6", "Friday = 7", "Saturday = 8"*/	
	int					nUploadHour;			// �����ϴ�ʱ�� ,[0~23]��
	
	DWORD				dwReserved[300]; 		// ��������չ��
} ALARMCENTER_UP_CFG;

// ȫ���л���������
typedef struct __DH_PANORAMA_SWITCH_CFG 
{
	BOOL				bEnable;				// ʹ��
	int					nReserved[5];			// ����
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];
	DH_MSG_HANDLE		struHandle;				// ��������
} DH_PANORAMA_SWITCH_CFG;

typedef struct __ALARM_PANORAMA_SWITCH_CFG 
{
	int					nAlarmChnNum;			// ����ͨ������
	DH_PANORAMA_SWITCH_CFG stuPanoramaSwitch[DH_MAX_VIDEO_IN_NUM];
} ALARM_PANORAMA_SWITCH_CFG;

// ʧȥ���㱨������
typedef struct __DH_LOST_FOCUS_CFG
{
	BOOL				bEnable;				// ʹ��
	int					nReserved[5];			// ����
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];
	DH_MSG_HANDLE		struHandle;				// ��������
} DH_LOST_FOCUS_CFG;

typedef struct __ALARM_LOST_FOCUS_CFG 
{
	int					nAlarmChnNum;			// ����ͨ������
	DH_LOST_FOCUS_CFG stuLostFocus[DH_MAX_VIDEO_IN_NUM];
} ALARM_LOST_FOCUS_CFG;

// IP��ͻ��ⱨ������
typedef struct __ALARM_IP_COLLISION_CFG
{
	BOOL				bEnable;				// ʹ��
	DH_MSG_HANDLE		struHandle;				// ��������
	int                 nReserved[300];			// ����
} ALARM_IP_COLLISION_CFG;

//------------------------------�������ڵ�--------------------------------

// �ڵ���Ϣ
typedef struct __VIDEO_COVER_ATTR
{
	DH_RECT				rcBlock;				// ���ǵ���������
	int					nColor;					// ���ǵ���ɫ
	BYTE				bBlockType;				// ���Ƿ�ʽ��0���ڿ飬1��������
	BYTE				bEncode;				// ���뼶�ڵ���1����Ч��0������Ч
	BYTE				bPriview;				// Ԥ���ڵ��� 1����Ч��0������Ч
	char				reserved[29];			// ����
} VIDEO_COVER_ATTR;

// �������ڵ�����
typedef struct __DHDEV_VIDEOCOVER_CFG 
{
	DWORD				dwSize;
	char				szChannelName[DH_CHAN_NAME_LEN]; // ֻ��
	BYTE				bTotalBlocks;			// ֧�ֵ��ڵ�����
	BYTE				bCoverCount;			// �����õ��ڵ�����
	VIDEO_COVER_ATTR	CoverBlock[DH_MAX_VIDEO_COVER_NUM]; // ���ǵ�����	
	char				reserved[30];			// ����
}DHDEV_VIDEOCOVER_CFG;

////////////////////////////////IPC��Ʒ֧��////////////////////////////////

// ��������������Ϣ
typedef struct 
{
	int					nEnable;				// ����ʹ��
	char				szSSID[36];				// SSID
	int					nLinkMode;				// ����ģʽ��0��auto��1��adhoc��2��Infrastructure
	int					nEncryption;			// ���ܣ�0��off��2��WEP64bit��3��WEP128bit, 4:WPA-PSK-TKIP, 5: WPA-PSK-CCMP
	int					nKeyType;				// 0��Hex��1��ASCII
    int					nKeyID;					// ���
	union
	{
		char			szKeys[4][32];			// ��������
		char			szWPAKeys[128];			// nEncryptionΪ4��5ʱʹ�ã���128���ȣ����ý�������
	};
	int					nKeyFlag;
	char				reserved[12];
} DHDEV_WLAN_INFO;

// ѡ��ʹ��ĳ�������豸
typedef struct  
{
	char				szSSID[36];
	int					nLinkMode;				// ����ģʽ��0��adhoc��1��Infrastructure
	int 				nEncryption;			// ���ܣ�0��off��2��WEP64bit��3��WEP128bit
	char				reserved[48];
} DHDEV_WLAN_DEVICE;

// �������������豸�б�
typedef struct  
{
	DWORD				dwSize;
	BYTE				bWlanDevCount;			// �������������豸����
	DHDEV_WLAN_DEVICE	lstWlanDev[DH_MAX_WLANDEVICE_NUM];
	char				reserved[255];
} DHDEV_WLAN_DEVICE_LIST;

// ����ע���������
typedef struct  
{
	char				szServerIp[32];			// ע�������IP
	int					nServerPort;			// �˿ں�
	char				reserved[64];
} DHDEV_SERVER_INFO;

typedef struct  
{
	DWORD				dwSize;
	BYTE				bServerNum;				// ֧�ֵ����ip��
	DHDEV_SERVER_INFO	lstServer[DH_MAX_REGISTER_SERVER_NUM];
	BYTE				bEnable;				// ʹ��
	char				szDeviceID[32];			// �豸id
	char				reserved[94];
} DHDEV_REGISTER_SERVER;

// ����ͷ����
typedef struct __DHDEV_CAMERA_INFO
{
	BYTE				bBrightnessEn;			// ���ȿɵ���1���ɣ�0������
	BYTE				bContrastEn;			// �Աȶȿɵ�
	BYTE				bColorEn;				// ɫ�ȿɵ�
	BYTE				bGainEn;				// ����ɵ�
	BYTE				bSaturationEn;			// ���Ͷȿɵ�
	BYTE				bBacklightEn;			// ���ⲹ�� 0��ʾ��֧�ֱ��ⲹ��,1��ʾ֧��һ����������,�أ���2��ʾ֧��������������,��,�ͣ���3��ʾ֧��������������,��,��,�ͣ�
	BYTE				bExposureEn;			// �ع�ѡ��:
												// 0:��ʾ��֧���ع����
												// 1:��ʾֻ֧���Զ��ع�
												// n:�ع�ĵȼ���(1��ʾ֧���Զ��ع� 2~n��ʾ֧�ֵ��ֶ������ع�ĵȼ�)
	BYTE				bColorConvEn;			// �Զ��ʺ�ת���ɵ�
	BYTE				bAttrEn;				// ����ѡ�1���ɣ�0������
	BYTE				bMirrorEn;				// ����1��֧�֣�0����֧��
    BYTE				bFlipEn;				// ��ת��1��֧�֣�0����֧��
	BYTE				iWhiteBalance;			// ��ƽ�� 2 ֧���龰ģʽ�� 1 ֧�ְ�ƽ�� ��0 ��֧��
	BYTE				iSignalFormatMask;		// �źŸ�ʽ���룬��λ�ӵ͵���λ�ֱ�Ϊ��0-Inside(�ڲ�����) 1- BT656 2-720p 3-1080i  4-1080p  5-1080sF
	BYTE				bRotate90;				//	90����ת 0-��֧�� 1-֧��
    BYTE				bLimitedAutoExposure;   // �Ƿ�֧�ִ�ʱ�������޵��Զ��ع�
    BYTE				bCustomManualExposure;  // �Ƿ�֧���û��Զ����ֶ��ع�ʱ��
	BYTE				bRev[120];				// ����
} DHDEV_CAMERA_INFO;

// ����ͷ��������
typedef struct __DHDEV_CAMERA_CFG 
{
	DWORD				dwSize;
	BYTE				bExposure;				// �ع�ģʽ��ȡֵ��Χȡ�����豸��������0-�Զ��ع⣬1-�ع�ȼ�1��2-�ع�ȼ�2��n-1����ع�ȼ��� n��ʱ�������޵��Զ��ع� n+1�Զ���ʱ���ֶ��ع� (n==bExposureEn��
	BYTE				bBacklight;				// ���ⲹ�������ⲹ���ȼ�ȡֵ��Χȡ�����豸��������0-�رգ�1-���ⲹ��ǿ��1��2-���ⲹ��ǿ��2��n-��󱳹ⲹ���ȼ��� 
	BYTE				bAutoColor2BW;			// ��/ҹģʽ��2����(�ڰ�)��1���Զ���0����(��ɫ)
	BYTE				bMirror;				// ����1������0����
	BYTE				bFlip;					// ��ת��1������0����
	BYTE				bLensEn;				// �Զ���Ȧ��������: 1��֧�֣�0 ����֧��
	BYTE				bLensFunction;			// �Զ���Ȧ����: 1:�����Զ���Ȧ��0: �ر��Զ���Ȧ
	BYTE				bWhiteBalance;			// ��ƽ�� 0:Disabled,1:Auto 2:sunny 3:cloudy 4:home 5:office 6:night
	BYTE				bSignalFormat;			// �źŸ�ʽ0-Inside(�ڲ�����) 1- BT656 2-720p 3-1080i  4-1080p  5-1080sF
	BYTE				bRotate90;				// 0-����ת��1-˳ʱ��90�㣬2-��ʱ��90��
	BYTE                reserved[2];            // ����
	float				ExposureValue1;			// �Զ��ع�ʱ�����޻����ֶ��ع��Զ���ʱ��,����Ϊ��λ��ȡֵ0.1ms~80ms
	float				ExposureValue2;			// �Զ��ع�ʱ������,����Ϊ��λ��ȡֵ0.1ms~80ms
	
	char				bRev[108];				// ����
} DHDEV_CAMERA_CFG;

#define ALARM_MAX_NAME 64
// (����)���ⱨ������
typedef struct
{
	BOOL				bEnable;				// ��������ʹ��
	char				szAlarmName[DH_MAX_ALARM_NAME];	// ������������
	int					nAlarmInPattern;		// ���������벨��
	int					nAlarmOutPattern;		// �����������
	char				szAlarmInAddress[DH_MAX_ALARM_NAME];// ���������ַ
	int					nSensorType;			// �ⲿ�豸���������ͳ��� or ����
	int					nDefendEfectTime;		// ��������ʱʱ�䣬�ڴ�ʱ���ñ���������Ч
	int					nDefendAreaType;		// �������� 
	int					nAlarmSmoothTime;		// ����ƽ��ʱ�䣬���ڴ�ʱ�������ֻ��һ��������������������������Ե�����һ��
	char				reserved[128];
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];
	DH_MSG_HANDLE		struHandle;				// ����ʽ
} DH_INFRARED_INFO;

// ����ң��������
typedef struct 
{
	BYTE				address[ALARM_MAX_NAME];// ң������ַ
	BYTE				name[ALARM_MAX_NAME];	// ң��������
	BYTE				reserved[32];			// �����ֶ�
} DH_WI_CONFIG_ROBOT;

// ���߱����������
typedef struct 
{
	BYTE				address[ALARM_MAX_NAME];// ���������ַ
	BYTE				name[ALARM_MAX_NAME];	// �����������
	BYTE				reserved[32];			// �����ֶ�
} DH_WI_CONFIG_ALARM_OUT;

typedef struct  
{
	DWORD				dwSize;
	BYTE				bAlarmInNum;			// ���߱���������
	BYTE				bAlarmOutNum;			// ���߱��������
	DH_WI_CONFIG_ALARM_OUT AlarmOutAddr[16];	// ���������ַ
	BYTE				bRobotNum;				// ң��������
	DH_WI_CONFIG_ROBOT RobotAddr[16];			// ң������ַ
	DH_INFRARED_INFO	InfraredAlarm[16];
	char				reserved[256];
} DH_INFRARED_CFG;

// ����Ƶ��ⱨ����Ϣ
typedef struct
{
	int					channel;				// ����ͨ����
	int					alarmType;				// �������ͣ�0����Ƶֵ���ͣ�1����Ƶֵ����
	unsigned int		volume;					// ����ֵ
	char				reserved[256];
} NET_NEW_SOUND_ALARM_STATE;

typedef struct  
{
	int					channelcount;			// ������ͨ������
	NET_NEW_SOUND_ALARM_STATE SoundAlarmInfo[DH_MAX_ALARM_IN_NUM];
} DH_NEW_SOUND_ALARM_STATE;

// ץͼ�������Խṹ��
typedef struct 
{
	int					nChannelNum;			// ͨ����
	DWORD				dwVideoStandardMask;	// �ֱ���(��λ)������鿴ö��CAPTURE_SIZE						
	int					nFramesCount;			// Frequence[128]�������Ч����
	char				Frames[128];			// ֡��(����ֵ)
												// -25��25��1֡��-24��24��1֡��-23��23��1֡��-22��22��1֡
												// ����
												// 0����Ч��1��1��1֡��2��1��2֡��3��1��3֡
												// 4��1��4֡��5��1��5֡��17��1��17֡��18��1��18֡
												// 19��1��19֡��20��1��20֡
												// ����
												// 25: 1��25֡
	int					nSnapModeCount;			// SnapMode[16]�������Ч����
	char				SnapMode[16];			// (����ֵ)0����ʱ����ץͼ��1���ֶ�����ץͼ
	int					nPicFormatCount;		// Format[16]�������Ч����
	char 				PictureFormat[16];		// (����ֵ)0��BMP��ʽ��1��JPG��ʽ
	int					nPicQualityCount;		// Quality[32]�������Ч����
	char 				PictureQuality[32];		// ����ֵ
												// 100��ͼ������100%��80:ͼ������80%��60:ͼ������60%
												// 50:ͼ������50%��30:ͼ������30%��10:ͼ������10%
	char 				nReserved[128];			// ����
} DH_QUERY_SNAP_INFO;

typedef struct 
{
	int					nChannelCount;			// ͨ������
	DH_QUERY_SNAP_INFO  stuSnap[DH_MAX_CHANNUM];
} DH_SNAP_ATTR_EN;


/* IP���˹������� */
#define DH_IPIFILTER_NUM			200			// IP

// IP��Ϣ
typedef struct 
{
	DWORD				dwIPNum;				// IP����
	char				SZIP[DH_IPIFILTER_NUM][DH_MAX_IPADDR_LEN]; // IP
	char				byReserve[32];			// ����
} IPIFILTER_INFO;

// IP�������ýṹ��
typedef struct
{
	DWORD				dwSize;
	DWORD				dwEnable;				// ʹ��
	DWORD				dwType;					// ��ǰ�������ͣ�0�������� 1�����������豸ֻ��ʹһ��������Ч�������ǰ����������Ǻ�������
	IPIFILTER_INFO		BannedIP;				// ������
	IPIFILTER_INFO		TrustIP;				// ������
	char				byReserve[256];			// ����
} DHDEV_IPIFILTER_CFG;

/*�����Խ���������*/
typedef struct
{
	DWORD				dwSize;
	int					nCompression;			// ѹ����ʽ��ö��ֵ�����DH_TALK_CODING_TYPE��������豸֧�ֵ������Խ���������ѹ����ʽ��
	int					nMode;					// ����ģʽ��ö��ֵ��Ϊ0ʱ��ʾ��ѹ����ʽ��֧�ֱ���ģʽ��
												// ����ѹ����ʽ�������ö�Ӧ�ı����ʽ����
												// AMR���EM_ARM_ENCODE_MODE
	char				byReserve[256];			// ����
} DHDEV_TALK_ENCODE_CFG;

// ������mobile�������
// (�¼�������ý�����/���ŷ���)MMS���ýṹ��
typedef struct
{
	DWORD				dwSize;
	DWORD				dwEnable;				// ʹ��
	DWORD				dwReceiverNum;			// ���Ž����߸���
	char				SZReceiver[DH_MMS_RECEIVER_NUM][32];	// ���Ž����ߣ�һ��Ϊ�ֻ�����
    BYTE                byType;					// ������Ϣ���� 0:MMS��1:SMS
	char                SZTitle[32];			// ������Ϣ����
	char				byReserve[223];			// ����
} DHDEV_MMS_CFG;

// (���ż���������������)
typedef struct  
{
	DWORD				dwSize;
	DWORD				dwEnable;				// ʹ��
	DWORD				dwSenderNum;			// ���ŷ����߸���
	char				SZSender[DH_MMS_SMSACTIVATION_NUM][32];	// ���ŷ����ߣ�һ��Ϊ�ֻ�����
 	char				byReserve[256];			// ����
}DHDEV_SMSACTIVATION_CFG;

// (���ż���������������)
typedef struct  
{
	DWORD				dwSize;
	DWORD				dwEnable;				// ʹ��
	DWORD				dwCallerNum;			// �����߸���
	char				SZCaller[DH_MMS_DIALINACTIVATION_NUM][32];	// ������, һ��Ϊ�ֻ�����
 	char				byReserve[256];			// ����
}DHDEV_DIALINACTIVATION_CFG;
// ������mobile�������


// ���������ź�ǿ�Ƚṹ��
typedef struct
{
	DWORD				dwSize;
	DWORD				dwTDSCDMA;				// TD-SCDMAǿ�ȣ���Χ��0��100
	DWORD				dwWCDMA;				// WCDMAǿ�ȣ���Χ��0��100
	DWORD				dwCDMA1x;				// CDMA1xǿ�ȣ���Χ��0��100
	DWORD				dwEDGE;					// EDGEǿ�ȣ���Χ��0��100
	DWORD				dwEVDO;					// EVDOǿ�ȣ���Χ��0��100
	int					nCurrentType;			// ��ǰ����
												// 0	�豸��֧����һ��
												// 1    TD_SCDMA
												// 2	WCDMA
												// 3	CDMA_1x
												// 4	EDGE
												// 5	EVDO
	char				byReserve[252];			// ����
} DHDEV_WIRELESS_RSS_INFO;

typedef struct _DHDEV_SIP_CFG
{
	BOOL	bUnregOnBoot;						//Unregister on Reboot
	char	szAccoutName[64];					//Account Name
	char	szSIPServer[128];					//SIP Server
	char	szOutBoundProxy[128];				//Outbound Proxy
	DWORD	dwSIPUsrID;							//SIP User ID
	DWORD	dwAuthID;							//Authenticate ID
	char	szAuthPsw[64];						//Authenticate Password
	char	szSTUNServer[128];					//STUN Server
	DWORD	dwRegExp;							//Registration Expiration
	DWORD	dwLocalSIPPort;						//Local SIP Port
	DWORD	dwLocalRTPPort;						//Local RTP Port
	BYTE	bReserved[1024];					//reserved
} DHDEV_SIP_CFG;

typedef struct _DHDEV_SIP_STATE
{
	BOOL	bOnline;							//TRUE:online, FALSE:offline
	BYTE	bReserved[64];						//����
}DHDEV_SIP_STATE;

/***************************** ��̨Ԥ�Ƶ����� ***********************************/
typedef struct _POINTEANBLE
{
	BYTE				bPoint;	//Ԥ�Ƶ����Ч��Χ������[1,80]����Ч����Ϊ0��
	BYTE				bEnable;	//�Ƿ���Ч,0��Ч��1��Ч
	BYTE				bReserved[2];
} POINTEANBLE;

typedef struct _POINTCFG
{
	char				szIP[DH_MAX_IPADDR_LEN];// ip
	int					nPort;					// �˿�	
	POINTEANBLE			stuPointEnable[80];		// Ԥ�Ƶ�ʹ��
	BYTE				bReserved[256];
}POINTCFG;

typedef struct _DHDEV_POINT_CFG
{
	int					nSupportNum;			// ֻ�����������õ�ʱ����Ҫ���ظ�sdk����ʾ֧�ֵ�Ԥ�Ƶ���
	POINTCFG			stuPointCfg[16];		// ��ά�±��ʾͨ���š�Ҫ���õĵ���ֵ����ǰnSupportNum���±����档
	BYTE				bReserved[256];			// ����
}DHDEV_POINT_CFG;
////////////////////////////////����DVR֧��////////////////////////////////

// GPS��Ϣ(�����豸)
typedef struct _GPS_Info
{
    NET_TIME			revTime;				// ��λʱ��
	char				DvrSerial[50];			// �豸���к�
    double				longitude;				// ����(��λ�ǰ����֮�ȣ���Χ0-360��)
    double				latidude;				// γ��(��λ�ǰ����֮�ȣ���Χ0-180��)
    double				height;					// �߶�(��)
    double				angle;					// �����(��������Ϊԭ�㣬˳ʱ��Ϊ��)
    double				speed;					// �ٶ�(��λ�Ǻ��speed/1000*1.852����/Сʱ)
    WORD				starCount;				// ��λ����
    BOOL				antennaState;			// ����״̬(true �ã�false ��)
    BOOL				orientationState;		// ��λ״̬(true ��λ��false ����λ)
} GPS_Info,*LPGPS_Info;

// ץͼ�����ṹ��
typedef struct _snap_param
{
	unsigned int		Channel;				// ץͼ��ͨ��
	unsigned int		Quality;				// ���ʣ�1~6
	unsigned int		ImageSize;				// �����С��0��QCIF��1��CIF��2��D1
	unsigned int		mode;					// ץͼģʽ��0����ʾ����һ֡��1����ʾ��ʱ��������2����ʾ��������
	unsigned int		InterSnap;				// ʱ�䵥λ�룻��mode=1��ʾ��ʱ��������ʱ����ʱ����Ч
	unsigned int		CmdSerial;				// �������к�
	unsigned int		Reserved[4];
} SNAP_PARAMS, *LPSNAP_PARAMS;

// ץͼ��������
typedef struct 
{
	DWORD				dwSize;
	BYTE				bTimingEnable;				// ��ʱץͼ����(����ץͼ�����ڸ�������������������)
	BYTE                bReserved;
	short	            PicTimeInterval;			// ��ʱץͼʱ��������λΪ��,Ŀǰ�豸֧������ץͼʱ����Ϊ30����                           
	DH_VIDEOENC_OPT		struSnapEnc[SNAP_TYP_NUM]; // ץͼ�������ã���֧�����еķֱ��ʡ����ʡ�֡�����ã�֡���������Ǹ�������ʾһ��ץͼ�Ĵ�����
} DHDEV_SNAP_CFG;

//////////////////////////////////ATM֧��//////////////////////////////////

typedef struct
{
	int					Offset;					// ��־λ��λƫ��
	int					Length;					// ��־λ�ĳ���
	char				Key[16];				// ��־λ��ֵ
} DH_SNIFFER_FRAMEID;

typedef struct 
{
	int					Offset;					// ��־λ��λƫ��
	int					Offset2;				// Ŀǰû��Ӧ��
	int					Length;					// ��־λ�ĳ���
	int					Length2;				// Ŀǰû��Ӧ��
	char				KeyTitle[24];			// �����ֵ
} DH_SNIFFER_CONTENT;

// ����ץ������
typedef struct 
{
	DH_SNIFFER_FRAMEID	snifferFrameId;			// ÿ��FRAME ID ѡ��
	DH_SNIFFER_CONTENT	snifferContent[DH_SNIFFER_CONTENT_NUM];	// ÿ��FRAME��Ӧ��4��ץ������
} DH_SNIFFER_FRAME;

// ÿ��ץ����Ӧ�����ýṹ
typedef struct
{
	char				SnifferSrcIP[DH_MAX_IPADDR_LEN];	// ץ��Դ��ַ		
	int					SnifferSrcPort;			// ץ��Դ�˿�
	char				SnifferDestIP[DH_MAX_IPADDR_LEN];	// ץ��Ŀ���ַ
	int					SnifferDestPort;		// ץ��Ŀ��˿�
	char				reserved[28];			// �����ֶ�
	DH_SNIFFER_FRAME	snifferFrame[DH_SNIFFER_FRAMEID_NUM];	// 6��FRAME ѡ��
	int					displayPosition;		// ��ʾλ��
	int					recdChannelMask;		// ͨ������
} DH_ATM_SNIFFER_CFG;

typedef struct  
{
	DWORD				dwSize;
	DH_ATM_SNIFFER_CFG	SnifferConfig[4];
	char				reserved[256];			// �����ֶ�
} DHDEV_SNIFFER_CFG;

typedef DH_SNIFFER_FRAMEID DH_SNIFFER_FRAMEID_EX;
typedef DH_SNIFFER_CONTENT DH_SNIFFER_CONTENT_EX;

// ����ץ������
typedef struct  
{
	DH_SNIFFER_FRAMEID	snifferFrameId;								// ÿ��FRAME ID ѡ��
	DH_SNIFFER_CONTENT	snifferContent[DH_SNIFFER_CONTENT_NUM_EX];	// ÿ��FRAME��Ӧ��8��ץ������	
} DH_SNIFFER_FRAME_EX;

// ÿ��ץ����Ӧ�����ýṹ
typedef struct
{
	char				SnifferSrcIP[DH_MAX_IPADDR_LEN];					// ץ��Դ��ַ		
	int					SnifferSrcPort;										// ץ��Դ�˿�
	char				SnifferDestIP[DH_MAX_IPADDR_LEN];					// ץ��Ŀ���ַ
	int					SnifferDestPort;									// ץ��Ŀ��˿�
	DH_SNIFFER_FRAME_EX	snifferFrame[DH_SNIFFER_FRAMEID_NUM];				// 6��FRAME ѡ��
	int					displayPosition;									// ��ʾλ��
	int					recdChannelMask;									// ͨ������
	BOOL				bDateScopeEnable;									// ������Դʹ��
	BOOL				bProtocolEnable;									// Э��ʹ��
	char				szProtocolName[DH_SNIFFER_PROTOCOL_SIZE];			// Э������
	int					nSnifferMode;										// ץ����ʽ��0:net,1:232.
	char				reserved[256];
} DH_ATM_SNIFFER_CFG_EX;

// Atm�������Ͳ�ѯ���ؽṹ��
#define ATM_MAX_TRADE_TYPE_NAME	64
#define ATM_MAX_TRADE_NUM		1024

typedef struct __DH_ATM_QUERY_TRADE   
{
	int					nTradeTypeNum;										// ʵ�ʽ�����������
	int					nExceptionTypeNum;									// ʵ���쳣�¼�����
	char				szSupportTradeType[ATM_MAX_TRADE_NUM][ATM_MAX_TRADE_TYPE_NAME];    // �����¼�������
	char				szSupportExceptionType[ATM_MAX_TRADE_NUM][ATM_MAX_TRADE_TYPE_NAME];// �쳣�¼�������
} DH_ATM_QUERY_TRADE, *LPDH_ATM_QUERY_TRADE;

/////////////////////////////////������֧��/////////////////////////////////

// ��������Ϣ
typedef struct __DEV_DECODER_INFO 
{
	char			szDecType[64];			// ����
	int				nMonitorNum;			// TV����
	int				nEncoderNum;			// ����ͨ������
	BYTE			szSplitMode[16];		// ֧�ֵ�TV����ָ�������������ʽ��ʾ��0Ϊ��β
	BYTE            bMonitorEnable[16];		// ��TVʹ��
	char			reserved[64];
} DEV_DECODER_INFO, *LPDEV_DECODER_INFO;

// ���ӵı�������Ϣ
typedef struct __DEV_ENCODER_INFO 
{
	char			szDevIp[DH_MAX_IPADDR_LEN];			// ǰ��DVR��IP��ַ
	WORD			wDevPort;							// ǰ��DVR�Ķ˿ں�
	BYTE			bDevChnEnable;                      // ����ͨ��ʹ��
	BYTE			byEncoderID;						// ��Ӧ����ͨ����
	char			szDevUser[DH_USER_NAME_LENGTH_EX];	// �û���
	char			szDevPwd[DH_USER_PSW_LENGTH_EX];	// ����
	int				nDevChannel;						// ͨ����
	int				nStreamType;						// �������ͣ�0����������1��������; 2:snap
	BYTE			byConnType;							// 0��TCP��1��UDP��2���鲥
	BYTE			byWorkMode;							// 0��ֱ����1��ת��
	WORD			wListenPort;						// ָʾ��������Ķ˿ڣ�ת��ʱ��Ч
	DWORD			dwProtoType;						// Э������,
														// 0:������ǰ
														// 1:�󻪶���Э��
														// 2:��ϵͳ����Э��
														// 3:��DSSЭ��
														// 4:��rtspЭ��
	char			szDevName[64];						// ǰ���豸����
	BYTE            byVideoInType;                      // �ý���ͨ����ǰ��ƵԴ����:0-����(SD),1-����(HD),ע:�豸֧�ָ���ͨ������Ч		
	char			szDevIpEx[DH_MAX_IPADDR_OR_DOMAIN_LEN];// szDevIp��չ��ǰ��DVR��IP��ַ(������������)
	BYTE            bySnapMode;                         //ץͼģʽ(nStreamType==2ʱ��Ч) 0����ʾ����һ֡,1����ʾ��ʱ��������
	BYTE            byManuFactory;						//Ŀ���豸����������,0:Private(��˽��)
	BYTE            byDeviceType;                       //Ŀ���豸���豸����,0:IPC
	char			reserved[48];
} DEV_ENCODER_INFO, *LPDEV_ENCODER_INFO;

// TV������Ϣ
typedef struct __DEV_DECODER_TV 
{
	int				nID;								// TV��
	BOOL			bEnable;							// ʹ�ܣ�������ر�
	int				nSplitType;							// ����ָ���
	DEV_ENCODER_INFO stuDevInfo[16];					// �������������Ϣ
	char			reserved[16];
} DEV_DECODER_TV, *LPDEV_DECODER_TV;

// ���������������Ϣ
typedef struct __DEC_COMBIN_INFO
{
	int				nCombinID;							// ���ID
	int             nSplitType;							// ����ָ��� 
	BYTE            bDisChn[16];						// ��ʾͨ��
	char			reserved[16];
} DEC_COMBIN_INFO, *LPDEC_COMBIN_INFO;

// ��������Ѳ��Ϣ
#define DEC_COMBIN_NUM 			32						// ��Ѳ��ϸ���
typedef struct __DEC_TOUR_COMBIN 
{
	int				nTourTime;							// ��Ѳ���(��)
	int				nCombinNum;							// ��ϸ���
	BYTE			bCombinID[DEC_COMBIN_NUM];			// ��ϱ�
	char			reserved1[32];
	BYTE			bCombinState[DEC_COMBIN_NUM];		// ������ʹ��״̬��0���أ�1����
	char			reserved2[32];
} DEC_TOUR_COMBIN, *LPDEC_TOUR_COMBIN;

// �������ط�����
typedef enum __DEC_PLAYBACK_MODE
{
	Dec_By_Device_File = 0,								// ǰ���豸�����ļ���ʽ
	Dec_By_Device_Time,									// ǰ���豸����ʱ�䷽ʽ
} DEC_PLAYBACK_MODE;

// �������طſ�������
typedef enum __DEC_CTRL_PLAYBACK_TYPE
{
	Dec_Playback_Seek = 0,								// �϶�
	Dec_Playback_Play,									// ����
	Dec_Playback_Pause,									// ��ͣ
	Dec_Playback_Stop,									// ֹͣ
} DEC_CTRL_PLAYBACK_TYPE;

//��Ѳ��������
typedef enum __DEC_CTRL_TOUR_TYPE
{
	Dec_Tour_Stop = 0,									// ֹͣ
	Dec_Tour_Start,										// ��ʼ
	Dec_Tour_Pause,										// ��ͣ
	Dec_Tour_Resume,									// �ָ�
} DEC_CTRL_TOUR_TYPE;

// ���ļ��ط�ǰ���豸����
typedef struct __DEC_PLAYBACK_FILE_PARAM 
{
	char			szDevIp[DH_MAX_IPADDR_LEN];			// ǰ��DVR��IP��ַ
	WORD			wDevPort;							// ǰ��DVR�Ķ˿ں�
	BYTE			bDevChnEnable;                      // ����ͨ��ʹ��
	BYTE			byEncoderID;						// ��Ӧ����ͨ����
	char			szDevUser[DH_USER_NAME_LENGTH_EX];	// �û���
	char			szDevPwd[DH_USER_PSW_LENGTH_EX];	// ����
	NET_RECORDFILE_INFO stuRecordInfo;					// ¼���ļ���Ϣ
	char			reserved[12];
} DEC_PLAYBACK_FILE_PARAM, *LPDEC_PLAYBACK_FILE_PARAM;

// ��ʱ��ط�ǰ���豸����
typedef struct __DEC_PLAYBACK_TIME_PARAM 
{
	char			szDevIp[DH_MAX_IPADDR_LEN];			// ǰ��DVR��IP��ַ
	WORD			wDevPort;							// ǰ��DVR�Ķ˿ں�
	BYTE			bDevChnEnable;                      // ����ͨ��ʹ��
	BYTE			byEncoderID;						// ��Ӧ����ͨ����
	char			szDevUser[DH_USER_NAME_LENGTH_EX];	// �û���
	char			szDevPwd[DH_USER_PSW_LENGTH_EX];	// ����
	int				nChannelID;
	NET_TIME		startTime;
	NET_TIME		endTime;
	char			reserved[12];
} DEC_PLAYBACK_TIME_PARAM, *LPDEC_PLAYBACK_TIME_PARAM;

// ��ǰ����ͨ��״̬��Ϣ(����ͨ��״̬����������Ϣ��)
typedef struct __DEV_DECCHANNEL_STATE
{
	BYTE			byEncoderID;						// ��Ӧ����ͨ����
	BYTE            byChnState;                         // ��ǰ����ͨ�����ڲ���״̬:0�����У�1��ʵʱ���ӣ�2���ط� 3 - ��Ѳ
	BYTE			byFrame;                            // ��ǰ����֡��
	BYTE            byReserved;                         // ����
	int				nChannelFLux;						// ����ͨ����������
	int             nDecodeFlux;						// ����������
	char            szResolution[16];                   // ��ǰ���ݷֱ���
	char			reserved[256];
} DEV_DECCHANNEL_STATE, *LPDEV_DECCHANNEL_STATE;

// �豸TV���������Ϣ
typedef struct __DEV_VIDEOOUT_INFO
{
	DWORD				dwVideoStandardMask;			// NSP,��Ƶ��ʽ���룬��λ��ʾ�豸�ܹ�֧�ֵ���Ƶ��ʽ(�ݲ�֧��)
	int					nVideoStandard;                 // NSP,��ǰ����ʽ(�ݲ�֧�֣���ʹ��DHDEV_SYSTEM_ATTR_CFG��byVideoStandard����ʽ��ȡ������)
	DWORD				dwImageSizeMask;				// �ֱ������룬��λ��ʾ�豸�ܹ�֧�ֵķֱ���
	int                 nImageSize;                     // ��ǰ�ķֱ���
	char				reserved[256];
}DEV_VIDEOOUT_INFO, *LPDEV_VIDEOOUT_INFO;

// ����TV���
typedef struct __DEV_TVADJUST_CFG
{
	int					nTop;							// �ϲ�߾ࣨ0��100��
	int					nBotton;						// �²�߾ࣨ0��100��
	int					nLeft;							// ���߾ࣨ0��100��
	int                 nRight;							// �Ҳ�߾ࣨ0��100��
	int					reserved[32];
}DHDEV_TVADJUST_CFG, *LPDHDEV_TVADJUST_CFG;

// ������Ѳ����
typedef struct __DEV_DECODER_TOUR_SINGLE_CFG
{
	char		szIP[128];								// ǰ���豸ip.����"10.7.5.21". ������������֧��.��Ҫ��'\0'����.
	int			nPort;									// ǰ���豸�˿�.(0, 65535).
	int			nPlayChn;								// ����ǰ���豸ͨ��[1, 16].
	int			nPlayType;								// ����ǰ����������, 0:������; 1:������.
	char		szUserName[32];							// ��½ǰ���豸���û���,��Ҫ��'\0'����.
	char		szPassword[32];							// ��½ǰ���豸������,��Ҫ��'\0'����.
	int			nInterval;								// ��Ѳ���ʱ��[10, 120],��λ��.
	BYTE		bReserved[64];							// �����ֽ�,������չ.
}DHDEV_DECODER_TOUR_SINGLE_CFG;

typedef struct __DEV_DECODER_TOUR_CFG
{
	int								nCfgNum;			// ���ýṹ����Ŀ. �������֧��32��.����֧����Ŀ����ͨ��������ѯ.
	DHDEV_DECODER_TOUR_SINGLE_CFG	tourCfg[64];		// ��ѯ��������,��Ч�ṹ������ɳ�Ա"nCfgNum"ָ��. ����32��������չ.
	BYTE							bReserved[256];		// �����ֽ�,������չ.
}DHDEV_DECODER_TOUR_CFG;
/////////////////////////////////����汾/////////////////////////////////

// �����豸ץͼ�����ӿ�����Ϣ
typedef struct __NET_SNAP_COMMANDINFO 
{
	char				szCardInfo[16];			// ������Ϣ
	char				reserved[64];			// ����
} NET_SNAP_COMMANDINFO, LPNET_SNAP_COMMANDINFO;

typedef struct
{
	int					nChannelNum;			// ͨ����
	char				szUseType[32];			// ͨ����;
	DWORD				dwStreamSize;			// ������С(��λ��kb/s)
	char				reserved[32];			// ����
} DHDEV_USE_CHANNEL_STATE;

typedef struct 
{
	char				szUserName[32];			// �û���
	char				szUserGroup[32];		// �û���
	NET_TIME			time;					// ����ʱ��
	int					nOpenedChannelNum;		// ������ͨ������
	DHDEV_USE_CHANNEL_STATE	channelInfo[DH_MAX_CHANNUM];
	char				reserved[64];
} DHDEV_USER_NET_INFO;

// ��������״̬��Ϣ
typedef	struct 
{
	int					nUserCount;				// �û�����
	DHDEV_USER_NET_INFO	stuUserInfo[32];
	char				reserved[256];
}DHDEV_TOTAL_NET_STATE;

// ͼ��ˮӡ����
typedef struct __DHDEV_WATERMAKE_CFG 
{
	DWORD				dwSize;
	int					nEnable;				// ʹ��
	int					nStream;				// ����(1��n)0-��������
	int					nKey;					// ��������(1-���֣�2-ͼƬ)
	char				szLetterData[DH_MAX_WATERMAKE_LETTER];	//	����
	char				szData[DH_MAX_WATERMAKE_DATA]; // ͼƬ����
	BYTE				bReserved[512];			// ����
} DHDEV_WATERMAKE_CFG;

// �洢λ�����ýṹ�壬ÿͨ����������,ÿͨ������ѡ����ִ洢����, Ŀǰ��������, ���ƶ�, Զ�̴洢.
typedef struct 
{
	DWORD				dwSize;
	DWORD				dwLocalMask;			// ���ش洢���룻��λ��ʾ��
												// ��һλ��ϵͳԤ¼���ڶ�λ����ʱ¼�񣬵���λ������¼��
												// ����λ������¼�񣬵���λ������¼�񣬵���λ���ֶ�¼��
	DWORD				dwMobileMask;			// ���ƶ��洢���� �洢�����籾�ش洢����
	int					RemoteType;				// Զ�̴洢���� 0: Ftp  1: Smb 
	DWORD				dwRemoteMask;			// Զ�̴洢���� �洢�����籾�ش洢����
	DWORD				dwRemoteSecondSelLocal;	// Զ���쳣ʱ���ش洢����
	DWORD				dwRemoteSecondSelMobile;// Զ���쳣ʱ���ƶ��洢����
	char				SubRemotePath[MAX_PATH_STOR]; // Զ��Ŀ¼, ���г���Ϊ240
	DWORD				dwFunctionMask;			//��������λ����λ��ʾ��bit0 = 1:����ץͼ�¼������洢λ�ù���
	
	char				reserved[124];
} DH_STORAGE_STATION_CFG;

#define MAX_ALARM_DECODER_NUM 16
typedef struct  
{
	DWORD				dwAlarmDecoder;			// �������֧��8����������ڣ���8λ���Ժ���չ
	BYTE				bDecoderIndex;			// ��ʾ�ڼ�������������
	BYTE				bReserved[3];
} ALARM_DECODER;

// ��������������
typedef struct
{
	int					nAlarmDecoderNum;
	ALARM_DECODER		stuAlarmDecoder[MAX_ALARM_DECODER_NUM];
	BYTE				bReserved[32];
} ALARM_DECODER_ALARM;

//DSP����
typedef struct  
{
	BOOL				bError;			//0,DSP���� 1,DSP�쳣
	DWORD				dwErrorMask;	//��λ��ʾ����0��ʾ�д˴���0��ʾû�С�(Ŀǰÿ�α���ֻ��һλ��Ч)
										//bit		DSP����
										//1			DSP����ʧ�� 
										//2			DSP����
										//3			��ʽ���� 
										//4			�ֱ��ʲ�֧��
										//5			���ݸ�ʽ��֧��
										//6			�Ҳ���I֡
	DWORD               dwDecChnnelMask;//��λ��ʾ�������Ľ���ͨ���ţ�dwErrorMaskΪDSP������ʽ���ԣ��ֱ��ʲ�֧�֣����ݸ�ʽ��֧��ʱ������Ч
	
	BYTE				bReserved[28];
}DSP_ALARM;

// ���˱�����������
typedef struct  
{
	int		nFDDINum;
	BYTE	bAlarm[256];
} ALARM_FDDI_ALARM;

// ����Ƶ��ⱨ������
typedef struct
{
	BOOL				bEnable;				// ��������ʹ��
	int					Volume_min;				// ��������Сֵ
	int					Volume_max;				// ���������ֵ
	char				reserved[128];	
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];	
	DH_MSG_HANDLE		struHandle;				// ����ʽ
} DH_AUDIO_DETECT_INFO;

typedef struct  
{
	DWORD				dwSize;
	int					AlarmNum;
	DH_AUDIO_DETECT_INFO AudioDetectAlarm[DH_MAX_AUDIO_IN_NUM];
	char				reserved[256];
} DH_AUDIO_DETECT_CFG;

typedef struct
{
	BOOL				bTourEnable;			// ��Ѳʹ��
	int					nTourPeriod;			// ��Ѳ�������λ��, 5-300 
	DWORD				dwChannelMask;			// ��Ѳ��ͨ����������ʽ��ʾ
	char				reserved[64];
}DH_VIDEOGROUP_CFG;

// ����������Ʋ�������
typedef struct
{
	DWORD				dwSize;
	int					nMatrixNum;				// �������(ע���������޸�)
	DH_VIDEOGROUP_CFG	struVideoGroup[DH_MATRIX_MAXOUT];
	char				reserved[32];
} DHDEV_VIDEO_MATRIX_CFG;   

// WEB·������
typedef struct 
{
	DWORD				dwSize;
	BOOL				bSnapEnable;			// �Ƿ�ץͼ
	int					iSnapInterval;			// ץͼ����
	char				szHostIp[DH_MAX_IPADDR_LEN]; // HTTP����IP
	WORD				wHostPort;
	int					iMsgInterval;			// ״̬��Ϣ���ͼ��
	char				szUrlState[DH_MAX_URL_LEN];	// ״̬��Ϣ�ϴ�URL
	char				szUrlImage[DH_MAX_URL_LEN];	// ͼƬ�ϴ�Url
	char				szDevId[DH_MAX_DEV_ID_LEN];	// ������web���
	BYTE				byReserved[2];
} DHDEV_URL_CFG;

// OEM��ѯ
typedef struct  
{
	char				szVendor[DH_MAX_STRING_LEN];
	char				szType[DH_MAX_STRING_LEN];
	char				reserved[128];
} DHDEV_OEM_INFO;


//��ƵOSD��������
typedef struct 
{ 
	DWORD	rgbaFrontground;		// �����ǰ�������ֽڱ�ʾ���ֱ�Ϊ�졢�̡�����͸����
	DWORD	rgbaBackground;			// ����ı��������ֽڱ�ʾ���ֱ�Ϊ�졢�̡�����͸����	
	RECT	rcRelativePos;			// λ��,����߾��������ı���*8191	
	BOOL	bPreviewBlend;			// Ԥ������ʹ��	
	BOOL	bEncodeBlend;			// �������ʹ��
	BYTE    bReserved[4];           // ����
} DH_DVR_VIDEO_WIDGET;

typedef struct 
{
	DH_DVR_VIDEO_WIDGET	StOSD_POS; 								// OSD���ӵ�λ�úͱ���ɫ
	char 				SzOSD_Name[DH_VIDEO_OSD_NAME_NUM]; 		// OSD���ӵ�����
}DH_DVR_VIDEOITEM;
 
// ÿ��ͨ����OSD��Ϣ
typedef struct 
{
	DWORD 				dwSize;
	DH_DVR_VIDEOITEM 	StOSDTitleOpt [DH_VIDEO_CUSTOM_OSD_NUM]; 	// ÿ��ͨ����OSD��Ϣ
	BYTE    			bReserved[16];                  		  // ����
} DH_DVR_VIDEOOSD_CFG;

// ����CDMA/GPRS������Ϣ
// ʱ��νṹ															    
typedef struct 
{
	BYTE				bEnable;				  // ʱ���ʹ�ܣ�1��ʾ��ʱ�����Ч��0��ʾ��ʱ�����Ч��
	BYTE				bBeginHour;
	BYTE				bBeginMin;
	BYTE				bBeginSec;
	BYTE				bEndHour;
	BYTE				bEndMin;
	BYTE				bEndSec;
	BYTE    			bReserved;                //����
} DH_3G_TIMESECT, *LPDH_3G_TIMESECT;

typedef struct 
{
	DWORD 				dwSize;
	BOOL				bEnable;					// ����ģ��ʹ�ܱ�־
	DWORD               dwTypeMask;                 // �豸֧��������������;��λ��ʾ,��һλ;�Զ�ѡ��;
													// �ڶ�λ��TD-SCDMA���磻����λ��WCDMA����;
													// ����λ��CDMA 1.x����;����λ��CDMA2000����;����λ��GPRS����;
													// ����λ��EVDO����;�ڰ�λ��WIFI
	DWORD               dwNetType;                  // ��ǰ�������������ͣ�ΪEM_GPRSCDMA_NETWORK_TYPEֵ
	char				szAPN[128];					// ���������
	char				szDialNum[128];				// ���ź���
	char				szUserName[128];			// �����û���
	char				szPWD[128];					// ��������
	
	/* �������豸��ֻ������ */
	BOOL				iAccessStat;				// ��������ע��״̬
    char				szDevIP[16];				// ǰ���豸����IP���ַ���, ����'\0'��������16byte  
	char				szSubNetMask [16];			// ǰ���豸�����������룬�ַ���, ����'\0'��������16byte 
	char				szGateWay[16];				// ǰ���豸�������أ��ַ���, ����'\0'��������16byte 
	/* �������豸��ֻ������ */
	int                 iKeepLive;					// ����ʱ��
	DH_3G_TIMESECT		stSect[DH_N_WEEKS][DH_N_TSECT];// 3G����ʱ��Σ���Чʱ���֮�ڣ��������ţ���Чʱ���֮�⣬�رղ��š�
	BYTE                byActivate;                  //�Ƿ���Ҫ����������ż���
	char				Reserved[171];				// �����ֽڣ�������չ
} DHDEV_CDMAGPRS_CFG;

// ¼������������
typedef struct  
{
	DWORD 				dwSize;
	int					nType;						// 0:��ʱ�䣬1������С
	int					nValue;						// nType = 0ʱ:��λ���ӣ�nType = 1ʱ:��λKB
	char				Reserved[128];				// �����ֽڣ�������չ
} DHDEV_RECORD_PACKET_CFG;

// (����)����ע���������Ϣ
typedef struct __DEV_AUTOREGISTER_INFO 
{
	LONG			lConnectionID;						// ����ID
	char			szServerIp[DH_MAX_IPADDR_LEN];		// ����ע���������IP
	int				nPort;								// ����ע��������˿�0- 65535
	int             nState;                             // ��������״̬��0��ע��ʧ�ܣ�1-ע��ɹ�; 2-����ʧ��
	char			reserved[16];
} DEV_AUTOREGISTER_INFO;

typedef struct __DEV_SERVER_AUTOREGISTER 
{
	DWORD					dwSize;
	int						nRegisterSeverCount;											// ����ע�����������
	DEV_AUTOREGISTER_INFO	stuDevRegisterSeverInfo[DH_CONTROL_AUTO_REGISTER_NUM];			// ������ע���������Ϣ
	char					reserved[256];
} DEV_SERVER_AUTOREGISTER, *LPDEV_SERVER_AUTOREGISTER;

// ������¼�����ϴ�
typedef struct
{
	DWORD				dwSize;		
	char				szSourFile[MAX_PATH_STOR];	// Դ�ļ�·��
	int					nFileSize;					// Դ�ļ���С�����С�ڵ���0��sdk���м����ļ���С.
	char				szBurnFile[MAX_PATH_STOR];	// ��¼����ļ���
	BYTE				bReserved[64];
} DHDEV_BURNFILE_TRANS_CFG;

// ֣������ķEC_U��Ƶ���ݵ���ʹ������
typedef struct 
{
	BOOL				bEnable;					// ʹ��
	int					nPort;						// �˿�
	BYTE				bReserved[64];
} DHDEV_OSD_ENABLE_CFG;

// �����������:��������ʹ������
typedef struct	
{
	BOOL				bAutoBootEnable;				//�Զ�����ʹ��
	DWORD				dwAutoBootTime;					//ÿ���Զ�����ʱ���, ��λ��,��0����.[0,82800]
	BOOL				bAutoShutDownEnable;			//�Զ��ػ�ʹ��
	DWORD				dwAutoShutDownTime;				//ÿ���Զ��ػ�ʱ���, ��λ��,��0����.[0,82800]
	DWORD				dwShutDownDelayTime;			//��ʱ�ػ�ʱ��,��λ��.
	BYTE				bEventNoDisk;					//1,(Ignore:����); 2,(Reboot:����ϵͳ)
	BYTE				bReserved[511];					//reserved byte.
} DHDEV_ABOUT_VEHICLE_CFG;

// atm:��ѯ�������������Ϣ
typedef struct  
{
	DWORD				dwDataSource;					//1:Net(����), 2:Com(����), 3:Net&Com(����ʹ���)
	char				szProtocol[32][33];				//Э������,��󳤶�32(����һ��������).
	BYTE				bReserved[256];
} DHDEV_ATM_OVERLAY_GENERAL;
 
// atm:���õ�������
typedef struct 
{
	DWORD				dwDataSource;					//1:Net(����), 2:Com(����)
	char				szProtocol[33];					//Э�����֣���DHDEV_ATM_OVERLAY_GENERAL����
	BYTE				bReserved_1[3];
	DWORD				dwChannelMask;					//���ӵ�ͨ���ţ������ʾ��
	BYTE				bMode;							//1:Preview(Ԥ������), 2:Encode(�������), 3:Preview&Encode(Ԥ���ͱ������)
	BYTE				bLocation;						//1:LeftTop(����), 2:LeftBottom(����), 3:RightTop(����), 4:RightBottom(����)
	BYTE				bReserved_2[258];
} DHDEV_ATM_OVERLAY_CONFIG;

#define DH_MAX_BACKUPDEV_NUM			16
#define DH_MAX_BACKUP_NAME				32

// �����豸�б�
typedef struct 
{
	int					nBackupDevNum;								// ʵ�ʵı����豸����
	char				szBackupDevNames[DH_MAX_BACKUPDEV_NUM][DH_MAX_BACKUP_NAME]; // �����豸������
} DHDEV_BACKUP_LIST, *LPDHDEV_BACKUP_LIST;

// �����豸��������
typedef enum __BACKUP_TYPE
{
	BT_DHFS = 0,							// ���ļ�ϵͳ
	BT_DISK,								// �ƶ�Ӳ��
	BT_CDRW									// ���̿�¼
} DHDEV_BACKUP_TYPE;	

// �����豸�ӿ�����
typedef enum __BACKUP_BUS
{
	BB_USB = 0,								// usb�ӿ�
	BB_1394,								// 1394�ӿ�
	BB_IDE,									// ide�ӿ�
	BB_ESATA,								// esata�ӿ�
} DHDEV_BACKUP_BUS;	

typedef struct 
{
	char				szName[32];						// �豸�� 
	BYTE				byType;							// �����豸�Ľ������� ö��ֵ��BACKUP_TYPE
	BYTE				byBus;							// �����豸�Ľӿ����� ö��ֵ��BACKUP_BUS
	UINT				nCapability;					// ������, ��kBytesΪ��λ
	UINT				nRemain;						// ʣ������,��kBytesΪ��λ
	char				szDirectory[128];				// Զ�̱��ݵ�Ŀ¼
} DHDEV_BACKUP_INFO, *LPDHDEV_BACKUP_INFO;

typedef struct 
{
	char				szName[32];						// �豸�� 
	UINT				nCapability;					// ������, ��kBytesΪ��λ
	UINT				nRemain;						// ��ǰ����ʣ������,��kBytesΪ��λ
} DHDEV_BACKUP_FEEDBACK, *LPDHDEV_BACKUP_FEEDBACK;

#define  DH_MAX_BACKUPRECORD_NUM 1024

typedef struct  
{
	char				szDeviceName[DH_MAX_BACKUP_NAME];			//�����豸����
	int					nRecordNum;									//���ݼ�¼����
	NET_RECORDFILE_INFO	stuRecordInfo[DH_MAX_BACKUPRECORD_NUM];		//���ݼ�¼��Ϣ
} BACKUP_RECORD, *LPBACKUP_RECORD;

/////////////////////////////////ƽ̨����/////////////////////////////////

// ƽ̨�������� �� U��ͨ
typedef struct
{
    BOOL				bChnEn;
    char				szChnId[DH_INTERVIDEO_UCOM_CHANID];
} DH_INTERVIDEO_UCOM_CHN_CFG;

typedef struct
{
	DWORD				dwSize;
	BOOL				bFuncEnable;			// ���빦��ʹ��
	BOOL				bAliveEnable;			// ����ʹ��
	DWORD				dwAlivePeriod;			// �������ڣ���λ�룬0-3600
	char				szServerIp[DH_MAX_IPADDR_LEN]; // CMS��IP
	WORD				wServerPort;			// CMS��Port
    char				szRegPwd[DH_INTERVIDEO_UCOM_REGPSW]; // ע������
	char				szDeviceId[DH_INTERVIDEO_UCOM_DEVID];// �豸id
	char				szUserName[DH_INTERVIDEO_UCOM_USERNAME];
	char				szPassWord[DH_INTERVIDEO_UCOM_USERPSW];
    DH_INTERVIDEO_UCOM_CHN_CFG  struChnInfo[DH_MAX_CHANNUM]; // ͨ��id,en
} DHDEV_INTERVIDEO_UCOM_CFG;

// ƽ̨�������� �� ��������
typedef struct
{
	DWORD				dwSize;
    unsigned short		usCompanyID[2];			// ��˾ID����ֵ����ͬ�ĵ���������˾������4�ֽڶ��룬Ŀǰֻ�������һ��
    char				szDeviceNO[32];			// ǰ���豸���кţ��ַ���������'\0'��������32byte
    char				szVSName[32];			// ǰ���豸���ƣ��ַ���������'\0'��������16byte
    char				szVapPath[32];			// VAP·��
    unsigned short		usTcpPort;				// TCP�˿ڣ���ֵ����ֵ 1~65535 
    unsigned short		usUdpPort;				// UDP�˿ڣ���ֵ����ֵ 1~65535
    bool				bCsEnable[4];			// ���ķ�����ʹ�ܱ�־����ֵ��trueʹ�ܣ�false��ʹ�ܣ�����4�ֽڶ��룬Ŀǰֻ�������һ��
    char				szCsIP[16];				// ���ķ�����IP��ַ���ַ���������'\0'��������16byte
    unsigned short		usCsPort[2];			// ���ķ������˿ڣ���ֵ����ֵ 1~65535������4�ֽڶ��룬Ŀǰֻ�������һ��
    bool				bHsEnable[4];			// ����������ʹ�ܱ�־����ֵ��trueʹ�ܣ�false��ʹ�ܣ�����4�ֽڶ��룬Ŀǰֻ�������һ��
    char				szHsIP[16];				// ����������IP��ַ���ַ���������'\0'��������16byte
    unsigned short		usHsPort[2];			// �����������˿ڣ���ֵ����ֵ 1~65535������4�ֽڶ��룬Ŀǰֻ�������һ��
    int					iHsIntervalTime;		// ����������������ڣ���ֵ(��λ:��)
    bool				bRsEnable[4];			// ע�������ʹ�ܱ�־����ֵ��trueʹ�ܣ�false��ʹ�ܣ�����4�ֽڶ��룬Ŀǰֻ�������һ��
    char				szRsIP[16];				// ע�������IP��ַ���ַ���������'\0'��������16byte
    unsigned short		usRsPort[2];			// ע��������˿ڣ���ֵ����ֵ 1~65535������4�ֽڶ��룬Ŀǰֻ�������һ��
    int					iRsAgedTime;			// ע���������Чʱ�䣬��ֵ(��λ:Сʱ)
    char				szAuthorizeServerIp[16];// ��Ȩ������IP
    unsigned short		usAuthorizePort[2];		// ��Ȩ�������˿ڣ�����4�ֽڶ��룬Ŀǰֻ�������һ��
    char				szAuthorizeUsername[32];// ��Ȩ�������ʺ�
    char				szAuthorizePassword[36];// ��Ȩ����������
    
    char				szIpACS[16];			// ACS(�Զ�ע�������) IP
    unsigned short		usPortACS[2];			// ACS Port������4�ֽڶ��룬Ŀǰֻ�������һ��
    char				szUsernameACS[32];		// ACS�û���
    char				szPasswordACS[36];		// ACS����
    bool				bVideoMonitorEnabled[4];// DVS�Ƿ����ϱ�ǰ����Ƶ�źż����Ϣ����ֵ��trueʹ�ܣ�false��ʹ��
    int					iVideoMonitorInterval;	// �ϱ�����(����)
    
    char				szCoordinateGPS[64];	// GPS����
    char				szPosition[32];			// �豸λ��
    char				szConnPass[36];			// �豸������
} DHDEV_INTERVIDEO_BELL_CFG;

// ƽ̨�������� �� ������ά
typedef struct  
{
	DWORD				dwSize;
	unsigned short		nSevPort;				// �������˿ڣ���ֵ����ֵ1~65535
    char				szSevIp[DH_INTERVIDEO_NSS_IP]; // ������IP��ַ���ַ���������'\0'��������32byte
    char				szDevSerial[DH_INTERVIDEO_NSS_SERIAL]; // ǰ���豸���кţ��ַ���������'\0'��������32byte
    char				szUserName[DH_INTERVIDEO_NSS_USER];
    char				szPwd[DH_INTERVIDEO_NSS_PWD];
} DHDEV_INTERVIDEO_NSS_CFG;


////////////////////////////////HDVRר��//////////////////////////////////
// ����������չ�ṹ��
typedef struct 
{
	/* ��Ϣ����ʽ������ͬʱ���ִ���ʽ������
	 * 0x00000001 - �����ϴ�
	 * 0x00000002 - ����¼��
	 * 0x00000004 - ��̨����
	 * 0x00000008 - �����ʼ�
	 * 0x00000010 - ������Ѳ
	 * 0x00000020 - ������ʾ
	 * 0x00000040 - �������
	 * 0x00000080 - Ftp�ϴ�
	 * 0x00000100 - ����
	 * 0x00000200 - ������ʾ
	 * 0x00000400 - ץͼ
	*/

	/* ��ǰ������֧�ֵĴ���ʽ����λ�����ʾ */
	DWORD				dwActionMask;

	/* ������������λ�����ʾ�����嶯������Ҫ�Ĳ����ڸ��Ե����������� */
	DWORD				dwActionFlag;
	
	/* �������������ͨ�������������������Ϊ1��ʾ��������� */ 
	BYTE				byRelAlarmOut[DH_MAX_ALARMOUT_NUM_EX];
	DWORD				dwDuration;				/* ��������ʱ�� */

	/* ����¼�� */
	BYTE				byRecordChannel[DH_MAX_VIDEO_IN_NUM_EX]; /* ����������¼��ͨ����Ϊ1��ʾ������ͨ�� */
	DWORD				dwRecLatch;				/* ¼�����ʱ�� */

	/* ץͼͨ�� */
	BYTE				bySnap[DH_MAX_VIDEO_IN_NUM_EX];
	/* ��Ѳͨ�� */
	BYTE				byTour[DH_MAX_VIDEO_IN_NUM_EX];

	/* ��̨���� */
	DH_PTZ_LINK			struPtzLink[DH_MAX_VIDEO_IN_NUM_EX];
	DWORD				dwEventLatch;			/* ������ʼ��ʱʱ�䣬sΪ��λ����Χ��0~15��Ĭ��ֵ��0 */
	/* �����������������ͨ�������������������Ϊ1��ʾ��������� */ 
	BYTE				byRelWIAlarmOut[DH_MAX_ALARMOUT_NUM_EX];
	BYTE				bMessageToNet;
	BYTE                bMMSEn;                /*���ű���ʹ��*/
	BYTE                bySnapshotTimes;       /*���ŷ���ץͼ���� */
	BYTE				bMatrixEn;				/*!< ����ʹ�� */
	DWORD				dwMatrix;				/*!< �������� */			
	BYTE				bLog;					/*!< ��־ʹ�ܣ�Ŀǰֻ����WTN��̬�����ʹ�� */
	BYTE				byReserved[511];   
} DH_MSG_HANDLE_EX;

// �ⲿ������չ
typedef struct
{
	BYTE				byAlarmType;			// ���������ͣ�0�����գ�1������
	BYTE				byAlarmEn;				// ����ʹ��
	BYTE				byReserved[2];						
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT]; //NSP
	DH_MSG_HANDLE_EX	struHandle;				// ����ʽ
} DH_ALARMIN_CFG_EX, *LPDHDEV_ALARMIN_CFG_EX; 

// ��̬��ⱨ��
typedef struct 
{
	BYTE				byMotionEn;				// ��̬��ⱨ��ʹ��
	BYTE				byReserved;
	WORD				wSenseLevel;			// ������
	WORD				wMotionRow;				// ��̬������������
	WORD				wMotionCol;				// ��̬������������
	BYTE				byDetected[DH_MOTION_ROW][DH_MOTION_COL]; // ����������32*32������
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];	//NSP
	DH_MSG_HANDLE_EX		struHandle;				//����ʽ
} DH_MOTION_DETECT_CFG_EX;

// ��Ƶ��ʧ����
typedef struct
{
	BYTE				byAlarmEn;				// ��Ƶ��ʧ����ʹ��
	BYTE				byReserved[3];
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];	//NSP
	DH_MSG_HANDLE_EX	struHandle;				// ����ʽ
} DH_VIDEO_LOST_CFG_EX;

// ͼ���ڵ�����
typedef struct
{
	BYTE				byBlindEnable;			// ʹ��
	BYTE				byBlindLevel;			// ������1-6
	BYTE				byReserved[2];
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];	//NSP
	DH_MSG_HANDLE_EX	struHandle;				// ����ʽ
} DH_BLIND_CFG_EX;

// Ӳ����Ϣ(�ڲ�����)
typedef struct 
{
	BYTE				byNoDiskEn;				// ��Ӳ��ʱ����
	BYTE				byReserved_1[3];
	DH_TSECT			stNDSect[DH_N_WEEKS][DH_N_REC_TSECT]; //NSP
	DH_MSG_HANDLE_EX	struNDHandle;			// ����ʽ
	
	BYTE				byLowCapEn;				// Ӳ�̵�����ʱ����
	BYTE				byLowerLimit;			// ������ֵ��0-99
	BYTE				byReserved_2[2];
	DH_TSECT			stLCSect[DH_N_WEEKS][DH_N_REC_TSECT]; //NSP
	DH_MSG_HANDLE_EX	struLCHandle;			// ����ʽ
	
	BYTE				byDiskErrEn;			// Ӳ�̹��ϱ���
	BYTE				bDiskNum;
	BYTE				byReserved_3[2];
	DH_TSECT			stEDSect[DH_N_WEEKS][DH_N_REC_TSECT]; //NSP
	DH_MSG_HANDLE_EX	struEDHandle;			// ����ʽ
} DH_DISK_ALARM_CFG_EX;

typedef struct
{
	BYTE				byEnable;
	BYTE				byReserved[3];
	DH_MSG_HANDLE_EX	struHandle;
} DH_NETBROKEN_ALARM_CFG_EX;

// ǰ�˽������ò�����Ϣ
typedef struct __DEV_ENCODER_CFG 
{
	int					nChannels;				// ����ͨ������
	DEV_ENCODER_INFO	stuDevInfo[32];			// ������ͨ����ǰ�˱�������Ϣ
	BYTE				byHDAbility;			// ����ͨ��������֧�ָ�����Ƶ����(Ϊ0��ʾ��֧�ָ�������)
												// ע�����֧�ָ������ã�����ͨ��Ϊ0~N-1������N֮�������ͨ������Ϊ���壬�п��ܱ��治�ɹ�
	BYTE				bTVAdjust;				// �豸�Ƿ�֧��TV����, 0:��֧�� 1:֧��.
	BYTE				bDecodeTour;			// �豸�Ƿ�֧�ֽ�����Ѳ, 0:��֧�� ����0:��ʾǰ��֧����Ѳ����豸��.
	BYTE				bRemotePTZCtl;			// ָʾ�Ƿ�֧��Զ����̨���ơ�
	char				reserved[12];
} DEV_ENCODER_CFG, *LPDEV_ENCODER_CFG;

/////////////////////////////////��̭����/////////////////////////////////

// ��ѯ�豸����״̬ͨ����Ϣ����ؽӿ�����̭���벻Ҫʹ��
typedef struct
{
	BYTE				byRecordStatic;			// ͨ���Ƿ���¼��0����¼��1���ֶ�¼��2���Զ�¼��
	BYTE				bySignalStatic;			// ���ӵ��ź�״̬��0��������1���źŶ�ʧ
	BYTE				byHardwareStatic;		// ͨ��Ӳ��״̬��0��������1���쳣������DSP����
	char				reserve;
	DWORD				dwBitRate;				// ʵ������
	DWORD				dwLinkNum;				// �ͻ������ӵĸ���
	DWORD				dwClientIP[DH_MAX_LINK];// �ͻ��˵�IP��ַ
} NET_DEV_CHANNELSTATE, *LPNET_DEV_CHANNELSTATE;

// ��ѯ�豸����״̬����ؽӿ�����̭���벻Ҫʹ��
typedef struct
{
	DWORD				dwDeviceStatic;			// �豸��״̬��0x00��������0x01��CPUռ�ù��ߣ�0x02��Ӳ������
	NET_DEV_DISKSTATE	stHardDiskStatic[DH_MAX_DISKNUM]; 
	NET_DEV_CHANNELSTATE stChanStatic[DH_MAX_CHANNUM];	//ͨ����״̬
	BYTE				byAlarmInStatic[DH_MAX_ALARMIN];//�����˿ڵ�״̬��0��û�б�����1���б���
	BYTE				byAlarmOutStatic[DH_MAX_ALARMOUT]; //��������˿ڵ�״̬��0��û�������1���б������
	DWORD				dwLocalDisplay;			// ������ʾ״̬��0��������1��������
} NET_DEV_WORKSTATE, *LPNET_DEV_WORKSTATE;

// Э����Ϣ
typedef struct 
{
	char				protocolname[12];		// Э����
	unsigned int		baudbase;				// ������
	unsigned char		databits;				// ����λ
	unsigned char		stopbits;				// ֹͣλ
	unsigned char		parity;					// У��λ
	unsigned char		reserve;
} PROTOCOL_INFO, *LPPROTOCOL_INFO;

// �����Խ���������
typedef struct 
{
	// ��Ƶ�������
	BYTE				byInFormatTag;			// �������ͣ���PCM
	BYTE				byInChannels;			// ������
	WORD				wInBitsPerSample;		// �������					
	DWORD				dwInSamplesPerSec;		// ������

	// ��Ƶ�������
	BYTE				byOutFormatTag;			// �������ͣ���PCM
	BYTE				byOutChannels;			// ������
	WORD				wOutBitsPerSample;		// �������				
	DWORD				dwOutSamplesPerSec;		// ������
} DHDEV_TALK_CFG, *LPDHDEV_TALK_CFG;

/************************************************************************
 ** �ص���������
 ***********************************************************************/

// �������ӶϿ��ص�����ԭ��
typedef void (CALLBACK *fDisConnect)(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);

// �������ӻָ��ص�����ԭ��
typedef void (CALLBACK *fHaveReConnect)(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);

// ��̬�����ӶϿ��ص�����ԭ��
typedef void (CALLBACK *fSubDisConnect)(EM_INTERFACE_TYPE emInterfaceType, BOOL bOnline, LONG lOperateHandle, LONG lLoginID, DWORD dwUser);

// ʵʱ�������ݻص�����ԭ��
typedef void (CALLBACK *fRealDataCallBack)(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);

// ʵʱ�������ݻص�����ԭ��--��չ
typedef void (CALLBACK *fRealDataCallBackEx)(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG param, DWORD dwUser);

// ��Ļ���ӻص�����ԭ��
typedef void (CALLBACK *fDrawCallBack)(LONG lLoginID, LONG lPlayHandle, HDC hDC, DWORD dwUser);

// �ط����ݻص�����ԭ��
typedef int (CALLBACK *fDataCallBack)(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);

// �طŽ��Ȼص�����ԭ��
typedef void (CALLBACK *fDownLoadPosCallBack)(LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, DWORD dwUser);

// ��ʱ��طŽ��Ȼص�����ԭ��
typedef void (CALLBACK *fTimeDownLoadPosCallBack) (LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, DWORD dwUser);

// ��Ϣ�ص�����ԭ��
typedef BOOL (CALLBACK *fMessCallBack)(LONG lCommand, LONG lLoginID, char *pBuf, DWORD dwBufLen, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);

// �����������ص�����ԭ��
typedef int (CALLBACK *fServiceCallBack)(LONG lHandle, char *pIp, WORD wPort, LONG lCommand, void *pParam, DWORD dwParamLen, DWORD dwUserData);

// �����Խ�����Ƶ���ݻص�����ԭ��
typedef void (CALLBACK *pfAudioDataCallBack)(LONG lTalkHandle, char *pDataBuf, DWORD dwBufSize, BYTE byAudioFlag, DWORD dwUser);

// �����豸����ص�����ԭ��
typedef void (CALLBACK *fUpgradeCallBack) (LONG lLoginID, LONG lUpgradechannel, int nTotalSize, int nSendSize, DWORD dwUser);

// ͸�����ڻص�����ԭ��
typedef void (CALLBACK *fTransComCallBack) (LONG lLoginID, LONG lTransComChannel, char *pBuffer, DWORD dwBufSize, DWORD dwUser);

// ��ѯ�豸��־���ݻص�����ԭ��
typedef void (CALLBACK *fLogDataCallBack)(LONG lLoginID, char *pBuffer, DWORD dwBufSize, DWORD nTotalSize, BOOL bEnd, DWORD dwUser);

// ץͼ�ص�����ԭ��
typedef void (CALLBACK *fSnapRev)(LONG lLoginID, BYTE *pBuf, UINT RevLen, UINT EncodeType, DWORD CmdSerial, DWORD dwUser);

// GPS��Ϣ���Ļص�
typedef void (CALLBACK *fGPSRev)(LONG lLoginID, GPS_Info GpsInfo, DWORD dwUserData);

// �첽���ݻص�
typedef void (CALLBACK *fMessDataCallBack)(LONG lCommand, LPNET_CALLBACK_DATA lpData, DWORD dwUser);

// �ļ�����ص�
typedef void (CALLBACK *fTransFileCallBack)(LONG lHandle, int nTransType, int nState, int nSendSize, int nTotalSize, DWORD dwUser);


/************************************************************************
 ** �ӿڶ���
 ***********************************************************************/

// SDK��ʼ��
CLIENT_API BOOL CALL_METHOD CLIENT_Init(fDisConnect cbDisConnect, DWORD dwUser);

// SDK�˳�����
CLIENT_API void CALL_METHOD CLIENT_Cleanup();

//------------------------------------------------------------------------

// ���ö��������ɹ��ص����������ú�SDK�ڲ������Զ�����
CLIENT_API void CALL_METHOD CLIENT_SetAutoReconnect(fHaveReConnect cbAutoConnect, DWORD dwUser);

// ���ö�̬�����Ӷ��߻ص�������ĿǰSVR�豸�ļ��Ӻͻط��Ƕ����ӵġ�
CLIENT_API void CALL_METHOD CLIENT_SetSubconnCallBack(fSubDisConnect cbSubDisConnect, DWORD dwUser);

// ���غ���ִ��ʧ�ܴ���
CLIENT_API DWORD CALL_METHOD CLIENT_GetLastError(void);

// ���������豸��ʱʱ��ͳ��Դ���
CLIENT_API void CALL_METHOD CLIENT_SetConnectTime(int nWaitTime, int nTryTimes);

// ���õ�½���绷��
CLIENT_API void CALL_METHOD CLIENT_SetNetworkParam(NET_PARAM *pNetParam);

// ��ȡSDK�İ汾��Ϣ
CLIENT_API DWORD CALL_METHOD CLIENT_GetSDKVersion();

//------------------------------------------------------------------------

// ���豸ע��
CLIENT_API LONG CALL_METHOD CLIENT_Login(char *pchDVRIP, WORD wDVRPort, char *pchUserName, char *pchPassword, LPNET_DEVICEINFO lpDeviceInfo, int *error = 0);

// ��չ�ӿڣ�nSpecCap = 0ΪTCP��ʽ�µĵ��룬nSpecCap = 2Ϊ����ע��ĵ��룬nSpecCap = 3Ϊ�鲥��ʽ�µĵ��룬nSpecCap = 4ΪUDP��ʽ�µĵ��룬nSpecCap = 6Ϊֻ���������µĵ��룬nSpecCap = 7ΪSSL����
CLIENT_API LONG CALL_METHOD CLIENT_LoginEx(char *pchDVRIP, WORD wDVRPort, char *pchUserName, char *pchPassword, int nSpecCap, void* pCapParam, LPNET_DEVICEINFO lpDeviceInfo, int *error = 0);

// ���豸ע��
CLIENT_API BOOL CALL_METHOD CLIENT_Logout(LONG lLoginID);

//------------------------------------------------------------------------

// ��ʼʵʱ����
CLIENT_API LONG CALL_METHOD CLIENT_RealPlay(LONG lLoginID, int nChannelID, HWND hWnd);

// ��ʼʵʱ����--��չ
CLIENT_API LONG CALL_METHOD CLIENT_RealPlayEx(LONG lLoginID, int nChannelID, HWND hWnd, DH_RealPlayType rType = DH_RType_Realplay);

// �໭��ʵʱԤ��
CLIENT_API LONG CALL_METHOD CLIENT_MultiPlay(LONG lLoginID, HWND hWnd);

// ֹͣ�໭��ʵʱԤ��
CLIENT_API BOOL CALL_METHOD CLIENT_StopMultiPlay(LONG lMultiHandle);

// ץͼ��hPlayHandleΪ���ӻ�طž��
CLIENT_API BOOL CALL_METHOD CLIENT_CapturePicture(LONG hPlayHandle, const char *pchPicFileName);

// ����ʵʱ�������ݻص�
CLIENT_API BOOL CALL_METHOD CLIENT_SetRealDataCallBack(LONG lRealHandle, fRealDataCallBack cbRealData, DWORD dwUser);

// ����ʵʱ�������ݻص�--��չ
CLIENT_API BOOL CALL_METHOD CLIENT_SetRealDataCallBackEx(LONG lRealHandle, fRealDataCallBackEx cbRealData, DWORD dwUser, DWORD dwFlag);

// ����ͼ��������
CLIENT_API BOOL	CALL_METHOD CLIENT_AdjustFluency(LONG lRealHandle, int nLevel);

// ��������Ϊ�ļ�
CLIENT_API BOOL CALL_METHOD CLIENT_SaveRealData(LONG lRealHandle, const char *pchFileName);

// ������������Ϊ�ļ�
CLIENT_API BOOL CALL_METHOD CLIENT_StopSaveRealData(LONG lRealHandle);

// ����ͼ������
CLIENT_API BOOL CALL_METHOD CLIENT_ClientSetVideoEffect(LONG lPlayHandle, unsigned char nBrightness, unsigned char nContrast, unsigned char nHue, unsigned char nSaturation);

// ��ȡͼ������
CLIENT_API BOOL CALL_METHOD CLIENT_ClientGetVideoEffect(LONG lPlayHandle, unsigned char *nBrightness, unsigned char *nContrast, unsigned char *nHue, unsigned char *nSaturation);

// ������Ļ���ӻص�
CLIENT_API void CALL_METHOD CLIENT_RigisterDrawFun(fDrawCallBack cbDraw, DWORD dwUser);

// ������
CLIENT_API BOOL CALL_METHOD CLIENT_OpenSound(LONG hPlayHandle);

// ����������lPlayHandleΪ���ӻ�طž��
CLIENT_API BOOL CALL_METHOD CLIENT_SetVolume(LONG lPlayHandle, int nVolume);

// �ر�����
CLIENT_API BOOL CALL_METHOD CLIENT_CloseSound();

// ֹͣʵʱԤ��
CLIENT_API BOOL CALL_METHOD CLIENT_StopRealPlay(LONG lRealHandle);

// ֹͣʵʱԤ��--��չ
CLIENT_API BOOL CALL_METHOD CLIENT_StopRealPlayEx(LONG lRealHandle);

//------------------------------------------------------------------------

// ͨ����̨����
CLIENT_API BOOL CALL_METHOD CLIENT_PTZControl(LONG lLoginID, int nChannelID, DWORD dwPTZCommand, DWORD dwStep, BOOL dwStop);

// ����̨����
CLIENT_API BOOL CALL_METHOD CLIENT_DHPTZControl(LONG lLoginID, int nChannelID, DWORD dwPTZCommand, unsigned char param1, unsigned char param2, unsigned char param3, BOOL dwStop);

// ����̨������չ�ӿڣ�֧����ά���ٶ�λ
CLIENT_API BOOL CALL_METHOD CLIENT_DHPTZControlEx(LONG lLoginID, int nChannelID, DWORD dwPTZCommand, LONG lParam1, LONG lParam2, LONG lParam3, BOOL dwStop);

//------------------------------------------------------------------------

// ��ѯʱ������Ƿ���¼���ļ�
CLIENT_API BOOL CALL_METHOD CLIENT_QueryRecordTime(LONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char* pchCardid, BOOL *bResult, int waittime=1000);

// ��ѯʱ����ڵ�����¼���ļ�
CLIENT_API BOOL CALL_METHOD CLIENT_QueryRecordFile(LONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char* pchCardid, LPNET_RECORDFILE_INFO nriFileinfo, int maxlen, int *filecount, int waittime=1000, BOOL bTime = FALSE);

// ��ѯ����¼��ʱ��
CLIENT_API BOOL CALL_METHOD CLIENT_QueryFurthestRecordTime(LONG lLoginID, int nRecordFileType, char *pchCardid, NET_FURTHEST_RECORD_TIME* pFurthrestTime, int nWaitTime);

// ��ʼ����¼���ļ�
CLIENT_API LONG	CALL_METHOD CLIENT_FindFile(LONG lLoginID, int nChannelId, int nRecordFileType, char* cardid, LPNET_TIME time_start, LPNET_TIME time_end, BOOL bTime, int waittime);

// ����¼���ļ�
CLIENT_API int	CALL_METHOD CLIENT_FindNextFile(LONG lFindHandle,LPNET_RECORDFILE_INFO lpFindData);

// ����¼���ļ�����
CLIENT_API BOOL CALL_METHOD CLIENT_FindClose(LONG lFindHandle);

// ���ļ���ʽ�ط�
CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByRecordFile(LONG lLoginID, LPNET_RECORDFILE_INFO lpRecordFile, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwUserData);

// ���ļ���ʽ�ط�--��չ
CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByRecordFileEx(LONG lLoginID, LPNET_RECORDFILE_INFO lpRecordFile, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwPosUser, fDataCallBack fDownLoadDataCallBack, DWORD dwDataUser);

// ��ʱ�䷽ʽ�ط�
CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByTime(LONG lLoginID, int nChannelID, LPNET_TIME lpStartTime, LPNET_TIME lpStopTime, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwPosUser);

// ��ʱ�䷽ʽ�ط�--��չ
CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByTimeEx(LONG lLoginID, int nChannelID, LPNET_TIME lpStartTime, LPNET_TIME lpStopTime, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwPosUser, fDataCallBack fDownLoadDataCallBack, DWORD dwDataUser);

// ��λ¼��ط���ʼ��
CLIENT_API BOOL CALL_METHOD CLIENT_SeekPlayBack(LONG lPlayHandle, unsigned int offsettime, unsigned int offsetbyte);

// ��ͣ��ָ�¼��ط�
CLIENT_API BOOL CALL_METHOD CLIENT_PausePlayBack(LONG lPlayHandle, BOOL bPause);

// ���¼��ط�
CLIENT_API BOOL CALL_METHOD CLIENT_FastPlayBack(LONG lPlayHandle);

// ����¼��ط�
CLIENT_API BOOL CALL_METHOD CLIENT_SlowPlayBack(LONG lPlayHandle);

// ����¼��ط�
CLIENT_API BOOL CALL_METHOD CLIENT_StepPlayBack(LONG lPlayHandle, BOOL bStop);

// ���ûط�֡��
CLIENT_API BOOL CALL_METHOD CLIENT_SetFramePlayBack(LONG lPlayHandle, int framerate);

// ��ȡ�ط�֡��
CLIENT_API BOOL CALL_METHOD CLIENT_GetFramePlayBack(LONG lPlayHandle, int *fileframerate, int *playframerate);

// �ָ������ط��ٶ�
CLIENT_API BOOL CALL_METHOD CLIENT_NormalPlayBack(LONG lPlayHandle);

// ��ȡ�ط�OSDʱ��
CLIENT_API BOOL CALL_METHOD CLIENT_GetPlayBackOsdTime(LONG lPlayHandle, LPNET_TIME lpOsdTime, LPNET_TIME lpStartTime, LPNET_TIME lpEndTime);

// ֹͣ¼��ط�
CLIENT_API BOOL CALL_METHOD CLIENT_StopPlayBack(LONG lPlayHandle);

// ����¼���ļ�
CLIENT_API LONG CALL_METHOD CLIENT_DownloadByRecordFile(LONG lLoginID,LPNET_RECORDFILE_INFO lpRecordFile, char *sSavedFileName, fDownLoadPosCallBack cbDownLoadPos, DWORD dwUserData);

// ͨ��ʱ������¼��
CLIENT_API LONG CALL_METHOD CLIENT_DownloadByTime(LONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char *sSavedFileName, fTimeDownLoadPosCallBack cbTimeDownLoadPos, DWORD dwUserData);

// ��ѯ¼�����ؽ���
CLIENT_API BOOL CALL_METHOD CLIENT_GetDownloadPos(LONG lFileHandle, int *nTotalSize, int *nDownLoadSize);

// ֹͣ¼������
CLIENT_API BOOL CALL_METHOD CLIENT_StopDownload(LONG lFileHandle);

//------------------------------------------------------------------------

// ���ñ����ص�����
CLIENT_API void CALL_METHOD CLIENT_SetDVRMessCallBack(fMessCallBack cbMessage,DWORD dwUser);

// ���豸���ı���
CLIENT_API BOOL CALL_METHOD CLIENT_StartListen(LONG lLoginID);

// ���豸���ı���--��չ
CLIENT_API BOOL CALL_METHOD CLIENT_StartListenEx(LONG lLoginID);

// ֹͣ���ı���
CLIENT_API BOOL CALL_METHOD CLIENT_StopListen(LONG lLoginID);

//------------------------------------------------------------------------

// ����ע�Ṧ�ܣ���������nTimeout��������Ч
CLIENT_API LONG CALL_METHOD CLIENT_ListenServer(char* ip, WORD port, int nTimeout, fServiceCallBack cbListen, DWORD dwUserData);

// ֹͣ����
CLIENT_API BOOL CALL_METHOD CLIENT_StopListenServer(LONG lServerHandle);

// ��Ӧ�豸��ע������
CLIENT_API BOOL CALL_METHOD CLIENT_ResponseDevReg(char *devSerial, char* ip, WORD port, BOOL bAccept);

//------------------------------------------------------------------------

// �����ϴ����ܣ���������dwTimeOut��������Ч
CLIENT_API LONG CALL_METHOD CLIENT_StartService(WORD wPort, char *pIp = NULL, fServiceCallBack pfscb = NULL, DWORD dwTimeOut = 0xffffffff, DWORD dwUserData = 0);

// ֹͣ����
CLIENT_API BOOL CALL_METHOD CLIENT_StopService(LONG lHandle);

//------------------------------------------------------------------------

// ���������Խ�ģʽ���ͻ��˷�ʽ���Ƿ�������ʽ
CLIENT_API BOOL CALL_METHOD CLIENT_SetDeviceMode(LONG lLoginID, EM_USEDEV_MODE emType, void* pValue);

// �������Խ�
CLIENT_API LONG CALL_METHOD CLIENT_StartTalkEx(LONG lLoginID, pfAudioDataCallBack pfcb, DWORD dwUser);

// ��ʼPC��¼��
CLIENT_API BOOL CALL_METHOD CLIENT_RecordStart();

// ����PC��¼��
CLIENT_API BOOL CALL_METHOD CLIENT_RecordStop();

// �����������ݵ��豸
CLIENT_API LONG CALL_METHOD CLIENT_TalkSendData(LONG lTalkHandle, char *pSendBuf, DWORD dwBufSize);

// ������Ƶ����
CLIENT_API void CALL_METHOD CLIENT_AudioDec(char *pAudioDataBuf, DWORD dwBufSize);

// ���������Խ�������
CLIENT_API BOOL CALL_METHOD CLIENT_SetAudioClientVolume(LONG lTalkHandle, WORD wVolume);

// ֹͣ�����Խ�
CLIENT_API BOOL CALL_METHOD CLIENT_StopTalkEx(LONG lTalkHandle);

// ����豸���㲥��
CLIENT_API BOOL CALL_METHOD CLIENT_AudioBroadcastAddDev(LONG lLoginID);

// �ӹ㲥����ɾ���豸
CLIENT_API BOOL CALL_METHOD CLIENT_AudioBroadcastDelDev(LONG lLoginID);

// ��Ƶ����--��ʼ��(�ض���׼��ʽ->�󻪸�ʽ)
CLIENT_API int  CALL_METHOD CLIENT_InitAudioEncode(DH_AUDIO_FORMAT aft);

// ��Ƶ����--���ݱ���
CLIENT_API int	CALL_METHOD	CLIENT_AudioEncode(LONG lTalkHandle, BYTE *lpInBuf, DWORD *lpInLen, BYTE *lpOutBuf, DWORD *lpOutLen);

// ��Ƶ����--����˳�
CLIENT_API int	CALL_METHOD	CLIENT_ReleaseAudioEncode();

//------------------------------------------------------------------------

// ��ѯ�豸��־
CLIENT_API BOOL CALL_METHOD CLIENT_QueryLog(LONG lLoginID, char *pLogBuffer, int maxlen, int *nLogBufferlen, int waittime=3000);

// ��ѯ�豸��־���Է�ҳ��ʽ��ѯ
CLIENT_API BOOL CALL_METHOD CLIENT_QueryDeviceLog(LONG lLoginID, QUERY_DEVICE_LOG_PARAM *pQueryParam, char *pLogBuffer, int nLogBufferLen, int *pRecLogNum, int waittime=3000);

// ���ͨ��¼��״̬
CLIENT_API BOOL CALL_METHOD CLIENT_QueryRecordState(LONG lLoginID, char *pRSBuffer, int maxlen, int *nRSBufferlen, int waittime=1000);

// ��ѯ�豸״̬
CLIENT_API BOOL CALL_METHOD CLIENT_QueryDevState(LONG lLoginID, int nType, char *pBuf, int nBufLen, int *pRetLen, int waittime=1000);

// ��ѯϵͳ������Ϣ
CLIENT_API BOOL CALL_METHOD CLIENT_QuerySystemInfo(LONG lLoginID, int nSystemType, char *pSysInfoBuffer, int maxlen, int *nSysInfolen, int waittime=1000);

// ��ȡͨ������
CLIENT_API LONG CALL_METHOD CLIENT_GetStatiscFlux(LONG lLoginID, LONG lPlayHandle);

// ��ȡ��̨��Ϣ
CLIENT_API BOOL  CALL_METHOD CLIENT_GetPtzOptAttr(LONG lLoginID,DWORD dwProtocolIndex,LPVOID lpOutBuffer,DWORD dwBufLen,DWORD *lpBytesReturned,int waittime=500);

//------------------------------------------------------------------------

// �����豸
CLIENT_API BOOL CALL_METHOD CLIENT_RebootDev(LONG lLoginID);

// �ر��豸
CLIENT_API BOOL CALL_METHOD CLIENT_ShutDownDev(LONG lLoginID);

// �豸����
CLIENT_API BOOL CALL_METHOD CLIENT_ControlDevice(LONG lLoginID, CtrlType type, void *param, int waittime = 1000);

// ����ͨ��¼��״̬
CLIENT_API BOOL CALL_METHOD CLIENT_SetupRecordState(LONG lLoginID, char *pRSBuffer, int nRSBufferlen);

// ��ѯIO״̬
CLIENT_API BOOL CALL_METHOD CLIENT_QueryIOControlState(LONG lLoginID, DH_IOTYPE emType, 
                                           void *pState, int maxlen, int *nIOCount, int waittime=1000);

// IO����
CLIENT_API BOOL CALL_METHOD CLIENT_IOControl(LONG lLoginID, DH_IOTYPE emType, void *pState, int maxlen);

// ǿ��I֡��nChannelID��ͨ���ţ�nSubChannel����������(0������1��������1)
CLIENT_API BOOL CALL_METHOD CLIENT_MakeKeyFrame(LONG lLoginID, int nChannelID, int nSubChannel=0);

//------------------------------------------------------------------------

// ��ѯ�û���Ϣ
CLIENT_API BOOL CALL_METHOD CLIENT_QueryUserInfo(LONG lLoginID, USER_MANAGE_INFO *info, int waittime=1000);

// ��ѯ�û���Ϣ--��չ
CLIENT_API BOOL CALL_METHOD CLIENT_QueryUserInfoEx(LONG lLoginID, USER_MANAGE_INFO_EX *info, int waittime=1000);

// �����豸�û�
CLIENT_API BOOL CALL_METHOD CLIENT_OperateUserInfo(LONG lLoginID, int nOperateType, void *opParam, void *subParam, int waittime=1000);

// �����豸�û�--��չ
CLIENT_API BOOL CALL_METHOD CLIENT_OperateUserInfoEx(LONG lLoginID, int nOperateType, void *opParam, void *subParam, int waittime=1000);

//------------------------------------------------------------------------

// ����͸������ͨ��
CLIENT_API LONG CALL_METHOD CLIENT_CreateTransComChannel(LONG lLoginID, int TransComType, unsigned int baudrate, unsigned int databits, unsigned int stopbits, unsigned int parity, fTransComCallBack cbTransCom, DWORD dwUser);

// ͸�����ڷ�������
CLIENT_API BOOL CALL_METHOD CLIENT_SendTransComData(LONG lTransComChannel, char *pBuffer, DWORD dwBufSize);

// �ͷ�ͨ������ͨ��
CLIENT_API BOOL CALL_METHOD CLIENT_DestroyTransComChannel(LONG lTransComChannel);

// ��ѯ͸������״̬
CLIENT_API BOOL   CALL_METHOD CLIENT_QueryTransComParams(LONG lLoginID, int TransComType, DH_COMM_STATE* pCommState, int nWaitTime = 500);

//------------------------------------------------------------------------

// ��ʼ�����豸����
CLIENT_API LONG CALL_METHOD CLIENT_StartUpgrade(LONG lLoginID, char *pchFileName, fUpgradeCallBack cbUpgrade, DWORD dwUser);

// ��ʼ�����豸����--��չ
CLIENT_API LONG CALL_METHOD CLIENT_StartUpgradeEx(LONG lLoginID, EM_UPGRADE_TYPE emType, char *pchFileName, fUpgradeCallBack cbUpgrade, DWORD dwUser);

// ��������
CLIENT_API BOOL CALL_METHOD CLIENT_SendUpgrade(LONG lUpgradeID);

// ���������豸����
CLIENT_API BOOL CALL_METHOD CLIENT_StopUpgrade(LONG lUpgradeID);

//------------------------------------------------------------------------

// ��ѯ������Ϣ
CLIENT_API BOOL  CALL_METHOD CLIENT_GetDevConfig(LONG lLoginID, DWORD dwCommand, LONG lChannel, LPVOID lpOutBuffer, DWORD dwOutBufferSize, LPDWORD lpBytesReturned,int waittime=500);

// ����������Ϣ
CLIENT_API BOOL  CALL_METHOD CLIENT_SetDevConfig(LONG lLoginID, DWORD dwCommand, LONG lChannel, LPVOID lpInBuffer, DWORD dwInBufferSize, int waittime=500);

// �����ýӿڣ���ѯ������Ϣ(��Json��ʽ�����������SDK)
CLIENT_API BOOL CALL_METHOD CLIENT_GetNewDevConfig(LONG lLoginID, char* szCommand, int nChannelID, char* szOutBuffer, DWORD dwOutBufferSize, int *error, int waittime=500);

// �����ýӿڣ�����������Ϣ(��Json��ʽ�����������SDK)
CLIENT_API BOOL CALL_METHOD CLIENT_SetNewDevConfig(LONG lLoginID, char* szCommand, int nChannelID, char* szInBuffer, DWORD dwInBufferSize, int *error, int *restart, int waittime=500);

//------------------------------------------------------------------------

// ��ѯ�豸ͨ������
CLIENT_API BOOL CALL_METHOD CLIENT_QueryChannelName(LONG lLoginID, char *pChannelName, int maxlen, int *nChannelCount, int waittime=1000);

// �����豸ͨ������
CLIENT_API BOOL  CALL_METHOD CLIENT_SetupChannelName(LONG lLoginID,char *pbuf, int nbuflen);

// �����豸ͨ���ַ�����
CLIENT_API BOOL  CALL_METHOD CLIENT_SetupChannelOsdString(LONG lLoginID, int nChannelNo, DH_CHANNEL_OSDSTRING* struOsdString, int nbuflen);

// ��ѯ�豸��ǰʱ��
CLIENT_API BOOL CALL_METHOD CLIENT_QueryDeviceTime(LONG lLoginID, LPNET_TIME pDeviceTime, int waittime=1000);

// �����豸��ǰʱ��
CLIENT_API BOOL CALL_METHOD CLIENT_SetupDeviceTime(LONG lLoginID, LPNET_TIME pDeviceTime);

// �����豸��������
CLIENT_API BOOL CALL_METHOD CLIENT_SetMaxFlux(LONG lLoginID, WORD wFlux);

//------------------------------------------------------------------------

// ���������ļ�
CLIENT_API LONG CALL_METHOD CLIENT_ImportConfigFile(LONG lLoginID, char *szFileName, fDownLoadPosCallBack cbUploadPos, DWORD dwUserData, DWORD param=0);

// ֹͣ���������ļ�
CLIENT_API BOOL CALL_METHOD CLIENT_StopImportCfgFile(LONG lImportHandle);

// ���������ļ�
CLIENT_API LONG CALL_METHOD CLIENT_ExportConfigFile(LONG lLoginID, DH_CONFIG_FILE_TYPE emConfigFileType, char *szSavedFilePath, fDownLoadPosCallBack cbDownLoadPos, DWORD dwUserData);

// ֹͣ���������ļ�
CLIENT_API BOOL CALL_METHOD CLIENT_StopExportCfgFile(LONG lExportHandle);

//------------------------------------------------------------------------

// ͨ���豸���ֻ����豸���к���DDNS��������ѯ�豸IP
CLIENT_API BOOL CALL_METHOD CLIENT_GetDVRIPByResolveSvr(char *pchDVRIP, WORD wDVRPort, BYTE *sDVRName, WORD wDVRNameLen, BYTE *sDVRSerialNumber, WORD wDVRSerialLen, char* sGetIP);

// ������������IPC��NVS���豸
CLIENT_API BOOL CALL_METHOD CLIENT_SearchDevices(char* szBuf, int nBufLen, int* pRetLen, DWORD dwSearchTime);

//------------------------------------------------------------------------

// ƽ̨����ӿ�
CLIENT_API BOOL CALL_METHOD CLIENT_GetPlatFormInfo(LONG lLoginID, DWORD dwCommand, int nSubCommand, int nParam, LPVOID lpOutBuffer, DWORD dwOutBufferSize, LPDWORD lpBytesReturned,int waittime=500);
CLIENT_API BOOL CALL_METHOD CLIENT_SetPlatFormInfo(LONG lLoginID, DWORD dwCommand, int nSubCommand, int nParam, LPVOID lpInBuffer, DWORD dwInBufferSize, int waittime=500);


///////////////////////////////�����豸�ӿ�///////////////////////////////

// ����ץͼ�ص�����
CLIENT_API void CALL_METHOD CLIENT_SetSnapRevCallBack(fSnapRev OnSnapRevMessage, DWORD dwUser);

// ץͼ����
CLIENT_API BOOL CALL_METHOD CLIENT_SnapPicture(LONG lLoginID, SNAP_PARAMS par);

// ����GPS���Ļص�����
CLIENT_API void CALL_METHOD CLIENT_SetSubcribeGPSCallBack(fGPSRev OnGPSMessage, DWORD dwUser);

// GPS��Ϣ����
CLIENT_API BOOL CALL_METHOD CLIENT_SubcribeGPS (LONG lLoginID, BOOL bStart, LONG KeepTime, LONG InterTime);


//////////////////////////////�������豸�ӿ�//////////////////////////////

// ��ѯ��������Ϣ
CLIENT_API BOOL CALL_METHOD CLIENT_QueryDecoderInfo(LONG lLoginID, LPDEV_DECODER_INFO lpDecInfo, int waittime=1000);

// ��ѯ��������ǰTV��Ϣ
CLIENT_API BOOL CALL_METHOD CLIENT_QueryDecoderTVInfo(LONG lLoginID, int nMonitorID, LPDEV_DECODER_TV lpMonitorInfo, int waittime=1000);

// ��ѯ����ͨ����Ϣ
CLIENT_API BOOL CALL_METHOD CLIENT_QueryDecEncoderInfo(LONG lLoginID, int nEncoderID, LPDEV_ENCODER_INFO lpEncoderInfo, int waittime=1000);

// ���ý�����TV���ʹ��
CLIENT_API BOOL CALL_METHOD CLIENT_SetDecTVOutEnable(LONG lLoginID, BYTE *pDecTVOutEnable, int nBufLen, int waittime=1000);

//------------------------------------------------------------------------

// �����첽�ص�����
CLIENT_API BOOL CALL_METHOD CLIENT_SetOperateCallBack(LONG lLoginID, fMessDataCallBack cbMessData, DWORD dwUser);

// ���ƽ�����TV����ָ�ӿ�Ϊ�첽��ʽ
CLIENT_API LONG CALL_METHOD CLIENT_CtrlDecTVScreen(LONG lLoginID, int nMonitorID, BOOL bEnable, int nSplitType, BYTE *pEncoderChannel, int nBufLen, void* userdata=NULL);

// ������TV�����л����ӿ�Ϊ�첽��ʽ
// ����nMonitorID(TV��)��nWndID(�����)ת��ΪnEncoderID(����ͨ��)��ʽ��nEncoderID��nMonitorID*nSplitNum(�ָ���)+nWndID
CLIENT_API LONG CALL_METHOD CLIENT_SwitchDecTVEncoder(LONG lLoginID, int nEncoderID, LPDEV_ENCODER_INFO lpEncoderInfo, void* userdata=NULL);

//------------------------------------------------------------------------

// ���ӻ������
CLIENT_API int CALL_METHOD CLIENT_AddTourCombin(LONG lLoginID, int nMonitorID, int nSplitType, BYTE *pEncoderChannnel, int nBufLen, int waittime=1000);

// ɾ���������
CLIENT_API BOOL CALL_METHOD CLIENT_DelTourCombin(LONG lLoginID, int nMonitorID, int nCombinID, int waittime=1000);

// �޸Ļ������
CLIENT_API BOOL CALL_METHOD CLIENT_SetTourCombin(LONG lLoginID, int nMonitorID, int nCombinID, int nSplitType, BYTE *pEncoderChannel, int nBufLen, int waittime=1000);

// ��ѯ������ϣ�nCombinID��Χ0��32
CLIENT_API BOOL CALL_METHOD CLIENT_QueryTourCombin(LONG lLoginID, int nMonitorID, int nCombinID, LPDEC_COMBIN_INFO lpDecCombinInfo, int waittime=1000);

// ������Ѳ����
CLIENT_API BOOL CALL_METHOD CLIENT_SetDecoderTour(LONG lLoginID, int nMonitorID, LPDEC_TOUR_COMBIN lpDecTourInfo, int waittime=1000);

// ��ѯ��Ѳ����
CLIENT_API BOOL CALL_METHOD CLIENT_QueryDecoderTour(LONG lLoginID, int nMonitorID, LPDEC_TOUR_COMBIN lpDecTourInfo, int waittime=1000);

// ��ѯ��ǰ����ͨ������Ϣ
CLIENT_API BOOL CALL_METHOD CLIENT_QueryDecChannelFlux(LONG lLoginID, int nEncoderID, LPDEV_DECCHANNEL_STATE lpChannelStateInfo, int waittime=1000);

//������������Ѳ
CLIENT_API BOOL CALL_METHOD CLIENT_CtrlDecoderTour(LONG lLoginID, int nMonitorID, DEC_CTRL_TOUR_TYPE emActionParam, int waittime = 1000);
//------------------------------------------------------------------------

typedef void (CALLBACK *fDecPlayBackPosCallBack)(LONG lLoginID, int nEncoderID, DWORD dwTotalSize, DWORD dwPlaySize, DWORD dwUser);

// ���ûطŽ��Ȼص�����
CLIENT_API BOOL CALL_METHOD CLIENT_SetDecPlaybackPos(LONG lLoginID, fDecPlayBackPosCallBack cbPlaybackPos, DWORD dwUser);

// ������TV����طţ��ӿ�Ϊ�첽��ʽ
CLIENT_API LONG CALL_METHOD CLIENT_DecTVPlayback(LONG lLoginID, int nEncoderID, DEC_PLAYBACK_MODE emPlaybackMode, LPVOID lpInBuffer, DWORD dwInBufferSize, void* userdata=NULL);

// ������TV����طſ���
CLIENT_API BOOL CALL_METHOD CLIENT_CtrlDecPlayback(LONG lLoginID, int nEncoderID, DEC_CTRL_PLAYBACK_TYPE emCtrlType, int nValue, int waittime=1000);


///////////////////////////////����汾�ӿ�///////////////////////////////

// ��ѯ�豸��־--��չ
CLIENT_API BOOL CALL_METHOD CLIENT_QueryLogEx(LONG lLoginID, DH_LOG_QUERY_TYPE logType, char *pLogBuffer, int maxlen, int *nLogBufferlen, void* reserved, int waittime=3000);

// ����ע���ض����ܣ�������������
CLIENT_API LONG CALL_METHOD CLIENT_ControlConnectServer(LONG lLoginID, char* RegServerIP, WORD RegServerPort, int TimeOut=3000);

// ��������ע��
CLIENT_API BOOL CALL_METHOD CLIENT_ControlRegisterServer(LONG lLoginID, LONG ConnectionID, int waittime=1000);

// �Ͽ���������
CLIENT_API BOOL CALL_METHOD CLIENT_ControlDisconnectRegServer(LONG lLoginID, LONG ConnectionID);

// ��ѯ�豸������ע���������Ϣ
CLIENT_API BOOL CALL_METHOD CLIENT_QueryControlRegServerInfo(LONG lLoginID, LPDEV_SERVER_AUTOREGISTER lpRegServerInfo, int waittime=2000);

// �ļ��ϴ��ӿ�
CLIENT_API LONG CALL_METHOD CLIENT_FileTransmit(LONG lLoginID, int nTransType, char* szInBuf, int nInBufLen, fTransFileCallBack cbTransFile, DWORD dwUserData, int waittime);

/////////////////////////////////��̭�ӿ�/////////////////////////////////

// ��ѯϵͳ���������ã��ýӿ�����̭����ʹ��CLIENT_GetDevConfig
CLIENT_API BOOL CALL_METHOD CLIENT_QueryConfig(LONG lLoginID, int nConfigType, char *pConfigbuf, int maxlen, int *nConfigbuflen, int waittime=1000);

// ����ϵͳ���������ã��ýӿ�����̭����ʹ��CLIENT_SetDevConfig
CLIENT_API BOOL CALL_METHOD CLIENT_SetupConfig(LONG lLoginID, int nConfigType, char *pConfigbuf, int nConfigbuflen, int waittime=1000);

// �ýӿ�����̭���벻Ҫʹ��
CLIENT_API BOOL CALL_METHOD CLIENT_Reset(LONG lLoginID, BOOL bReset);

// ��ѯ����Э�� �� �ýӿ�����̭����ʹ��CLIENT_GetDevConfig
CLIENT_API BOOL CALL_METHOD CLIENT_QueryComProtocol(LONG lLoginID, int nProtocolType, char *pProtocolBuffer, int maxlen, int *nProtocollen, int waittime=1000);

// ��ʼ�Խ����ýӿ�����̭����ʹ��CLIENT_StartTalkEx
CLIENT_API BOOL CALL_METHOD CLIENT_StartTalk(LONG lRealHandle, BOOL bCustomSend=false);

// �����Խ����ýӿ�����̭����ʹ��CLIENT_StopTalkEx
CLIENT_API BOOL CALL_METHOD CLIENT_StopTalk(LONG lRealHandle);

// �����Զ���Խ����ݣ��ýӿ�����̭����ʹ��CLIENT_TalkSendData
CLIENT_API BOOL CALL_METHOD CLIENT_SendTalkData_Custom(LONG lRealHandle, char *pBuffer, DWORD dwBufSize);

// ����ʵʱԤ�������С
CLIENT_API BOOL CALL_METHOD CLIENT_SetPlayerBufNumber(LONG lRealHandle, DWORD dwBufNum);

// ͨ��ʱ�������ļ�
CLIENT_API BOOL CALL_METHOD CLIENT_GetFileByTime(LONG lLoginID, int nChannelID, LPNET_TIME lpStartTime, LPNET_TIME lpStopTime, char *sSavedFileName);

// ����طſ���
CLIENT_API BOOL CALL_METHOD CLIENT_PlayBackControl(LONG lPlayHandle, DWORD dwControlCode, DWORD dwInValue, DWORD *lpOutValue);

// ��ѯ�豸����״̬ �ýӿ�����̭����ʹ��CLIENT_QueryDevState
CLIENT_API BOOL CALL_METHOD CLIENT_GetDEVWorkState(LONG lLoginID, LPNET_DEV_WORKSTATE lpWorkState, int waittime=1000);

// �첽��ѯ�豸��־
CLIENT_API BOOL CALL_METHOD CLIENT_QueryLogCallback(LONG lLoginID, fLogDataCallBack cbLogData, DWORD dwUser);



#ifdef __cplusplus
}
#endif

#endif // DHNETSDK_H










































