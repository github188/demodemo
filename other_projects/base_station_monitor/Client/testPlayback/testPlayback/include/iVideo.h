/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * iVideo.h - E:\Project\resource\component\DevSDK\发布\iVideo\
 *
 * Copyright (C) 2010 QiYang Technologies, All Rights Reserved.
 *
 * $Id: iVideo.h 0001 2009-09-29 11:50:17 WuJunjie Exp $
 *
 * explain
 *     设备SDK接口
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
#define CLIENT_API  __declspec(dllimport)   //VC 用
#endif

#define CALLBACK __stdcall
#define CALL_METHOD  __stdcall  //__cdecl

#else	//linux

#define CLIENT_API	extern "C"
#define CALL_METHOD
#define CALLBACK

//#define RELEASE_HEADER	//发布头文件
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
#else	//内部编译
#include "../netsdk/osIndependent.h"
#endif

#endif


#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
//	查询类型
typedef enum
{
	ABILITY_WATERMARK_CFG = 17,			//水印配置能力
	ABILITY_WIRELESS_CFG = 18,			//wireless配置能力
	ABILITY_DEVALL_INFO = 26,			//设备的能力列表
	ABILITY_CARD_QUERY = 0x0100,		//卡号查询能力
	ABILITY_MULTIPLAY = 0x0101,			//多画面预览能力
	ABILITY_INFRARED = 0X0121,			//无线报警能力
} DH_SYS_ABILITY;

//////////////////////////////////////////////////////////////////////////
//	设备支持功能列表
enum 
{
	EN_FTP = 0,							//FTP 按位，1：传送录像文件 2：传送抓图文件
	EN_SMTP,							//SMTP 按位，1：报警传送文本邮件 2：报警传送图片
	EN_NTP,								//NTP	按位：1：调整系统时间
	EN_AUTO_MAINTAIN,					//自动维护 按位：1：重启 2：关闭 3:删除文件
	EN_VIDEO_COVER,						//区域遮挡 按位：1：多区域遮挡
	EN_AUTO_REGISTER,					//主动注册	按位：1：注册后sdk主动登陆
	EN_DHCP,							//DHCP	按位：1：DHCP
	EN_UPNP,							//UPNP	按位：1：UPNP
	EN_COMM_SNIFFER,					//串口抓包 按位：1:CommATM
	EN_NET_SNIFFER,						//网络抓包 按位： 1：NetSniffer
	EN_BURN,							//刻录功能 按位：1：查询刻录状态
	EN_VIDEO_MATRIX,					//视频矩阵 按位：1：是否支持视频矩阵
	EN_AUDIO_DETECT,					//音频检测 按位：1：是否支持音频检测
	EN_STORAGE_STATION,					//存储位置 按位：1：Ftp服务器(Ips) 2：U盘 3：NFS 4：DISK 5：SBM
	EN_IPSSEARCH,						//IPS存储查询 按位：1：IPS存储查询	
	EN_SNAP,							//抓图  按位：1：分辨率2：帧率3：抓图方式4：抓图文件格式5：图画质量
	EN_DEFAULTNIC,						//支持默认网卡查询 按位 1：支持
	EN_SHOWQUALITY,						//CBR模式下显示画质配置项 按位 1:支持
};

typedef struct 
{
	DWORD IsFucEnable[512];				//功能列表能力集,下标对应上述的枚举值,按位表示子功能
} DH_DEV_ENABLE_INFO;

//////////////////////////////////////////////////////////////////////////
//	卡号查询能力结构体
typedef struct 
{
	char		IsCardQueryEnable;
	char		iRev[3];
} DH_CARD_QUERY_EN;

//////////////////////////////////////////////////////////////////////////
//	wireless能力结构体
typedef struct 
{
	char		IsWirelessEnable;
	char		iRev[3];
} DH_WIRELESS_EN;

//////////////////////////////////////////////////////////////////////////
//	图象水印能力结构体
typedef struct 
{
	char		isSupportWM;	//1 支持；0 不支持
	char		supportWhat;	//0：文字水印；1：图片水印；2：同时支持文字水印和图片水印
	char		reserved[2];
} DH_WATERMAKE_EN;

//////////////////////////////////////////////////////////////////////////
//	多画面预览能力结构体
typedef struct  
{
	int			nEnable;			//1 支持；0 不支持
	DWORD		dwMultiPlayMask;	//保留
	char		reserved[4];
} DH_MULTIPLAY_EN;

typedef struct  
{
	BOOL bSupport;//是否支持
	int	 nAlarmInCount;//输入个数
	int  nAlarmOutCount;//输出个数
	int  nRemoteAddrCount;//遥控器个数
	BYTE reserved[32];
}DH_WIRELESS_ALARM_INFO;

/************************************************************************
 ** 常量定义
 ***********************************************************************/
#define DH_SERIALNO_LEN 		48		//设备序列号字符长度
#define DH_MAX_DISKNUM 			32		//最大硬盘个数
#define DH_MAX_SDCARDNUM		32		//最大SD卡个数
#define DH_MAX_BURNING_DEV_NUM	32		//最大刻录设备个数
#define DH_BURNING_DEV_NAMELEN	32		//刻录设备名字最大长度
#define DH_MAX_LINK 			6		
#define DH_MAX_CHANNUM 			16		//最大通道个数
#define DH_MAX_ALARMIN 			128		//最大报警输入个数
#define DH_MAX_ALARMOUT 		64		//最大报警输出个数

#define DH_MAX_RIGHT_NUM		100		//用户权限个数上限
#define DH_MAX_GROUP_NUM		20		//用户组个数上限
#define DH_MAX_USER_NUM			200		//用户个数上限
#define DH_RIGHT_NAME_LENGTH	32		//权限名长度
#define DH_USER_NAME_LENGTH		8		//用户名长度
#define DH_USER_PSW_LENGTH		8		//用户密码长度
#define DH_MEMO_LENGTH			32		//备注长度

#define DH_MAX_STRING_LEN		128

//侦听设备回调类型
#define DH_DVR_SERIAL_RETURN 1   //设备发送序列号回调
#define DH_DVR_DISCONNECT -1      //验证期间设备断线回调

//通道画面字符叠加相关常量
#define MAX_STRING_LINE_LEN	6		//最多六行
#define MAX_PER_STRING_LEN  20		//每行最大长度

//回调报警信息类型，对应CLIENT_StartListen
#define DH_COMM_ALARM			0x1100	//常规报警信息(包括外部报警，视频丢失，动态检测)
#define DH_SHELTER_ALARM		0x1101	//视频遮挡报警
#define DH_DISK_FULL_ALARM		0x1102	//硬盘满报警
#define DH_DISK_ERROR_ALARM		0x1103	//硬盘故障报警
#define DH_SOUND_DETECT_ALARM	0x1104	//音频检测报警
#define DH_ALARM_DECODER_ALARM	0x1105	//报警解码器报警

//扩展回调报警信息类型，对应CLIENT_StartListenEx
//各报警类型详细分开
#define DH_ALARM_ALARM_EX			0x2101	//外部报警
#define DH_MOTION_ALARM_EX			0x2102	//动态检测报警
#define DH_VIDEOLOST_ALARM_EX		0x2103	//视频丢失报警
#define DH_SHELTER_ALARM_EX			0x2104	//遮挡报警
#define DH_SOUND_DETECT_ALARM_EX	0x2105	//音频检测报警
#define DH_DISKFULL_ALARM_EX		0x2106	//硬盘满报警
#define DH_DISKERROR_ALARM_EX		0x2107	//坏硬盘报警
#define DH_ENCODER_ALARM_EX			0x210A	//编码器报警
#define DH_URGENCY_ALARM_EX			0x210B	//紧急报警
#define DH_WIRELESS_ALARM_EX		0x210C	//无线报警
#define DH_NEW_SOUND_DETECT_ALARM_EX 0x210D //新音频检测报警报警信息的结构体见DH_NEW_SOUND_ALARM_STATE;

#define DH_CONFIG_RESULT_EVENT_EX	0X3000  //修改配置的返回码返回结构见DEV_SET_RESULT
#define DH_REBOOT_EVENT_EX			0x3001	//设备重启事件
#define DH_AUTO_TALK_START_EX		0x3002  //设备主动开始语音对讲
#define DH_AUTO_TALK_STOP_EX		0x3003  //设备主动停止语音对讲

//查询设备状态接口参数：查询类型 用于CLIENT_QueryDevState参数
#define DH_DEVSTATE_COMM_ALARM		0x0001	//查询普通报警(包括外部报警，视频丢失，动态检测)
#define DH_DEVSTATE_SHELTER_ALARM	0x0002	//查询遮挡报警
#define DH_DEVSTATE_RECORDING		0x0003	//查询录象状态
#define DH_DEVSTATE_DISK			0x0004	//查询硬盘状态
#define DH_DEVSTATE_RESOURCE		0x0005	//系统资源状态
#define DH_DEVSTATE_BITRATE			0x0006	//获取通道码流
#define DH_DEVSTATE_CONN			0x0007	//获取设备连接状态
#define DH_DEVSTATE_PROTOCAL_VER	0x0008	//获取网络协议版本号,	pBuf = int*
#define DH_DEVSTATE_TALK_ECTYPE		0x0009	//获取设备支持的语音对讲格式列表，见结构体DHDEV_TALKFORMAT_LIST
#define DH_DEVSTATE_SD_CARD			0x000A	//获取SD卡信息（IPC类产品）
#define DH_DEVSTATE_BURNING_DEV			0x000B	//查询刻录机信息
#define DH_DEVSTATE_BURNING_PROGRESS	0x000C	//查询刻录进度
#define DH_DEVSTATE_PLATFORM		0x000D	//获取设备支持的接入平台
#define DH_DEVSTATE_CAMERA			0x000E	//获取摄像头属性信息，pBuf = DHDEV_CAMERA_INFO *，可以有多个结构体
#define DH_DEVSTATE_SOFTWARE		0x000F	//设备软件版本信息
#define DH_DEVSTATE_LANGUAGE        0x0010  //设备支持的语音种类
#define DH_DEVSTATE_DSP				0x0011	//DSP能力描述
#define	DH_DEVSTATE_OEM				0x0012	//OEM
#define	DH_DEVSTATE_NET				0x0013  //网络运行状态信息
#define DH_DEVSTATE_TYPE			0x0014  //设备类型
#define DH_DEVSTATE_SNAP			0x0015	 //抓图功能能力查询
//抓图视频编码类型
#define CODETYPE_MPEG4   0
#define CODETYPE_H264    1
#define CODETYPE_JPG     2

//错误类型代号，CLIENT_GetLastError函数的返回
#define _EC(x)						(0x80000000|x)
#define NET_NOERROR 				0				//没有错误
#define NET_ERROR					-1				//未知错误
#define NET_SYSTEM_ERROR			_EC(1)			//Windows系统出错
#define NET_NETWORK_ERROR			_EC(2)			//网络错误，可能是因为网络超时
#define NET_DEV_VER_NOMATCH			_EC(3)			//设备协议不匹配
#define NET_INVALID_HANDLE			_EC(4)			//句柄无效
#define NET_OPEN_CHANNEL_ERROR		_EC(5)			//打开通道失败
#define NET_CLOSE_CHANNEL_ERROR		_EC(6)			//关闭通道失败
#define NET_ILLEGAL_PARAM			_EC(7)			//用户参数不合法
#define NET_SDK_INIT_ERROR			_EC(8)			//SDK初始化出错
#define NET_SDK_UNINIT_ERROR		_EC(9)			//SDK清理出错
#define NET_RENDER_OPEN_ERROR		_EC(10)			//申请render资源出错
#define NET_DEC_OPEN_ERROR			_EC(11)			//打开解码库出错
#define NET_DEC_CLOSE_ERROR			_EC(12)			//关闭解码库出错
#define NET_MULTIPLAY_NOCHANNEL		_EC(13)			//多画面预览中检测到通道数为0
#define NET_TALK_INIT_ERROR			_EC(14)			//录音库初始化失败
#define NET_TALK_NOT_INIT			_EC(15)			//录音库未经初始化
#define	NET_TALK_SENDDATA_ERROR		_EC(16)			//发送音频数据出错
#define NET_REAL_ALREADY_SAVING		_EC(17)			//实时数据已经处于保存状态
#define NET_NOT_SAVING				_EC(18)			//未保存实时数据
#define NET_OPEN_FILE_ERROR			_EC(19)			//打开文件出错
#define NET_PTZ_SET_TIMER_ERROR		_EC(20)			//启动云台控制定时器失败
#define NET_RETURN_DATA_ERROR		_EC(21)			//对返回数据的校验出错
#define NET_INSUFFICIENT_BUFFER		_EC(22)			//没有足够的缓存
#define NET_NOT_SUPPORTED			_EC(23)			//当前SDK未支持该功能
#define NET_NO_RECORD_FOUND			_EC(24)			//查询不到录象
#define NET_NOT_AUTHORIZED			_EC(25)			//无操作权限
#define NET_NOT_NOW					_EC(26)			//暂时无法执行
#define NET_NO_TALK_CHANNEL			_EC(27)			//未发现对讲通道
#define NET_NO_AUDIO				_EC(28)			//未发现音频
#define NET_NO_INIT					_EC(29)			//CLientSDK未经初始化
#define NET_DOWNLOAD_END			_EC(30)			//下载已结束
#define NET_EMPTY_LIST				_EC(31)			//查询结果为空
 
#define NET_ERROR_GETCFG_SYSATTR	_EC(32)			//获取配置失败位置：系统属性
#define NET_ERROR_GETCFG_SERIAL		_EC(33)			//获取配置失败位置：序列号
#define NET_ERROR_GETCFG_GENERAL	_EC(34)			//获取配置失败位置：常规属性
#define NET_ERROR_GETCFG_DSPCAP		_EC(35)			//获取配置失败位置：DSP能力描述
#define NET_ERROR_GETCFG_NETCFG		_EC(36)			//获取配置失败位置：网络属性
#define NET_ERROR_GETCFG_CHANNAME	_EC(37)			//获取配置失败位置：通道名称
#define NET_ERROR_GETCFG_VIDEO		_EC(38)			//获取配置失败位置：视频属性
#define NET_ERROR_GETCFG_RECORD		_EC(39)			//获取配置失败位置：录象定时配置
#define NET_ERROR_GETCFG_PRONAME	_EC(40)			//获取配置失败位置：解码器协议名称
#define NET_ERROR_GETCFG_FUNCNAME	_EC(41)			//获取配置失败位置：232串口功能名称
#define NET_ERROR_GETCFG_485DECODER	_EC(42)			//获取配置失败位置：解码器属性
#define NET_ERROR_GETCFG_232COM		_EC(43)			//获取配置失败位置：232串口属性
#define NET_ERROR_GETCFG_ALARMIN	_EC(44)			//获取配置失败位置：外部报警输入属性
#define NET_ERROR_GETCFG_ALARMDET	_EC(45)			//获取配置失败位置：图像检测报警属性
#define NET_ERROR_GETCFG_SYSTIME	_EC(46)			//获取配置失败位置：设备时间
#define NET_ERROR_GETCFG_PREVIEW	_EC(47)			//获取配置失败位置：预览参数
#define NET_ERROR_GETCFG_AUTOMT		_EC(48)			//获取配置失败位置：自动维护配置
#define NET_ERROR_GETCFG_VIDEOMTRX	_EC(49)			//获取配置失败位置：视频矩阵配置
#define NET_ERROR_GETCFG_COVER		_EC(50)			//获取配置失败位置：视频区域遮挡
#define NET_ERROR_GETCFG_WATERMAKE	_EC(51)			//获取配置失败位置：图象水印配置
#define NET_ERROR_GETCFG_ETHERNET	_EC(300)		//获取配置失败位置：网卡配置
#define NET_ERROR_GETCFG_WLAN		_EC(301)		//获取配置失败位置：无线网络信息
#define NET_ERROR_GETCFG_WLANDEV	_EC(302)		//获取配置失败位置：搜索无线网络设备
#define NET_ERROR_GETCFG_REGISTER	_EC(303)		//获取配置失败位置：主动注册参数配置
#define NET_ERROR_GETCFG_CAMERA		_EC(304)		//获取配置失败位置：摄像头属性配置
#define NET_ERROR_GETCFG_INFRARED	_EC(305)		//获取配置失败位置：红外报警配置
#define NET_ERROR_GETCFG_SOUNDALARM	_EC(306)		//获取配置失败位置：音频报警配置
#define NET_ERROR_GETCFG_STORAGE    _EC(307)		//获取配置失败位置：存储位置配置
#define NET_ERROR_GETCFG_MAIL		_EC(308)		//获取邮件配置失败


#define NET_ERROR_SETCFG_GENERAL	_EC(55)			//设置配置失败位置：常规属性
#define NET_ERROR_SETCFG_NETCFG		_EC(56)			//设置配置失败位置：网络属性
#define NET_ERROR_SETCFG_CHANNAME	_EC(57)			//设置配置失败位置：通道名称
#define NET_ERROR_SETCFG_VIDEO		_EC(58)			//设置配置失败位置：视频属性
#define NET_ERROR_SETCFG_RECORD		_EC(59)			//设置配置失败位置：录象定时配置
#define NET_ERROR_SETCFG_485DECODER	_EC(60)			//设置配置失败位置：解码器属性
#define NET_ERROR_SETCFG_232COM		_EC(61)			//设置配置失败位置：232串口属性
#define NET_ERROR_SETCFG_ALARMIN	_EC(62)			//设置配置失败位置：外部报警输入属性
#define NET_ERROR_SETCFG_ALARMDET	_EC(63)			//设置配置失败位置：图像检测报警属性
#define NET_ERROR_SETCFG_SYSTIME	_EC(64)			//设置配置失败位置：设备时间
#define NET_ERROR_SETCFG_PREVIEW	_EC(65)			//设置配置失败位置：预览参数
#define NET_ERROR_SETCFG_AUTOMT		_EC(66)			//设置配置失败位置：自动维护配置
#define NET_ERROR_SETCFG_VIDEOMTRX	_EC(67)			//设置配置失败位置：视频矩阵配置
#define NET_ERROR_SETCFG_COVER		_EC(69)			//设置配置失败位置：视频区域遮挡
#define NET_ERROR_SETCFG_WATERMAKE	_EC(60)			//设置配置失败位置：图象水印配置
#define NET_ERROR_SETCFG_WLAN		_EC(61)			//设置配置失败位置：无线网络信息
#define NET_ERROR_SETCFG_WLANDEV	_EC(62)			//设置配置失败位置：选择无线网络设备
#define NET_ERROR_SETCFG_REGISTER	_EC(63)			//设置配置失败位置：主动注册参数配置
#define NET_ERROR_SETCFG_CAMERA		_EC(64)			//设置配置失败位置：摄像头属性配置
#define NET_ERROR_SETCFG_INFRARED	_EC(65)			//设置配置失败位置：红外报警配置
#define NET_ERROR_SETCFG_SOUNDALARM	_EC(66)			//设置配置失败位置：音频报警配置
#define NET_ERROR_SETCFG_STORAGE    _EC(67)			//设置配置失败位置：存储位置配置

