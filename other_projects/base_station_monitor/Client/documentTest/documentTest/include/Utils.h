/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * Utils.h - E:\Project\resource\component\DevSDK\发布\iVideo\
 *
 * Copyright (C) 2010 QiYang Technologies, All Rights Reserved.
 *
 * $Id: Utils.h 0001 2009-09-29 11:45:13 WuJunjie Exp $
 *
 * explain
 *     exp
 *
 * Update:
 *     2009-09-29 11:45 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef __UTIL_H__
#define __UTIL_H__

#include "iVideo.h"

#define STR_STRATEGY_IMAGE_FIRST	"ImgQlty-First"
#define STR_STRATEGY_FLUENCY_FIRST	"Fluency-First"
#define STR_STRATEGY_AUTO			"Auto"
#ifdef _DEBUG
#define IVIDEO_DEBUG_LOG_FILE  "c://iVideo.log"
int __LogString(char *pStr) ;
#endif
//IP字符串格式与DWORD格式的转换函数，暂未使用
//long getaddrctol(char* addr);
//std::string getaddrltoc(const long& addr);

//字符格式转换函数 W2A
void W2A_mine(wchar_t *lpszW, char *lpszA);
size_t Utf_Unicode(wchar_t &temp, unsigned char * pUTF8);
void Change_Utf8_Unicode(unsigned char * pUTF8, char *destbuf);
////////////////////////////////////////////////////////////////////////////
//字符格式转换函数 A2W
void A2W_Mine(char *lpszA, wchar_t *lpszW);
size_t Single_UNICODE_UTF8(char * dest_str, wchar_t src_wchar);
void Change_Unicode_UTF8(char *pStrGBKData, int nlen, char *pbuf, int buflen);

//解析设备返回用户信息
/////////////parse returned userinfo///////////////////
/*不再使用
int ParseListInfoEx(int listType, char *buf, int bufLen, void *des, DWORD *lstLen, int nMaxLength,void* special = NULL);
*/
int ParseListInfo(int listType, char *buf, int bufLen, void *des, DWORD *lstLen, void* special = NULL);
///////////parse operation-right item//////////
int ParseRightItemEx(char *buf, int bufLen, OPR_RIGHT_EX *rItem,DWORD *dwListLength,int nMaxLength);
int ParseRightItem(char *buf, int bufLen, OPR_RIGHT *rItem);
///////////parse right-code string////////////
int ParseRightCode(char *buf, int bufLen, DWORD *dwList, DWORD *lstLen);
///////////parse group item//////////
int ParseGroupItemEx(char *buf, int bufLen, USER_GROUP_INFO_EX *gpItem,DWORD *dwListLength,int nMaxLength);
int ParseGroupItem(char *buf, int bufLen, USER_GROUP_INFO *gpItem);
///////////parse user item//////////
int ParseUserItemEx(char *buf, int bufLen, USER_INFO_EX *urItem, DWORD *dwListLength,void* special,int nMaxLength);
int ParseUserItem(char *buf, int bufLen, USER_INFO *urItem, void* special);

//将上层的用户信息结构打成协议包
///////////Construct right-code string/////////////////
int ConstructRightCode(char *des, DWORD *rights, int rNum, int *len, int maxlen);
///////////Build packet////////////////
int BulidUserInfoBuf(int type, void *opParam, void *subParam, char *buf, int *buflen, int maxlen, void* special=0);
int BulidUserInfoBufEx(int type, void *opParam, void *subParam, char *buf, int *buflen, int maxlen, int nMaxLength,void* special=0);
//NET_TIME结构处理函数
//比较时间
bool operator<= (const NET_TIME& lhs, const NET_TIME& rhs);
bool operator>= (const NET_TIME& lhs, const NET_TIME& rhs);
//计算偏移时间
DWORD GetOffsetTime(NET_TIME st, NET_TIME et);
NET_TIME GetSeekTimeByOffsetTime(const NET_TIME& bgtime, unsigned int offsettime);

//写视频文件头
void  WriteVideoFileHeader(FILE *file, int nType, int nFrameRate, 
                           int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond);

int split_str(
		const char * const src,
		char *left,
		char *right,
		const char * const spliter,
		size_t src_len);

size_t trim_blank(char *src, size_t src_len);

//新的通道名协议
int ParseChannelNameBuf(char* src, int srclen, char* des, int deslen);
//int BuildChannelNameBuf(char* src, int srclen, char* des, int* deslen);

//设备版本号
int ParseVersionString(char* src, int srclen, DWORD* des);

//新的ddns配置协议
//Begin: Modify by li_deming(11517) 2008-2-20
//int ParseMultiDdnsString(char* src, int srclen, DHDEV_MULTI_DDNS_CFG* des);
//int ParseDdnsItem(char* src, int srclen, DH_DDNS_SERVER_CFG* des);
//int BuildMultiDdnsString(DHDEV_MULTI_DDNS_CFG* src, char* des, int* deslen);
//int BuildDdnsItem(DH_DDNS_SERVER_CFG* src, char* des, int* deslen);
int ParseMultiDdnsString(char* src, int srclen, DHDEV_MULTI_DDNS_CFG* des,DWORD* dwsign);
int ParseDdnsItem(char* src, int srclen, DH_DDNS_SERVER_CFG* des,DWORD* dwsign);
int BuildMultiDdnsString(DHDEV_MULTI_DDNS_CFG* src, char* des, int* deslen,DWORD dwsign);
int BuildDdnsItem(DH_DDNS_SERVER_CFG* src, char* des, int* deslen,DWORD dwsign);
//End:li_deming(11517)

//URL配置协议
int	ParseUrlCfgString(char* src, int srclen, DHDEV_URL_CFG* des);
int BuildUrlCfgString(DHDEV_URL_CFG* src, char* des, int* deslen);

//传输策略配置协议
int	ParseTransStrategyCfgString(char* src, int srclen, DHDEV_TRANSFER_STRATEGY_CFG* des);
int BuildTransStrategyCfgString(DHDEV_TRANSFER_STRATEGY_CFG* src, char* des, int* deslen);

//录象下载配置协议
int	ParseDownloadStrategyCfgString(char* src, int srclen, DHDEV_DOWNLOAD_STRATEGY_CFG* des);
int BuildDownloadStrategyCfgString(DHDEV_DOWNLOAD_STRATEGY_CFG* src, char* des, int* deslen);

#ifdef WIN32
//日期：字符串->结构体类型DHDEVTIME
int	ConvertTimeType(char* src, int srclen, DHDEVTIME* des);
#endif

//IP转换
char * Ip2Str(const unsigned int iIp, char *pStr);
unsigned int Str2Ip(const char *pStr);


#endif //__UTIL_H__

