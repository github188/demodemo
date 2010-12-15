// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1AB56EE8_B58E_4EDA_B4FE_C579222C9EE9__INCLUDED_)
#define AFX_STDAFX_H__1AB56EE8_B58E_4EDA_B4FE_C579222C9EE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include "dhnetsdk.h"
typedef struct
{
	DWORD dwSize;
	DH_ALARMIN_CFG_EX 		struLocalAlmIn[DH_MAX_ALARM_IN_NUM_EX];
	DH_ALARMIN_CFG_EX		struNetAlmIn[DH_MAX_ALARM_IN_NUM_EX];
	DH_MOTION_DETECT_CFG_EX struMotion[DH_MAX_VIDEO_IN_NUM_EX];
	DH_VIDEO_LOST_CFG_EX	struVideoLost[DH_MAX_VIDEO_IN_NUM_EX];
	DH_BLIND_CFG_EX			struBlind[DH_MAX_VIDEO_IN_NUM_EX];
	DH_DISK_ALARM_CFG_EX	struDiskAlarm;
	DH_NETBROKEN_ALARM_CFG_EX	struNetBrokenAlarm;
} DHDEV_ALARM_SCHEDULE_EX;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1AB56EE8_B58E_4EDA_B4FE_C579222C9EE9__INCLUDED_)