#define NET_AUDIOENCODE_NOTINIT		_EC(70)			//音频编码接口没有成功初始化
#define NET_DATA_TOOLONGH			_EC(71)			//数据过长

#define NET_UNSUPPORTED				_EC(72)			//设备不支持该操作
#define NET_DEVICE_BUSY				_EC(73)			//设备资源不足
#define NET_SERVER_STARTED			_EC(74)			//服务器已经启动
#define NET_SERVER_STOPPED			_EC(75)			//服务器尚未成功启动

#define NET_LISTER_INCORRECT_SERIAL	_EC(80)			//输入序列号有误
#define NET_QUERY_DISKINFO_FAILED	_EC(81)			//获取硬盘信息失败

#define NET_LOGIN_ERROR_PASSWORD	_EC(100)		//密码不正确
#define NET_LOGIN_ERROR_USER		_EC(101)		//帐户不存在
#define NET_LOGIN_ERROR_TIMEOUT		_EC(102)		//等待登录返回超时
#define NET_LOGIN_ERROR_RELOGGIN	_EC(103)		//帐号已登录
#define NET_LOGIN_ERROR_LOCKED		_EC(104)		//帐号已被锁定
#define NET_LOGIN_ERROR_BLACKLIST	_EC(105)		//帐号已被列为黑名单
#define NET_LOGIN_ERROR_BUSY		_EC(106)		//资源不足，系统忙
#define NET_LOGIN_ERROR_CONNECT		_EC(107)		//连接主机失败"
#define NET_LOGIN_ERROR_NETWORK		_EC(108)		//网络连接失败"

#define NET_RENDER_SOUND_ON_ERROR	_EC(120)		//Render库打开音频出错
#define NET_RENDER_SOUND_OFF_ERROR	_EC(121)		//Render库关闭音频出错
#define NET_RENDER_SET_VOLUME_ERROR	_EC(122)		//Render库控制音量出错
#define NET_RENDER_ADJUST_ERROR		_EC(123)		//Render库设置画面参数出错
#define NET_RENDER_PAUSE_ERROR		_EC(124)		//Render库暂停播放出错
#define NET_RENDER_SNAP_ERROR		_EC(125)		//Render库抓图出错
#define NET_RENDER_STEP_ERROR		_EC(126)		//Render库步进出错
#define NET_RENDER_FRAMERATE_ERROR	_EC(127)		//Render库设置帧率出错

#define NET_GROUP_EXIST				_EC(140)		//组名已存在
#define	NET_GROUP_NOEXIST			_EC(141)		//组名不存在
#define NET_GROUP_RIGHTOVER			_EC(142)		//组的权限超出权限列表范围
#define NET_GROUP_HAVEUSER			_EC(143)		//组下有用户，不能删除
#define NET_GROUP_RIGHTUSE			_EC(144)		//组的某个权限被用户使用，不能出除
#define NET_GROUP_SAMENAME			_EC(145)		//新组名同已有组名重复
#define	NET_USER_EXIST				_EC(146)		//用户已存在
#define NET_USER_NOEXIST			_EC(147)		//用户不存在
#define NET_USER_RIGHTOVER			_EC(148)		//用户权限超出组权限
#define NET_USER_PWD				_EC(149)		//保留帐号，不容许修改密码
#define NET_USER_FLASEPWD			_EC(150)		//密码不正确
#define NET_USER_NOMATCHING			_EC(151)		//密码不匹配


#define NET_CONFIG_DEVBUSY			_EC(999)		//暂时无法设置
#define NET_CONFIG_DATAILLEGAL		_EC(1000)		//配置数据不合法

//远程配置接口CLIENT_GetDEVConfig,CLIENT_GetDEVConfig参数：命令定义
#define DH_DEV_DEVICECFG			1		//获取设备参数
#define DH_DEV_NETCFG				2		//获取网络参数
#define DH_DEV_CHANNELCFG			3		//获取通道配置－图像及压缩参数等
#define DH_DEV_PREVIEWCFG 			4		//获取预览参数
#define DH_DEV_RECORDCFG			5		//获取录像时间参数
#define DH_DEV_COMMCFG				6		//获取串口参数
#define DH_DEV_ALARMCFG 			7		//获取报警参数
#define DH_DEV_TIMECFG 				8		//获取DVR时间
#define DH_DEV_TALKCFG				9		//对讲参数
#define DH_DEV_AUTOMTCFG			10		//自动维护配置		
#define	DH_DEV_VEDIO_MARTIX			11		//本机矩阵控制策略配置	
#define DH_DEV_MULTI_DDNS			12		//多ddns服务器配置
#define DH_DEV_SNAP_CFG				13		//抓图相关配置
#define DH_DEV_WEB_URL_CFG			14		//HTTP路径配置
#define DH_DEV_FTP_PROTO_CFG		15		//FTP上传配置
#define DH_DEV_INTERVIDEO_CFG		16		//平台接入配置，此时channel参数代表平台类型，
											//channel=4  代表贝尔阿尔卡特
											//channel=10	 代表中兴力维
											//channel=11 代表U网通
#define DH_DEV_VIDEO_COVER			17		//区域遮挡配置
#define DH_DEV_TRANS_STRATEGY		18		//传输策略配置，画质优先\流畅性优先
#define DH_DEV_DOWNLOAD_STRATEGY	19		//录象下载策略配置，高速下载\普通下载
#define DH_DEV_WATERMAKE_CFG		20		//图象水印配置
#define DH_DEV_WLAN_CFG				21		//无线网络配置
#define DH_DEV_WLAN_DEVICE_CFG		22		//搜索无线设备配置
#define DH_DEV_REGISTER_CFG			23		//主动注册参数配置
#define DH_DEV_CAMERA_CFG			24		//摄像头属性配置
#define DH_DEV_INFRARED_CFG 		25		//获取红外报警参数
#define DH_DEV_SNIFFER_CFG			26		//Sniffer抓包配置
#define DH_DEV_MAIL_CFG				27		//邮件配置,建议以后采用此类型配置
#define DH_DEV_DNS_CFG				28		//DNS服务器配置
#define DH_DEV_NTP_CFG				29		//NTP配置
#define DH_DEV_AUDIO_DETECT_CFG		30		//音频检测配置
#define DH_DEV_STORAGE_STATION_CFG  31      //存储位置配置
#define DH_DEV_PTZ_OPT_CFG			32		//云台操作属性


#define DH_DEV_USER_END_CFG			1000

/////////////////////远程配置结构体相关常量///////////////////////
#define DH_MAX_MAIL_ADDR_LEN		128	//邮件发(收)人地址最大长度
#define DH_MAX_MAIL_SUBJECT_LEN		64	//邮件主题最大长度
#define DH_MAX_IPADDR_LEN			16	//IP地址字符串长度
#define DH_MACADDR_LEN				40	//MAC地址字符串长度
#define DH_MAX_URL_LEN				128	//URL字符串长度
#define DH_MAX_DEV_ID_LEN			48	//机器编号最大长度
#define	DH_MAX_HOST_NAMELEN			64  //32	//主机名字符串长度，
#define DH_MAX_HOST_PSWLEN			32	//长度
#define DH_MAX_NAME_LEN				16	//通用名字字符串长度
#define DH_MAX_ETHERNET_NUM			2	//以太网口最大个数
#define	DH_DEV_SERIALNO_LEN			48	//序列号字符串长度
#define DH_DEV_TYPE_LEN				32	//设备类型字符串长度
#define DH_N_WEEKS					7	//一周的天数	
#define DH_N_TSECT					6	//通用时间段个数
#define DH_N_REC_TSECT				6	//录像时间段个数
#define DH_N_COL_TSECT				2	//颜色时间段个数	
#define DH_CHAN_NAME_LEN			32	//通道名长度,dvr DSP能力限制，最多32字节		
#define DH_N_ENCODE_AUX				3	//扩展码流个数	
#define DH_N_TALK					1	//最多对讲通道个数
#define DH_N_COVERS					1	//遮挡区域个数	
#define DH_N_CHANNEL				16	//最大通道个数	
#define DH_N_ALARM_TSECT			2	//报警提示时间段个数
#define DH_MAX_ALARMOUT_NUM			16	//报警输出口个数上限
#define DH_MAX_VIDEO_IN_NUM			16	//视频输入口个数上限
#define DH_MAX_ALARM_IN_NUM			16	//报警输入口个数上限
#define DH_MAX_DISK_NUM				16	//硬盘个数上限，暂定为16
#define DH_MAX_DECODER_NUM			16	//解码器（485）个数上限	
#define DH_MAX_232FUNCS				10	//232串口功能个数上限
#define DH_MAX_232_NUM				2	//232串口个数上限
#define DH_MAX_DECPRO_LIST_SIZE		100	//解码器协议列表个数上限
#define DH_FTP_MAXDIRLEN			240	//FTP文件目录最大长度
#define DH_MATRIX_MAXOUT			16	//矩阵输出口最大个数
#define DH_TOUR_GROUP_NUM			6	//矩阵输出组最大个数
#define DH_MAX_DDNS_NUM				10	//设备支持的ddns服务器最大个数
#define DH_MAX_SERVER_TYPE_LEN		32	//ddns服务器类型 最大字符串长度
#define DH_MAX_DOMAIN_NAME_LEN		256	//ddns域名 最大字符串长度
#define DH_MAX_DDNS_ALIAS_LEN		32	//ddns服务器别名 最大字符串长度
#define DH_MOTION_ROW				32	//动态检测区域的行数
#define DH_MOTION_COL				32	//动态检测区域的列数
#define	DH_FTP_USERNAME_LEN			64  //FTP配置，用户名最大长度
#define	DH_FTP_PASSWORD_LEN			64  //FTP配置，密码最大长度
#define	DH_TIME_SECTION				2	//FTP配置，每天时间段个数
#define DH_FTP_MAX_PATH				240	//FTP配置，文件路径名最大长度
#define DH_INTERVIDEO_UCOM_CHANID	32	//平台接入配置，U网通通道ID
#define DH_INTERVIDEO_UCOM_DEVID	32	//平台接入配置，U网通设备ID
#define DH_INTERVIDEO_UCOM_REGPSW	16	//平台接入配置，U网通注册密码
#define DH_INTERVIDEO_UCOM_USERNAME	32	//平台接入配置，U网通用户名
#define DH_INTERVIDEO_UCOM_USERPSW	32	//平台接入配置，U网通密码
#define DH_INTERVIDEO_NSS_IP		32	//平台接入配置，中兴力维IP
#define DH_INTERVIDEO_NSS_SERIAL	32	//平台接入配置，中兴力维serial
#define DH_INTERVIDEO_NSS_USER		32	//平台接入配置，中兴力维user
#define DH_INTERVIDEO_NSS_PWD		50	//平台接入配置，中兴力维password

#define DH_MAX_VIDEO_COVER_NUM		16	//遮挡区域最大个数
#define DH_MAX_WATERMAKE_DATA		4096	//水印图片数据最大长度
#define DH_MAX_WATERMAKE_LETTER		128	//水印文字最大长度
#define DH_MAX_WLANDEVICE_NUM		10	//最多搜索出的无线设备个数
#define DH_MAX_ALARM_NAME			64	//地址长度
#define DH_MAX_REGISTER_SERVER_NUM	10	//主动注册服务器个数
#define DH_SNIFFER_FRAMEID_NUM		6   //6个FRAME ID 选项
#define DH_SNIFFER_CONTENT_NUM		4   //每个FRAME对应的4个抓包内容
#define DH_MAX_PROTOCOL_NAME_LENGTH 20
#define DH_SNIFFER_GROUP_NUM		4   //4组抓包设置
#define MAX_PATH_STOR              240  //远程目录的长度


//分辨率列表，用于与分辨率掩码进行与、或操作
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

//云台属性列表，用于与分辨率掩码进行与、或操作
//lowMask
#define PTZ_DIRECTION				0x00000001				/*!< 方向 */
#define PTZ_ZOOM					0x00000002	   			/*!< 变倍 */
#define PTZ_FOCUS					0x00000004				/*!< 聚焦 */
#define PTZ_IRIS					0x00000008				/*!< 光圈 */
#define PTZ_ALARM					0x00000010				/*!< 报警功能 */
#define PTZ_LIGHT					0x00000020				/*!< 灯光 */
#define PTZ_SETPRESET				0x00000040				/*!< 设置预置点 */
#define PTZ_CLEARPRESET				0x00000080				/*!< 清除预置点 */
#define PTZ_GOTOPRESET				0x00000100				/*!< 转至预置点 */
#define PTZ_AUTOPANON				0x00000200				/*!< 水平开始 */
#define PTZ_AUTOPANOFF				0x00000400				/*!< 水平结束 */
#define PTZ_SETLIMIT				0x00000800				/*!< 设置边界 */
#define PTZ_AUTOSCANON				0x00001000				/*!< 自动扫描开始 */
#define PTZ_AUTOSCANOFF				0x00002000				/*!< 自动扫描开停止 */
#define PTZ_ADDTOUR					0x00004000				/*!< 增加巡航点 */
#define PTZ_DELETETOUR				0x00008000				/*!< 删除巡航点 */
#define PTZ_STARTTOUR				0x00010000				/*!< 开始巡航 */
#define PTZ_STOPTOUR				0x00020000				/*!< 结束巡航 */
#define PTZ_CLEARTOUR				0x00040000				/*!< 删除巡航 */
#define PTZ_SETPATTERN				0x00080000				/*!< 设置模式 */
#define PTZ_STARTPATTERN			0x00100000				/*!< 开始模式 */
#define PTZ_STOPPATTERN				0x00200000				/*!< 停止模式 */
#define PTZ_CLEARPATTERN			0x00400000				/*!< 清除模式 */
#define PTZ_POSITION				0x00800000				/*!< 快速定位 */
#define PTZ_AUX						0x01000000				/*!< 辅助开关 */
#define PTZ_MENU					0x02000000				/*!< 球机菜单 */
#define PTZ_EXIT					0x04000000				/*!< 退出球机菜单 */
#define PTZ_ENTER					0x08000000				/*!< 确认 */
#define PTZ_ESC						0x10000000				/*!< 取消 */
#define PTZ_MENUUPDOWN				0x20000000				/*!< 菜单上下操作 */
#define PTZ_MENULEFTRIGHT			0x40000000				/*!< 菜单左右操作 */
#define PTZ_OPT_NUM					0x80000000				//*< 操作的个数 */

//hiMask
#define PTZ_DEV						0x00000001				/*!< 云台控制 */
#define PTZ_MATRIX					0x00000002				/*!< 矩阵控制 */

//编码模式列表，用于与编码模式掩码进行与、或操作
#define DH_CAPTURE_COMP_DIVX_MPEG4	0x00000001
#define DH_CAPTURE_COMP_MS_MPEG4 	0x00000002
#define DH_CAPTURE_COMP_MPEG2		0x00000004
#define DH_CAPTURE_COMP_MPEG1		0x00000008
#define DH_CAPTURE_COMP_H263		0x00000010
#define DH_CAPTURE_COMP_MJPG		0x00000020
#define DH_CAPTURE_COMP_FCC_MPEG4	0x00000040
#define DH_CAPTURE_COMP_H264		0x00000080

//码流控制列表
#define DH_CAPTURE_BRC_CBR			0
#define DH_CAPTURE_BRC_VBR			1
//#define DH_CAPTURE_BRC_MBR			2

//报警联动动作，用于与报警联动动作掩码进行与、或操作
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

//"恢复默认设置"掩码，多个设置可用或操作
#define DH_RESTORE_COMMON			0x00000001	//普通设置
#define DH_RESTORE_CODING			0x00000002	//编码设置
#define DH_RESTORE_VIDEO			0x00000004	//录像设置
#define DH_RESTORE_COMM				0x00000008	//串口设置
#define DH_RESTORE_NETWORK			0x00000010	//网络设置
#define DH_RESTORE_ALARM			0x00000020	//报警设置
#define DH_RESTORE_VIDEODETECT		0x00000040	//视频检测
#define DH_RESTORE_PTZ				0x00000080	//云台控制
#define DH_RESTORE_OUTPUTMODE		0x00000100	//输出模式
#define DH_RESTORE_CHANNELNAME		0x00000200	//通道名称
#define DH_RESTORE_ALL				0x80000000	//全部重置


////////////////end of远程配置结构体相关常量///////////////////////


/************************************************************************
 ** 枚举定义
 ***********************************************************************/
//设备类型
enum NET_DEVICE_TYPE 
{
	NET_PRODUCT_NONE = 0,
	NET_DVR_NONREALTIME_MACE, /*!< 非实时MACE */
	NET_DVR_NONREALTIME, /*!< 非实时 */
	NET_NVS_MPEG1, /*!< 网络视频服务器 */
	NET_DVR_MPEG1_2, /*!< MPEG1二路录像机 */
	NET_DVR_MPEG1_8, /*!< MPEG1八路录像机 */
	NET_DVR_MPEG4_8, /*!< MPEG4八路录像机 */
	NET_DVR_MPEG4_16, /*!< MPEG4十六路录像机 */
	NET_DVR_MPEG4_SX2, /*!< LB系列录像机 */
	NET_DVR_MEPG4_ST2, /*!< GB系列录像机 */
	NET_DVR_MEPG4_SH2, /*!< HB系列录像机 */
	NET_DVR_MPEG4_GBE, /*!< GBE系列录像机 */
	NET_DVR_MPEG4_NVSII, /*!< II代网络视频服务器 */
	NET_DVR_STD_NEW, /*!< 新标准配置协议 */
	NET_DVR_DDNS, /*DDNS服务器*/
	NET_DVR_ATM,  /* ATM机 */
	NET_NB_SERIAL, /* 二代非实时NB系列机器 */
	NET_LN_SERIAL, /* LN系列产品 */
	NET_BAV_SERIAL, /* BAV系列产品 */
	NET_SDIP_SERIAL, /* SDIP系列产品 */
	NET_IPC_SERIAL, /* IPC系列产品 */
	NET_NVS_B,		/* NVS B系列 */
	NET_NVS_C,		/* NVS H系列 */
	NET_NVS_S,		/* NVS S系列 */
	NET_NVS_E,		/* NVS E系列 */
	NET_DVR_NEW_PROTOCOL /*从QueryDevState中查询设备类型,以字符串格式*/
};

