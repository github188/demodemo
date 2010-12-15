
#ifndef DHCONFIGSDK_H
#define DHCONFIGSDK_H

#ifdef WIN32

#ifdef DHCONFIGSDK_EXPORTS
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

#endif


#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************
 ** ��������
 ***********************************************************************/

#define MAX_CHANNEL_NUM			8
#define MAX_VIDEO_CHANNEL_NUM	256					// ���ͨ����256
#define MAX_CHANNELNAME_LEN		32					// ���ͨ�����Ƴ���
#define MAX_VIDEOSTREAM_NUM		3					// �����������
#define MAX_VIDEO_COVER_NUM		16					// ����ڵ��������
#define WEEK_DAY_NUM			7					// һ�ܵ�����
#define MAX_REC_TSECT			6					// ¼��ʱ��θ���
#define MAX_WATERMARK_LEN		4096				// ����ˮӡ������󳤶�
#define MAX_MOTION_ROW			32					// ��̬������������
#define MAX_MOTION_COL			32					// ��̬������������
#define MAX_IMAGESIZE_NUM		256					// ���֧�ֵķֱ��ʸ���
#define MAX_FPS_NUM				1024				// ���֧�ֵ�֡�ʸ���
#define MAX_QUALITY_NUM			32					// ���֧�ֵĻ��ʸ���
#define MAX_ADDRESS_LEN			256					// ���ĵ�ַ����
#define MAX_USERNAME_LEN		64					// ����û�������
#define MAX_PASSWORD_LEN		64					// ������볤��
#define MAX_DIRECTORY_LEN		256					// �ļ��������ַ�������
#define MAX_NAS_TIME_SECTION	2					// ����洢ʱ��θ���
#define MAX_NAME_LEN			32					// ͨ�������ַ�������
#define MAX_DECPRO_LIST_SIZE	100					// ������Э���б��������


/************************************************************************
 ** ��������
 ***********************************************************************/

#define CFG_CMD_ENCODE			"Encode"			// ͼ��ͨ����������(��ӦCFG_ENCODE_INFO)
#define CFG_CMD_RECORD			"Record"			// ��ʱ¼������
#define CFG_CMD_ALARMINPUT		"Alarm"				// �ⲿ���뱨������
#define CFG_CMD_NETALARMINPUT	"NetAlarm"			// ���籨������
#define CFG_CMD_MOTIONDETECT	"MotionDetect"		// ��̬��ⱨ������
#define CFG_CMD_VIDEOLOST		"LossDetect"		// ��Ƶ��ʧ��������
#define CFG_CMD_VIDEOBLIND		"BlindDetect"		// ��Ƶ�ڵ���������
#define CFG_CMD_STORAGENOEXIST	"StorageNotExist"	// �޴洢�豸��������
#define CFG_CMD_STORAGEFAILURE	"StorageFailure"	// �洢�豸���ʳ���������
#define CFG_CMD_STORAGELOWSAPCE	"StorageLowSpace"	// �洢�豸�ռ䲻�㱨������
#define CFG_CMD_NETABORT		"NetAbort"			// ����Ͽ���������	
#define CFG_CMD_IPCONFLICT		"IPConflict"		// IP��ͻ��������
#define CFG_CMD_SNAPCAPINFO		"SnapInfo"			// ץͼ������ѯ
#define CFG_CMD_NAS				"NAS"				// ����洢����������
#define CFG_CMD_PTZ				"Ptz"				// ��̨����
#define CFG_CMD_WATERMARK		"WaterMark"			// ��Ƶˮӡ����


/************************************************************************
 ** ������Ϣ(ÿ���ṹ�嶼��������Ϣ�������ֲ�ͬ�豸����汾�Ͳ�ͬ�ͺ��豸)
 ***********************************************************************/

//-----------------------------ͼ��ͨ������-------------------------------

