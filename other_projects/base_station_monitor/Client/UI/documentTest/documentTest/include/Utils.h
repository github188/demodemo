/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * Utils.h - E:\Project\resource\component\DevSDK\����\iVideo\
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
//IP�ַ�����ʽ��DWORD��ʽ��ת����������δʹ��
//long getaddrctol(char* addr);
//std::string getaddrltoc(const long& addr);

//�ַ���ʽת������ W2A
void W2A_mine(wchar_t *lpszW, char *lpszA);
size_t Utf_Unicode(wchar_t &temp, unsigned char * pUTF8);
void Change_Utf8_Unicode(unsigned char * pUTF8, char *destbuf);
////////////////////////////////////////////////////////////////////////////
//�ַ���ʽת������ A2W
void A2W_Mine(char *lpszA, wchar_t *lpszW);
size_t Single_UNICODE_UTF8(char * dest_str, wchar_t src_wchar);
void Change_Unicode_UTF8(char *pStrGBKData, int nlen, char *pbuf, int buflen);

//�����豸�����û���Ϣ
/////////////parse returned userinfo///////////////////
/*����ʹ��
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

//���ϲ���û���Ϣ�ṹ���Э���
///////////Construct right-code string/////////////////
int ConstructRightCode(char *des, DWORD *rights, int rNum, int *len, int maxlen);
///////////Build packet////////////////
int BulidUserInfoBuf(int type, void *opParam, void *subParam, char *buf, int *buflen, int maxlen, void* special=0);
int BulidUserInfoBufEx(int type, void *opParam, void *subParam, char *buf, int *buflen, int maxlen, int nMaxLength,void* special=0);
//NET_TIME�ṹ������
//�Ƚ�ʱ��
bool operator<= (const NET_TIME& lhs, const NET_TIME& rhs);
bool operator>= (const NET_TIME& lhs, const NET_TIME& rhs);
//����ƫ��ʱ��
DWORD GetOffsetTime(NET_TIME st, NET_TIME et);
NET_TIME GetSeekTimeByOffsetTime(const NET_TIME& bgtime, unsigned int offsettime);

//д��Ƶ�ļ�ͷ
void  WriteVideoFileHeader(FILE *file, int nType, int nFrameRate, 
                           int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond);

int split_str(
		const char * const src,
		char *left,
		char *right,
		const char * const spliter,
		size_t src_len);

size_t trim_blank(char *src, size_t src_len);

//�µ�ͨ����Э��
int ParseChannelNameBuf(char* src, int srclen, char* des, int deslen);
//int BuildChannelNameBuf(char* src, int srclen, char* des, int* deslen);

//�豸�汾��
int ParseVersionString(char* src, int srclen, DWORD* des);

//�µ�ddns����Э��
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

//URL����Э��
int	ParseUrlCfgString(char* src, int srclen, DHDEV_URL_CFG* des);
int BuildUrlCfgString(DHDEV_URL_CFG* src, char* des, int* deslen);

//�����������Э��
int	ParseTransStrategyCfgString(char* src, int srclen, DHDEV_TRANSFER_STRATEGY_CFG* des);
int BuildTransStrategyCfgString(DHDEV_TRANSFER_STRATEGY_CFG* src, char* des, int* deslen);

//¼����������Э��
int	ParseDownloadStrategyCfgString(char* src, int srclen, DHDEV_DOWNLOAD_STRATEGY_CFG* des);
int BuildDownloadStrategyCfgString(DHDEV_DOWNLOAD_STRATEGY_CFG* src, char* des, int* deslen);

#ifdef WIN32
//���ڣ��ַ���->�ṹ������DHDEVTIME
int	ConvertTimeType(char* src, int srclen, DHDEVTIME* des);
#endif

//IPת��
char * Ip2Str(const unsigned int iIp, char *pStr);
unsigned int Str2Ip(const char *pStr);


#endif //__UTIL_H__