//通用云台控制命令
typedef enum _PTZ_ControlType
{
	DH_PTZ_UP_CONTROL = 0,				//上
	DH_PTZ_DOWN_CONTROL,			//下
	DH_PTZ_LEFT_CONTROL,			//左
	DH_PTZ_RIGHT_CONTROL,			//右
	DH_PTZ_ZOOM_ADD_CONTROL,		//变倍+
	DH_PTZ_ZOOM_DEC_CONTROL,		//变倍-
	DH_PTZ_FOCUS_ADD_CONTROL,		//调焦+
	DH_PTZ_FOCUS_DEC_CONTROL,		//调焦-
	DH_PTZ_APERTURE_ADD_CONTROL,	//光圈+
	DH_PTZ_APERTURE_DEC_CONTROL,   //光圈-
    DH_PTZ_POINT_MOVE_CONTROL,     //转至预置点
    DH_PTZ_POINT_SET_CONTROL,      //设置
    DH_PTZ_POINT_DEL_CONTROL,      //删除
    DH_PTZ_POINT_LOOP_CONTROL,     //点间巡航
    DH_PTZ_LAMP_CONTROL            //灯光雨刷
}DH_PTZ_ControlType;

//云台控制扩展命令
typedef enum _EXTPTZ_ControlType
{
	DH_EXTPTZ_LEFTTOP = 0X20,		//左上
	DH_EXTPTZ_RIGHTTOP ,			//右上
	DH_EXTPTZ_LEFTDOWN ,			//左下
	DH_EXTPTZ_RIGHTDOWN,			//右下
	DH_EXTPTZ_ADDTOLOOP,			//加入预置点到巡航	巡航线路	预置点值	
	DH_EXTPTZ_DELFROMLOOP,			//删除巡航中预置点	巡航线路	预置点值	
    DH_EXTPTZ_CLOSELOOP,			//清除巡航	巡航线路		
	DH_EXTPTZ_STARTPANCRUISE,		//开始水平旋转			
	DH_EXTPTZ_STOPPANCRUISE,		//停止水平旋转			
	DH_EXTPTZ_SETLEFTBORDER,		//设置左边界			
	DH_EXTPTZ_SETRIGHTBORDER,		//设置右边界			
	DH_EXTPTZ_STARTLINESCAN,		//开始线扫			
    DH_EXTPTZ_CLOSELINESCAN,		//停止线扫			
    DH_EXTPTZ_SETMODESTART,		//设置模式开始	模式线路		
    DH_EXTPTZ_SETMODESTOP,			//设置模式结束	模式线路		
	DH_EXTPTZ_RUNMODE,				//运行模式	模式线路		
	DH_EXTPTZ_STOPMODE,			//停止模式	模式线路		
	DH_EXTPTZ_DELETEMODE,			//清除模式	模式线路		
	DH_EXTPTZ_REVERSECOMM,			//翻转命令			
	DH_EXTPTZ_FASTGOTO,			//快速定位	水平坐标(8192)	垂直坐标(8192)	变倍(4)
	DH_EXTPTZ_AUXIOPEN,			//x34	辅助开关开	辅助点		
	DH_EXTPTZ_AUXICLOSE,			//0x35	辅助开关关	辅助点
	DH_EXTPTZ_OPENMENU = 0X36,		//打开球机菜单
	DH_EXTPTZ_CLOSEMENU ,			//关闭菜单
	DH_EXTPTZ_MENUOK,				//菜单确定
	DH_EXTPTZ_MENUCANCEL,			//菜单取消
	DH_EXTPTZ_MENUUP,				//菜单上
	DH_EXTPTZ_MENUDOWN,			//菜单下
	DH_EXTPTZ_MENULEFT,			//菜单左
	DH_EXTPTZ_MENURIGHT,			//菜单右
	DH_EXTPTZ_ALARMHANDLE = 0x40,	//报警联动云台 parm1:报警输入通道 parm2:报警联动类型1-预置点2-线扫3-巡航 parm3:联动值，如预置点号
	DH_EXTPTZ_MATRIXSWITCH = 0x41,	//矩阵切换 parm1:监视器号(视频输出号) parm2:(视频输入号) parm3:矩阵号
	DH_EXTPTZ_LIGHTCONTROL,			//灯光控制器
	DH_EXTPTZ_UP_TELE = 0x70,		//上 + TELE，param1=速度（1-8），下同
	DH_EXTPTZ_DOWN_TELE,			//下 + TELE
	DH_EXTPTZ_LEFT_TELE,			//左 + TELE
	DH_EXTPTZ_RIGHT_TELE,			//右 + TELE
	DH_EXTPTZ_LEFTUP_TELE,			//左上 + TELE
	DH_EXTPTZ_LEFTDOWN_TELE,		//左下 + TELE
	DH_EXTPTZ_TIGHTUP_TELE,			//右上 + TELE
	DH_EXTPTZ_RIGHTDOWN_TELE,		//右下 + TELE
	DH_EXTPTZ_UP_WIDE,				//上 + WIDE，param1=速度（1-8），下同
	DH_EXTPTZ_DOWN_WIDE,			//下 + WIDE
	DH_EXTPTZ_LEFT_WIDE,			//左 + WIDE
	DH_EXTPTZ_RIGHT_WIDE,			//右 + WIDE
	DH_EXTPTZ_LEFTUP_WIDE,			//左上 + WIDE
	DH_EXTPTZ_LEFTDOWN_WIDE,		//左下 + WIDE
	DH_EXTPTZ_TIGHTUP_WIDE,			//右上 + WIDE
	DH_EXTPTZ_RIGHTDOWN_WIDE,		//右下 + WIDE
	DH_EXTPTZ_TOTAL,				//最大命令值
}DH_EXTPTZ_ControlType;

//IO控制命令
typedef enum _IOTYPE
{
	DH_ALARMINPUT = 1,				//报警输入
	DH_ALARMOUTPUT = 2,			//报警输出
	
	DH_DECODER_ALARMOUT = 3,	//报警解码器输出
	DH_WIRELESS_ALARMOUT =5,    //无线报警输出
}DH_IOTYPE;

//查询日志扩展接口参数：设备日志类型
typedef enum _DH_LOG_QUERY_TYPE
{
	DHLOG_ALL = 0,		//所有日志
	DHLOG_SYSTEM,		//系统日志
	DHLOG_CONFIG,		//配置日志
	DHLOG_STORAGE,		//存储相关
	DHLOG_ALARM,		//报警日志
	DHLOG_RECORD,		//录象相关
	DHLOG_ACCOUNT,		//帐号相关
	DHLOG_CLEAR,		//清除日志
	DHLOG_PLAYBACK		//回放相关
}DH_LOG_QUERY_TYPE;

//返回的日志信息结构体－日志类型
typedef enum _DH_LOG_TYPE
{
	//system logs
	DH_LOG_REBOOT = 0x0000,
	DH_LOG_SHUT,
	//config logs
	DH_LOG_CONFSAVE = 0x0100,
	DH_LOG_CONFLOAD,
	//fs logs
	DH_LOG_FSERROR = 0x0200,   //文件系统错误
	DH_LOG_HDD_WERR,           //硬盘写错误
	DH_LOG_HDD_RERR,           //硬盘读错误
	DH_LOG_HDD_TYPE,           //设置硬盘类型
	DH_LOG_HDD_FORMAT,         //格式化硬盘
	//alarm logs
	DH_LOG_ALM_IN = 0x0300,
	DH_LOG_ALM_END = 0x0302,
	DH_LOG_INFRAREDALM_IN = 0x03a0, //无线报警输入
	DH_LOG_INFRAREDALM_END,       //无线报警结束
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

//接口CLIENT_QueryLogEx返回的日志信息结构体－日志类型
//条件：int nType = 1;参数reserved = &nType.
typedef enum _DH_NEWLOG_TYPE
{
	//system logs
	DH_NEWLOG_REBOOT	 = 0x0000,		//系统重启
	DH_NEWLOG_SHUT,						//系统关机
	
	//config logs
	DH_NEWLOG_CONFSAVE = 0x0100,
	DH_NEWLOG_CONFLOAD,
	
	//fs logs
	DH_NEWLOG_FSERROR	 = 0x0200,		//文件系统错误
	DH_NEWLOG_HDD_WERR,					//硬盘写错误
	DH_NEWLOG_HDD_RERR,					//硬盘读错误
	DH_NEWLOG_HDD_TYPE,					//设置硬盘类型
	DH_NEWLOG_HDD_FORMAT,				//格式化硬盘
	DH_NEWLOG_HDD_NOSPACE,				//当前工作盘空间不足
	
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
	DH_NEWLOG_SEARCHLOG,				//查询日志
	
	//record operation
	DH_NEWLOG_SEARCH	 = 0x0700,		//录像查询
	DH_NEWLOG_DOWNLOAD,					//录像下载
	DH_NEWLOG_PLAYBACK,					//录像回放
	DH_NEWLOG_BACKUP,					/*!< 备份录像文件 */
	DH_NEWLOG_BACKUPERROR,				/*!< 备份录像文件失败 */
	
	DH_NEWLOG_TYPE_NR = 8,		
}DH_NEWLOG_TYPE;


//实时预览扩展接口增加的参数：预览类型
typedef enum _RealPlayType
{
	DH_RType_Realplay = 0,	//实时预览
	DH_RType_Multiplay,	//多画面预览
		
	DH_RType_Realplay_0,	//实时监视-主码流，等同于DH_RType_Realplay
	DH_RType_Realplay_1,	//实时监视-从码流1
	DH_RType_Realplay_2,	//实时监视-从码流2
	DH_RType_Realplay_3,	//实时监视-从码流3
		
	DH_RType_Multiplay_1,	//多画面预览－1画面
	DH_RType_Multiplay_4,	//多画面预览－4画面
	DH_RType_Multiplay_8,	//多画面预览－8画面
	DH_RType_Multiplay_9,	//多画面预览－9画面
	DH_RType_Multiplay_16,	//多画面预览－16画面

	DH_RType_Multiplay_6,	//多画面预览－6画面
	DH_RType_Multiplay_12,	//多画面预览－12画面
} DH_RealPlayType;

//设备控制接口参数：控制类型定义
typedef enum _CtrlType
{
	DH_CTRL_REBOOT = 0,	//重启设备	
	DH_CTRL_SHUTDOWN,	//关闭设备
	DH_CTRL_DISK,		//硬盘管理
	//网络键盘
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
	//触发报警
	DH_TRIGGER_ALARM_IN = 100,
	DH_TRIGGER_ALARM_OUT,
	//矩阵控制
	DH_CTRL_MATRIX,
	//SD卡控制（ipc产品）参数同硬盘控制
	DH_CTRL_SDCARD,
	//刻录机控制
	DH_BURNING_START,		//开始刻录
	DH_BURNING_STOP,		//结束刻录

	DH_BURNING_ADDPWD,	//叠加密码(以0为结尾的字符串,最大长度8位)
	DH_BURNING_ADDHEAD,	//叠加片头(以0为结尾的字符串,最大长度1024字节,支持分行,行分隔符'\n')，
	DH_BURNING_ADDSIGN,	//叠加打点到刻录信息(参数无)
	DH_BURNING_ADDCURSTOMINFO,	//自定义叠加(以0为结尾的字符串,最大长度1024字节,支持分行,行分隔符'\n')

	//恢复设备的默认设置
	DH_CTRL_RESTOREDEFAULT,

	//设备触发DSP抓图
	DH_CTRL_CAPTURE_START,		//触发设备抓图

	//清除日志
	DH_CTRL_CLEARLOG,
	//无线报警
	DH_TRIGGER_ALARM_WIRELESS = 200,
}CtrlType;

//配置类型，目前使用该枚举的接口已淘汰，请不要使用
typedef enum _CFG_INDEX
{
    CFG_GENERAL =  0,		// 普通
	CFG_COMM   		,		// 串口
	CFG_NET    		,		// 网络
	CFG_RECORD 		,		// 录像
	CFG_CAPTURE		,		// 图像设置
	CFG_PTZ    		,		// 云台
	CFG_DETECT 		,		// 动态检测
	CFG_ALARM  		,		// 报警
	CFG_DISPLAY		,		// 显示
	CFG_RESERVED	,		// 保留，使类型连贯
	CFG_TITLE  		=10,	// 通道标题
	CFG_MAIL  		=11,	// 邮件功能
	CFG_EXCAPTURE	=12,	// 预览图像设置
	CFG_PPPOE		=13,	// pppoe设置
	CFG_DDNS		=14,	// DDNS设置
	CFG_SNIFFER		=15,	// 网络监视捕获设置
	CFG_DSPINFO		=16,	// 编码能力信息
	CFG_COLOR		=126,  //颜色配置信息
	CFG_ALL,				// 保留
} CFG_INDEX;

typedef enum _SNAP_TYPE
{
	SNAP_TYP_TIMING = 0,
	SNAP_TYP_ALARM,
	SNAP_TYP_NUM,
} SNAP_TYPE;

//分辨率枚举，供DH_DSP_ENCODECAP使用
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
	DH_TALK_CLIENT_MODE,		//设置客户端方式进行语音对讲
	DH_TALK_SERVER_MODE,		//设置服务器方式进行语音对讲
	DH_TALK_ENCODE_TYPE,		//设置语音对讲编码格式
	DH_ALARM_LISTEN_MODE        //设置报警订阅方式

} EM_USEDEV_MODE;

typedef enum __EM_UPGRADE_TYPE
{
	DH_UPGRADE_BIOS_TYPE=1,		//BIOS升级
	DH_UPGRADE_WEB_TYPE,		//WEB升级
	DH_UPGRADE_BOOT_YPE,		//BOOT升级
	DH_UPGRADE_CHARACTER_TYPE,	//汉字库
	DH_UPGRADE_LOGO_TYPE,		//LOGO
	DH_UPGRADE_EXE_TYPE,		//EXE 例如播放器等
} EM_UPGRADE_TYPE;

//语言种类
typedef enum __LANGUAGE_TYPE
{
	DH_LANGUAGE_ENGLISH,				//英文	
	DH_LANGUAGE_CHINESE_SIMPLIFIED,		//简体中文	
	DH_LANGUAGE_CHINESE_TRADITIONAL,	//繁体中文	
	DH_LANGUAGE_ITALIAN,				//意大利文	
	DH_LANGUAGE_SPANISH,				//西班牙文
	DH_LANGUAGE_JAPANESE,				//日文版	
	DH_LANGUAGE_RUSSIAN,				//俄文版		
	DH_LANGUAGE_FRENCH,					//法文版		
	DH_LANGUAGE_GERMAN, 				//德文版		
	DH_LANGUAGE_PORTUGUESE,				//葡萄牙语	
	DH_LANGUAGE_TURKEY,					//土尔其语	
	DH_LANGUAGE_POLISH,					//波兰语	
	DH_LANGUAGE_ROMANIAN,				//罗马尼亚	
	DH_LANGUAGE_HUNGARIAN,              //匈牙利语	
	DH_LANGUAGE_FINNISH,				//芬兰语	
	DH_LANGUAGE_ESTONIAN,				//爱沙尼亚语	
	DH_LANGUAGE_KOREAN,					//韩语	
	DH_LANGUAGE_FARSI,					//波斯语 	
	DH_LANGUAGE_DANSK,					//丹麦语
	DH_LANGUAGE_CZECHISH,				//捷克文
	DH_LANGUAGE_BULGARIA,				//保加利亚文
} DH_LANGUAGE_TYPE;

//语音对讲格式
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
 ** 结构体定义
 ***********************************************************************/
//时间
typedef struct 
{
	DWORD dwYear;		//年
	DWORD dwMonth;		//月
	DWORD dwDay;		//日
	DWORD dwHour;		//时
	DWORD dwMinute;		//分
	DWORD dwSecond;		//秒
} NET_TIME,*LPNET_TIME;

//设备信息
typedef struct {
	BYTE sSerialNumber[DH_SERIALNO_LEN];	//序列号
	BYTE byAlarmInPortNum;				//DVR报警输入个数
	BYTE byAlarmOutPortNum;				//DVR报警输出个数
	BYTE byDiskNum;						//DVR 硬盘个数
	BYTE byDVRType;						//DVR类型, 见枚举DHDEV_DEVICE_TYPE
	BYTE byChanNum;						//DVR 通道个数
} NET_DEVICEINFO, *LPNET_DEVICEINFO;

//普通报警信息
typedef struct {
	int channelcount;
	int alarminputcount;
	unsigned char alarm[16];			//外部报警
	unsigned char motiondection[16];	//动态检测
	unsigned char videolost[16];		//视频丢失
} NET_CLIENT_STATE;

//查询设备工作状态硬盘信息
typedef struct {
	DWORD dwVolume;		//硬盘的容量
	DWORD dwFreeSpace;	//硬盘的剩余空间
	DWORD dwStatus;		//硬盘的状态,0-休眠,1-活动,2-故障等
}NET_DEV_DISKSTATE,*LPNET_DEV_DISKSTATE;

//录像文件信息
typedef struct {
    unsigned int     ch;              //通道号
    char             filename[128];   //文件名
    unsigned int     size;            //文件长度
    NET_TIME         starttime;       //开始时间
    NET_TIME         endtime;         //结束时间
    unsigned int     driveno;         //磁盘号
    unsigned int     startcluster;    //起始簇号
	int				 nRecordFileType; //录象文件类型  0：普通录象；1：报警录象；2：移动检测；3：卡号录象
} NET_RECORDFILE_INFO, *LPNET_RECORDFILE_INFO;

//报警IO控制
typedef struct 
{
	unsigned short index;	//端口序号
	unsigned short state;	//端口状态
}ALARM_CONTROL;

typedef struct 
{
	int	decoderNo;	//报警解码器号,从0开始
	unsigned short  alarmChn; //报警输出口,从0开始
	unsigned short  alarmState; //报警输出状态,1-打开　0-关闭
}DECODER_ALARM_CONTROL;

///////////////////用户帐号管理相关结构体/////////////////////////////////
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
	DWORD		dwReusable;		//本用户是否允许复用 1-复用，0-不复用
	
}USER_INFO;

typedef struct _USER_GROUP_INFO
{
	DWORD		dwID;
	char		name[DH_USER_NAME_LENGTH];
	DWORD		dwRightNum;
	DWORD		rights[DH_MAX_RIGHT_NUM];
	char		memo[DH_MEMO_LENGTH];
}USER_GROUP_INFO;