// ����
typedef enum tagCFG_IMAGE_QUALITY
{
	IMAGE_QUALITY_Q10 = 1,							// ͼ������10%
	IMAGE_QUALITY_Q30,								// ͼ������30%
	IMAGE_QUALITY_Q50,								// ͼ������50%
	IMAGE_QUALITY_Q60,								// ͼ������60%
	IMAGE_QUALITY_Q80,								// ͼ������80%
	IMAGE_QUALITY_Q100,								// ͼ������100%
} CFG_IMAGE_QUALITY;

// ��Ƶѹ����ʽ
typedef enum tagCFG_VIDEO_COMPRESSION
{
	VIDEO_FORMAT_MPEG4,								// MPEG4
	VIDEO_FORMAT_MS_MPEG4,							// MS-MPEG4
	VIDEO_FORMAT_MPEG2,								// MPEG2
	VIDEO_FORMAT_MPEG1,								// MPEG1
	VIDEO_FORMAT_H263,								// H.263
	VIDEO_FORMAT_MJPG,								// MJPG
	VIDEO_FORMAT_FCC_MPEG4,							// FCC-MPEG4
	VIDEO_FORMAT_H264,								// H.264
} CFG_VIDEO_COMPRESSION;

// ��������ģʽ
typedef enum tagCFG_BITRATE_CONTROL
{
	BITRATE_CBR,									// �̶�����
	BITRATE_VBR,									// �ɱ�����
} CFG_BITRATE_CONTROL;

// �ֱ���ö��
typedef enum tagCFG_CAPTURE_SIZE
{
	IMAGE_SIZE_D1,								// 704*576(PAL)  704*480(NTSC)
	IMAGE_SIZE_HD1,								// 352*576(PAL)  352*480(NTSC)
	IMAGE_SIZE_BCIF,							// 704*288(PAL)  704*240(NTSC)
	IMAGE_SIZE_CIF,								// 352*288(PAL)  352*240(NTSC)
	IMAGE_SIZE_QCIF,							// 176*144(PAL)  176*120(NTSC)
	IMAGE_SIZE_VGA,								// 640*480
	IMAGE_SIZE_QVGA,							// 320*240
	IMAGE_SIZE_SVCD,							// 480*480
	IMAGE_SIZE_QQVGA,							// 160*128
	IMAGE_SIZE_SVGA,							// 800*592
	IMAGE_SIZE_XVGA,							// 1024*768
	IMAGE_SIZE_WXGA,							// 1280*800
	IMAGE_SIZE_SXGA,							// 1280*1024  
	IMAGE_SIZE_WSXGA,							// 1600*1024  
	IMAGE_SIZE_UXGA,							// 1600*1200
	IMAGE_SIZE_WUXGA,							// 1920*1200
	IMAGE_SIZE_LTF,								// 240*192
	IMAGE_SIZE_720,								// 1280*720
	IMAGE_SIZE_1080,							// 1920*1080
	IMAGE_SIZE_1_3M,							// 1280*960
	IMAGE_SIZE_NR  
} CFG_CAPTURE_SIZE;

// ��Ƶ��ʽ
typedef struct tagCFG_VIDEO_FORMAT
{
	// ����
	bool				abCompression;
	bool				abWidth;
	bool				abHeight;
	bool				abBitRateControl;
	bool				abBitRate;
	bool				abFrameRate;
	bool				abIFrameInterval;
	bool				abImageQuality;
	bool				abFrameType;

	// ��Ϣ
	CFG_VIDEO_COMPRESSION emCompression;			// ��Ƶѹ����ʽ
	int					nWidth;						// ��Ƶ���
	int					nHeight;					// ��Ƶ�߶�
	CFG_BITRATE_CONTROL	emBitRateControl;			// ��������ģʽ
	int					nBitRate;					// ��Ƶ����(kbps)
	int					nFrameRate;					// ��Ƶ֡��
	int					nIFrameInterval;			// I֡���(1-100)������50��ʾÿ49��B֡��P֡������һ��I֡��
	CFG_IMAGE_QUALITY	emImageQuality;				// ͼ������
	int					nFrameType;					// ���ģʽ��0��DHAV��1��"PS"
} CFG_VIDEO_FORMAT;

