// ClientDemo8Dlg.h : header file
//

#if !defined(AFX_CLIENTDEMO8DLG_H__F9602288_0FBB_4940_BBAD_D1845C536298__INCLUDED_)
#define AFX_CLIENTDEMO8DLG_H__F9602288_0FBB_4940_BBAD_D1845C536298__INCLUDED_

#include "dhnetsdk.h"
#include <afxtempl.h>
#include <afxmt.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CClientDemo8Dlg dialog
//GPS module information 
typedef struct tagModuleInfo {
	long lLoginID;
	CString strDvrIP;
	WORD wPort;
	CString strDvrUserName;
	CString strDvrPwd;
}DH_ModuleInfo;

//GPS information package 
typedef struct tagGPSInfoPackage {
	long lLoginID;
	GPS_Info info;
}GPSInfoPackage;


class CClientDemo8Dlg : public CDialog
{
// Construction
public:
	//The interface to callback
	void ReceiveGPSMessage(LONG lLoginID,GPS_Info GpsInfo);
	void DeviceDisConnect(LONG lLoginID, char *sDVRIP, LONG nDVRPort);
	int ListenCallBack(LONG lHandle, char *pIp, WORD wPort, LONG lCommand, void *pParam, DWORD dwParamLen);
	//Callback function when device disconnected 
	friend void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);
	//Enabel to listen server callback fucntion  
	friend int CALLBACK ServerCallBack(LONG lHandle, char *pIp, WORD wPort, LONG lCommand, void *pParam, DWORD dwParamLen, DWORD dwUserData);
	//GPS message subscription callback 
	friend void CALLBACK GPSRev(LONG lLoginID, GPS_Info GpsInfo, DWORD dwUserData);
	
	CCriticalSection m_cs;
	CClientDemo8Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CClientDemo8Dlg)
	enum { IDD = IDD_CLIENTDEMO8_DIALOG };
	CListCtrl	m_ctlGPSInfo;
	CIPAddressCtrl	m_ctlLocalIP;
	CString	m_strUserName;
	CString	m_strPwd;
	int		m_nPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDemo8Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CClientDemo8Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStartlisten();
	afx_msg void OnButtonStoplisten();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString GetLocalIP();
	UINT m_nTimer;
	CList<GPSInfoPackage,GPSInfoPackage&> m_listGPSInfoPack;
	void InsertListViewItem(CString strDvrIP,CString strGPS,CString strX,CString strY,CString strStatus);
	void InitListView();
	void GPSLogout(char *pIP);
	int  GPSLogin(char *pIP,WORD wPort,char *pUserName,char *pPwd, void *pParam);
	void GPSLogoutAll();
	//GPS module information 
	CList<DH_ModuleInfo,DH_ModuleInfo&> m_listGPSModules;
	void ShowLoginErrorReason(int nError);
	//Listen service handle 
	long m_hServerHandle;
	CString GetDvrIP();
	//Log in handle 
	long m_LoginID;
	void InitNetSDK();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDEMO8DLG_H__F9602288_0FBB_4940_BBAD_D1845C536298__INCLUDED_)