//用户信息配置结构
typedef struct _USER_MANAGE_INFO
{
	DWORD				dwRightNum;
	OPR_RIGHT			rightList[DH_MAX_RIGHT_NUM];
	DWORD				dwGroupNum;
	USER_GROUP_INFO		groupList[DH_MAX_GROUP_NUM];
	DWORD				dwUserNum;
	USER_INFO			userList[DH_MAX_USER_NUM];
	DWORD				dwSpecial; //特殊信息， 1-支持用户复用，　0-不支持用户复用
}USER_MANAGE_INFO;

//////////////////////////////////////////////////////////////////////////////
///////////////////用户帐号管理扩展相关结构体/////////////////////////////////
#define DH_USER_NAME_LENGTH_EX  16  //用户名长度
#define DH_USER_PSW_LENGTH_EX   16  //密码
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
	DWORD   dwFouctionMask;//掩码
	
						   /* 掩码,包括
						    * 0x00000001 - 支持用户复用
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

//用户信息配置结构
typedef struct _USER_MANAGE_INFO_EX
{
	
	DWORD				dwRightNum;
	OPR_RIGHT_EX		rightList[DH_MAX_RIGHT_NUM];
	DWORD				dwGroupNum;
	USER_GROUP_INFO_EX  groupList[DH_MAX_GROUP_NUM];
	DWORD				dwUserNum;
	USER_INFO_EX		userList[DH_MAX_USER_NUM];
	DWORD				dwFouctionMask;//掩码    
							 /* 掩码,包括
							 * 0x00000001 - 支持用户复用
							 * 0x00000002 - 密码修改需要校验
							 */
	BYTE  byNameMaxLength;       //支持用户名的最大长度
	BYTE  byPSWMaxLength;       //支持的密码最大长度
	BYTE  byReserve[254];
}USER_MANAGE_INFO_EX;
///////////////////end of 用户帐号管理扩展相关结构体////////////////////////////
///////////////////////////////////////////////////////////////////////////////

////////////////////日志结构体相关///////////////////////////
//日志时间结构
typedef struct _DHDEVTIME
{
	DWORD second		:6;					//	秒	1-60		
	DWORD minute		:6;					//	分	1-60		
	DWORD hour			:5;					//	时	1-24		
	DWORD day			:5;					//	日	1-31		
	DWORD month			:4;					//	月	1-12		
	DWORD year			:6;					//	年	2000-2063	
}DHDEVTIME, *LPDHDEVTIME;

typedef struct _DH_LOG_ITEM
{
    DHDEVTIME time;					//日期
    unsigned short type;			//类型
    unsigned char reserved;			//保留
    unsigned char data;				//数据
    unsigned char context[8];		//内容
}DH_LOG_ITEM, *LPDH_LOG_ITEM;

//接口CLIENT_QueryLogEx返回的日志信息结构体
//条件：int nType = 1;参数reserved = &nType.
typedef struct _DH_NEWLOG_ITEM
{
	DHDEVTIME	time; 				//日期
	WORD	type;					//类型
	WORD	data;					//数据
	char  	szOperator[8]; 			//用户名
	BYTE	context[16];		    //内容	
}DH_NEWLOG_ITEM, *LPDH_NEWLOG_ITEM;

typedef struct _LOG_ITEM_RECORD
{
	DHDEVTIME	time;		//时间
	BYTE	channel;		//通道
	BYTE	type;		//录像类型
	BYTE	reserved[2];
}LOG_ITEM_RECORD, *LPLOG_ITEM_RECORD;
////////////////////end of 日志结构体相关///////////////////////////

//回调视频数据帧的帧参数结构体
typedef struct _tagVideoFrameParam
{
	BYTE encode;		//编码类型
	BYTE frametype;		//I = 0, P = 1, B = 2...
	BYTE format;		//PAL - 0, NTSC - 1
	BYTE size;			//CIF - 0, HD1 - 1, 2CIF - 2, D1 - 3, VGA - 4, QCIF - 5, QVGA - 6 ,
						//SVCD - 7,QQVGA - 8, SVGA - 9, XVGA - 10,WXGA - 11,SXGA - 12,WSXGA - 13,UXGA - 14,WUXGA - 15,
	DWORD fourcc;		//如果是H264编码则总为0，MPEG4这里总是填写FOURCC('X','V','I','D');
	DWORD reserved;		//保留
	NET_TIME struTime;	//时间信息
}tagVideoFrameParam;

//回调音频数据帧的帧参数结构体
typedef struct _tagCBPCMDataParam
{
	BYTE channels;		// 声道数
	BYTE samples;			// 采样 0 - 8000, 1 - 11025, 2 - 16000, 3 - 22050, 4 - 32000, 5 - 44100, 6 - 48000
	BYTE depth;			// 采样深度 取值8或者16等。直接表示
	BYTE param1;			// 0 - 指示无符号，1-指示有符号
	DWORD reserved;		// 保留
}tagCBPCMDataParam;

//通道画面字幕叠加的数据结构
typedef struct _DH_CHANNEL_OSDSTRING
{
	BOOL bEnable;	//使能
	DWORD dwPosition[MAX_STRING_LINE_LEN];	//各行字符的位置　用1-9整数表示，与小键盘位置对应
											//		7左上	8上		9右上
											//		4左		5中		6右
											//		1左下	2下		3右下
	char szStrings[MAX_STRING_LINE_LEN][MAX_PER_STRING_LEN]; //最多六行字符，每行最多20个字节
}DH_CHANNEL_OSDSTRING;

////////////////////设备控制接口的控制参数定义//////////////////////
typedef struct _DISKCTRL_PARAM
{
	DWORD dwSize;		//size of this structure, for version control
	int nIndex;			//disk index
	int ctrlType;		//control type. 
						// 0 - 清除数据, 1 - 设为读写盘, 2 - 设为只读盘
						// 3 - 设为冗余盘, 4 - 恢复错误, 5 - 设为快照盘
}DISKCTRL_PARAM;

typedef struct _ALARMCTRL_PARAM
{
	DWORD dwSize;
	int nAlarmNo;		//报警通道号，从0开始
	int nAction;		//0-触发报警，1-停止报警
}ALARMCTRL_PARAM;

typedef struct _MATRIXCTRL_PARAM
{
	DWORD dwSize;
	int nChannelNo;		//视频输入号，从0开始
	int nMatrixNo;		//矩阵输出号，从0开始
}MATRIXCTRL_PARAM;

typedef struct _BURNING_PARM
{
	int channelMask;	//通道号掩码，每位表示一个通道
	int	devMask;		//设备掩码，每位表示一台设备
}BURNNG_PARM;


////////////////////end of设备控制接口的控制参数定义//////////////////////

////////////////////查询设备状态的参数定义////////////////////////////////
typedef struct _DH_HARDDISK_STATE
{
	DWORD	dwDiskNum;		//硬盘个数
	NET_DEV_DISKSTATE	stDisks[DH_MAX_DISKNUM]; //各硬盘信息
}DH_HARDDISK_STATE,*LPDH_HARDDISK_STATE;

typedef DH_HARDDISK_STATE  DH_SDCARD_STATE;	//SD卡，数据结构同硬盘信息

typedef struct _NET_DEV_BURNING
{
	DWORD dwDriverType;	//刻录驱动器类型 0 -	DHFS, 1 - DISK, 2- CDRW
	DWORD dwBusType;	//总线类型　0- USB, 1-1394, 2-IDE
	DWORD dwTotalSpace;	//总容量(KB)
	DWORD dwRemainSpace;	//剩余容量(KB)
	BYTE  dwDriverName[DH_BURNING_DEV_NAMELEN]; //刻录驱动器名称
}NET_DEV_BURNING,*LPNET_DEV_BURNING;

typedef struct _DH_BURNING_DEVINFO
{
	DWORD	dwDevNum;		//刻录设备个数
	NET_DEV_BURNING	stDevs[DH_MAX_BURNING_DEV_NUM]; //各刻录设备信息
}DH_BURNING_DEVINFO, *LPDH_BURNING_DEVINFO;

typedef struct _DH_BURNING_PROGRESS
{
	BYTE	bBurning;		//旧刻录机表示是否正在刻录
							/*
							 * 新刻录机的值：
							 *0;   可以刻录
							 *1;   非光盘设备,设备类型不正确
							 *2;   没有有效的设备,即剩余空间为0
							 *3;   有其他光盘在刻录
							 *4;   设备为非空闲状态,即在备份、刻录或回放中
							 */
	BYTE	bRomType;		//才有盘片类型(新刻录机才有这个值)(0:大华文件系统；1：移动硬盘或u盘；2：光盘)
	BYTE	bOperateType;   //操作类型(新刻录机才有这个值)(0：空闲；1：正在备份中；2：正在刻录中；3：正在进行光盘回放)
	BYTE	bType;			//备份或刻录得过程状态(新刻录机才有这个值)(0：停止或结束 ；1：开始；2：出错；3：满;4：正在初始化)	
	//**************************
	NET_TIME stTimeStart;	//开始时间　
	DWORD	dwTimeElapse;	//已刻录时间(秒)
	DWORD	dwTotalSpace;	//光盘总容量
	DWORD	dwRemainSpace;	//光盘剩余容量
	DWORD	dwBurned;		//已刻录容量
	DWORD	dwStatus;		/*
							为了兼容老设备，保留起来的，新刻录机没有实际用处
							-1：表示新刻录机。如果不为-1，表示旧刻录机
							下面是老设备的刻录状态（老设备用的）如果不为-1，表示旧刻录机：
							0-正在初始化，
							1-正在刻录，以上各变量有效
							2-刻录出错
							3-盘满停止
							*/
}DH_BURNING_PROGRESS, *LPDH_BURNING_PROGRESS;

//设备支持语言结构体
typedef struct _DH_LANGUAGE_DEVINFO
{
	DWORD	dwLanguageNum;		    //支持语言个数
	BYTE    byLanguageType[252];    //枚举值,详见DH_LANGUAGE_TYPE
}DH_DEV_LANGUAGE_INFO, *LPDH_DEV_LANGUAGE_INFO;


typedef struct  
{
	DH_TALK_CODING_TYPE		encodeType;		//编码类型
	int						nAudioBit;		//用实际的值表示，如8位 则填值为8
	DWORD					dwSampleRate;	//采样率，如16k 则填值为16000
	char					reserved[64];
}DHDEV_TALKDECODE_INFO;

//设备支持的语音对讲格式
typedef struct 
{
	int						nSupportNum;	//个数
	DHDEV_TALKDECODE_INFO	type[64];		//编码格式
	char					reserved[64];
}DHDEV_TALKFORMAT_LIST;

////////////////////end of 查询设备状态的参数定义////////////////////////////////

//GPS信息结构体
typedef struct _GPS_Info
{
    NET_TIME revTime;        //定位时间
	char DvrSerial[50];      //设备序列号
    double longitude;        //经度
    double latidude;         //纬度
    double height;           //高度（米）
    double angle;            //方向角（正北方向为原点，顺时针为正）
    double speed;            //速度（公里/小时）
    WORD starCount;          //定位星数
    BOOL antennaState;       //天线状态(true 好，false 坏)
    BOOL orientationState;    //定位状态（true 定位，false 不定位）
}GPS_Info,*LPGPS_Info;

//抓图参数结构体
typedef struct _snap_param
{
	UINT Channel;				//抓图的通道
	UINT Quality;				//画质1~6, 此参数只针对车载DVR设备有效
	UINT ImageSize;				//画面大小 0:QCIF,1:CIF,2:D1, 此参数只针对车载DVR设备有效
	UINT mode;					//抓图模式 0：表示请求一帧; 1：表示定时发送请求; 2：表示连续请求,车载DVR设备有效。
	UINT InterSnap;				//时间单位秒 若mode=1表示定时发送请求时，此时间有效，并且只有车载DVR有效。
	UINT CmdSerial;				//请求序列号
	UINT Reserved[4];
}SNAP_PARAMS,*LPSNAP_PARAMS;

//网络触发设备抓图结构体
typedef struct __NET_SNAP_COMMANDINFO 
{
	char			szCardInfo[16];		//卡号信息
	char			reserved[64];		//保留
} NET_SNAP_COMMANDINFO, LPNET_SNAP_COMMANDINFO;

//协议信息
typedef struct 
{
	char            protocolname[12]; //协议名
	unsigned int	baudbase;	      //波特率
	unsigned char	databits; 	      //数据位
	unsigned char	stopbits;	      //停止位
	unsigned char	parity;		      //校验位
	unsigned char   reserve;
} PROTOCOL_INFO, *LPPROTOCOL_INFO;

//音频格式信息
typedef struct {
	BYTE	byFormatTag;				//编码类型，如0-PCM
	WORD	nChannels;				//声道数
	WORD	wBitsPerSample;			//采样深度			
	DWORD	nSamplesPerSec;			//采样率
} DH_AUDIO_FORMAT, *LPDH_AUDIO_FORMAT;

//查询设备工作状态通道信息:相关接口已淘汰，请不要使用
typedef struct {
	BYTE byRecordStatic;		//通道是否在录像,0-不录像,1-手动录像，2-自动录象
	BYTE bySignalStatic;		//连接的信号状态,0-正常,1-信号丢失
	BYTE byHardwareStatic;		//通道硬件状态,0-正常,1-异常,例如DSP死掉
	char reserve;
	DWORD dwBitRate;			//实际码率
	DWORD dwLinkNum;			//客户端连接的个数
	DWORD dwClientIP[DH_MAX_LINK];	//客户端的IP地址
}NET_DEV_CHANNELSTATE,*LPNET_DEV_CHANNELSTATE;

//查询设备工作状态:相关接口已淘汰，请不要使用
typedef struct{
	DWORD dwDeviceStatic; 	//设备的状态,0x00 正常,0x01 CPU占用过高,0x02 硬件错误
	NET_DEV_DISKSTATE  stHardDiskStatic[DH_MAX_DISKNUM]; 
	NET_DEV_CHANNELSTATE stChanStatic[DH_MAX_CHANNUM];//通道的状态
	BYTE  byAlarmInStatic[DH_MAX_ALARMIN]; //报警端口的状态,0-没有报警,1-有报警
	BYTE  byAlarmOutStatic[DH_MAX_ALARMOUT]; //报警输出端口的状态,0-没有输出,1-有报警输出
	DWORD  dwLocalDisplay;//本地显示状态,0-正常,1-不正常
} NET_DEV_WORKSTATE,*LPNET_DEV_WORKSTATE;


/************************************************************************
** 侦听服务器回调原形
***********************************************************************/
typedef int (CALLBACK *fServiceCallBack)(LONG lHandle, char *pIp, WORD wPort, LONG lCommand, void *pParam, DWORD dwParamLen, DWORD dwUserData);

/************************************************************************
 ** 服务器断开回调原形
 ***********************************************************************/
typedef void (CALLBACK *fDisConnect)(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);

//	断线重连成功回调函数
typedef void (CALLBACK *fHaveReConnect)(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);

/************************************************************************
 ** 实时预览回调原形
 ***********************************************************************/
typedef void(CALLBACK *fRealDataCallBack) (LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);

/************************************************************************
 ** 原始数据回调原形
 ***********************************************************************/
typedef int(CALLBACK *fDataCallBack) (LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);

/************************************************************************
 ** 消息（报警）回调原形
 ***********************************************************************/
typedef BOOL (CALLBACK *fMessCallBack)(LONG lCommand, LONG lLoginID, char *pBuf,
			DWORD dwBufLen, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);

/************************************************************************
 ** 屏幕叠加回调原形
 ***********************************************************************/
typedef void (CALLBACK *fDrawCallBack)(LONG lLoginID, LONG lPlayHandle, HDC hDC, DWORD dwUser);

/************************************************************************
 ** 实时预览回调原形-扩展
 ***********************************************************************/
typedef void(CALLBACK *fRealDataCallBackEx) (LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG param, DWORD dwUser);

/************************************************************************
 ** 回放录像进度回调原形
 ***********************************************************************/
typedef void(CALLBACK *fDownLoadPosCallBack) (LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, DWORD dwUser);

/************************************************************************
 ** 通过时间下载录像进度回调原形
 现在按时间下载没有使用这个回调函数，使用的是回放录像进度回调原形. //by linjianyan
 ***********************************************************************/
typedef void(CALLBACK *fTimeDownLoadPosCallBack) (LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, DWORD dwUser);

/************************************************************************
 ** 查询设备日志数据回调原形
 ***********************************************************************/
typedef void(CALLBACK *fLogDataCallBack) (LONG lLoginID, char *pBuffer, DWORD dwBufSize, DWORD nTotalSize, BOOL bEnd, DWORD dwUser);

/************************************************************************
 ** 透明串口回调原形
 ***********************************************************************/
typedef void(CALLBACK *fTransComCallBack) (LONG lLoginID, LONG lTransComChannel, char *pBuffer, DWORD dwBufSize, DWORD dwUser);

/************************************************************************
 ** 升级设备程序回调原形
 ***********************************************************************/
typedef void(CALLBACK *fUpgradeCallBack) (LONG lLoginID, LONG lUpgradechannel, int nTotalSize, int nSendSize, DWORD dwUser);

/************************************************************************
 ** 语音对讲的音频数据回调原形
 ***********************************************************************/
typedef void(CALLBACK *pfAudioDataCallBack) (LONG lTalkHandle, char *pDataBuf, DWORD dwBufSize, BYTE byAudioFlag, DWORD dwUser);

/***********************************************************************
** 抓图回调原形
***********************************************************************/
typedef void( CALLBACK *fSnapRev)(LONG lLoginID, BYTE *pBuf, UINT RevLen, UINT EncodeType, DWORD CmdSerial, DWORD dwUser);

/************************************************************************
 ** SDK初始化
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_Init(fDisConnect cbDisConnect, DWORD dwUser);

/************************************************************************
 ** SDK退出清理
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_Cleanup();

/************************************************************************
 ** 设置断线重连成功回调函数，设置后SDK内部断线自动重连
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_SetAutoReconnect(fHaveReConnect cbAutoConnect, DWORD dwUser);

/************************************************************************
 ** 返回函数执行失败代码
 ***********************************************************************/
CLIENT_API DWORD CALL_METHOD CLIENT_GetLastError(void);

/************************************************************************
 ** 设置连接服务器超时时间和尝试次数
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_SetConnectTime(int nWaitTime, int nTryTimes);


/************************************************************************
 ** 获取SDK的版本信息
 ***********************************************************************/
CLIENT_API DWORD CALL_METHOD CLIENT_GetSDKVersion();