// ��Ƶ�������
typedef struct tagCFG_VIDEOENC_OPT
{
	// ����
	bool				abVideoEnable;
	bool				abAudioEnable;
	bool				abSnapEnable;

	// ��Ϣ
	BOOL				bVideoEnable;				// ��Ƶʹ��
	CFG_VIDEO_FORMAT	stuVideoFormat;				// ��Ƶ��ʽ
	BOOL				bAudioEnable;				// ��Ƶʹ��
	BOOL				bSnapEnable;				// ��ʱץͼʹ��
} CFG_VIDEOENC_OPT;

// RGBA��Ϣ
typedef struct tagCFG_RGBA
{
	int					nRed;
	int					nGreen;
	int					nBlue;
	int					nAlpha;
} CFG_RGBA;

// ������Ϣ
typedef struct tagCFG_RECT
{
	int					nLeft;
	int					nTop;
    int					nRight;
    int					nBottom;				
} CFG_RECT;

// �ڵ���Ϣ
typedef struct tagCFG_COVER_INFO
{
	// ����
	bool				abBlockType;
	bool				abEncodeBlend;
	bool				abPreviewBlend;

	// ��Ϣ
	CFG_RECT			stuRect;					// ���ǵ���������
	CFG_RGBA			stuColor;					// ���ǵ���ɫ
	int					nBlockType;					// ���Ƿ�ʽ��0���ڿ飬1��������
	int					nEncodeBlend;				// ���뼶�ڵ���1����Ч��0������Ч
	int					nPreviewBlend;				// Ԥ���ڵ���1����Ч��0������Ч
} CFG_COVER_INFO;

// �������ڵ�����
typedef struct tagCFG_VIDEO_COVER
{
	int                 nTotalBlocks;						// ֧�ֵ��ڵ�����
	int					nCurBlocks;							// �����õĿ���
	CFG_COVER_INFO		stuCoverBlock[MAX_VIDEO_COVER_NUM];	// ���ǵ�����	
} CFG_VIDEO_COVER;

// OSD��Ϣ
typedef struct tagCFG_OSD_INFO
{
	// ����
	bool				abShowEnable;

	// ��Ϣ
	CFG_RGBA			stuFrontColor;				// ǰ����ɫ
	CFG_RGBA			stuBackColor;				// ������ɫ
	CFG_RECT			stuRect;					// ��������
	BOOL				bShowEnable;				// ��ʾʹ��
} CFG_OSD_INFO;

// ������ɫ����
typedef struct tagCFG_COLOR_INFO
{
	int					nBrightness;				// ����(0-100)
	int					nContrast;					// �Աȶ�(0-100)
	int					nSaturation;				// ���Ͷ�(0-100)
	int					nHue;						// ɫ��(0-100)
	int					nGain;						// ����(0-100)
	BOOL				bGainEn;					// ����ʹ��
} CFG_COLOR_INFO;

// ͼ��ͨ��������Ϣ
typedef struct tagCFG_ENCODE_INFO
{
	int                 nChannelID;							// ͨ����(0��ʼ)
	char				szChnName[MAX_CHANNELNAME_LEN];		// ͨ������
	CFG_VIDEOENC_OPT	stuMainStream[MAX_VIDEOSTREAM_NUM];	// ��������0����ͨ¼��1-����¼��2������¼��
	CFG_VIDEOENC_OPT	stuExtraStream[MAX_VIDEOSTREAM_NUM];// ��������0��������1��1��������2��2��������3
	CFG_VIDEOENC_OPT	stuSnapFormat[MAX_VIDEOSTREAM_NUM];	// ץͼ��0����ͨץͼ��1������ץͼ��2������ץͼ
	DWORD				dwCoverAbilityMask;					// �����ڸ��������룬��λ�ֱ��Ǳ���Ԥ����¼���������
	DWORD				dwCoverEnableMask;					// �����ڸ�ʹ�����룬��λ�ֱ��Ǳ���Ԥ����¼���������
	CFG_VIDEO_COVER		stuVideoCover;						// ���򸲸�
	CFG_OSD_INFO		stuChnTitle;						// ͨ������
	CFG_OSD_INFO		stuTimeTitle;						// ʱ�����
	CFG_COLOR_INFO		stuVideoColor;						// ������ɫ
} CFG_ENCODE_INFO;

// ˮӡ����
typedef struct tagCFG_WATERMARK_INFO 
{
	int                 nChannelID;					// ͨ����(0��ʼ)
	BOOL				bEnable;					// ʹ�ܿ���
	int					nStreamType;				// ��������(1��n)��0����������
	int					nDataType;					// �������ͣ�1�����֣�2��ͼƬ
	char				pData[MAX_WATERMARK_LEN];	// �ַ���ˮӡ����
} CFG_WATERMARK_INFO;

//-------------------------------¼������---------------------------------

// ʱ�����Ϣ
typedef struct tagCFG_TIME_SECTION 
{
	DWORD				dwRecordMask;						// ¼�����룬��λ�ֱ�Ϊ��̬���¼�񡢱���¼�񡢶�ʱ¼��Bit3~Bit15������Bit16��̬���ץͼ��Bit17����ץͼ��Bit18��ʱץͼ
	int					nBeginHour;
	int					nBeginMin;
	int					nBeginSec;
	int					nEndHour;
	int					nEndMin;
	int					nEndSec;	
} CFG_TIME_SECTION;

// ��ʱ¼��������Ϣ
typedef struct tagCFG_RECORD_INFO
{
	int                 nChannelID;					// ͨ����(0��ʼ)
	CFG_TIME_SECTION	stuTimeSection[WEEK_DAY_NUM][MAX_REC_TSECT]; // ʱ���
	int					nPreRecTime;				// Ԥ¼ʱ�䣬Ϊ��ʱ��ʾ�ر�(0~300)
	BOOL				bRedundancyEn;				// ¼�����࿪��
	int					nStreamType;				// 0����������1��������1��2��������2��3��������3
} CFG_RECORD_INFO;

//-------------------------------��������---------------------------------

// ��̨��������
typedef enum tagCFG_LINK_TYPE
{
	LINK_TYPE_NONE,									// ������
	LINK_TYPE_PRESET,								// ����Ԥ�õ�
	LINK_TYPE_TOUR,									// ����Ѳ��
	LINK_TYPE_PATTERN,								// �����켣
} CFG_LINK_TYPE;

// ������̨��Ϣ
typedef struct tagCFG_PTZ_LINK
{
	CFG_LINK_TYPE		emType;						// ��������
	int					nValue;						// ����ȡֵ�ֱ��ӦԤ�õ�ţ�Ѳ���ŵȵ�
} CFG_PTZ_LINK;