/************************************************************************
 ** 向设备注册
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_Login(char *pchDVRIP, WORD wDVRPort, char *pchUserName, char *pchPassword, LPNET_DEVICEINFO lpDeviceInfo, int *error = 0);

//	nSpecCap = 0为TCP方式下的登入；nSpecCap = 2为主动注册的登入；nSpecCap = 3为组播方式下的登入；nSpecCap = 4为UDP方式下的登入
CLIENT_API LONG CALL_METHOD CLIENT_LoginEx(char *pchDVRIP, WORD wDVRPort, char *pchUserName, char *pchPassword, int nSpecCap, void* pCapParam, LPNET_DEVICEINFO lpDeviceInfo, int *error = 0);

/************************************************************************
 ** 向设备注销
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_Logout(LONG lLoginID);

/************************************************************************
 ** 
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_SetDVRMessCallBack(fMessCallBack cbMessage,DWORD dwUser);


/************************************************************************
 ** 设置语音对讲模式
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetDeviceMode(LONG lLoginID, EM_USEDEV_MODE emType, void* pValue);


/************************************************************************
 ** 开始帧听设备
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StartListen(LONG lLoginID);

//对应LB设备程序版本
CLIENT_API BOOL CALL_METHOD CLIENT_StartListenEx(LONG lLoginID);

/************************************************************************
 ** 停止帧听设备
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopListen(LONG lLoginID);

/************************************************************************
 ** 开始侦听服务器，侦听移动DVR连接
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_ListenServer(char* ip, WORD port, int nTimeout, fServiceCallBack cbListen, DWORD dwUserData);

/************************************************************************
 ** 停止侦听服务器，停止侦听移动DVR连接
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopListenServer(LONG lServerHandle);

/************************************************************************
 ** 响应设备的注册请求
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_ResponseDevReg(char *devSerial, char* ip, WORD port, BOOL bAccept);

/************************************************************************
 ** 启动服务端口
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_StartService(WORD wPort, char *pIp = NULL, fServiceCallBack pfscb = NULL, 
												DWORD dwTimeOut = 0xffffffff, DWORD dwUserData = 0);

/************************************************************************
 ** 停止服务端口
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopService(LONG lHandle);

/************************************************************************
 ** 设置屏幕叠加回调
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_RigisterDrawFun(fDrawCallBack cbDraw, DWORD dwUser);


/************************************************************************
 ** 开始实时预览
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_RealPlay(LONG lLoginID, int nChannelID, HWND hWnd);

CLIENT_API LONG CALL_METHOD CLIENT_RealPlayEx(LONG lLoginID, int nChannelID, HWND hWnd, DH_RealPlayType rType = DH_RType_Realplay);

CLIENT_API BOOL	CALL_METHOD CLIENT_AdjustFluency(LONG lRealHandle, int nLevel);

/************************************************************************
 ** 停止实时预览
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopRealPlay(LONG lRealHandle);

/************************************************************************
 ** 停止实时预览_扩展
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopRealPlayEx(LONG lRealHandle);

/************************************************************************
 ** 保存数据为文件
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SaveRealData(LONG lRealHandle, const char *pchFileName);


/************************************************************************
 ** 结束保存数据为文件
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopSaveRealData(LONG lRealHandle);


/************************************************************************
 ** 设置图像属性
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_ClientSetVideoEffect(LONG lPlayHandle, unsigned char nBrightness, unsigned char nContrast, unsigned char nHue, unsigned char nSaturation);


/************************************************************************
 ** 获取图像属性
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_ClientGetVideoEffect(LONG lPlayHandle, unsigned char *nBrightness, unsigned char *nContrast, unsigned char *nHue, unsigned char *nSaturation);


/************************************************************************
 ** 打开声音
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_OpenSound(LONG hPlayHandle);


/************************************************************************
 ** 关闭声音
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_CloseSound();



/************************************************************************
 ** 打开语音对讲
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_StartTalkEx(LONG lLoginID, pfAudioDataCallBack pfcb, DWORD dwUser);

/************************************************************************
 ** 设置语音对讲的音量
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetAudioClientVolume(LONG lTalkHandle, WORD wVolume);

/************************************************************************
 ** 发送语音数据到设备
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_TalkSendData(LONG lTalkHandle, char *pSendBuf, DWORD dwBufSize);

/************************************************************************
 ** 开始PC端录音
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_RecordStart();

/************************************************************************
 ** 结束PC端录音
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_RecordStop();

/************************************************************************
 ** 解码音频数据
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_AudioDec(char *pAudioDataBuf, DWORD dwBufSize);

/************************************************************************
 ** 停止语音对讲
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopTalkEx(LONG lTalkHandle);

/************************************************************************
 ** 添加设备到广播组
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_AudioBroadcastAddDev(LONG lLoginID);

/************************************************************************
 ** 从广播组中删除设备
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_AudioBroadcastDelDev(LONG lLoginID);

/************************************************************************
 ** 设置音量
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetVolume(LONG lPlayHandle, int nVolume);

/************************************************************************
 ** 音频编码--初始化(特定标准格式->自定义格式)
 ***********************************************************************/
CLIENT_API int  CALL_METHOD CLIENT_InitAudioEncode(DH_AUDIO_FORMAT aft);

/************************************************************************
 ** 音频编码--数据编码
 ***********************************************************************/
CLIENT_API int	CALL_METHOD	CLIENT_AudioEncode(LONG lTalkHandle, BYTE *lpInBuf, DWORD *lpInLen, BYTE *lpOutBuf, DWORD *lpOutLen);

/************************************************************************
 ** 音频编码--完成退出
 ***********************************************************************/
CLIENT_API int	CALL_METHOD	CLIENT_ReleaseAudioEncode();

/************************************************************************
 ** 设置实时预览回调
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetRealDataCallBack(LONG lRealHandle, fRealDataCallBack cbRealData, DWORD dwUser);

/************************************************************************
 ** 设置实时预览回调-扩展
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetRealDataCallBackEx(LONG lRealHandle, fRealDataCallBackEx cbRealData, DWORD dwUser, DWORD dwFlag);

/************************************************************************
 ** 抓图
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_CapturePicture(LONG hPlayHandle, const char *pchPicFileName);

/************************************************************************
 ** 查询某段时间内是否有录像
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryRecordTime(LONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char* pchCardid, BOOL *bResult, int waittime=1000);

/************************************************************************
 ** 通过时间查询录像文件
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryRecordFile(LONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char* pchCardid, LPNET_RECORDFILE_INFO nriFileinfo, int maxlen, int *filecount, int waittime=1000, BOOL bTime = FALSE);

/************************************************************************
 ** 回放录像文件
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByRecordFile(LONG lLoginID, LPNET_RECORDFILE_INFO lpRecordFile, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwUserData);

/************************************************************************
 ** 回放录像文件扩展_增加了数据回调参数
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByRecordFileEx(LONG lLoginID, LPNET_RECORDFILE_INFO lpRecordFile, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwPosUser, fDataCallBack fDownLoadDataCallBack, DWORD dwDataUser);

/************************************************************************
 ** 暂停录像回放
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_PausePlayBack(LONG lPlayHandle, BOOL bPause);

/************************************************************************
 ** 定位录像回放起始点
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SeekPlayBack(LONG lPlayHandle, unsigned int offsettime, unsigned int offsetbyte);

/************************************************************************
 ** 停止录像回放
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopPlayBack(LONG lPlayHandle);

/************************************************************************
 ** 步进录像回放
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StepPlayBack(LONG lPlayHandle, BOOL bStop);

/************************************************************************
 ** 快进录像回放
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_FastPlayBack(LONG lPlayHandle);

/************************************************************************
 ** 慢进录像回放
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SlowPlayBack(LONG lPlayHandle);

/************************************************************************
 ** 恢复正常回放速度
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_NormalPlayBack(LONG lPlayHandle);

/************************************************************************
 ** 设置回放帧速
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetFramePlayBack(LONG lPlayHandle, int framerate);

/************************************************************************
 ** 获取回放帧速
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_GetFramePlayBack(LONG lPlayHandle, int *fileframerate, int *playframerate);

/************************************************************************
 ** 获取回放OSD时间
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_GetPlayBackOsdTime(LONG lPlayHandle, LPNET_TIME lpOsdTime, LPNET_TIME lpStartTime, LPNET_TIME lpEndTime);

/************************************************************************
 ** 下载录像文件
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_DownloadByRecordFile(LONG lLoginID,LPNET_RECORDFILE_INFO lpRecordFile, char *sSavedFileName, fDownLoadPosCallBack cbDownLoadPos, DWORD dwUserData);

/************************************************************************
 ** 通过时间下载录像
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_DownloadByTime(LONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char *sSavedFileName, fTimeDownLoadPosCallBack cbTimeDownLoadPos, DWORD dwUserData);

/************************************************************************
 ** 停止录像下载
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopDownload(LONG lFileHandle);

/************************************************************************
 ** 查询录像下载进度
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_GetDownloadPos(LONG lFileHandle, int *nTotalSize, int *nDownLoadSize);

/************************************************************************
 ** 多画面实时预览
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_MultiPlay(LONG lLoginID, HWND hWnd);

/************************************************************************
 ** 停止多画面实时预览
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopMultiPlay(LONG lMultiHandle);

/************************************************************************
 ** 查询系统服务器配置 －该接口已淘汰，请使用CLIENT_GetDevConfig
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryConfig(LONG lLoginID, int nConfigType, char *pConfigbuf, int maxlen, int *nConfigbuflen, int waittime=1000);

/************************************************************************
 ** 设置系统服务器配置 －该接口已淘汰，请使用CLIENT_SetDevConfig
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetupConfig(LONG lLoginID, int nConfigType, char *pConfigbuf, int nConfigbuflen, int waittime=1000);

/************************************************************************
 ** 查询设备通道名称
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryChannelName(LONG lLoginID, char *pChannelName, int maxlen, int *nChannelCount, int waittime=1000);

/************************************************************************
 ** 设置设备通道名称
 ***********************************************************************/
CLIENT_API BOOL  CALL_METHOD CLIENT_SetupChannelName(LONG lLoginID,char *pbuf, int nbuflen);

/************************************************************************
 ** 设置设备通道字符叠加
 ***********************************************************************/
CLIENT_API BOOL  CALL_METHOD CLIENT_SetupChannelOsdString(LONG lLoginID, int nChannelNo, DH_CHANNEL_OSDSTRING* struOsdString, int nbuflen);

/************************************************************************
 ** 通用云台控制
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_PTZControl(LONG lLoginID, int nChannelID, DWORD dwPTZCommand, DWORD dwStep, BOOL dwStop);

/************************************************************************
 ** 大华云台控制
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_DHPTZControl(LONG lLoginID, int nChannelID, DWORD dwPTZCommand, 
                                    unsigned char param1, unsigned char param2, unsigned char param3, BOOL dwStop);

/************************************************************************
 ** 大华云台控制扩展接口，支持三维快速定位
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_DHPTZControlEx(LONG lLoginID, int nChannelID, DWORD dwPTZCommand, 
                                    LONG lParam1, LONG lParam2, LONG lParam3, BOOL dwStop);

/************************************************************************
 ** 异步查询设备日志
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryLogCallback(LONG lLoginID, fLogDataCallBack cbLogData, DWORD dwUser);

/************************************************************************
 ** 同步查询设备日志
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryLog(LONG lLoginID, char *pLogBuffer, int maxlen, int *nLogBufferlen, int waittime=3000);

CLIENT_API BOOL CALL_METHOD CLIENT_QueryLogEx(LONG lLoginID, DH_LOG_QUERY_TYPE logType, char *pLogBuffer, int maxlen, int *nLogBufferlen, void* reserved, int waittime=3000);

/************************************************************************
 ** 创建透明串口通道
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_CreateTransComChannel(LONG lLoginID, int TransComType, 
                                      unsigned int baudrate, unsigned int databits,
                                      unsigned int stopbits, unsigned int parity,
                                      fTransComCallBack cbTransCom, DWORD dwUser);

/************************************************************************
 ** 透明串口发送数据
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SendTransComData(LONG lTransComChannel, char *pBuffer, DWORD dwBufSize);

/************************************************************************
 ** 释放通明串口通道
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_DestroyTransComChannel(LONG lTransComChannel);

/************************************************************************
 ** 该接口已淘汰，请不要使用
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_Reset(LONG lLoginID, BOOL bReset);

/************************************************************************
 ** 开始升级设备程序
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_StartUpgrade(LONG lLoginID, char *pchFileName, fUpgradeCallBack cbUpgrade, DWORD dwUser);

CLIENT_API LONG CALL_METHOD CLIENT_StartUpgradeEx(LONG lLoginID, EM_UPGRADE_TYPE emType, char *pchFileName, fUpgradeCallBack cbUpgrade, DWORD dwUser);


/************************************************************************
 ** 发送数据
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SendUpgrade(LONG lUpgradeID);

/************************************************************************
 ** 结束升级设备程序
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopUpgrade(LONG lUpgradeID);

/************************************************************************
 ** 查道通道录像状态
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryRecordState(LONG lLoginID, char *pRSBuffer, int maxlen, int *nRSBufferlen, int waittime=1000);

/************************************************************************
 ** 设置通道录像状态
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetupRecordState(LONG lLoginID, char *pRSBuffer, int nRSBufferlen);

/************************************************************************
 ** 查询设备当前时间
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryDeviceTime(LONG lLoginID, LPNET_TIME pDeviceTime, int waittime=1000);

/************************************************************************
 ** 设置设备当前时间
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetupDeviceTime(LONG lLoginID, LPNET_TIME pDeviceTime);

/************************************************************************
 ** 查询串口协议 － 该接口已淘汰，请使用CLIENT_GetDevConfig
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryComProtocol(LONG lLoginID, int nProtocolType, char *pProtocolBuffer, int maxlen, int *nProtocollen, int waittime=1000);

/************************************************************************
 ** 查询系统信息 － 该接口已淘汰，请使用CLIENT_GetDevConfig
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QuerySystemInfo(LONG lLoginID, int nSystemType, char *pSysInfoBuffer, int maxlen, int *nSysInfolen, int waittime=1000);


/************************************************************************
建议以后用扩展接口(有些设备用户名最大长度是16个字节的)
*************************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryUserInfoEx(LONG lLoginID, USER_MANAGE_INFO_EX *info, int waittime=1000);

/************************************************************************
 ** 查询设备用户(老接口。支持8位用户名长度。)
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryUserInfo(LONG lLoginID, USER_MANAGE_INFO *info, int waittime=1000);

/************************************************************************
 ** 操作设备用户
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_OperateUserInfo(LONG lLoginID, int nOperateType, void *opParam, void *subParam, int waittime=1000);
/************************************************************************
 ** 操作设备用户（新接口，支持8位和16位的用户名密码长度）。
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_OperateUserInfoEx(LONG lLoginID, int nOperateType, void *opParam, void *subParam, int waittime=1000);

/************************************************************************
 ** 开始对讲 － 该接口已淘汰，请使用CLIENT_StartTalkEx
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StartTalk(LONG lRealHandle, BOOL bCustomSend=false);

/************************************************************************
 ** 结束对讲 － 该接口已淘汰，请使用CLIENT_StopTalkEx
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_StopTalk(LONG lRealHandle);

/************************************************************************
 ** 发送自定义对讲数据 － 该接口已淘汰，请使用CLIENT_TalkSendData
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SendTalkData_Custom(LONG lRealHandle, char *pBuffer, DWORD dwBufSize);

/************************************************************************
 ** 获取通道码流
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_GetStatiscFlux(LONG lLoginID, LONG lPlayHandle);

/************************************************************************
 ** 查询IO状态
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_QueryIOControlState(LONG lLoginID, DH_IOTYPE emType, 
                                           void *pState, int maxlen, int *nIOCount, int waittime=1000);

/************************************************************************
 ** IO控制
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_IOControl(LONG lLoginID, DH_IOTYPE emType, void *pState, int maxlen);

/************************************************************************
 ** 查询设备工作状态 该接口已淘汰，请使用CLIENT_QueryDevState
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_GetDEVWorkState(LONG lLoginID, LPNET_DEV_WORKSTATE lpWorkState, int waittime=1000);

/************************************************************************
 ** 强制I帧
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_MakeKeyFrame(LONG lLoginID, int nChannelID, int nSubChannel=0);

/************************************************************************
 ** 设置设备限制码流
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetMaxFlux(LONG lLoginID, WORD wFlux);

/************************************************************************
 ** 通过设备名字或者设备序列号向DDNS服务器查询设备IP
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_GetDVRIPByResolveSvr(char *pchDVRIP, WORD wDVRPort, 
                                            BYTE *sDVRName, WORD wDVRNameLen, 
                                            BYTE *sDVRSerialNumber, WORD wDVRSerialLen, 
                                            char* sGetIP);

/************************************************************************
 ** 通过时间回放录像
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByTime(LONG lLoginID, int nChannelID, 
                                      LPNET_TIME lpStartTime, LPNET_TIME lpStopTime, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwPosUser);

/************************************************************************
 ** 通过时间回放录像扩展_增加了数据回调参数
 ***********************************************************************/
CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByTimeEx(LONG lLoginID, int nChannelID, 
                                      LPNET_TIME lpStartTime, LPNET_TIME lpStopTime, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwPosUser,
									  fDataCallBack fDownLoadDataCallBack, DWORD dwDataUser);

/************************************************************************
 ** 开始查找录像文件
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
 ** 查找录像文件
 ***********************************************************************/
CLIENT_API int	CALL_METHOD CLIENT_FindNextFile(LONG lFindHandle,LPNET_RECORDFILE_INFO lpFindData);

/************************************************************************
 ** 结束录像文件查找
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_FindClose(LONG lFindHandle);

/************************************************************************
 ** 重启设备：返回值 TRUE 成功， FALSE 失败
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_RebootDev(LONG lLoginID);

/************************************************************************
 ** 关闭设备：返回值 TRUE 成功， FALSE 失败
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_ShutDownDev(LONG lLoginID);

/************************************************************************
 ** 设备控制：返回值 TRUE 成功， FALSE 失败
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_ControlDevice(LONG lLoginID, CtrlType type, void *param, int waittime = 1000);

/************************************************************************
 ** 主动查询设备状态，包括报警、硬盘、录象状态等。
 ***********************************************************************/
//	当 nType  = DH_DEVSTATE_BURNING_PROGRESS 时,*pRetLen表示查询设备的id号.这样做的原因是没有其它可以使用的入口参数。
CLIENT_API BOOL CALL_METHOD CLIENT_QueryDevState(LONG lLoginID, int nType, char *pBuf, int nBufLen, int *pRetLen, int waittime=1000);