// ����������Ϣ
typedef struct tagCFG_ALARM_MSG_HANDLE
{
	//����
	bool				abRecordMask;
	bool				abRecordEnable;
	bool				abRecordLatch;
	bool				abAlarmOutMask;
	bool				abAlarmOutEn;
	bool				abAlarmOutLatch;	
	bool				abExAlarmOutMask;
	bool				abExAlarmOutEn;
	bool				abPtzLinkEn;
	bool				abTourMask;
	bool				abTourEnable;
	bool				abSnapshot;
	bool				abSnapshotEn;
	bool				abSnapshotPeriod;
	bool				abSnapshotTimes;
	bool				abTipEnable;
	bool				abMailEnable;
	bool				abMessageEnable;
	bool				abBeepEnable;
	bool				abVoiceEnable;
	bool				abMatrixMask;
	bool				abMatrixEnable;
	bool				abEventLatch;
	bool				abLogEnable;
	bool				abDelay;
	bool				abVideoMessageEn;
	bool				abMMSEnable;
	bool				abMessageToNetEn;

	//��Ϣ
	int					nChannelCount;								// �豸����Ƶͨ����
	int					nAlarmOutCount;								// �豸�ı����������
	DWORD				dwRecordMask[MAX_CHANNEL_NUM];				// ¼��ͨ������(��λ)
	BOOL				bRecordEnable;								// ¼��ʹ��
	int					nRecordLatch;								// ¼����ʱʱ��(��)
	DWORD				dwAlarmOutMask[MAX_CHANNEL_NUM];			// �������ͨ������
	BOOL				bAlarmOutEn;								// �������ʹ��
	int					nAlarmOutLatch;								// ���������ʱʱ��(��)
	DWORD				dwExAlarmOutMask[MAX_CHANNEL_NUM];			// ��չ�������ͨ������
	BOOL				bExAlarmOutEn;								// ��չ�������ʹ��
	CFG_PTZ_LINK		stuPtzLink[MAX_VIDEO_CHANNEL_NUM];			// ��̨������
	BOOL				bPtzLinkEn;									// ��̨����ʹ��
	DWORD				dwTourMask[MAX_CHANNEL_NUM];				// ��ѯͨ������
	BOOL				bTourEnable;								// ��ѯʹ��
	DWORD				dwSnapshot[MAX_CHANNEL_NUM];				// ����ͨ��������
	BOOL				bSnapshotEn;								// ����ʹ��
	int					nSnapshotPeriod;							// ��������(��)
	int					nSnapshotTimes;								// ���Ĵ���
	BOOL				bTipEnable;									// ������Ϣ����ʾ
	BOOL				bMailEnable;								// �����ʼ��������ͼƬ����Ϊ����
	BOOL				bMessageEnable;								// �ϴ�������������
	BOOL				bBeepEnable;								// ����
	BOOL				bVoiceEnable;								// ������ʾ
	DWORD				dwMatrixMask[MAX_CHANNEL_NUM];				// ������Ƶ����ͨ������
	BOOL				bMatrixEnable;								// ������Ƶ����
	int					nEventLatch;								// ������ʼ��ʱʱ��(��)��0��15
	BOOL				bLogEnable;									// �Ƿ��¼��־
	int					nDelay;										// ����ʱ����ʱ����Ч����λΪ��
	BOOL				bVideoMessageEn;							// ������ʾ��Ļ����Ƶ�����ӵ���Ļ�����¼����ͣ�ͨ���ţ����ʱ��
	BOOL				bMMSEnable;									// ���Ͳ���ʹ��
	BOOL				bMessageToNetEn;							// ��Ϣ�ϴ�������ʹ��
} CFG_ALARM_MSG_HANDLE;

// �ⲿ��������
typedef struct tagCFG_ALARMIN_INFO
{
	int					nChannelID;									// ����ͨ����(0��ʼ)
	BOOL				bEnable;									// ʹ�ܿ���
	char				szChnName[MAX_CHANNELNAME_LEN];				// ����ͨ������
	int					nAlarmType;									// ���������ͣ�0�����գ�1������
	CFG_ALARM_MSG_HANDLE stuEventHandler;							// ��������
	CFG_TIME_SECTION	stuTimeSection[WEEK_DAY_NUM][MAX_REC_TSECT];// �¼���Ӧʱ���
} CFG_ALARMIN_INFO;

// �������뱨������
typedef struct tagCFG_NETALARMIN_INFO 
{
	int					nChannelID;									// ����ͨ����(0��ʼ)
	BOOL				bEnable;									// ʹ�ܿ���
	char				szChnName[MAX_CHANNELNAME_LEN];				// ����ͨ������
	int					nAlarmType;									// ���������ͣ�0�����գ�1������
	CFG_ALARM_MSG_HANDLE stuEventHandler;							// ��������
	CFG_TIME_SECTION	stuTimeSection[WEEK_DAY_NUM][MAX_REC_TSECT];// �¼���Ӧʱ���
} CFG_NETALARMIN_INFO;