///////////////以下是车载dvr相关的功能接口 /////////////////////////
/************************************************************************
 ** 设置抓图回调函数
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_SetSnapRevCallBack(fSnapRev OnSnapRevMessage, DWORD dwUser);

/************************************************************************
 ** 抓图请求
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SnapPicture(LONG lLoginID, SNAP_PARAMS par);

/************************************************************************
 ** GPS信息订阅回调
 ***********************************************************************/
typedef void( CALLBACK *fGPSRev)(LONG lLoginID,           //设备句柄
                                 GPS_Info  GpsInfo,       //GPS信息
                                 DWORD dwUserData);       //用户数据

/************************************************************************
 ** 设置GPS订阅回调函数
 ***********************************************************************/
CLIENT_API void CALL_METHOD CLIENT_SetSubcribeGPSCallBack(fGPSRev OnGPSMessage, DWORD dwUser);

/************************************************************************
 ** GPS信息订阅
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SubcribeGPS (
									LONG lLoginID,        //设备句柄
									BOOL bStart,		  //是否启动
                                    LONG KeepTime,        //订阅持续时间（单位秒）
                                    LONG InterTime);      //订阅时间内GPS发送频率（单位秒）  

/////////////////////////end of 车载dvr相关的功能 ///////////////////////////////


/////////////////////////未实现//////////////////////////
/************************************************************************
 ** 设置实时预览缓冲大小
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_SetPlayerBufNumber(LONG lRealHandle, DWORD dwBufNum);

/************************************************************************
 ** 通过时间下载文件，CLIENT_DownloadByTime已实现该功能
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_GetFileByTime(LONG lLoginID, int nChannelID, 
                                     LPNET_TIME lpStartTime, LPNET_TIME lpStopTime, char *sSavedFileName);

/************************************************************************
 ** 网络回放控制，已细化为各控制接口了
 ***********************************************************************/
CLIENT_API BOOL CALL_METHOD CLIENT_PlayBackControl(LONG lPlayHandle, 
                                       DWORD dwControlCode, DWORD dwInValue, DWORD *lpOutValue);

//////////////////////////end of 未实现//////////////////////


/************************************************************************
** 远程配置设备接口 配置类型见DH_DEV_DEVICECFG 各宏
***********************************************************************/
CLIENT_API BOOL  CALL_METHOD CLIENT_GetDevConfig(LONG lLoginID, DWORD dwCommand, LONG lChannel, LPVOID lpOutBuffer, DWORD dwOutBufferSize, LPDWORD lpBytesReturned,int waittime=500);
CLIENT_API BOOL  CALL_METHOD CLIENT_SetDevConfig(LONG lLoginID, DWORD dwCommand, LONG lChannel, LPVOID lpInBuffer, DWORD dwInBufferSize, int waittime=500);


/************************************************************************
 ** 平台接入接口
 ***********************************************************************/
CLIENT_API BOOL  CALL_METHOD CLIENT_GetPlatFormInfo(LONG lLoginID, DWORD dwCommand, int nSubCommand, int nParam, LPVOID lpOutBuffer, DWORD dwOutBufferSize, LPDWORD lpBytesReturned,int waittime=500);
CLIENT_API BOOL  CALL_METHOD CLIENT_SetPlatFormInfo(LONG lLoginID, DWORD dwCommand, int nSubCommand, int nParam, LPVOID lpInBuffer, DWORD dwInBufferSize, int waittime=500);

/************************************************************************
**  获取云台信息	
***********************************************************************/
CLIENT_API BOOL  CALL_METHOD CLIENT_GetPtzOptAttr(LONG lLoginID,DWORD dwProtocolIndex,LPVOID lpOutBuffer,DWORD dwBufLen,DWORD *lpBytesReturned,int waittime=500);

/******************************************************************************
** 以下是各部分配置结构体
 *****************************************************************************/

/************************************************************************
 *	系统信息
************************************************************************/

/*版本信息:高16位表示主版本号，低16位表示次版本号
 *关于时间的数据组织是：yyyymmdd */
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

/* DSP能力描述(CLIENT_GetDevConfig里的)，以后建议用CLIENT_QueryDevState接口 */
typedef struct 
{
	DWORD	dwVideoStandardMask;	//视频制式掩码，按位表示设备能够支持的视频制式
	DWORD	dwImageSizeMask;		//分辨率掩码，按位表示设备能够支持的分辨率设置
	DWORD	dwEncodeModeMask;		//编码模式掩码，按位表示设备能够支持的编码模式设置	
	
	DWORD	dwStreamCap;			//按位表示设备支持的多媒体功能，
									//第一位表示支持主码流
									//第二位表示支持辅码流1
									//第三位表示支持辅码流2
									//第五位表示支持jpg抓图
	DWORD	dwImageSizeMask_Assi[8];//表示主码流为各分辨率时，支持的辅码流分辨率掩码。

	DWORD	dwMaxEncodePower;		//- DSP 支持的最高编码能力 
	WORD	wMaxSupportChannel;		//- 每块 DSP 支持最多输入视频通道数 
	WORD	wChannelMaxSetSync;		//- DSP 每通道的最大编码设置是否同步 0-不同步, 1-同步
} DH_DSP_ENCODECAP, *LPDH_DSP_ENCODECAP;

/* 设备信息配置结构 */
typedef struct 
{
	DWORD dwSize;
	/*下面是设备的只读部分*/
	DH_VERSION_INFO			stVersion;
	DH_DSP_ENCODECAP		stDspEncodeCap;		//DSP能力描述
	BYTE	szDevSerialNo[DH_DEV_SERIALNO_LEN];	//序列号
	BYTE	byDevType;							//设备类型，见枚举DHDEV_DEVICE_TYPE
	BYTE	szDevType[DH_DEV_TYPE_LEN];			//设备详细型号，字符串格式，可能为空
	BYTE	byVideoCaptureNum;					//视频口数量
	BYTE	byAudioCaptureNum;					//音频口数量
	BYTE	byTalkInChanNum;					//NSP
	BYTE	byTalkOutChanNum;					//NSP
	BYTE	byDecodeChanNum;					//NSP
	BYTE	byAlarmInNum;						//报警输入口数
	BYTE	byAlarmOutNum;						//报警输出口数
	BYTE	byNetIONum;							//网络口数
	BYTE	byUsbIONum;							//USB口数量
	BYTE	byIdeIONum;							//IDE数量
	BYTE	byComIONum;							//串口数量
	BYTE	byLPTIONum;							//并口数量
	BYTE	byVgaIONum;							//NSP
	BYTE	byIdeControlNum;					//NSP
	BYTE	byIdeControlType;					//NSP
	BYTE	byCapability;						//NSP，扩展描述
	BYTE	byMatrixOutNum;						//视频矩阵输出口数

	/*下面是设备的可写部分*/
	BYTE	byOverWrite;						//硬盘满处理方式（覆盖、停止）
	BYTE	byRecordLen;						//录象打包长度
	BYTE	byStartChanNo;						//NSP
	WORD	wDevNo;								//设备编号，用于遥控
	BYTE	byVideoStandard;					//视频制式
	BYTE	byDateFormat;						//日期格式
	BYTE	byDateSprtr;						//日期分割符(0-"."， 1-"-"， 2-"/")
	BYTE	byTimeFmt;							//时间格式 (0-24小时，1－12小时)
	BYTE	byLanguage;						    //枚举值详见DH_LANGUAGE_TYPE
} DHDEV_SYSTEM_ATTR_CFG, *LPDHDEV_SYSTEM_ATTR_CFG;

/************************************************************************
 *	图像通道
************************************************************************/

/* 录象类型－定时、动态检测、报警 */
typedef enum _REC_TYPE
{
	DH_REC_TYPE_TIM = 0,
	DH_REC_TYPE_MTD,
	DH_REC_TYPE_ALM,
	DH_REC_TYPE_NUM,
}REC_TYPE;

/* 时间段结构 */															    
typedef struct 
{
	BOOL	bEnable;	//当表示录像时间段时，按位表示三个使能，从低位到高位分别表示动检录象、报警录象、普通录象
	int		iBeginHour;
	int		iBeginMin;
	int		iBeginSec;
	int		iEndHour;
	int		iEndMin;
	int		iEndSec;
} DH_TSECT, *LPDH_TSECT;

/* 区域:各边距按整长8192的比例 */
typedef struct {
   long		left;
   long		top;
   long		right;
   long		bottom;
} DH_RECT,*LPDH_RECT;

/* OSD属性结构 */
typedef struct  tagENCODE_WIDGET
{
	DWORD		rgbaFrontground;		/*!< 物件的前景RGB，和透明度 */
	DWORD		rgbaBackground;			/*!< 物件的后景RGB，和透明度*/
	DH_RECT		rcRect;					/*!< 位置 */
	BYTE		bShow;					/*!< 物件显示 */
	BYTE		byReserved[3];
} DH_ENCODE_WIDGET, *LPDH_ENCODE_WIDGET;

/* 通道音视频属性 */
typedef struct 
{
	//视频参数
	BYTE	byVideoEnable;				/* 视频使能:1－打开，0－关闭 */
	BYTE	byBitRateControl;			/* 码流控制,参照常量定义 */
	BYTE	byFramesPerSec;				/* 帧率 */
	BYTE	byEncodeMode;				/* 编码模式,参照常量定义 */
	BYTE	byImageSize;				/* 分辨率参,参照常量定义 */
	BYTE	byImageQlty;				/* 档次1-6 */
	WORD	wLimitStream;				/* 限码流参数, 范围：50~4*1024 (k) */

	//音频参数
	BYTE	byAudioEnable;				/* 音频使能:1－打开，0－关闭 */
	BYTE	wFormatTag;					/* 编码类型，如PCM */
	WORD	nChannels;					/* 声道数 */
	WORD	wBitsPerSample;				/* 采样深度 */		
	DWORD	nSamplesPerSec;				/* 采样率 */
	
	BYTE	bIFrameInterval;			/* I帧间隔帧数量，描述两个I帧之间的P帧个数，0-149*/
	char	reserved;
} DH_VIDEOENC_OPT, *LPDH_VIDEOENC_OPT;

/* 画面颜色属性 */
typedef struct 
{
	DH_TSECT	stSect;
	BYTE		byBrightness;			/*! 亮度	0-100*/
	BYTE		byContrast;				/*! 对比度	0-100*/
	BYTE		bySaturation;			/*!	饱和度	0-100*/
	BYTE		byHue;					/*!	色度	0-100*/
	BYTE		byGainEn;				/*! 增益使能	 */ 
	BYTE		byGain;					/*! 增益	0-100*/
	BYTE		byReserved[2];
} DH_COLOR_CFG, *LPDH_COLOR_CFG;

/* 图像通道属性结构体 */
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
	BYTE				byBlindEnable;			/*! 区域遮盖开关　0x00不使能遮盖，0x01仅遮盖设备本地预览，0x10仅遮盖录像（及网络预览），0x11都遮盖	*/
	BYTE				byBlindMask;			/*! 区域遮盖掩码  第一位：设备本地预览；第二位：录像(及网络预览) */
	BYTE				byReserved[2];
} DHDEV_CHANNEL_CFG, *LPDHDEV_CHANNEL_CFG;


/************************************************************************
 *	区域遮挡配置
************************************************************************/
typedef struct __VIDEO_COVER_ATTR
{
	DH_RECT				rcBlock;			// 覆盖的区域坐标
	int					nColor;				// 覆盖的颜色
	BYTE				bBlockType;			// 覆盖方式：0-黑块，1-马赛克
	BYTE				bEncode;			// 编码级遮挡, 1—生效, 0—不生郊
	BYTE				bPriview;           // 预览遮挡, 1—生效, 0—不生效
	char				reserved[29];		// 保留
}VIDEO_COVER_ATTR;

typedef struct __DHDEV_VIDEOCOVER_CFG 
{
	DWORD				dwSize;
	char				szChannelName[DH_CHAN_NAME_LEN];	// 只读
	BYTE				bTotalBlocks;		// 支持的遮挡块数
	BYTE				bCoverCount;		// 已设置的遮挡块数
	VIDEO_COVER_ATTR	CoverBlock[DH_MAX_VIDEO_COVER_NUM];		// 覆盖的区域	
	char				reserved[30];		// 保留
}DHDEV_VIDEOCOVER_CFG;


/************************************************************************
 *	预览图像参数
************************************************************************/

/* 预览图像参数 */
typedef struct 
{
	DWORD dwSize;
	DH_VIDEOENC_OPT	stPreView;
	DH_COLOR_CFG  stColorCfg[DH_N_COL_TSECT];
}DHDEV_PREVIEW_CFG;

/************************************************************************
* 语音对讲音频属性结构体－未实现，待扩展 *
************************************************************************/

typedef struct 
{
	//音频输入参数
	BYTE	byInFormatTag;					//编码类型，如PCM
	BYTE	byInChannels;					//声道数
	WORD	wInBitsPerSample;				//采样深度					
	DWORD	dwInSamplesPerSec;				//采样率

	//音频输出参数
	BYTE	byOutFormatTag;					//编码类型，如PCM
	BYTE	byOutChannels;					//声道数
	WORD	wOutBitsPerSample;				//采样深度				
	DWORD	dwOutSamplesPerSec;				//采样率
} DHDEV_TALK_CFG, *LPDHDEV_TALK_CFG;

/************************************************************************
 *	定时录象
************************************************************************/

typedef struct 
{
	DWORD		dwSize;
	DH_TSECT	stSect[DH_N_WEEKS][DH_N_REC_TSECT];
	BYTE		byPreRecordLen;					/*! 预录时间,单位是s,0表示不预录*/
	BYTE		byRedundancyEn;					/*! 录像冗余开关*/
	BYTE		byReserved[2];
} DHDEV_RECORD_CFG, *LPDH_RECORD_CFG;

/************************************************************************
 *	报警配置
************************************************************************/

typedef struct 
{
	int iType;
	int iValue;
} DH_PTZ_LINK, LPDH_PTZ_LINK;

typedef struct 
{
	/******************************************************************************
	** 消息触发配置
	**
	** 当设备产生消息,譬如产生一个输入报警,或者视频丢失时,需要做相应的处理.
	** 此处定义如何处理一个消息
	*****************************************************************************/

	/* 消息处理方式,可以同时多种处理方式,包括
	 * 0x00000001 - 网络:上传管理服务器
	 * 0x00000002 - 录像:触发
	 * 0x00000004 - 云台联动
	 * 0x00000008 - 发送邮件
	 * 0x00000010 - 设备本地报警轮巡
	 * 0x00000020 - 设备提示使能
	 * 0x00000040 - 设备报警输出使能
	 * 0x00000080 - Ftp上传使能
	 * 0x00000100 - 蜂鸣
	 * 0x00000200 - 语音提示
	 * 0x00000400 - 抓图使能*/

	/*当前报警所支持的处理方式，按位掩码表示*/
	DWORD		dwActionMask;

	/* 触发动作，按位掩码表示，具体动作所需要的参数在各自的配置中体现*/
	DWORD		dwActionFlag;
	
	/* 报警触发的输出通道,报警触发的输出,为 1 表示触发该输出  */ 
	BYTE		byRelAlarmOut[DH_MAX_ALARMOUT_NUM];
	DWORD		dwDuration;		/* 报警持续时间 */

	/*联动录象*/
	BYTE		byRecordChannel[DH_MAX_VIDEO_IN_NUM];		/* 报警触发的录象通道,为1表示触发该通道 */
	DWORD		dwRecLatch;		/* 录象持续时间 */

	/*抓图通道*/
	BYTE		bySnap[DH_MAX_VIDEO_IN_NUM];
	/*轮巡通道*/
	BYTE		byTour[DH_MAX_VIDEO_IN_NUM];

	/*  云台联动: */
	DH_PTZ_LINK	struPtzLink[DH_MAX_VIDEO_IN_NUM];
	DWORD		dwEventLatch;   /*!< 联动开始延时时间，s为单位 范围是0－－15 默认值是0*/
	/* 报警触发的无线输出通道,报警触发的输出,为 1 表示触发该输出  */ 
	BYTE		byRelWIAlarmOut[DH_MAX_ALARMOUT_NUM];
	BYTE		bMessageToNet;	//消息上传给网络使能
	BYTE		byReserved[111];       
} DH_MSG_HANDLE;

/* 外部报警 */
typedef struct
{
	BYTE		byAlarmType;						/* 报警器类型,0：常闭,1：常开  */
	BYTE		byAlarmEn;							/*报警使能*/
	BYTE		byReserved[2];						
	DH_TSECT	stSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
	DH_MSG_HANDLE	struHandle;	/* 处理方式  */
} DH_ALARMIN_CFG,*LPDHDEV_ALARMIN_CFG; 

/* 动态检测报警 */
typedef struct 
{
	BYTE			byMotionEn;							/* 动态检测报警使能*/
	BYTE			byReserved;
	WORD			wSenseLevel;						/*灵敏度*/
	WORD			wMotionRow;							/* 动态检测区域的行数 */
	WORD			wMotionCol;							/* 动态检测区域的列数 */
	BYTE			byDetected[DH_MOTION_ROW][DH_MOTION_COL];	/*检测区域，最多32*32块区域*/
	DH_TSECT		stSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
	DH_MSG_HANDLE	struHandle;	/*处理方式*/
}DH_MOTION_DETECT_CFG;

/* 视频丢失报警 */
typedef struct
{
	BYTE			byAlarmEn;							/*视频丢失报警使能*/
	BYTE			byReserved[3];
	DH_TSECT		stSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
	DH_MSG_HANDLE	struHandle;	/* 处理方式  */
} DH_VIDEO_LOST_CFG;

/* 图像遮挡报警 */
typedef struct
{
	BYTE			byBlindEnable;						/* 使能 */
	BYTE			byBlindLevel;						/* 灵敏度1-6 */
	BYTE			byReserved[2];
	DH_TSECT	stSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
	DH_MSG_HANDLE	struHandle;	/* 处理方式  */
} DH_BLIND_CFG;

/* 硬盘消息(内部报警) */
typedef struct 
{
	BYTE				byNoDiskEn;				/* 无硬盘时报警 */
	BYTE				byReserved_1[3];
	DH_TSECT			stNDSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
	DH_MSG_HANDLE		struNDHandle;				/* 处理方式  */

	BYTE				byLowCapEn;				/* 硬盘低容量时报警*/
	BYTE				byLowerLimit;			/* 容量阀值 0-99*/
	BYTE				byReserved_2[2];
	DH_TSECT			stLCSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
	DH_MSG_HANDLE		struLCHandle;			/* 处理方式  */

	BYTE				byDiskErrEn;			/* 硬盘故障报警*/
	BYTE				byReserved_3[3];
	DH_TSECT			stEDSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
	DH_MSG_HANDLE		struEDHandle;	/* 处理方式  */

} DH_DISK_ALARM_CFG;

typedef struct
{
	BYTE				byEnable;
	BYTE				byReserved[3];
	DH_MSG_HANDLE		struHandle;
} DH_NETBROKEN_ALARM_CFG;
///*音频报警*/
//typedef struct 
//{
//	BYTE		bySoundAlarmEn;			/*音频报警使能*/
//	BYTE		byVol;					/*报警音量阀值*/
//	BYTE		byReserved[2];			/*保留字节*/
//	DH_TSECT	stSect[DH_N_WEEKS][DH_N_REC_TSECT];		//NSP
//	DH_MSG_HANDLE	struHandle;	/* 处理方式  */
//}DH_SOUND_ALARM_CFG;


/* 报警布防 */
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
	//可能还有设备异常
} DHDEV_ALARM_SCHEDULE;

/************************************************************************
 *	网络配置
************************************************************************/
/*                                                                      */
/*以太网配置*/
typedef struct 
{
	char	sDevIPAddr[DH_MAX_IPADDR_LEN];			/* DVR IP 地址  */ 
	char	sDevIPMask[DH_MAX_IPADDR_LEN];			/* DVR IP 地址掩码  */ 
	char	sGatewayIP[DH_MAX_IPADDR_LEN];			/* 网关地址  */ 

	/* 10M/100M  自适应,索引 
	 * 1-10MBase - T
	 * 2-10MBase-T 全双工 
	 * 3-100MBase - TX
	 * 4-100M 全双工
	 * 5-10M/100M  自适应 
	 */
	//为了扩展将DWORD拆成四个
	BYTE	dwNetInterface;							/* NSP */
	BYTE	bTranMedia;								/* 0：有线 1：无线 */
	BYTE	bValid;									/* 按位表示，第一位：1：有效 0：无效；第二位：0：DHCP关闭 1：DHCP使能；第三位：0：不支持DHCP 1：支持DHCP */
	BYTE	bDefaultEth;							/* 是否作为默认的网卡 1：默认 0：非默认 */
	char	byMACAddr[DH_MACADDR_LEN];				/* MAC地址，只读 */
} DH_ETHERNET; 

/* 远程主机配置 */
typedef struct 
{
	BYTE	byEnable;								/* 连接使能  */
	BYTE	byReserved;
	WORD	wHostPort;								/* 远程主机 端口  */
	char	sHostIPAddr[DH_MAX_IPADDR_LEN];			/* 远程主机 IP 地址  */ 		
	char	sHostUser[DH_MAX_HOST_NAMELEN];			/* 远程主机 用户名 */ 
	char	sHostPassword[DH_MAX_HOST_PSWLEN];		/* 远程主机 密码 */ 
}DH_REMOTE_HOST;

/* 邮件配置 */
typedef struct 
{
	char	sMailIPAddr[DH_MAX_IPADDR_LEN];			/*邮件服务器IP地址*/
	WORD	wMailPort;								/*邮件服务器端口*/
	WORD	wReserved;								/* 保留 */
	char	sSenderAddr[DH_MAX_MAIL_ADDR_LEN];		/*发送地址*/
	char	sUserName[DH_MAX_NAME_LEN];			/* 用户名 */
	char	sUserPsw[DH_MAX_NAME_LEN];				/* 用户密码 */
	char	sDestAddr[DH_MAX_MAIL_ADDR_LEN];			/*目的地址*/
	char	sCcAddr[DH_MAX_MAIL_ADDR_LEN];			/*抄送地址*/
	char	sBccAddr[DH_MAX_MAIL_ADDR_LEN];			/*暗抄地址*/
	char	sSubject[DH_MAX_MAIL_SUBJECT_LEN];		/*标题*/
}DH_MAIL_CFG;

/*网络配置结构体*/
typedef struct
{ 
	DWORD				dwSize; 

	char				sDevName[DH_MAX_NAME_LEN];				/* 设备主机名 */

	WORD				wTcpMaxConnectNum;						/* TCP最大连接数(一般指视频数据请求数) */
	WORD				wTcpPort;								/* TCP帧听端口 */
	WORD				wUdpPort;								/* UDP侦听端口 */
	WORD				wHttpPort;								/* HTTP端口号 */
	WORD				wHttpsPort;							/* HTTPS端口号 */
	WORD				wSslPort;								/* SSL端口号*/
	DH_ETHERNET			stEtherNet[DH_MAX_ETHERNET_NUM];/* 以太网口  */ 

	DH_REMOTE_HOST		struAlarmHost;		/* 报警服务器 */
	DH_REMOTE_HOST		struLogHost;		/* 日志服务器 */
	DH_REMOTE_HOST		struSmtpHost;		/* SMTP服务器 */
	DH_REMOTE_HOST		struMultiCast;		/* 多播组 */
	DH_REMOTE_HOST		struNfs;			/* NFS服务器 */
	DH_REMOTE_HOST		struPppoe;			/* PPPoE服务器 */
	char				sPppoeIP[DH_MAX_IPADDR_LEN]; /* PPPoE注册返回的IP */
	DH_REMOTE_HOST		struDdns;			/* DDNS服务器 */
	char				sDdnsHostName[DH_MAX_HOST_NAMELEN];		/* DDNS主机名*/
	DH_REMOTE_HOST		struDns;			/* DNS服务器*/
	DH_MAIL_CFG			struMail;			/* 邮件配置 */
} DHDEV_NET_CFG;

/************************************************************************
 *	串口配置
************************************************************************/

/* 串口基本属性 */
typedef struct
{
	BYTE	byDataBit;		//数据位 0-5,1-6,2-7,3-8
	BYTE	byStopBit;		//停止位 0-1位, 1-1.5位, 2-2位 
	BYTE	byParity;		//校验位 0-no, 1-odd, 2-even 
	BYTE	byBaudRate;	//{0-300,1-600,2-1200,3-2400,4-4800
	// 5-9600,6-19200,7-38400,8-57600,9-115200} 
} DH_COMM_PROP;

/* 485解码器配置 */
typedef struct
{ 
	DH_COMM_PROP	struComm;

	BYTE			wProtocol;			/*!< 协议类型 保存协议的下标，动态变化 */
	BYTE			byReserved;			/*!保留  */	
	BYTE			wDecoderAddress;	/*解码器地址:0 - 255*/ 
	BYTE 			byMartixID;			/*矩阵号*/
} DH_485_CFG; 

/* 232串口配置 */
typedef struct
{
	DH_COMM_PROP	struComm;
	BYTE			byFunction;/*!< 串口功能，对应串口配置取到的功能名列表 */
	BYTE			byReserved[3];
} DH_RS232_CFG;

/* 串口配置结构体 */
typedef struct
{
	DWORD			dwSize;
	//解码器协议
	DWORD			dwDecProListNum;				//协议个数
	char			DecProName[DH_MAX_DECPRO_LIST_SIZE][DH_MAX_NAME_LEN]; //协议名列表
	DH_485_CFG		stDecoder[DH_MAX_DECODER_NUM];		//各解码器当前属性

	DWORD			dw232FuncNameNum;				//232功能个数
	char			s232FuncName[DH_MAX_232FUNCS][DH_MAX_NAME_LEN];	//功能名列表
	DH_RS232_CFG	st232[DH_MAX_232_NUM];		//各232串口当前属性
} DHDEV_COMM_CFG;


/************************************************************************
 *	自动维护配置
 ***********************************************************************/
/* 自动维护属性 */
typedef struct
{
	DWORD dwSize;

	//自动重启
	BYTE byAutoRebootDay; // 0-从不, 1-每天, 2-每星期日, 3-每星期一,.....
	BYTE byAutoRebootTime;  // 0-0:00 1-1:00,........23-:23:00 */
	//自动删除文件
	BYTE byAutoDeleteFilesTime; /*! 0-从不, 1-24H, 2-48H, 3-72H, 4-96H, 5-ONE WEEK, 6-ONE MONTH */

	BYTE reserved[13]; // 保留位
} DHDEV_AUTOMT_CFG;

/************************************************************************
 * 本机矩阵控制策略配置
 ************************************************************************/

typedef struct
{
	BOOL		bTourEnable;			//	轮巡使能
	int			nTourPeriod;			//	轮巡间隔，单位秒, 5-300 
	DWORD		dwChannelMask;			//	轮巡的通道，掩码形式表示
	char		reserved[64];
}DH_VIDEOGROUP_CFG;

typedef struct
{
	DWORD	dwSize;
	int		nMatrixNum;			//	矩阵个数(注：不允许修改)
	DH_VIDEOGROUP_CFG struVideoGroup[DH_MATRIX_MAXOUT];
	char	reserved[32];
}DHDEV_VIDEO_MATRIX_CFG;   

/************************************************************************
 *	多ddns配置结构体
************************************************************************/

typedef struct
{
	DWORD	dwId;				//ddns服务器id号
	BOOL	bEnable;			//使能，同一时间只能有一个ddns服务器处于使能状态
	char	szServerType[DH_MAX_SERVER_TYPE_LEN];	//服务器类型，希网..
	char	szServerIp[DH_MAX_DOMAIN_NAME_LEN];		//服务器ip或者域名
	DWORD	dwServerPort;						//服务器端口
	char	szDomainName[DH_MAX_DOMAIN_NAME_LEN];	//dvr域名，如jeckean.3322.org
	char	szUserName[DH_MAX_HOST_NAMELEN];	//用户名
	char	szUserPsw[DH_MAX_HOST_PSWLEN];		//密码
	char	szAlias[DH_MAX_DDNS_ALIAS_LEN];		//服务器别名，如"dahua inter ddns"
	DWORD	dwAlivePeriod;						//DDNS 保活时间
	char	reserved[256];
}DH_DDNS_SERVER_CFG;

typedef struct
{
	DWORD	dwSize;
	DWORD	dwDdnsServerNum;	
	DH_DDNS_SERVER_CFG struDdnsServer[DH_MAX_DDNS_NUM];	
}DHDEV_MULTI_DDNS_CFG;


/************************************************************************
 *   邮件配置结构体
 ************************************************************************/
typedef struct 
{
	char	sMailIPAddr[DH_MAX_DOMAIN_NAME_LEN];	/*邮件服务器地址(IP或者域名)*/
	char	sSubMailIPAddr[DH_MAX_DOMAIN_NAME_LEN];
	WORD	wMailPort;								/*邮件服务器端口*/
	WORD	wSubMailPort;
	WORD	wReserved;								/*保留*/
	char	sSenderAddr[DH_MAX_MAIL_ADDR_LEN];		/*发送地址*/
	char	sUserName[DH_MAX_NAME_LEN];				/*用户名*/
	char	sUserPsw[DH_MAX_NAME_LEN];				/*用户密码*/
	char	sDestAddr[DH_MAX_MAIL_ADDR_LEN];		/*目的地址*/
	char	sCcAddr[DH_MAX_MAIL_ADDR_LEN];			/*抄送地址*/
	char	sBccAddr[DH_MAX_MAIL_ADDR_LEN];			/*暗抄地址*/
	char	sSubject[DH_MAX_MAIL_SUBJECT_LEN];		/*标题*/
	char	reserved[256];
} DHDEV_MAIL_CFG;


/************************************************************************
 *   设备软件版本信息
 ************************************************************************/
typedef struct  
{
	char	szDevSerialNo[DH_DEV_SERIALNO_LEN];		//序列号
	char	byDevType;								//设备类型，见枚举DHDEV_DEVICE_TYPE
	char	szDevType[DH_DEV_TYPE_LEN];				//设备详细型号，字符串格式，可能为空
	int		nProtocalVer;							//协议版本号
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
 *   DSP能力描述
 ************************************************************************/
typedef struct 
{
	DWORD	dwVideoStandardMask;	//视频制式掩码，按位表示设备能够支持的视频制式
	DWORD	dwImageSizeMask;		//分辨率掩码，按位表示设备能够支持的分辨率设置
	DWORD	dwEncodeModeMask;		//编码模式掩码，按位表示设备能够支持的编码模式设置	
	
	DWORD	dwStreamCap;			//按位表示设备支持的多媒体功能，
									//第一位表示支持主码流
									//第二位表示支持辅码流1
									//第三位表示支持辅码流2
									//第五位表示支持jpg抓图
	DWORD	dwImageSizeMask_Assi[32];//表示主码流为各分辨率时，支持的辅码流分辨率掩码。

	DWORD	dwMaxEncodePower;		//- DSP 支持的最高编码能力 
	WORD	wMaxSupportChannel;		//- 每块 DSP 支持最多输入视频通道数 
	WORD	wChannelMaxSetSync;		//- DSP 每通道的最大编码设置是否同步 0-不同步, 1-同步

	char	reserved[128];
} DHDEV_DSP_ENCODECAP, *LPDHDEV_DSP_ENCODECAP;


/************************************************************************
 *   抓图功能配置
 ************************************************************************/
typedef struct 
{
	DWORD	dwSize;
	BOOL	bTimingEnable;		//定时抓图开关（报警抓图开关在各报警联动配置中体现）
	DH_VIDEOENC_OPT struSnapEnc[SNAP_TYP_NUM];	//抓图编码配置，现支持其中的分辨率、画质、帧率设置，帧率在这里是负数，表示一秒抓图的次数。
}DHDEV_SNAP_CFG;

/************************************************************************
 *   web路径配置
 ************************************************************************/
typedef struct 
{
	DWORD	dwSize;
	BOOL	bSnapEnable;	//是否抓图
	int		iSnapInterval;	//抓图周期
	char	szHostIp[DH_MAX_IPADDR_LEN]; /* HTTP主机IP */
	WORD	wHostPort;
	int		iMsgInterval;	//状态消息发送间隔
	char	szUrlState[DH_MAX_URL_LEN];		//状态消息上传URL
	char	szUrlImage[DH_MAX_URL_LEN];		//图片上传Url
	char	szDevId[DH_MAX_DEV_ID_LEN];		//机器的web编号
	BYTE	byReserved[2];
}DHDEV_URL_CFG;

/************************************************************************
 *   图象水印配置
 ************************************************************************/
typedef struct __DHDEV_WATERMAKE_CFG 
{
	DWORD				dwSize;
	int					nEnable;			//	使能
	int					nStream;			//	码流（1～n）0-所有码流
	int					nKey;				//	数据类型(1-文字，2-图片)
	char				szLetterData[DH_MAX_WATERMAKE_LETTER];	//	文字
	char				szData[DH_MAX_WATERMAKE_DATA];		//	图片数据
	BYTE				bReserved[512];		//	保留
} DHDEV_WATERMAKE_CFG;

/************************************************************************
 *   FTP上传配置
 ************************************************************************/
typedef struct
{
	struct
	{
		DH_TSECT	struSect;	//该时间段内的“使能”无效，可忽略
		BOOL		bMdEn;		//上传动态检测录象
		BOOL		bAlarmEn;	//上传外部报警录象
		BOOL		bTimerEn;	//上传普通定时录像
		DWORD		dwRev[4];
	} struPeriod[DH_TIME_SECTION];
}DH_FTP_UPLOAD_CFG;

typedef struct
{
	DWORD				dwSize;
	BOOL				bEnable;		//是否启用
	char				szHostIp[DH_MAX_IPADDR_LEN];		//主机IP
	WORD				wHostPort;		//主机端口
	char				szDirName[DH_FTP_MAX_PATH];			//FTP目录路径
	char				szUserName[DH_FTP_USERNAME_LEN];	//用户名
	char				szPassword[DH_FTP_PASSWORD_LEN];	//密码
	int					iFileLen;					//文件长度
	int					iInterval;					//相邻文件时间间隔
	DH_FTP_UPLOAD_CFG	struUploadCfg[DH_MAX_CHANNUM][DH_N_WEEKS];
	BYTE 				reserved[130];
}DHDEV_FTP_PROTO_CFG;

// DNS服务器配置
typedef struct  
{
	char				szPrimaryIp[DH_MAX_IPADDR_LEN];
	char				szSecondaryIp[DH_MAX_IPADDR_LEN];
	char				reserved[256];
}DHDEV_DNS_CFG;


/************************************************************************
 *   录象下载策略配置
 ************************************************************************/
typedef struct
{
	DWORD dwSize;
	
	BOOL bEnable;	//TRUE－高速下载，FALSE-普通下载
}DHDEV_DOWNLOAD_STRATEGY_CFG;

/************************************************************************
 *   网络传输策略配置
 ************************************************************************/
typedef struct
{
	DWORD dwSize;
	
	BOOL bEnable;
	int  iStrategy;	//0-画质优先， 1-流畅性优先， 2-自动。
}DHDEV_TRANSFER_STRATEGY_CFG;


/************************************************************************
 *	IPC的一些配置
************************************************************************/
//	配置无线网络信息
typedef struct 
{
	int					nEnable;			// 无线使能
	char				szSSID[36];			// SSID
	int					nLinkMode;			// 连接模式 0:auto 1:adhoc 2:Infrastructure
	int					nEncryption;		// 加密 0: off 2:WEP64bit 3: WEP128bit
	int					nKeyType;			// 0:Hex 1:ASCII
    int					nKeyID;				// 序号
	char				szKeys[4][32];		// 四组密码
	int					nKeyFlag;
	char				reserved[12];
} DHDEV_WLAN_INFO;

//	选择使用某个无线设备
typedef struct  
{
	char				szSSID[36];
	int					nLinkMode;			// 连接模式 0:adhoc 1:Infrastructure
	int 				nEncryption;		// 加密 0: off  2:WEP64bit 3: WEP128bit
	char				reserved[48];
} DHDEV_WLAN_DEVICE;

//	搜索到的无线设备列表
typedef struct  
{
	DWORD				dwSize;
	BYTE				bWlanDevCount;		// 搜索到的无线设备个数
	DHDEV_WLAN_DEVICE	lstWlanDev[DH_MAX_WLANDEVICE_NUM];
	char				reserved[255];
} DHDEV_WLAN_DEVICE_LIST;

//	主动注册参数配置
typedef struct  
{
	char				szServerIp[32];		//注册服务器IP
	int					nServerPort;		//端口号
	char				reserved[64];
} DHDEV_SERVER_INFO;

typedef struct  
{
	DWORD				dwSize;
	BYTE				bServerNum;         //支持的最大ip数
	DHDEV_SERVER_INFO	lstServer[DH_MAX_REGISTER_SERVER_NUM];
	BYTE				bEnable;			//使能
	char				szDeviceID[32];		//设备id。
	char				reserved[94];
} DHDEV_REGISTER_SERVER;

//	摄像头属性
typedef struct __DHDEV_CAMERA_INFO
{
	BYTE				bBrightnessEn;		// 亮度可调 1：可；0：不可
	BYTE				bContrastEn;		// 对比度可调
	BYTE				bColorEn;			// 色度可调
	BYTE				bGainEn;			// 增益可调
	BYTE				bSaturationEn;		// 饱和度可调
	BYTE				bBacklightEn;		// 背光补偿可调
	BYTE				bExposureEn;		// 曝光选择可调
	BYTE				bColorConvEn;		// 自动彩黑转换可调
	BYTE				bAttrEn;			// 属性选项 1：可；0：不可
	BYTE				bMirrorEn;			// 镜像 1：支持；0：不支持
    BYTE				bFlipEn;			// 翻转 1：支持；0：不支持
	BYTE				bRev[125];			// 保留
} DHDEV_CAMERA_INFO;

//	摄像头属性配置
typedef struct __DHDEV_CAMERA_CFG 
{
	DWORD				dwSize;
	BYTE				bExposure;			//曝光模式 1-9:手动曝光等级; 0:自动曝光
	BYTE				bBacklight;			//背光补偿 3:强；2：中；1：弱； 0:关
	BYTE				bAutoColor2BW;		//日/夜模式 2:开；1：自动; 0:关
	BYTE				bMirror;			//镜像  1 开， 0关
	BYTE				bFlip;				//翻转  1 开， 0关
	char				bRev[123];			//保留
} DHDEV_CAMERA_CFG;

#define ALARM_MAX_NAME 64
//(无线)红外报警配置
typedef struct
{
	BOOL				bEnable;							//报警输入使能
	char				szAlarmName[DH_MAX_ALARM_NAME];		//报警输入名称
	int					nAlarmInPattern;					//报警器输入波形
	int					nAlarmOutPattern;					//报警输出波形
	char				szAlarmInAddress[DH_MAX_ALARM_NAME];//报警输入地址
	int					nSensorType;						//外部设备传感器类型常开 or 常闭
	int					nDefendEfectTime;					//布撤防延时时间，在此时间后该报警输入有效
	int					nDefendAreaType;					//防区类型 
	int					nAlarmSmoothTime;					//报警平滑时间，即在此时间内如果只有一个报警输入连续输入两次则忽略掉后面一次
	char				reserved[128];
	
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];
	DH_MSG_HANDLE		struHandle;	/* 处理方式  */
} DH_INFRARED_INFO;