// ��̬��ⱨ������
typedef struct tagCFG_MOTION_INFO 
{
	int					nChannelID;									// ����ͨ����(0��ʼ)
	BOOL				bEnable;									// ʹ�ܿ���
	int					nSenseLevel;								// ������1��6
	int					nMotionRow;									// ��̬������������
	int					nMotionCol;									// ��̬������������
	BYTE				byRegion[MAX_MOTION_ROW][MAX_MOTION_COL];	// ����������32*32������
	CFG_ALARM_MSG_HANDLE stuEventHandler;							// ��������
	CFG_TIME_SECTION	stuTimeSection[WEEK_DAY_NUM][MAX_REC_TSECT];// �¼���Ӧʱ���
} CFG_MOTION_INFO;

// ��Ƶ��ʧ��������
typedef struct tagCFG_VIDEOLOST_INFO 
{
	int					nChannelID;									// ����ͨ����(0��ʼ)
	BOOL				bEnable;									// ʹ�ܿ���
	CFG_ALARM_MSG_HANDLE stuEventHandler;							// ��������
	CFG_TIME_SECTION	stuTimeSection[WEEK_DAY_NUM][MAX_REC_TSECT];// �¼���Ӧʱ���
} CFG_VIDEOLOST_INFO;

// ��Ƶ�ڵ���������
typedef struct tagCFG_SHELTER_INFO 
{
	int					nChannelID;									// ����ͨ����(0��ʼ)
	BOOL				bEnable;									// ʹ�ܿ���
	int					nSenseLevel;								// ������
	CFG_ALARM_MSG_HANDLE stuEventHandler;							// ��������
	CFG_TIME_SECTION	stuTimeSection[WEEK_DAY_NUM][MAX_REC_TSECT];// �¼���Ӧʱ���
} CFG_SHELTER_INFO;

// �޴洢�豸��������
typedef struct tagCFG_STORAGENOEXIST_INFO 
{
	BOOL				bEnable;						// ʹ�ܿ���
	CFG_ALARM_MSG_HANDLE stuEventHandler;				// ��������
} CFG_STORAGENOEXIST_INFO;

// �洢�豸���ʳ���������
typedef struct tagCFG_STORAGEFAILURE_INFO 
{
	BOOL				bEnable;						// ʹ�ܿ���
	CFG_ALARM_MSG_HANDLE stuEventHandler;				// ��������
} CFG_STORAGEFAILURE_INFO;

// �洢�豸�ռ䲻�㱨������
typedef struct tagCFG_STORAGELOWSAPCE_INFO 
{
	BOOL				bEnable;						// ʹ�ܿ���
	int					nLowerLimit;					// Ӳ��ʣ���������ޣ��ٷ���(0~99)
	CFG_ALARM_MSG_HANDLE stuEventHandler;				// ��������
} CFG_STORAGELOWSAPCE_INFO;

// ����Ͽ���������
typedef struct tagCFG_NETABORT_INFO 
{
	BOOL				bEnable;						// ʹ�ܿ���
	CFG_ALARM_MSG_HANDLE stuEventHandler;				// ��������
} CFG_NETABORT_INFO;

// IP��ͻ��������
typedef struct tagCFG_IPCONFLICT_INFO 
{
	BOOL				bEnable;						// ʹ�ܿ���
	CFG_ALARM_MSG_HANDLE stuEventHandler;				// ��������
} CFG_IPCONFLICT_INFO;

//-------------------------------ץͼ��������---------------------------------
// ץͼ��������
typedef struct tagCFG_SNAPCAPINFO_INFO  
{
	int					nChannelID;						// ץͼͨ����(0��ʼ)
	DWORD				dwIMageSizeNum;					// ֧�ֵķֱ�����Ϣ
	CFG_CAPTURE_SIZE	emIMageSizeList[MAX_IMAGESIZE_NUM];
	DWORD				dwFramesPerSecNum;				// ֧�ֵ�֡����Ϣ
	int					nFramesPerSecList[MAX_FPS_NUM];
	DWORD				dwQualityMun;					// ֧�ֵĻ�����Ϣ
	CFG_IMAGE_QUALITY	emQualityList[MAX_QUALITY_NUM];

	DWORD               dwMode;							// ģʽ,��λ����һλ����ʱ���ڶ�λ���ֶ���
	DWORD               dwFormat;						// ͼƬ��ʽģʽ,��λ����һλ��bmp���ڶ�λ��jpg��
} CFG_SNAPCAPINFO_INFO;


//-------------------------------����洢����������---------------------------------
// ����洢����������
typedef struct tagCFG_CHANNEL_TIME_SECTION 
{
	CFG_TIME_SECTION stuTimeSection[WEEK_DAY_NUM][MAX_NAS_TIME_SECTION];//�洢ʱ���
} CFG_CHANNEL_TIME_SECTION;

typedef struct tagCFG_NAS_INFO
{
	BOOL						bEnable;									// ʹ�ܿ���
	int							nVersion;									// ����洢�������汾0=�ϵ�FTP��1=NAS�洢
	int							nProtocol;									// Э������0=FTP 1=SMB
	char						szAddress[MAX_ADDRESS_LEN];					// IP��ַ��������
	int							nPort;										// �˿ں�
	char						szUserName[MAX_USERNAME_LEN];				// �ʻ���
	char						szPassword[MAX_PASSWORD_LEN];				// ����
	char						szDirectory[MAX_DIRECTORY_LEN];				// �����Ŀ¼��
	int							nFileLen;									// �ļ�����
	int							nInterval;									// �����ļ�ʱ����
	CFG_CHANNEL_TIME_SECTION	stuChnTime[MAX_VIDEO_CHANNEL_NUM];			// �洢ʱ���
} CFG_NAS_INFO;

//------------------------------��̨����--------------------------------------------
// Э����
typedef struct tagCFG_PRONAME
{
	char				name[MAX_NAME_LEN];			// Э����
} CFG_PRONAME;

// ���ڻ�������
typedef struct tagCFG_COMM_PROP
{
	BYTE				byDataBit;					// ����λ��0��5��1��6��2��7��3��8
	BYTE				byStopBit;					// ֹͣλ��0��1λ��1��1.5λ��2��2λ
	BYTE				byParity;					// У��λ��0����У�飬1����У�飻2��żУ��
	BYTE				byBaudRate;					// �����ʣ�0��300��1��600��2��1200��3��2400��4��4800��													// 5��9600��6��19200��7��38400��8��57600��9��115200
} CFG_COMM_PROP;

// ��̨����
typedef struct tagCFG_PTZ_INFO
{
	// ����
	bool				abMartixID;
	bool				abCamID;
	bool				abPTZType;

	// ��Ϣ
	int					nChannelID;								// ͨ����(0��ʼ)	
	CFG_PRONAME         stuDecProName[MAX_DECPRO_LIST_SIZE];	// Э�����б�(ֻ��)
	int					nProName;								// Э�����ͣ���Ӧ"Э�����б�"�±�
	int					nDecoderAddress;						// ��������ַ��0 - 255
	CFG_COMM_PROP		struComm;
	int					nMartixID;								// �����
	int					nPTZType;								// ��̨����0-���ݣ�������̨ 1-Զ��������̨
	int					nCamID;									// ����ͷID
} CFG_PTZ_INFO;
  

/************************************************************************
 ** �ӿڶ���
 ***********************************************************************/

// ������ѯ����������Ϣ
CLIENT_API BOOL  CALL_METHOD CLIENT_ParseData(char* szCommand, char* szInBuffer, LPVOID lpOutBuffer, DWORD dwOutBufferSize, void* pReserved);

// ���Ҫ���õ�������Ϣ
CLIENT_API BOOL  CALL_METHOD CLIENT_PacketData(char* szCommand, LPVOID lpInBuffer, DWORD dwInBufferSize, char* szOutBuffer, DWORD dwOutBufferSize);


#ifdef __cplusplus
}
#endif

#endif // DHCONFIGSDK_H