//无线遥控器配置
typedef struct 
{
	BYTE  address[ALARM_MAX_NAME];							// 遥控器地址
	BYTE  name[ALARM_MAX_NAME];								// 遥控器名称
	BYTE  reserved[32];										// 保留字段
}DH_WI_CONFIG_ROBOT;

//无线报警输出配置
typedef struct 
{
	BYTE  address[ALARM_MAX_NAME];							//报警输出地址
	BYTE  name[ALARM_MAX_NAME];								//报警输出名称
	BYTE  reserved[32];										//保留字段
}DH_WI_CONFIG_ALARM_OUT;

typedef struct  
{
	DWORD				dwSize;
	BYTE				bAlarmInNum;						//无线报警输入数
	BYTE				bAlarmOutNum;						//无线报警输出数
	DH_WI_CONFIG_ALARM_OUT AlarmOutAddr[16];				//报警输出地址
	BYTE				bRobotNum;							//遥控器个数
	DH_WI_CONFIG_ROBOT RobotAddr[16];						//遥控器地址
	DH_INFRARED_INFO	InfraredAlarm[16];
	char				reserved[256];
} DH_INFRARED_CFG;

//新音频检测报警信息
typedef struct {
	int channel;						//报警通道号
	int alarmType;						//报警类型0--音频值过低 1---音频值过高
	unsigned int volume;				//音量值
	char 	reserved[256];
} NET_NEW_SOUND_ALARM_STATE;

typedef struct  
{
	int channelcount;                   //报警的通道个数
	NET_NEW_SOUND_ALARM_STATE  SoundAlarmInfo[DH_MAX_ALARM_IN_NUM];
}DH_NEW_SOUND_ALARM_STATE;

//新音频检测报警配置
typedef struct
{
	BOOL				bEnable;							//报警输入使能
	int					Volume_min;							//音量的最小值
	int					Volume_max;							//音量的最大值
	char				reserved[128];	
	DH_TSECT			stSect[DH_N_WEEKS][DH_N_REC_TSECT];	
	DH_MSG_HANDLE		struHandle;	/* 处理方式  */
} DH_AUDIO_DETECT_INFO;

typedef struct  
{
	DWORD					dwSize;
	int						AlarmNum;
	DH_AUDIO_DETECT_INFO	AudioDetectAlarm[DH_MAX_ALARM_IN_NUM];
	char					reserved[256];
} DH_AUDIO_DETECT_CFG;

//存储位置设置结构体，每通道独立设置,每通道可以选择各种存储类型, 目前包括本地, 可移动, 远程存储.
typedef struct 
{
	DWORD 	dwSize;
	DWORD 	dwLocalMask;      //本地存储掩码  按位表示：
								//第一位	系统预录
								//第二位	定时录像
								//第三位	动检录像
								//第四位	报警录像
								//第五位	卡号录像
								//第六位	手动录像

	DWORD 	dwMobileMask;				//可移动存储掩码 存储掩码如本地存储掩码
	int  	RemoteType;					//远程存储类型 0: Ftp  1: Smb 
	DWORD 	dwRemoteMask;				//远程存储掩码 存储掩码如本地存储掩码
	DWORD 	dwRemoteSecondSelLocal;		//远程异常时本地存储掩码
	DWORD 	dwRemoteSecondSelMobile;	//远程异常时可移动存储掩码
	char 	SubRemotePath[MAX_PATH_STOR];  //远程目录, 其中长度为260
	char	reserved[128];
}DH_STORAGE_STATION_CFG;

/************************************************************************
 *   网络抓包配置
 ************************************************************************/
typedef struct
{
	int					Offset;			//标志位的位偏移
	int					Length;			//标志位的长度
	char				Key[16];		//标志位的值
} DH_SNIFFER_FRAMEID;

typedef struct 
{
	int					Offset;			//标志位的位偏移
	int					Offset2;		//目前没有应用
	int					Length;			//标志位的长度
	int					Length2;		//目前没有应用
	char				KeyTitle[24];	//标题的值
} DH_SNIFFER_CONTENT;

typedef struct 
{
	DH_SNIFFER_FRAMEID	snifferFrameId;								//每个FRAME ID 选项
	DH_SNIFFER_CONTENT	snifferContent[DH_SNIFFER_CONTENT_NUM];		//每个FRAME对应的4个抓包内容
} DH_SNIFFER_FRAME;

//每组抓包对应的配置结构
typedef struct
{
	char				SnifferSrcIP[DH_MAX_IPADDR_LEN];			//抓包源地址		
	int					SnifferSrcPort;								//抓包源端口
	char				SnifferDestIP[DH_MAX_IPADDR_LEN]; 			//抓包目标地址
	int					SnifferDestPort;							//抓包目标端口
	char				reserved[28];								//保留字段
	DH_SNIFFER_FRAME	snifferFrame[DH_SNIFFER_FRAMEID_NUM];		//6个FRAME 选项
	int					displayPosition;							//显示位置
	int					recdChannelMask;							//通道掩码
} DH_ATM_SNIFFER_CFG;

typedef struct  
{
	DWORD dwSize;
	DH_ATM_SNIFFER_CFG SnifferConfig[4];
	char	reserved[256];								//保留字段
}DHDEV_SNIFFER_CFG;


/************************************************************************
 *   OEM查询
 ************************************************************************/
typedef struct  
{
	char				szVendor[DH_MAX_STRING_LEN];
	char				szType[DH_MAX_STRING_LEN];
	char				reserved[128];
}DHDEV_OEM_INFO;


/************************************************************************
 *   NTP配置
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
	BOOL				bEnable;			//	是否启用
	int					nHostPort;			//	NTP服务器默认端口为123
	char				szHostIp[32];		//	主机IP
	char				szDomainName[128];	//	域名
	int					nType;				//	不可设置 0：表示IP，1：表示域名，2：表示IP和域名
	int					nUpdateInterval;	//	更新时间(分钟)
	int					nTimeZone;			//	见DH_TIME_ZONE_TYPE
	char				reserved[128];
}DHDEV_NTP_CFG;


/************************************************************************
 *   平台接入配置 － U网通
 ************************************************************************/
typedef struct
{
    BOOL bChnEn;
    char szChnId[DH_INTERVIDEO_UCOM_CHANID];
}DH_INTERVIDEO_UCOM_CHN_CFG;

typedef struct
{
	DWORD dwSize;
	BOOL bFuncEnable;		// 接入功能使能与否 0 - 使能
	BOOL bAliveEnable;		// 心跳使能与否
	DWORD dwAlivePeriod;	// 心跳周期，单位秒，0-3600
	char szServerIp[DH_MAX_IPADDR_LEN];		// CMS的IP
	WORD wServerPort;						// CMS的Port
    char szRegPwd[DH_INTERVIDEO_UCOM_REGPSW]; //注册密码
	char szDeviceId[DH_INTERVIDEO_UCOM_DEVID];	//设备id
	char szUserName[DH_INTERVIDEO_UCOM_USERNAME];
	char szPassWord[DH_INTERVIDEO_UCOM_USERPSW];
    DH_INTERVIDEO_UCOM_CHN_CFG  struChnInfo[DH_MAX_CHANNUM];//通道id,en
}DHDEV_INTERVIDEO_UCOM_CFG;

/************************************************************************
 *   平台接入配置 － 阿尔卡特
 ************************************************************************/
typedef struct
{
	DWORD dwSize;
    unsigned short usCompanyID[2];  /* 公司ID,数值,不同的第三方服务公司,考虑4字节对齐,目前只用数组第一个 */
    char szDeviceNO[32];  /* 前端设备序列号，字符串, 包括'\0'结束符共32byte */
    char szVSName[32];  /* 前端设备名称，字符串, 包括'\0'结束符共16byte */
    char szVapPath[32];  /* VAP路径 */
    unsigned short usTcpPort;  /* TCP 端口,数值: 数值 1~65535 */    
    unsigned short usUdpPort;  /* UDP 端口,数值: 数值 1~65535 */    
    bool bCsEnable[4];  /* 中心服务器使能标志, 数值: true使能, false不使能,考虑4字节对齐,目前只用数组第一个 */
    char szCsIP[16];  /* 中心服务器IP地址, 字符串, 包括'\0'结束符共16byte */
    unsigned short usCsPort[2];  /* 中心服务器端口,数值: 数值 1~65535,考虑4字节对齐,目前只用数组第一个 */    
    bool bHsEnable[4];  /* 心跳服务器使能标志, 数值: true使能, false不使能,考虑4字节对齐,目前只用数组第一个 */
    char szHsIP[16];  /* 心跳服务器IP地址, 字符串, 包括'\0'结束符共16byte */
    unsigned short usHsPort[2];  /* 心跳服务器端口,数值: 数值 1~65535,考虑4字节对齐,目前只用数组第一个 */ 
    int iHsIntervalTime;  /* 心跳服务器间隔周期,数值(单位:秒) */ 
    bool bRsEnable[4];  /* 注册服务器使能标志, 数值: true使能, false不使能,考虑4字节对齐,目前只用数组第一个 */
    char szRsIP[16];  /* 注册服务器IP地址, 字符串, 包括'\0'结束符共16byte */
    unsigned short usRsPort[2];  /* 注册服务器端口,数值: 数值 1~65535,考虑4字节对齐,目前只用数组第一个 */
    int iRsAgedTime;  /* 注册服务器有效时间,数值(单位:小时) */
    char szAuthorizeServerIp[16];  /* 鉴权服务器IP */
    unsigned short usAuthorizePort[2];  /* 鉴权服务器端口,考虑4字节对齐,目前只用数组第一个 */
    char szAuthorizeUsername[32];  /* 鉴权服务器帐号 */
    char szAuthorizePassword[36];  /* 鉴权服务器密码 */
    
    char szIpACS[16];  /* ACS(自动注册服务器) IP */
    unsigned short usPortACS[2];  /* ACS Port,考虑4字节对齐,目前只用数组第一个 */
    char szUsernameACS[32];  /* ACS用户名 */
    char szPasswordACS[36];  /* ACS密码 */
    bool bVideoMonitorEnabled[4];  /* DVS是否定期上报前端视频信号监控信息, 数值: true使能, false不使能 */
    int iVideoMonitorInterval;  /* 上报周期（分钟） */
    
    char szCoordinateGPS[64];  /* GPS坐标 */
    char szPosition[32];  /* 设备位置 */
    char szConnPass[36];  /* 设备接入码 */
}DHDEV_INTERVIDEO_BELL_CFG;

/************************************************************************
 *   平台接入配置 － 中兴力维
 ************************************************************************/
typedef struct  
{
	DWORD dwSize;
	unsigned short		nSevPort;							//服务器端口, 数值, 数值1~65535
    char				szSevIp[DH_INTERVIDEO_NSS_IP];		//服务器IP地址, 字符串, 包括'\0'结束符共32byte
    char				szDevSerial[DH_INTERVIDEO_NSS_SERIAL];	//前端设备序列号，字符串,包括'\0'结束符共32byte
    char				szUserName[DH_INTERVIDEO_NSS_USER];
    char				szPwd[DH_INTERVIDEO_NSS_PWD];
}DHDEV_INTERVIDEO_NSS_CFG;

/************************************************************************
 *   网络运行状态信息
 ************************************************************************/

typedef struct
{
	int					nChannelNum;//通道号
	char				szUseType[32];//通道用途
	DWORD				dwStreamSize;//流量大小(单位:kb/s)
	char				reserved[32];//保留
}DHDEV_USE_CHANNEL_STATE;

typedef struct 
{
	char						szUserName[32];//用户名
	char						szUserGroup[32];//用户组
	NET_TIME					time;//登入时间
	int							nOpenedChannelNum;//开启的通道个数
	DHDEV_USE_CHANNEL_STATE		channelInfo[DH_MAX_CHANNUM];
	char						reserved[64];
}DHDEV_USER_NET_INFO;

typedef	struct 
{
	int							nUserCount;//用户数量
	DHDEV_USER_NET_INFO			stuUserInfo[32];
	char						reserved[256];
}DHDEV_TOTAL_NET_STATE;
/************************************************************************
 *   云台属性信息
 ************************************************************************/
#define  NAME_MAX_LEN 16
typedef struct 
{
	DWORD		dwHighMask; /*!< 操作的掩码高位 */
	DWORD		dwLowMask; /*!< 操作的掩码低位 */
	char		szName[NAME_MAX_LEN]; /*!< 操作的协议名NAME_MAX_LEN=16 */
	WORD		wCamAddrMin; /*!< 通道地址的最小值 */
	WORD		wCamAddrMax; /*!< 通道地址的最大值 */
	WORD		wMonAddrMin; /*!< 监视地址的最小值 */
	WORD		wMonAddrMax; /*!< 监视地址的最大值 */
	BYTE		bPresetMin; /*!< 预置点的最小值 */
	BYTE		bPresetMax; /*!< 预置点的最大值 */
	BYTE		bTourMin; /*!< 自动巡航线路的最小值 */
	BYTE		bTourMax; /*!< 自动巡航线路的最大值 */
	BYTE		bPatternMin; /*!< 轨迹线路的最小值 */
	BYTE		bPatternMax; /*!< 轨迹线路的最大值 */
	BYTE		bTileSpeedMin; /*!< 垂直速度的最小值 */
	BYTE		bTileSpeedMax; /*!< 垂直速度的最大值 */
	BYTE		bPanSpeedMin; /*!< 水平速度的最小值 */
	BYTE		bPanSpeedMax; /*!< 水平速度的最大值 */
	BYTE		bAuxMin; /*!< 辅助功能的最小值 */
	BYTE		bAuxMax; /*!< 辅助功能的最大值 */
	int			nInternal; /*!< 发送命令的时间间隔 */
	char		cType; /*!< 协议的类型 */
	char		Reserved[7];
}PTZ_OPT_ATTR;
/************************************************************************
 *   设置设备返信息
 ************************************************************************/
typedef struct
{
	DWORD		dwType;	 //类型(即GetDevConfig SetDevConfig的类型)
	WORD		wResultCode;//返回码：
					 /*		0:成功 
					  *		1:失败
					  *		2:数据不合法
					  *		3:暂时无法设置	
					  *		4:没有权限
					  */
	
	WORD   		wRebootSign;//重启标志：
					/*
					 *	0:不需要重启 1:需要重启才生效
					 */
	DWORD		dwReserved[2];//保留	
}DEV_SET_RESULT;

/******************************************************************************
* 抓图功能属性结构体
*****************************************************************************/

typedef struct 
{
	int			nChannelNum;//通道号

	DWORD		dwVideoStandardMask	;//分辨率(按位) 具体查看枚举 CAPTURE_SIZE						
	int			nFramesCount;//Frequence[128]数组的有效长度
	char 		Frames[128];	//帧率（按数值）
								//-25:25秒1帧
								//-24:24秒1帧
								//-23:23秒1帧
								//-22:22秒1帧
								//……
								//0:无效
								//1：1秒1帧
								//2：1秒2帧
								//3：1秒3帧
								//4：1秒4帧
								//5：1秒5帧
								//17：1秒17帧
								//18：1秒18帧
								//19：1秒19帧
								//20：1秒20帧
								//……
								//25: 1秒25帧
	int			nSnapModeCount;//SnapMode[16]数组的有效长度
	char		SnapMode[16];//（按数值） 0：定时触发抓图 1：手动触发抓图
	int			nPicFormatCount;//Format[16]数组的有效长度
	char 		PictureFormat[16];//（按数值）0:BMP格式 1:JPG格式
	int			nPicQualityCount;//Quality[32]数组的有效长度
	char 		PictureQuality[32];//（按数值）	
							//100:图象质量100%
							//80:图象质量80%
							//60:图象质量60%
							//50:图象质量50%
							//30:图象质量30%
							//10:图象质量10%
	char 		nReserved[128];//保留
} DH_QUERY_SNAP_INFO;

typedef struct 
{
	int			nChannelCount; //通道个数
	DH_QUERY_SNAP_INFO  stuSnap[DH_MAX_CHANNUM];
}DH_SNAP_ATTR_EN;
/******************************************************************************
* end of 配置结构体
*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // __IVIDEO_H__

